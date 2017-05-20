#ifndef __PIC_MANAGER_H__
#define __PIC_MANAGER_H__

#include <config.h>
#include <pic_operation.h>
#include <page_manager.h>
#include <file.h>

int RegisterPicFileParser(PT_PicFileParser ptPicFileParser);
void ShowPicFmts(void);
int PicFmtsInit(void);
int JPGParserInit(void);
int BMPParserInit(void);
PT_PicFileParser Parser(char *pcName);
PT_PicFileParser GetParser(PT_FileMap ptFileMap);

#endif /* __PIC_MANAGER_H__ */