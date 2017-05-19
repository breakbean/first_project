#ifndef __PIC_OPERATION_H__
#define __PIC_OPERATION_H__

typedef struct PixelDatas {
	int iWidth;
	int iHeight;
	int iBpp;
	int iLineBytes;
	int iTotalBytes;
	unsigned char *aucPixelDatas;
}T_PixelDatas, *PT_PixelDatas;

typedef struct PicFileParser {
	char *name;
	int (*isSupport)(PT_FileMap ptFileMap);
	int (*GetPixelDatas)(PT_FileMap ptFileMap, PT_PixelDatas ptPixelDatas);
	int (*FreePixelDatas)(PT_PixelDatas ptPixelDatas);
	struct PicFileParser *ptNext;
}T_PicFileParser, *PT_PicFileParser;

#endif /* __PIC_OPERATION_H__ */