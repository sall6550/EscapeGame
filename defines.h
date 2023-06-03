#pragma once

//define �� enum�� �빮�ڷ� �������ּ���

//DIRECTION
enum DIRECTION
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    NONE
};


//USER INPUT
#define LARROW 75
#define RARROW 77
#define UARROW 72
#define DARROW 80
#define ENTER 13
#define ESCAPE 27
#define KB_N 110
#define KB_M 109
#define BUILD 114
#define SPACE 32
#define L_ROTATE 113
#define R_ROTATE 101
//SYSTEM
#define MAXUSERBLOCK 20
#define MAX_HEIGHT 30
#define MAX_WIDTH 40


//OBJECT
#define BLANK 0
#define THORNS 1
#define BLOCK 2
#define PLAYER 3