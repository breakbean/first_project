#ifndef __FILE_H__
#define __FILE_H__

typedef struct FileMap {
	char strFileName[256];
	FILE *tFp;
	int iFileSize;
	unsigned char *pucFileMapMem;
}T_FileMap, *PT_FileMap;

int MapFile(PT_FileMap ptFileMap);
void UnMapFile(PT_FileMap ptFileMap);

#endif /* __FILE_H__ */