#include <config.h>
#include <debug_manager.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

#define SERVER_PORT  5678
#define PRINT_BUF_SIZE   (16*1024)

static int g_iSocketServer;
static struct sockaddr_in g_tSocketServerAddr;
static struct sockaddr_in g_tSocketClinetAddr;

static pthread_t g_tSendThreadID;
static pthread_t g_tRecvThreadID;
static pthread_mutex_t g_tNetDbgSendMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_tNetDbgSendConVar = PTHREAD_COND_INITIALIZER;

static int g_iHaveConnected = 0;

static char *g_pcNetPrintBuf;

static int g_iReadPos  = 0;
static int g_iWritePos = 0;

static int isFull(void)
{
	return (((g_iWritePos + 1) % PRINT_BUF_SIZE) == g_iReadPos);
}

static int isEmpty(void)
{
	return (g_iWritePos == g_iReadPos);
}

static int PutData(char cVal)
{
	if (isFull())
	{
		return -1;
	}
	else
	{
		g_pcNetPrintBuf[iWritePos] = cVal;
		iWritePos = (iWritePos + 1) % PRINT_BUF_SIZE;
		return 0;
	}
}

static int GetData(char *pcVal)
{
	if (isEmpty())
	{
		return -1;
	}
	else
	{
		*pcVal   = g_pcNetPrintBuf[iReadPos];
		iReadPos = (iReadPos + 1) % PRINT_BUF_SIZE;
		return 0;
	}
}

static void *NetDbgSendThreadFunction(void *pVoid)
{
	int i;
	char cVal;
	char strTmpBuf[512];
	int iAddrLen;
	int iSendLen;
	
	while (1)
	{
		pthread_mutex_lock(&g_tNetDbgSendMutex);
		pthread_cond_wait(&g_tNetDbgSendConVar, &g_tNetDbgSendMutex);	
		pthread_mutex_unlock(&g_tNetDbgSendMutex);
		
		while (g_iHaveConnected && !isEmpty())
		{
			i = 0;
			if ((i < 512) && (0 == GetData(cVal)))
			{
				strTmpBuf[i] = cVal;
				i++;
			}
			
			iAddrLen = sizeof(struct sockaddr);
			iSendLen = sendto(g_iSocketServer, strTmpBuf, i, 0, (const struct sockaddr *)&g_tSocketClinetAddr, iAddrLen);
		}
	}
	
	return NULL;
}

static void *NetDbgRecvThreadFunction(void *pVoid)
{
	socklen_t iAddrLen;
	int iRecvLen;
	char ucRecvBuf[1000];
	struct sockaddr_in tSocketClinetAddr;
	
	while (1)
	{
		iAddrLen = sizeof(sockaddr);
		iRecvLen = recvfrom(g_iSocketServer, ucRecvBuf, 999, 0, (struct sockaddr *)&tSocketClinetAddr, &iAddrLen);
		
		if (iRecvLen > 0)
		{
			if (strcmp(ucRecvBuf, "setclient") == 0)
			{
				g_tSocketClientAddr = tSocketClientAddr;
				g_iHaveConnected = 1;
			}
			else if (strncmp(ucRecvBuf, "dbglevel=", 9) == 0)
			{
				SetDbgLevel(ucRecvBuf);
			}
			else
			{
				SetDbgChanel(ucRecvBuf);
			}
		}
	}
	return NULL;
}


int NetDbgInit(void)
{
	int iRet;
	
	g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == g_iSocketServer)
	{
		printf("socket err\n");
		return -1;
	}
	
	g_tSocketServerAddr.sin_family 	    = AF_INET;
	g_tSocketServerAddr.sin_port 	    = htons(SERVER_PORT);
	g_tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(g_tSocketServerAddr.sin_zero, 0, 8);
	
	iRet = bind(g_iSocketServer, (const struct sockaddr *)&g_tSocketServerAddr, sizeof(struct sockaddr));
	if (iRet == -1)
	{
		printf("bind err\n");
		return -1;
	}
	
	g_pcNetPrintBuf = malloc(PRINT_BUF_SIZE);
	if (g_pcNetPrintBuf == NULL)
	{
		close(g_iSocketServer);
		return -1;
	}
	
	pthread_create(&g_tSendThreadID, NULL, NetDbgSendThreadFunction, NULL);
	pthread_create(&g_tRecvThreadID, NULL, NetDbgRecvThreadFunction, NULL);
	
	return 0;
}

int NetDbgExit(void)
{
	free(g_pcNetPrintBuf);
	close(g_iSocketServer);
	return 0;
}

int NetDbgPrint(char *strData)
{
	int i;
	for (i = 0; i < strlen(strData); i++)
	{
		if (0 != PutData(strData[i]))
			break;
	}
	
	pthread_mutex_lock(&g_tNetDbgSendMutex);
	pthread_cond_signal(&g_tNetDbgSendConVar);
	pthread_mutex_unlock(&g_tNetDbgSendMutex);
	
	return i;
}

static T_DebugOpr g_tNetDbgOpr = {
	.name 		= "netprint",
	.isCanUse   = 1,
	.DebugInit  = NetDbgInit,
	.DebugExit  = NetDbgExit,
	.DebugPrint = NetDbgPrint,
};

int NetPrintInit(void)
{
	return RegisterDebugOpr(&g_tNetDbgOpr);
}