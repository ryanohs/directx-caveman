#ifndef _GAME_H_
#define _GAME_H_

#include<d3d9.h>
#include<d3dx9.h>
#include<d3dx9math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include "dxgraphics.h"

#define APPTITLE "Game"
#define FULLSCREEN FALSE
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// macros for keyboard until I learn DirectInput
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

int Game_Init(HWND);
void Game_Frame(HWND);
void Game_End(HWND);

// sprite
typedef struct {
    int x, y;
    int width, height;
    int movex, movey;
    int curframe, lastframe;
    int animdelay, animcount;
    int jumpingFrame;
    bool faceLeft;
} SPRITE;


#endif