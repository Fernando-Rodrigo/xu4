/*
 * $Id$
 */

#ifndef EVENT_H
#define EVENT_H

#define U4_UP '['
#define U4_DOWN '/'
#define U4_LEFT ';'
#define U4_RIGHT '\''
#define U4_BACKSPACE 8
#define U4_ESC 27

typedef int (*KeyHandler)(int, void *);

typedef struct KeyHandlerNode {
    KeyHandler kh;
    void *data;
    struct KeyHandlerNode *next;
} KeyHandlerNode;

typedef struct ReadBufferActionInfo {
    int (*handleBuffer)(const char *);
    char *buffer;
    int bufferLen;
    int screenX, screenY;
} ReadBufferActionInfo;

typedef struct GetChoiceActionInfo {
    const char *choices;
    int (*handleChoice)(char);
} GetChoiceActionInfo;

void eventHandlerInit();
void eventHandlerMain();
void eventHandlerSetExitFlag(int flag);
int eventHandlerGetExitFlag();
void eventHandlerAddTimerCallback(void (*callback)());
void eventHandlerRemoveTimerCallback(void (*callback)());
void eventHandlerCallTimerCallbacks();
void eventHandlerPushKeyHandler(KeyHandler kh);
void eventHandlerPushKeyHandlerData(KeyHandler kh, void *data);
void eventHandlerPopKeyHandler();
KeyHandler eventHandlerGetKeyHandler();
void *eventHandlerGetKeyHandlerData();

int keyHandlerDefault(int key, void *data);
int keyHandlerReadBuffer(int key, void *data);
int keyHandlerGetChoice(int key, void *data);

#endif
