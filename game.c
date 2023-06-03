#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "block.h"

int gBoardHeight = 20;
int gBoardWidth = 60;
int speed = 500 / 15;

//아직 미구현
//0:공백 1:블록■ 2:투과블록□ 3:가시△ 4:가시▽ 5:가시◁ 6:가시▷ 7:투사체발사블록▣ 8:움직이는 블록▤ 9:파이프블록┌ 10:파이프블록┐
//11:┘12:└ 13:사이드퀘스트★ 14:중력무시 아이템♣ 15:적 무시 아이템♠ 16:1회 부활 아이템♥

//현재 0:공백 1:가시 2:바닥 3:플레이어
int gameBoardInfo[20][30]= {
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
};
//SYSTEM
COORD GetCurrentCursorPos();
void SetCurrentCursorPos(int x, int y);

//DRAW
void RemoveCursor();
int ShowMainMenu();
int ShowGame();
void ShowResult();
void DrawGameUI();
void UpdateGameUI();
int GamePause();
void ClearConsole();
void DrawGameBoard();
void Player();

//BLOCKMANAGE
int UserBlockID[MaxUserBlock] = { 0 };
int StageNumber = 1;
int CurrentUserBlock;
int page=1;
void UserBlockManage();
void BlockAllocator();
void ShowBlock(char blockinfo[4][4]);
void DeleteAllBlock();
void ClearConsole();
int main()
{
	srand(time(NULL));
	RemoveCursor();

	int control;

	while (1)
	{
		if (ShowMainMenu() == 0)
			break;
		//control = ShowMainMenu();

		//if (control == 0)	break;
		

		while (1)
		{
			BlockAllocator();
			if (ShowGame() == 0)
				break;

			//control = GamePause();

			//if (control == 0)
			//	break;
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
int ShowGame()
{
	int time = 0;
	DWORD startMsTime = GetTickCount();

	DrawGameUI();
	DrawGameBoard();
	UserBlockManage();
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

		if (_kbhit() != 0)
		{
			int key = _getch();

			switch (key)
			{
			case ESCAPE:
				if (GamePause() == 0)
				{
					ClearConsole();
					return 0;
				}
				else
					DrawGameBoard();
				break;
			case 110:
				if (page >= 5)
					break;
				page++;
				UserBlockManage();
				break;
			case 109:
				if (page <= 1)
					break;
				page--;
				UserBlockManage();
				break;
			}

		}

		//DrawGameBoard();
		Player();
	}
}
void ShowResult()
{

}
void DrawGameUI()
{
	int statusBoardWidth = 28;
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
	// 유저블록판그리기
	int x, y;
	x = gBoardWidth + 6, y = 7;
	for (int z = 1; z <= 3; z++) {
		for (int x = gBoardWidth + 2; x <= gBoardWidth + 30; x += 2)
		{
			SetCurrentCursorPos(x, y);
			if (x == gBoardWidth + 2)
				printf("├");
			else if (x == gBoardWidth + 30)
				printf("┤");
			else
				printf("─");
		}
		y += 6;
	}
	for (int y = 7; y < gBoardHeight ; y++) {
		x = gBoardWidth + 16;
		SetCurrentCursorPos(x, y);
		if (y == 7)
			printf("┬");
		else if (y == 19)
			printf("┴");
		else if ( y == 13)
			printf("┼");
		else
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
int GamePause()
{
	//0 = 메인 메뉴로 / 1 = 게임 재개

	int select = 1;

	for (int y = gBoardHeight / 2 - 4; y <= gBoardHeight / 2 + 4; y++)
	{
		for (int x = gBoardWidth / 2 - 15; x <= gBoardWidth / 2 + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf(" ");
		}
	}

	SetCurrentCursorPos(gBoardWidth / 2 - 10, gBoardHeight / 2 - 3);
	printf("G A M E   P A U S E D");

	SetCurrentCursorPos(gBoardWidth / 2 - 6, gBoardHeight / 2);
	printf("▶  RESUME");

	SetCurrentCursorPos(gBoardWidth / 2 - 9, gBoardHeight / 2 + 2);
	printf("RETURN TO MAIN MENU");



	while (1)
	{
		if (_kbhit() != 0)
		{
			int key = _getch();

			switch (key)
			{
			case DARROW:
				if (select == 1)
				{
					select = 0;
					SetCurrentCursorPos(gBoardWidth / 2 - 6, gBoardHeight / 2);
					printf("  ");
					SetCurrentCursorPos(gBoardWidth / 2 - 13, gBoardHeight / 2 + 2);
					printf("▶");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos(gBoardWidth / 2 - 13, gBoardHeight / 2 + 2);
					printf("  ");
					SetCurrentCursorPos(gBoardWidth / 2 - 6, gBoardHeight / 2);
					printf("▶");
				}
				break;
			case ENTER:
				return select;
				break;
			}
		}
	}
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

void DrawGameBoard()
{
	int x, y;
	int cursX, cursY;
	for (y = 0; y < 20; y++)
	{
		for (x = 0; x < 30; x++)
		{
			cursX = x * 2+2;
			cursY = y+1;
			SetCurrentCursorPos(cursX, cursY);
			if (gameBoardInfo[y][x] == 0)
			{
				printf("  ");
			}
			else if (gameBoardInfo[y][x] == 1) {
				printf("△");
			}
			else if (gameBoardInfo[y][x] == 2) { //바닥
				printf("==");
			}
			else if (gameBoardInfo[y][x] == 3) { 
				printf("■");
			}
			else { printf("  "); }
		}
	}
}

void Player()
{

}
void UserBlockManage()
{
	int x, y;
	x = gBoardWidth + 6, y = 8;
	

	// 화면에 현재 페이지 유저블록 출력
	for (int i = 4*page - 4; i < 4*page ; i++)
	{
		SetCurrentCursorPos(x, y);
		
		if (UserBlockID[i] == -1) {
			DeleteAllBlock();
			SetCurrentCursorPos(x , y + 2);
			printf("empty");
		}
		else {
			DeleteAllBlock();
			ShowBlock(blockModel[UserBlockID[i]]);
		}
		SetCurrentCursorPos(x+2, y+4);
		printf("-0%d-",i%4+1);
		
		if (i % 2 == 0)
			x += 14;
		else
		{
			x -= 14;
			y += 6;
		}

	}
	SetCurrentCursorPos(gBoardWidth + 4, gBoardHeight);
	printf(" page : %d/%d n:next m:prev", page,5);
}
void BlockAllocator() // 초기블럭할당자.
{
	if (StageNumber == 1)
	{
		CurrentUserBlock = 0;
		for (int i = 0; i < MaxUserBlock; i++) {
			if (i < 7)
			{
				UserBlockID[i] = i * 4;
				CurrentUserBlock++;
			}
			else
				UserBlockID[i] = -1;
			
		}
	}
}
void ShowBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("■");
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}
void DeleteBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1)
				printf(" ");
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}
void DeleteAllBlock()
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			printf("  ");
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}