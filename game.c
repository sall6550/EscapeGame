#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "block.h"

int gBoardHeight = 20;
int gBoardWidth = 60;
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];
int speed = 500 / 15;

//SYSTEM
COORD GetCurrentCursorPos();
void SetCurrentCursorPos(int x, int y);

//DRAW
void RemoveCursor();
int ShowMainMenu();
void ShowGame();
void ShowResult();
int ShowPause();
void DrawGameUI();
void UpdateGameUI();

void ClearConsole();


int main()
{
	srand(time(NULL));
	RemoveCursor();

	int control;

	while (1)
	{
		control = ShowMainMenu();

		if (control == 0)	break;
		

		while (1)
		{
			ShowGame();

			control = ShowPause();

			if (control == 0)
				break;
		}
	}

	return 0;
}


COORD GetCurrentCursorPos()
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}
void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
int ShowMainMenu()
{
	int select = 1;
	//1 = 게임시작 / 0 = 게임종료

	SetCurrentCursorPos(50, 10);
	printf("ESCAPE FROM COLLEGE");

	SetCurrentCursorPos(54, 20);
	printf("GAME START");

	SetCurrentCursorPos(57, 22);
	printf("EXIT");

	SetCurrentCursorPos(50, 20);
	printf("▶");

	while (1)
	{
		if (_kbhit() != 0)
		{
			int key = _getch();
			//printf("%d", key);

			switch (key)
			{
			case DARROW:
				if (select == 1)
				{
					select = 0;
					SetCurrentCursorPos(50, 20);
					printf("  ");
					SetCurrentCursorPos(53, 22);
					printf("▶");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos(53, 22);
					printf("  ");
					SetCurrentCursorPos(50, 20);
					printf("▶");
				}
				break;
			case ENTER:
				ClearConsole();
				return select;
				break;
			}
		}
	}
}
void ShowGame()
{
	int time = 0;
	DWORD startMsTime = GetTickCount();

	DrawGameUI();

	while (1)
	{
		DWORD curMsTime = GetTickCount();

		if ((curMsTime - startMsTime) * 0.001f >= time + 1)
		{
			time = (curMsTime - startMsTime) * 0.001f;

			int minute = time / 60;
			int second = time % 60;

			SetCurrentCursorPos(70, 4);
			printf("      ");

			SetCurrentCursorPos(71, 4);
			printf("%02d:%02d", minute, second);
		}
	}
}
void ShowResult()
{

}
void DrawGameUI()
{
	int statusBoardWidth = 20;
	int statusBoardHeight = gBoardHeight;

	for (int y = 0; y < gBoardHeight + 2; y += gBoardHeight + 1)
		for (int x = 2; x < gBoardWidth + statusBoardWidth + 2; x += 2)
		{
			SetCurrentCursorPos(x, y);
			printf("─");
		}

	for (int x = 0; x < gBoardWidth + 3; x += gBoardWidth + 2)
		for (int y = 1; y < gBoardHeight + 1; y++)
		{
			SetCurrentCursorPos(x, y);
			printf("│");
		}

	for (int y = 1; y < gBoardHeight + 1; y++)
	{
		SetCurrentCursorPos(gBoardWidth + statusBoardWidth + 2, y);
		printf("│");
	}

	SetCurrentCursorPos(0, 0);
	printf("┌");
	SetCurrentCursorPos(gBoardWidth + 2, gBoardHeight + 1);
	printf("┴");
	SetCurrentCursorPos(gBoardWidth + 2, 0);
	printf("┬");
	SetCurrentCursorPos(0, gBoardHeight + 1);
	printf("└");

	SetCurrentCursorPos(gBoardWidth + statusBoardWidth + 2, statusBoardHeight + 1);
	printf("┘");
	SetCurrentCursorPos(gBoardWidth + statusBoardWidth + 2, 0);
	printf("┐");

	SetCurrentCursorPos(71, 3);
	printf("TIME");
	SetCurrentCursorPos(71, 4);
	printf("%02d:%02d", 0, 0);
}
void UpdateGameUI()
{

}
int ShowPause()
{

}
void ClearConsole()
{
	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("  ");
		}
	}
}