#ifndef GAME_H
#define GAME_H

#include "game_vars.h"
#include "message.h"

#include <stdbool.h>

extern char byte_501C9C[];

extern bool gGameUiDisabled;
extern int dword_5186B8;
extern bool gIsMapper;
extern int* gGameGlobalVars;
extern int gGameGlobalVarsLength;
extern const char* asc_5186C8;
extern int dword_5186CC;

extern MessageList gMiscMessageList;
extern int dword_58E948;
extern int dword_58E94C;

int gameInitWithOptions(const char* windowTitle, bool isMapper, int a3, int a4, int argc, char** argv);
void gameReset();
void gameExit();
int gameHandleKey(int eventCode, bool isInCombatMode);
void gameUiDisable(int a1);
void gameUiEnable();
bool gameUiIsDisabled();
int gameGetGlobalVar(int var);
int gameSetGlobalVar(int var, int value);
int gameLoadGlobalVars();
int globalVarsRead(const char* path, const char* section, int* out_vars_num, int** out_vars);
int sub_443E2C();
int sub_443E34(int a1);
void sub_443E90();
int gameTakeScreenshot(int width, int height, unsigned char* buffer, unsigned char* palette);
void showHelp();
int showQuitConfirmationDialog();
int gameDbInit();
void showSplash();

#endif /* GAME_H */
