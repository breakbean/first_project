#ifndef __ENCODING_MANAGER_H__
#define __ENCODING_MANAGER_H__

typedef struct EncodingOpr {
	char *name;
	int iHeadLen;
	PT_FontOpr ptFontOprSupportedHead;
	int (*isSupport)(unsigned char *pucBufHead);
	int (*GetCodeFrmBuf)(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned char *pdwCode);
	struct EncodingOpr *ptNext;
}T_EncodingOpr, *PT_EncodingOpr;

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr);
PT_EncodingOpr Encode(char *pcName);
void ShowEncodingOpr(void);
PT_DispOpr GetDispOpr(char *pcName);
PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead);
int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);
int DelFontOprFrmEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);
int EncodingInit(void);
int AsciiEncodingInit(void);
int Utf16beEncodingInit(void);
int Utf16leEncodingInit(void);
int Utf8EncodingInit(void);
int GetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);

#endif /* __ENCODING_MANAGER_H__s */