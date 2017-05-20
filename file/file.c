#include <config.h>
#include <file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int MapFile(PT_FileMap ptFileMap)
{
	int iFd;
	FILE *tFp;
	struct stat tStat;
	
	tFp = fopen(ptFileMap->strFileName, "r+");
	if (tFp == NULL)
	{
		DBG_PRINTF("can't open %s\n", ptFileMap->strFileName);
		return -1;
	}
	
	ptFileMap->tFp = tfp;
	iFd = fileno(tFp);
	
	fstat(iFd, &tStat);
	ptFileMap->iFileSize = tStat.st_size;
	ptFileMap->pucFileMapMem = (unsigned char *)mmap(NULL, tStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, iFd, 0);
	if (ptFileMap->pucFileMapMem == (unsigned char *)-1)
	{
		DBG_PRINTF("mmap error!\n");
		return -1;
	}
	
	return 0;
}

void UnMapFile(PT_FileMap ptFileMap)
{
	munmap(ptFileMap->pucFileMapMem, ptFileMap->iFileSize);
	fclose(ptFileMap->tFp);
}