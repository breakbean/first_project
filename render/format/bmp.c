#include <config.h>
#include <pic_operation.h>
#include <picfmt_manager.h>
#include <file.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push)
#pragma pack(1)

typedef struct tagBITMAPFILEHEADER { /* bmfh */
	unsigned short bfType; 
	unsigned long  bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long  bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER { /* bmih */
	unsigned long  biSize;
	unsigned long  biWidth;
	unsigned long  biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned long  biCompression;
	unsigned long  biSizeImage;
	unsigned long  biXPelsPerMeter;
	unsigned long  biYPelsPerMeter;
	unsigned long  biClrUsed;
	unsigned long  biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop) /* 恢复先前的pack设置 */

static int isBMPFormat(PT_FileMap ptFileMap);
static int GetPixelDatasFrmBMP(PT_FileMap ptFileMap, PT_PixelDatas ptPixelDatas);
static int FreePixelDatasForBMP(PT_PixelDatas ptPixelDatas);

static T_PicFileParser g_tBMPParser = {
	.name = "bmp",
	.isSupport = isBMPFormat,
	.GetPixelDatas = GetPixelDatasFrmBMP,
	.FreePixelDatas = FreePixelDatasForBMP,
};

static int isBMPFormat(PT_FileMap ptFileMap)
{
	unsigned char *aFileHead = ptFileMap->pucFileMapMem;
	
	if (aFileHead[0] != 0x42 || aFileHead[1] != 0x4d)
		return 0;
	else
		return 1;
}

static int GetPixelDatasFrmBMP(PT_FileMap ptFileMap, PT_PixelDatas ptPixelDatas)
{
	BITMAPFILEHEADER *ptBITMAPFILEHEADER;
	BITMAPINFOHEADER *ptBITMAPINFOHEADER;
	
	unsigned char *aFileHead;
	
	int iWidth;
	int iHeight;
	int iBMPBpp;
	int y;
	
	int iLineWidthAlign;
	int iLineWidthReal;
	
	unsigned char *pucSrc;
	unsigned char *pucDest;
	
	aFileHead = ptFileMap->pucFileMapMem;
	ptBITMAPFILEHEADER = (ptBITMAPFILEHEADER *)aFileHead;
	ptBITMAPINFOHEADER = (ptBITMAPINFOHEADER *)(aFileHead + sizeof(BITMAPFILEHEADER));
	
	iWidth  = BITMAPFILEHEADER->biWidth;
	iHeight = BITMAPFILEHEADER->biHeight;
	iBMPBpp = BITMAPFILEHEADER->biBitCount;
	
	if (iBMPBpp != 24)
	{
		DBG_PRINTF("iBMPBpp = %d\n", iBMPBpp);
		DBG_PRINTF("sizeof(BITMAPFILEHEADER) = %d\n", sizeof(BITMAPFILEHEADER));
		return -1;
	}
	
	ptPixelDatas->iWidth 		= iWidth;
	ptPixelDatas->iHeight 		= iHeight;
	ptPixelDatas->iLineBytes    = iWidth * ptPixelDatas->iBpp / 8;
	ptPixelDatas->iTotalBytes   = ptPixelDatas->iHeight * ptPixelDatas->iLineBytes;
	ptPixelDatas->aucPixelDatas = malloc(ptPixelDatas->iTotalBytes);
	
	if (ptPixelDatas->aucPixelDatas == NULL)
	{
		return -1;
	}
	
	iLineWidthReal  = iWidth * iBMPBpp / 8;
	iLineWidthAlign = (iLineWidthReal + 3) & ~0x03;
	
	pucSrc  = aFileHead + ptBITMAPFILEHEADER->bfOffBits;
	pucSrc  = pucSrc + (iHeight - 1) * iLineWidthAlign;
	
	pucDest = ptPixelDatas->aucPixelDatas;
	
	for (y = 0; y < iHeight; y++)
	{
		ConvertOneLine(iWidth, iBMPBpp, ptPixelDatas->iBpp, pucSrc, pucDest);
		pucSrc  -= iLineWidthAlign;
		pucDest += ptPixelDatas->iLineBytes;
	}
	
	return 0;
}

static int FreePixelDatasForBMP(PT_PixelDatas ptPixelDatas);
