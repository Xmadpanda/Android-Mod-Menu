#ifndef DPZMOD_HACK_H
#define DPZMOD_HACK_H
#include <jni.h>

#define targetProcessName "com.unity.test" // Game Package Name
#define targetLib "libil2cpp.so"           // Enter the lib here

bool enableHack;
char *gameDataDir;
void *hackThread(void *arg);

#define HOOKAF(ret, func, ...) \
	ret (*orig##func)(__VA_ARGS__); \
	ret my##func(__VA_ARGS__)

#endif