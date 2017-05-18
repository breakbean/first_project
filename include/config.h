#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <debug_manager.h>

#define FB_DEVICE_NAME  "/dev/fb0"
#define DEFAULT_DIR     "/"

#define COLOR_BACKGROUND   0xE7DBB5  /* ���Ƶ�ֽ */
#define COLOR_FOREGROUND   0x514438  /* ��ɫ���� */

//defiine DBG_PRINTF(...)
#define DGB_PRINTF  DebugPrint

#define ICON_PATH  "/etc/digitpic/icons"

#endif /* __CONFIG_H__ */