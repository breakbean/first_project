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

static T_PicFileParser g_tBMPParser = {
  .name = "bmp",
  .isSupport = isBMPFormat,
  .GetPixelDatas = GetPixelDatasFrmBMP,
  .FreePixelDatas = FreePixelDatasForBMP,
};
