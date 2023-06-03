#pragma once

//define 및 enum은 대문자로 통일해주세요

//DIRECTION
enum DIRECTION
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    NONE
};

//TEXT COLOR
enum TEXTCOLOR
{
    BLACK,
    BLUE,
    GREEN,
    AQUA,
    RED,
    PURPLE,
    YELLOW,
    WHITE,
    GRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTAQUA,
    LIGHTRED,
    LIGHTPURPLE,
    LIGHTYELLOW,
    BRIGHTWHITE
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
#define MAX_HEIGHT 65
#define MAX_WIDTH 236


//OBJECT
#define BLANK 0
#define THORNS 1
#define BLOCK 2
#define PLAYER 3


//ITEM
#define DEFYING_GRAVITY_TIME 5   //중력무시아이템 시간설정
#define INVINCIBILITY_TIME 5 //무적아이템 시간설정