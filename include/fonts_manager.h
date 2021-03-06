#ifndef __FONTS_MANAGER_H__
#define __FONTS_MANAGER_H__

typedef struct FontBitMap {
	int iXLeft;
	int iYTop;
	int iXMax;
	int iYMax;
	int iBpp;
	int iPitch;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char *pucBuffer;
}T_FontBitMap, *PT_FontBitMap;

typedef struct FontOpr {
	char *name;
	int (*FontInit)(char *pcFontFile, unsigned int dwFontSize);
	int (*GetFontBitmap)(unsigned int dwCode, PT_FontBitMap pt_FontBitMap);
	void (*SetFontSize)(unsigned int dwFontSize);
	struct FontOpr *ptNext;
}T_FontOpr, *PT_FontOpr;

int RegisterFontOpr(PT_FontOpr ptFontOpr);
void ShowFontOpr(void);
int FontsInit(void);
int ASCIIInit(void);
int GBKInit(void);
int FreeTypeInit(void);
PT_FontOpr GetFontOpr(char *pcName);
void SetFontSize(unsigned int dwFontSize);
unsigned int GetFontSize(void);
int GetFontBitMap(unsigned int dwCode, PT_FontBitMap ptFontBitMap);
int SetFontsDetail(char *pcFontsName, char *pcFontsFile, unsigned int dwFontSize);

#endif /* __FONTS_MANAGER_H__ */