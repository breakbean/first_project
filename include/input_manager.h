#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <sys/time.h>
#include <pthread.h>

#define INPUT_TYPE_STDIN 		 0
#define INPUT_TYPE_TOUCHSCREEN   1

typedef struct InputEvent {
	struct timeval tTime;
	int iType;
	int iX;
	int iY;
	int iKey;
	int iPressure;
}T_InputEvent, *PT_InputEvent;

typedef struct InputOpr {
	char *name;
	pthread_t tThreadID;
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	int (*GetInputEvent)(PT_InputEvent ptInputEvent);
	struct InputOpr *ptNext;
}T_InputOpr *PT_InputOpr;

#endif /* __INPUT_MANAGER_H__ */