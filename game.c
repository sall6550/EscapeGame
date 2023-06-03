#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "block.h"
#include "defines.h"
#include "player.h"
#include "objects.h"
#pragma warning(disable:4996)


int gBoardHeight;
int gBoardWidth;
clock_t start1, finish1,start2,finish2;
double duration1,duration2;

//현재 0:공백 1:가시 2:바닥 3:플레이어
//방향은 ↑:1 / →:2 / ↓:3 / ←:4
//모든 오브젝트 정보는 세자리수로 통일하여 관리

//*추가
//block.h에 enum direction으로 ↑:1 / ↓:2 / →:3 / ←:4 만들어놨으니까
//이거 쓰시면 편할거에요
//그리고 define은 다 헤더파일 따로 만들어서 넣어놨어요


void StatusPrint();
void PrintPlayer();

//SYSTEM
COORD GetCurrentCursorPos();
void SetCurrentCursorPos(int x, int y);


//DRAW
void ControlConsoleWindow();
int ShowMainMenu();
int ShowGame();
void ShowResult();
void DrawGameUI();
void UpdateTimeUI(int time);
void UpdateGameUI();
int GamePause();
void ShowGameOver();
void ShowStageClear(int time);
void ClearConsole();


//MAP
int gameBoardInfo[MAX_HEIGHT][MAX_WIDTH] = { 0 };
int SideQuest = 0;
int StageClear = 0;
int StageNumber = 3;
int StageClearTime = 0;
int score;
void InitStageInfo();
int LoadStage(Node* mObjListHead);
void DrawGameBoard();
void ClearGameBoard();


//PLAYER
Player p;
COORD StartPosition;
void InitPlayer();
void ControlCharacter(int key);
int DetectCollisionForPlayer(int x, int y);
int MovePlayer();
void DiePlayer();


//BLOCKMANAGE
int UserBlockID[MAXUSERBLOCK] = { 0 };
int CurrentUserBlock=0;
int page = 1;
int bX, bY;
int blockid = 0;
int MODE = 0, prevblockid = -1;
int prevbX=0, prevbY = 0;
int collosion_redraw = 0;/*
void ShowRedBlock(char blockInfo[4][4]);
*/
Node* Pipeblock_Changeobj(Node* it,int col,int direction);// 파이프블럭
Node* changeObjLot(Node* it,int x,int y, int direction) ;
void AddBlock(int blockid);
void BlockListUpdate(int UseBlock);
void BlockBuild(int key);
void UserBlockManage();
void BlockAllocator();
void ShowBlock(char blockinfo[4][4],int color);
void DeleteBlock(char blockinfo[4][4]);
void DeleteAllBlock();
void MakeBlock(char blockInfo[4][4]);
int DetectCollisionForBlock(int x, int y, char blockInfo[4][4]);
//pipeobjects
int purple = 1; //
void drawPurplePuzzle(int collosion);
//MovingObjects
int detectCollisionInDirection(int x, int y, int direction);
int detectCollision(int x, int y);
int parseInfo(int info, int choice);
int detectCollisionMovingBlocks(int x, int y);
void moveAll(Node* headNode);
Node* searchCoord(Node* mObjListHead,int x, int y);
Node* thornMoveDirection(Node* it, int direction);
Node* mBlockMoveDirection(Node* it, int direction, int rotation);
void addObj(mObj obj, Node* listHeader);
void removeObj(Node* it);
void removeObjWithoutShowing(Node* it);
void freeAll(Node* headNode);
int detectCollisionForMBlock(int x, int y, int rotation);



//M A I N
int main()
{
	srand(time(NULL));
	ControlConsoleWindow();

	int control;

	while (1)
	{
		if (ShowMainMenu() == 0)
			break;
		MODE = 0;
		CurrentUserBlock = 0;
		BlockAllocator();
		while (1)
		{
			
			//메인메뉴로 = 0 / 스테이지 클리어 = 1
			control = ShowGame();
			
			if (control == 0)
				break;
		}
	}

	return 0;
}


//SYSTEM
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


//DRAW
void ControlConsoleWindow()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

	system("mode con cols=236 lines=65 | title Escape From Sejong");
}
int ShowMainMenu()
{
	int select = 1;
	//1 = 게임시작 / 0 = 게임종료

	SetCurrentCursorPos(70, 10);
	printf(" #######   #####     ####     ##     ######   #######           #######  ######     ###    ##   ## ");
	SetCurrentCursorPos(70, 11);
	printf(" ##       ##   ##   ##  ##    ##     ##   ##  ##                ##       ##   ##   ## ##   ##   ## ");
	SetCurrentCursorPos(70, 12);
	printf(" ##       ##       ##        ####    ##   ##  ##                ##       ##   ##  ##   ##  ### ### ");
	SetCurrentCursorPos(70, 13);
	printf(" #####     #####   ##        ## #    ######   #####             #####    ######   ##   ##  ## # ## ");
	SetCurrentCursorPos(70, 14);
	printf(" ##            ##  ##       ######   ##       ##                ##       ## ##    ##   ##  ## # ## ");
	SetCurrentCursorPos(70, 15);
	printf(" ##       ##   ##   ##  ##  ##   #   ##       ##                ##       ##  ##    ## ##   ##   ## ");
	SetCurrentCursorPos(70, 16);
	printf(" #######   #####     ####  ###   ##  ##       #######           ##       ##   ##    ###    ##   ## ");

	SetCurrentCursorPos(91, 19);
	printf("  #####   #######    ####     ###    ##   ##    ####  ");
	SetCurrentCursorPos(91, 20);
	printf(" ##   ##  ##           ##    ## ##   ###  ##   ##  ## ");
	SetCurrentCursorPos(91, 21);
	printf(" ##       ##           ##   ##   ##  ###  ##  ##      ");
	SetCurrentCursorPos(91, 22);
	printf("  #####   #####        ##   ##   ##  ## # ##  ##      ");
	SetCurrentCursorPos(91, 23);
	printf("      ##  ##       ##  ##   ##   ##  ## # ##  ##  ### ");
	SetCurrentCursorPos(91, 24);
	printf(" ##   ##  ##       ##  ##    ## ##   ##  ###   ##  ## ");
	SetCurrentCursorPos(91, 25);
	printf("  #####   #######   ####      ###    ##   ##    ##### ");

	SetCurrentCursorPos(115, 40);
	printf("GAME START");

	SetCurrentCursorPos(118, 44);
	printf("EXIT");

	SetCurrentCursorPos(108, 40);
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
					SetCurrentCursorPos(108, 40);
					printf("  ");
					SetCurrentCursorPos(111, 44);
					printf("▶");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos(111, 44);
					printf("  ");
					SetCurrentCursorPos(108, 40);
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
	int key = 0;
	
	DWORD countMsTime = GetTickCount();

	mObj dummy = { 0,0,0,0 };

	Node mObjListTail = { .obj = dummy, .nextNode = NULL, .prevNode = NULL };
	Node mObjListHead = { .obj = dummy, .nextNode = &mObjListTail, .prevNode = NULL };
	mObjListTail.prevNode = &mObjListHead;

	InitPlayer();
	LoadStage(&mObjListHead);
	DrawGameUI();
	DrawGameBoard();
	UserBlockManage();
	purple = 1;
	drawPurplePuzzle(0);
	while (1)
	{
		//플레이어 이동함수 여기넣었습니다
		if (MODE == 0)
			MovePlayer();
		PrintPlayer();

		DWORD curMsTime = GetTickCount();

		if ((curMsTime - countMsTime) * 0.001f >= 1)
		{
			countMsTime = GetTickCount();
			time++;

			UpdateTimeUI(time);
		}

		if (_kbhit() != 0)
		{
			key = _getch();
			if (key == BUILD &&gameBoardInfo[p.y ][p.x/2-1] != 000)     //0 -> MOVE MODE | 1 -> BUILD MOVE
			{ 
				if (MODE == 1)
				{
					MODE = 0;
					SetCurrentCursorPos(bX, bY);
					DeleteBlock(blockModel[prevblockid]);
				}
				else
				{
					prevblockid = -1;
					MODE = 1;
				}
				if (p.x > gBoardWidth )
					bX = p.x - 4;
				else
					bX = p.x + 4;
				if (p.y > gBoardHeight / 2)
					bY = p.y - 2;
				else
					bY = p.y + 2;

			}
			if(MODE==0)
				ControlCharacter(key);

			switch (key)
			{
			case ESCAPE:
				if (GamePause() == 0)
				{
					ClearConsole();
					return 0;
				}
				else
				{
					countMsTime = GetTickCount();
					DrawGameBoard();
				}
				break;
			case KB_N:
				if (page >= 5)
					break;
				page++;
				UserBlockManage();
				break;
			case KB_M:
				if (page <= 1)
					break;
				page--;
				UserBlockManage();
				break;
			}

		}

		if (MODE == 1 && key !=-1 && key !=BUILD)
			BlockBuild(key);

		if (p.isDead)
		{
			ShowGameOver();
			ClearConsole();
			return 0;
		}
		if (StageClear)
		{
			ShowStageClear(time);
			return 1;
		}

		moveAll(&mObjListHead);
		//StatusPrint();
		key = -1;
	}
	freeAll(&mObjListHead);
}
void ShowResult()
{

}
void DrawGameUI()
{
	int statusBoardWidth = 28;
	int statusBoardHeight = gBoardHeight;

	{
		for (int y = 0; y < gBoardHeight + 2; y += gBoardHeight + 1)
			for (int x = 2; x < (gBoardWidth * 2) + statusBoardWidth + 2; x += 2)
			{
				SetCurrentCursorPos(x, y);
				printf("─");
			}

		for (int x = 0; x < (gBoardWidth * 2) + 3; x += (gBoardWidth * 2) + 2)
			for (int y = 1; y < gBoardHeight + 1; y++)
			{
				SetCurrentCursorPos(x, y);
				printf("│");
			}

		for (int y = 1; y < gBoardHeight + 1; y++)
		{
			SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, y);
			printf("│");
		}
	}

	// 유저블록판그리기
	{
		int x, y;
		x = (gBoardWidth * 2) + 6, y = gBoardHeight - 1;
		for (int z = 1; z <= 3; z++)
		{
			for (int x = (gBoardWidth * 2) + 2; x <= (gBoardWidth * 2) + 30; x += 2)
			{
				SetCurrentCursorPos(x, y);
				if (x == (gBoardWidth * 2) + 2)
					printf("├");
				else if (x == (gBoardWidth * 2) + 30)
					printf("┤");
				else
					printf("─");
			}
			y -= 6;
		}
		for (int y = gBoardHeight - 1; y >= gBoardHeight - 13; y--)
		{
			x = (gBoardWidth * 2) + 16;
			SetCurrentCursorPos(x, y);
			if (y == gBoardHeight - 13)
				printf("┬");
			else if (y == gBoardHeight - 1)
				printf("┴");
			else if (y == gBoardHeight - 7)
				printf("┼");
			else
				printf("│");

		}
	}

	{
		SetCurrentCursorPos(0, 0);
		printf("┌");
		SetCurrentCursorPos((gBoardWidth * 2) + 2, gBoardHeight + 1);
		printf("┴");
		SetCurrentCursorPos((gBoardWidth * 2) + 2, 0);
		printf("┬");
		SetCurrentCursorPos(0, gBoardHeight + 1);
		printf("└");
		SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, 0);
		printf("┐");
		SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, gBoardHeight + 1);
		printf("┘");
	}

	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 2);
	printf("TIME");
	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 3);
	printf("%02d:%02d", 0, 0);
	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 5);
	for (int i = 1; i <= 3; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
		if (i <= SideQuest)
			printf("★");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

	if (p.isExtraLife)
	{
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) + 2, 7);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
		printf("♥ ");
	}
}
void UpdateTimeUI(int time)
{
	int minute = time / 60;
	int second = time % 60;

	SetCurrentCursorPos((gBoardWidth * 2) + 14, 3);
	printf("      ");

	SetCurrentCursorPos((gBoardWidth * 2) + 14, 3);
	printf("%02d:%02d", minute, second);
	//printf("\n x:%d y:%d", p.x, p.y);

}
void UpdateGameUI()
{
	int statusBoardWidth = 28;
	int statusBoardHeight = gBoardHeight;

	//아이템 남은시간 표시
	if (p.defyingGravity == 1 && p.invincibility == 0) {
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2)-6, 4);
		printf("                   ");
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 4);
		printf("중력무시 남은시간:%d", DEFYING_GRAVITY_TIME-(int)duration1);
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 5);
		printf("                   ");
	}
	else if (p.defyingGravity == 0 && p.invincibility == 1) {
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 4);
		printf("                   ");
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 4);
		printf("  무적 남은시간:%d", INVINCIBILITY_TIME - (int)duration2);
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 5);
		printf("                   ");
	}
	else if (p.defyingGravity == 1 && p.invincibility == 1) {
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 4);
		printf("                   ");
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 4);
		printf("중력무시 남은시간:%d", DEFYING_GRAVITY_TIME - (int)duration1);
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 5);
		printf("                   ");
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 5);
		printf("  무적 남은시간:%d", INVINCIBILITY_TIME - (int)duration2);
	}
	else if (p.defyingGravity == 0 && p.invincibility==0) {
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2)-6, 4);
		printf("                   ");
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 5);
		printf("                   ");
	}
	//사이드퀘스트 표시
	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	for (int i = 1; i <= 3; i++)
	{
		if (i <= SideQuest)
		{
			printf("★");
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

	//1회부활 아이템 표시
	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) + 2, 6);
	if (p.isExtraLife)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
		printf("♥");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	}
	else
	{
		printf("  ");
	}
}
int GamePause()
{
	//0 = 메인 메뉴로 / 1 = 게임 재개

	int select = 1;

	for (int y = gBoardHeight / 2 - 6; y <= gBoardHeight / 2 + 6; y++)
	{
		for (int x = (gBoardWidth * 2) / 2 - 15; x <= (gBoardWidth * 2) / 2 + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("  ");
		}
	}

	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y += 10)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("─");
		}
	}
	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y++)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x += 30)
		{
			SetCurrentCursorPos(x, y);
			printf("│");
		}
	}

	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 - 5);
	printf("┌");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 - 5);
	printf("┐");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 + 5);
	printf("┘");
	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 + 5);
	printf("└");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 10, gBoardHeight / 2 - 3);
	printf("G A M E   P A U S E D");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 6, gBoardHeight / 2);
	printf("▶  RESUME");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 9, gBoardHeight / 2 + 2);
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
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 6, gBoardHeight / 2);
					printf("  ");
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 13, gBoardHeight / 2 + 2);
					printf("▶");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 13, gBoardHeight / 2 + 2);
					printf("  ");
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 6, gBoardHeight / 2);
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
void ShowGameOver()
{
	for (int y = gBoardHeight / 2 - 6; y <= gBoardHeight / 2 + 6; y++)
	{
		for (int x = (gBoardWidth * 2) / 2 - 15; x <= (gBoardWidth * 2) / 2 + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("  ");
		}
	}

	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y += 10)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("─");
		}
	}
	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y++)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x += 30)
		{
			SetCurrentCursorPos(x, y);
			printf("│");
		}
	}

	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 - 5);
	printf("┌");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 - 5);
	printf("┐");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 + 5);
	printf("┘");
	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 + 5);
	printf("└");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 8, gBoardHeight / 2 - 3);
	printf("G A M E   O V E R !");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 8, gBoardHeight / 2 + 2);
	printf("PRESS ANY KEY TO");
	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 9, gBoardHeight / 2 + 3);
	printf("RETURN TO MAIN MENU");

	Sleep(1500);

	while (1)
	{
		if (_kbhit() != 0)
		{
			break;
		}
	}

	return 0;
}
void ShowStageClear(int time)
{
	for (int y = gBoardHeight / 2 - 6; y <= gBoardHeight / 2 + 6; y++)
	{
		for (int x = (gBoardWidth * 2) / 2 - 15; x <= (gBoardWidth * 2) / 2 + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("  ");
		}
	}

	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y += 10)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("─");
		}
	}
	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y++)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x += 30)
		{
			SetCurrentCursorPos(x, y);
			printf("│");
		}
	}

	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 - 5);
	printf("┌");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 - 5);
	printf("┐");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 + 5);
	printf("┘");
	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 + 5);
	printf("└");

	//시간보다 빨리 클리어하면 빨리 클리어 한 만큼 추가 점수, 늦게 클리어하면 늦게 클리어 한 만큼 감소
	int additional = time > StageClearTime ? (StageClearTime - time) * 100 : ((StageClearTime - time) / (float)StageClearTime) * 3000;
	//감소된 시간이 20000 이상일경우 보정
	additional = additional < -25000 ? -25000 : additional;
	//별 1개당 2500 추가, 블럭 1개 아낄때마다 500점
	additional = additional + (SideQuest * 2500) + (CurrentUserBlock * 500);
	score = 22500 + additional + (SideQuest * 2500);

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 10, gBoardHeight / 2 - 3);
	printf("S T A G E   C L E A R!");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 10, gBoardHeight / 2 - 1);
	printf("SCORE");
	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 10, gBoardHeight / 2);
	printf("%d", score);

	SetCurrentCursorPos((gBoardWidth * 2) / 2 + 6, gBoardHeight / 2 - 1);
	printf("GRADE");
	SetCurrentCursorPos((gBoardWidth * 2) / 2 + 6, gBoardHeight / 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	for (int i = 1; i <= 3; i++)
		if (score > i * 10000)
			printf("★");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 8, gBoardHeight / 2 + 2);
	printf("PRESS ANY KEY TO");
	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 9, gBoardHeight / 2 + 3);
	printf("MOVE TO NEXT STAGE");

	

	StageNumber++;


	Sleep(1500);

	while (1)
	{
		if (_kbhit() != 0)
		{
			ClearConsole();
			ClearGameBoard();
			break;
		}
	}
	BlockAllocator();
	return 0;
}
void ClearConsole()
{
	for (int y = 0; y < MAX_HEIGHT; y++)
	{
		for (int x = 0; x < MAX_WIDTH; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("  ");
		}
	}
}


//PLAYER
void InitPlayer()
{
	p.x = 4;
	p.y = 10;
	p.t_jump = 2;
	p.isDead = 0;
	p.isExtraLife = 0;
	p.defyingGravity = 0;
	p.invincibility = 0;
}
void ControlCharacter(int key)
{

	switch (key)
	{
	case LARROW:
	case RARROW:
	case UARROW:
	case DARROW:
		break;
	case BUILD:
		break;
	}


}
int DetectCollisionForPlayer(int x, int y)
{
	//플레이어가 움직이고싶은 좌표 x, y를 인자로 받음
	//만약 닿았을때 죽는 오브젝트와 충돌하면 -1를 리턴함
	//닿아도 움직임에 영향이 없는(통과하거나 먹는 오브젝트면) 0을 리턴함
	//닿아도 죽지는 않지만 움직임이 막히면 1을 리턴함

	int colID = gameBoardInfo[y - 1][x - 1] / 100;

	if ((x < 1 || x >= gBoardWidth+2) || (y < 1 || y >= gBoardHeight))
		return 1;
	if (colID == 1)
	{
		int type = gameBoardInfo[y - 1][x - 1] / 10 % 10;
		if (type == 2)
		{
			drawPurplePuzzle(1);
		}
	}
	if (colID == 3)
		return -1;
	else if (colID == 0 || colID == 7 || colID == 8)
	{
		int type = gameBoardInfo[y - 1][x - 1] / 10 % 10;

		//ITEM
		if (colID == 7)
		{
			switch (type)
			{
			case 1: //중력무시
				p.defyingGravity = 1;
				start1 = clock();
				break;
			case 2: //무적
				p.invincibility = 1;
				start2 = clock();
				break;
			case 3:
				p.isExtraLife = 1;
				UpdateGameUI();
				break;
			}
		}
		//POINT
		else if (colID == 8)
		{
			switch (type)
			{
			case 1:
				SideQuest++;
				UpdateGameUI();
				break;
			case 3:
				StageClear = 1;
				break;
			}
		}

		return 0;
	}
	else
		return 1;
}
int MovePlayer()
{
	int rtn;
	if (p.defyingGravity == 0 && p.invincibility == 0) {
		UpdateGameUI();
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 &&
			p.x > 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 - 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 왼쪽으로 1칸 이동
				p.x -= 2;
			}
			else if (rtn == -1)
				DiePlayer();

		}
		//오른쪽키 누를때
		//조건:캐릭터위치가 맵 안쪽에있고 &&
		//게임보드인포상 오른쪽 값이 0일때
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 오른쪽으로 1칸 이동
				p.x += 2;
			}
			else if (rtn == -1)
				DiePlayer();
		}

		//바닥 Y값(바닥까지 캐릭터가 떨어지게 하기위해 값 지정)
		int floor_Y = gBoardHeight + 1;
		if (GetAsyncKeyState(VK_UP) & 0x8000
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//캐릭터의 t_jump값을 0으로만들음
			if (rtn == 1)
				p.t_jump = 0;
			else if (rtn == -1)
				DiePlayer();
		}

		SetCurrentCursorPos(p.x, p.y);
		//캐릭터가 지나간자리를 공백으로만들기위해(점프했을때)
		printf("  ");
		//캐릭터가 지나간자리 게임보드인포를 0으로 만들음(점프했을때)
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;

		//점프
		//캐릭터가 맵 바닥보다 높이 있고 
		//&& 캐릭터 t_jump 가 2보다 크고
		//&& 캐릭터 아래 게임보드인포값이 0일때

		//아래
		if (p.y < floor_Y
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//캐릭터 아래로 내리기
			if (rtn == 0)
				p.y += 1;
			else if (rtn == -1)
				DiePlayer();
		}

		//캐릭터 t_jump 가 2보다작을때(up을 눌러서 t_jump가 0이됐을때)
		if (p.t_jump < 2) {
			//조건:캐릭터위에 벽이있지않으면(위쪽 충돌처리)
			//&&캐릭터가 맵 위쪽 넘어가지않게
			if (p.y > 1)
			{
				rtn = DetectCollisionForPlayer(p.x / 2, p.y - 1);

				if (rtn == 0)
					p.y -= 1;
				//캐릭터 위로 올리기
				//floor(반올림)을 쓰면 캐릭터가 덜 점프하게 할 수있는데 반응속도가 느려짐
				//p.y -= floor(p.t_jump);
			}
			//점프를 그만하게하기위한 t_jump값 조정
			p.t_jump += 0.6;

		}
	}
	//중력무시아이템 먹었을때
	else if (p.defyingGravity == 1 && p.invincibility == 0) {
		finish1 = clock();
		duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
		UpdateGameUI();
		if (duration1 > DEFYING_GRAVITY_TIME) {
			p.defyingGravity = 0;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 &&
			p.x > 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 - 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 왼쪽으로 1칸 이동
				p.x -= 2;
			}
			else if (rtn == -1)
				DiePlayer();

		}
		//오른쪽키 누를때
		//조건:캐릭터위치가 맵 안쪽에있고 &&
		//게임보드인포상 오른쪽 값이 0일때
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 오른쪽으로 1칸 이동
				p.x += 2;
			}
			else if (rtn == -1)
				DiePlayer();
		}

		//바닥 Y값(바닥까지 캐릭터가 떨어지게 하기위해 값 지정)
		int floor_Y = gBoardHeight + 1;
		//위에 키 누를때
		//조건:캐릭터 t_jump값이 2보다 클때 &&
		//캐릭터 아래 게임보드 인포값이 2 일때
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y-1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 오른쪽으로 1칸 이동
				p.y -= 1;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y+1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 오른쪽으로 1칸 이동
				p.y += 1;
			}
		}
	}
	//무적아이템 먹었을때
	else if (p.defyingGravity == 0 && p.invincibility == 1) {
		finish2 = clock();
		duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
		UpdateGameUI();
		if (duration2 > INVINCIBILITY_TIME) {
			p.invincibility = 0;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 &&
			p.x > 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 - 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 왼쪽으로 1칸 이동
				p.x -= 2;
			}

		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 오른쪽으로 1칸 이동
				p.x += 2;
			}
		}

		//바닥 Y값(바닥까지 캐릭터가 떨어지게 하기위해 값 지정)
		int floor_Y = gBoardHeight + 1;
		if (GetAsyncKeyState(VK_UP) & 0x8000
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//캐릭터의 t_jump값을 0으로만들음
			if (rtn == 1 || rtn == -1)
				p.t_jump = 0;
		}

		SetCurrentCursorPos(p.x, p.y);
		//캐릭터가 지나간자리를 공백으로만들기위해(점프했을때)
		printf("  ");
		//캐릭터가 지나간자리 게임보드인포를 0으로 만들음(점프했을때)
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;

		//점프
		//캐릭터가 맵 바닥보다 높이 있고 
		//&& 캐릭터 t_jump 가 2보다 크고
		//&& 캐릭터 아래 게임보드인포값이 0일때

		//아래
		if (p.y < floor_Y
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//캐릭터 아래로 내리기
			if (rtn == 0)
				p.y += 1;
		}

		//캐릭터 t_jump 가 2보다작을때(up을 눌러서 t_jump가 0이됐을때)
		if (p.t_jump < 2) {
			if (p.y > 1)
			{
				rtn = DetectCollisionForPlayer(p.x / 2, p.y - 1);

				if (rtn == 0)
					p.y -= 1;

			}
			p.t_jump += 0.6;
		}
	}
	//중력무시아이템과 무적아이템 둘다 먹었을때
	else if (p.defyingGravity == 1 && p.invincibility == 1) {
		finish1 = clock();
		finish2 = clock();
		duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
		duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
		UpdateGameUI();
		if (duration1 > DEFYING_GRAVITY_TIME) {
			p.defyingGravity = 0;
		}
		if (duration2 > INVINCIBILITY_TIME) {
			p.invincibility = 0;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 &&
			p.x > 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 - 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 왼쪽으로 1칸 이동
				p.x -= 2;
			}
		}
		//오른쪽키 누를때
		//조건:캐릭터위치가 맵 안쪽에있고 &&
		//게임보드인포상 오른쪽 값이 0일때
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 오른쪽으로 1칸 이동
				p.x += 2;
			}
		}

		//바닥 Y값(바닥까지 캐릭터가 떨어지게 하기위해 값 지정)
		int floor_Y = gBoardHeight + 1;
		//위에 키 누를때
		//조건:캐릭터 t_jump값이 2보다 클때 &&
		//캐릭터 아래 게임보드 인포값이 2 일때
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y - 1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 오른쪽으로 1칸 이동
				p.y -= 1;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 오른쪽으로 1칸 이동
				p.y += 1;
			}
		}
	}
}
void DiePlayer()
{
	if (!p.isExtraLife)
		p.isDead = 1;
	else
	{
		int revive = 0;

		p.isExtraLife = 0;
		UpdateGameUI();
		SetCurrentCursorPos(p.x, p.y);
		printf("  ");
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;

		p.x = StartPosition.X;
		p.y = StartPosition.Y;

		SetCurrentCursorPos(p.x, p.y);
		printf("●");
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 900;
	}
		
}


//MAP
//스테이지 진입전 초기화해줘야 할 정보들
void InitStageInfo()
{
	SideQuest = 0;
	StageClear = 0;
}
int LoadStage(Node* mObjListHead)
{
	InitStageInfo();

	char fileName[15];
	FILE* fp;
	int width, height;

	sprintf(fileName, "stage%d.txt", StageNumber);

	fp = fopen(fileName, "r");
	if (fp == NULL)
		return -1;

	fscanf(fp, "%d %d %d", &width, &height, &StageClearTime);
	gBoardWidth = width;
	gBoardHeight = height;

	for (int y = 0; y < gBoardHeight; y++)
	{
		for (int x = 0; x < gBoardWidth; x++)
		{
			fscanf(fp, "%d", &gameBoardInfo[y][x]);

			mObj obj;
			switch (parseInfo(gameBoardInfo[y][x], 0)) {
			case 4:
				obj.objId = gameBoardInfo[y][x];
				obj.x = x;
				obj.y = y;
				obj.delay = gameBoardInfo[y][x] % 10 * 250;
				addObj(obj, mObjListHead);
				break;
			case 5:
				obj.objId = gameBoardInfo[y][x];
				obj.x = x;
				obj.y = y;
				obj.delay = 800;
				addObj(obj, mObjListHead);
				switch (parseInfo(obj.objId, 2)) {
				case 2:
				case 4:
					gameBoardInfo[y][x + 1] = 500; // <-실행안됨. 수정필요
					gameBoardInfo[y][x - 1] = 500;
					break;
				case 3:
				case 1:
					gameBoardInfo[y + 1][x] = 500; // <-실행안됨. 수정필요
					gameBoardInfo[y - 1][x] = 500; 
					break;
				}
				
				break;
			case 10:
				obj.objId = gameBoardInfo[y][x];
				obj.x = x;
				obj.y = y;
				obj.delay = 0;
				addObj(obj, mObjListHead);
				break;
			}
			
			
		}
	}

	fclose(fp);
}
void DrawGameBoard()
{
	int x, y;
	int cursX, cursY;

	for (y = 1; y <= gBoardHeight; y++)
	{
		for (x = 1; x <= gBoardWidth; x++)
		{
			cursX = x * 2/*+2*/;
			cursY = y/*+1*/;
			SetCurrentCursorPos(cursX, cursY);

			int temp, hundred, ten, one;

			temp = gameBoardInfo[y - 1][x - 1];
			one = temp % 10;
			temp /= 10;
			ten = temp % 10;
			temp /= 10;
			hundred = temp % 10;

			switch (hundred)
			{
			case 0:
				printf("  ");
				break;
			case 1:
				switch (ten) {
				case 0:
					printf("■");
					break;
				case 1:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("■");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("●");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				}
			case 2:
				printf("□");
				break;
			case 3:
				switch (ten)
				{
				case UP:
					printf("▲");
					break;
				case DOWN:
					printf("▼");
					break;
				case RIGHT:
					printf("▶");
					break;
				case LEFT:
					printf("◀");
					break;
				}
				break;
			case 4:
				printf("▣");
				//방향 결정 필요
				break;
			case 5:
				printf("▤");
				//방향, 거리 결정 필요

				break;
			case 6:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
				printf("■");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				//방향 결정 필요
				break;
			case 7:
				switch (ten)
				{
				case 1:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
					printf("♣");    //중력무시
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("♠");    //무적
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 3:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
					printf("♥");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				}
				//사이드퀘스트 가산 필요
				break;
				//아이템 종류 결정 필요
				break;
			case 8:
				switch (ten)
				{
				case 1:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
					printf("★");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					p.x = x * 2;
					p.y = y;
					StartPosition.X = p.x;
					StartPosition.Y = p.y;
					SetCurrentCursorPos(x * 2, y);
					printf("●");
					break;
				case 3:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
					printf("※");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				}
				//사이드퀘스트 가산 필요
				break;
			case 9:
				printf("  ");
				//클리어포인트
				break;
			case 10:
				printf("  ");
				//이동블럭 전환점
				break;
			}
		}
	}
}
void DrawGameBoardPart()
{
	int cursX, cursY;
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);

			if (curPos.X + (x * 2) > gBoardWidth * 2 || curPos.Y + y > gBoardHeight)
				continue;
			int temp, hundred, ten, one;

			temp = gameBoardInfo[curPos.Y + y-1][curPos.X/2+x-1];
			one = temp % 10;
			temp /= 10;
			ten = temp % 10;
			temp /= 10;
			hundred = temp % 10;

			switch (hundred)
			{
			case 0:
				printf("  ");
				break;
			case 1:
				switch (ten) {
				case 0:
					printf("■");
					break;
				case 1:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("■");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("●");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				}
			case 2:
				printf("□");
				break;
			case 3:
				switch (ten)
				{
				case UP:
					printf("▲");
					break;
				case DOWN:
					printf("▼");
					break;
				case RIGHT:
					printf("▶");
					break;
				case LEFT:
					printf("◀");
					break;
				}
				break;
			case 4:
				printf("▣");
				//방향 결정 필요
				break;
			case 5:
				printf("▤");
				//방향, 거리 결정 필요

				break;
			case 6:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
				printf("■");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				//방향 결정 필요
				break;
			case 7:
				switch (ten)
				{
				case 1:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
					printf("♣");    //중력무시
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("♠");    //무적
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 3:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
					printf("♥");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				}
				//사이드퀘스트 가산 필요
				break;
				//아이템 종류 결정 필요
				break;
			case 8:
				switch (ten)
				{
				case 1:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
					printf("★");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					p.x = x * 2;
					p.y = y;
					StartPosition.X = p.x;
					StartPosition.Y = p.y;
					SetCurrentCursorPos(x * 2, y);
					printf("●");
					break;
				case 3:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
					printf("※");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				}
				//사이드퀘스트 가산 필요
				break;
			case 9:
				printf("  ");
				//클리어포인트
				break;
			case 10:
				printf("  ");
				//이동블럭 전환점
				break;
			}
		}
	}
}
void ClearGameBoard()
{
	int x, y;
	int cursX, cursY;

	for (y = 1; y <= gBoardHeight; y++)
	{
		for (x = 1; x <= gBoardWidth; x++)
		{
			gameBoardInfo[y - 1][x - 1] = 0;
		}
	}
}

//BLOCK MANAGE
void UserBlockManage()
{
	int x, y;
	x = (gBoardWidth * 2) + 6, y = gBoardHeight - 11;


	// 화면에 현재 페이지 유저블록 출력
	for (int i = 4 * page - 4; i < 4 * page; i++)
	{
		SetCurrentCursorPos(x, y);

		if (UserBlockID[i] == -1) {
			DeleteAllBlock();
			SetCurrentCursorPos(x, y + 2);
			printf("empty");
		}
		else {
			DeleteAllBlock();
			if (UserBlockID[i] >= 28)
				ShowBlock(blockModel[UserBlockID[i]], BLUE);
			else
				ShowBlock(blockModel[UserBlockID[i]], WHITE);
		}
		SetCurrentCursorPos(x + 2, y + 4);
		printf("-0%d-", i % 4 + 1);

		if (i % 2 == 0)
			x += 14;
		else
		{
			x -= 14;
			y += 6;
		}

	}
	SetCurrentCursorPos(gBoardWidth * 2 + 4, gBoardHeight);
	printf(" page : %d/%d n:next m:prev", page, 5);
}
void BlockAllocator() // 초기블럭할당자.
{
	//blockID == 28 pipe
	
	switch (StageNumber) 
	{
	case 1:
		for(int i=0;i<MAXUSERBLOCK;i++)
			UserBlockID[i] = -1;
		AddBlock(0);
		AddBlock(4);
		AddBlock(8);
		break;
	case 2:
		AddBlock(12);
		AddBlock(16);
		AddBlock((rand()%28)%4);
		AddBlock((rand() % 28) % 4);
		AddBlock((rand() % 28) % 4);
		break;
	case 3:

		AddBlock(20);
		AddBlock(24);
		AddBlock(28);
		break;
	}
	if (score /* - (30000 * StageNumber)*/ > 30000)
		AddBlock((rand() % 28) % 4);
}
void AddBlock(int blockid)
{
	UserBlockID[CurrentUserBlock] = blockid;
	CurrentUserBlock++;
}
void BlockListUpdate(int UseBlock)
{
	for (int i = UseBlock; i < MAXUSERBLOCK-1; i++)
	{
		UserBlockID[i] = UserBlockID[i + 1];
		if (UserBlockID[i + 1] == -1) {
			UserBlockID[i] = -1;
			break;
		}
	}
	CurrentUserBlock--;
}
void BlockBuild(int key)
{
	// 충돌이 발생하면 빨간색 위치는 자유자재로 움직일수 있고 충돌시 빨강
	// 충돌시 설치 불가능
	int color=WHITE;
	int categori = 1;
	if (UserBlockID[blockid] == 28)
		categori = 2;
	else if (UserBlockID[blockid] == 29)
		categori = 3;
	else if (UserBlockID[blockid] == 30)
		categori = 4;
	else if (UserBlockID[blockid] == 31)
		categori = 5;
	else
		categori = 1;
	SetCurrentCursorPos(prevbX, prevbY);
	switch (key)
		{
		case 49:
			blockid = (page - 1) * 4;
			break;
		case 50:
			blockid = (page - 1) * 4 + 1;
			break;
		case 51:
			blockid = (page - 1) * 4 + 2;
			break;
		case 52:
			blockid = (page - 1) * 4 + 3;
			break;
		case LARROW:
			if(-bX + p.x <=10 && bX>2)
				bX = bX - 2;
			
			break;
		case RARROW:
			if (bX - p.x <= 10 && bX+4<2*gBoardWidth)
				bX = bX + 2;
		
			break;
		case UARROW:
			if (-bY + p.y <= 5 && bY>1)
				bY--;
			
			break;
		case DARROW:
			if (bY - p.y <= 5 && bY+2 <gBoardHeight)
				bY++;
			
			break;
	
		case SPACE:
			if (!(DetectCollisionForBlock(bX, bY, blockModel[prevblockid])))
			{
				UserBlockID[blockid] = -1;
				//BlockListUpdate(blockid);
				
				MakeBlock(blockModel[prevblockid],categori);
				UserBlockManage();
				prevblockid = -1;
				MODE = 0;
				DrawGameBoard();
			}
			break;
		case L_ROTATE:
			UserBlockID[blockid] = 4 * (UserBlockID[blockid] / 4) + (UserBlockID[blockid] + 3) % 4;
			
			break;
		case R_ROTATE:
			UserBlockID[blockid] = 4 * (UserBlockID[blockid] / 4) + (UserBlockID[blockid] + 1) % 4;
			
			break;
		case BUILD:
		
			break;
		}
	
	if (prevblockid != -1)
	{
		DeleteBlock(blockModel[prevblockid]);
		if (collosion_redraw == 1)
		{
			DrawGameBoardPart();
			//DrawGameBoard();
			collosion_redraw = 0;
		}
	}
	SetCurrentCursorPos(bX, bY);
	if (UserBlockID[blockid] >= 28)
		color = BLUE;
	if ((DetectCollisionForBlock(bX, bY, blockModel[UserBlockID[blockid]])))
	{
		ShowBlock(blockModel[UserBlockID[blockid]], LIGHTRED);
		collosion_redraw = 1;
	}
	else
		ShowBlock(blockModel[UserBlockID[blockid]], color);
	color = WHITE;
	if (UserBlockID[blockid] == -1) {
		BlockListUpdate(blockid);
		UserBlockManage();
	}
	prevbX = bX, prevbY = bY;
	prevblockid = UserBlockID[blockid];
	
}
void ShowBlock(char blockInfo[4][4],int color)
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
				printf("■");
			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
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
				printf("  ");
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}
void MakeBlock(char blockInfo[4][4],int blockCategori)
{
	//1 기본블럭 2~5 파이프블럭 6~ ?
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	curPos.X = curPos.X - 2;
	curPos.Y--;
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
				if (blockInfo[y][x] == 1)
				{

					if (blockCategori == 2)
						gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 610;
					else if (blockCategori == 3)
						gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 620;
					else if (blockCategori == 4)
						gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 630;
					else if (blockCategori == 5)
						gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 640;
					else
						gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 100;

				}
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
int DetectCollisionForBlock(int x, int y, char blockInfo[4][4])
{
	int x1, y1;
	COORD curPos = GetCurrentCursorPos();
	x = x - 2;
	y--;
	for (y1 = 0; y1 < 4; y1++) {
		for (x1 = 0; x1 < 4; x1++) {
			if (blockInfo[y1][x1] == 1 && gameBoardInfo[y1 + y][x1 + x / 2] != 000)
				return 1;
			
		}
	}
	return 0;
}
//PURPLE
void drawPurplePuzzle(int collosion) {

	int x = 0, y = 0;
	if (StageNumber == 3)
	{
		if (collosion == 1)
		{
			if (purple == 1)
				purple = 0;
			else
				purple = 1;
		}
		if (purple == 1)
		{
			x = 7, y = 0;
			for (; y <= 4; y++)
				gameBoardInfo[y][x] = 110;
			gameBoardInfo[y - 1][x - 2] = 120;

			x = 30, y = 0;
			gameBoardInfo[33][35] = 0;
			for (x = 21; x <= 31; x++)
				for (y = 2; y <= 4; y++)
					gameBoardInfo[y][x] = 0;
		}
		else
		{
			x = 7, y = 0;
			for (; y <= 4; y++)
				gameBoardInfo[y][x] = 0;

			gameBoardInfo[y - 1][x - 2] = 0;

			gameBoardInfo[33][35] = 120;
			x = 30, y = 0;
			for (x = 21; x <= 31; x++)
				for (y = 2; y <= 4; y++)
					gameBoardInfo[y][x] = 110;
		}
		DrawGameBoard();
	}
}

//OBJECTS
int detectCollision(int x, int y) { // 이동블럭 충돌체크까지 (이동블럭의 중심 제외 나머지 부분은 일반블럭과 동일한 100 리턴)
	if (detectCollisionMovingBlocks(x, y) == 1)
	{
		return 100;
	}
	return(gameBoardInfo[y][x]);
}

int detectCollisionInDirection(int x, int y, int direction) 
{
	switch (direction) 
	{
	case 1:
		return(detectCollision(x, y - 1));
	case 2:
		return(detectCollision(x + 1, y));
	case 3:
		return(detectCollision(x, y + 1));
	case 4:
		return(detectCollision(x - 1, y));
	default:
		break;
	}
	return 0;
}

int detectCollisionMovingBlocks(int x, int y) { // 해당 좌표에 이동블럭의 중심을 제외한 양 날개부분이 존재하는지 검사, 이 함수 구현의 편의를 위해 gameboard 테두리 한칸씩은 일반 블럭으로 채워넣는것이 좋아보임
	int info = gameBoardInfo[y - 1][x];

	int info_id = parseInfo(info, 0);
	int info_rotation = parseInfo(info, 2);

	if (info_id == 5 && info_rotation % 2 == 1) {
		return 1;
	}

	info = gameBoardInfo[y + 1][x];

	info_id = parseInfo(info, 0);
	info_rotation = parseInfo(info, 2);

	if (info_id == 5 && info_rotation % 2 == 1) {
		return 1;
	}

	info = gameBoardInfo[y][x + 1];

	info_id = parseInfo(info, 0);
	info_rotation = parseInfo(info, 2);

	if (info_id == 5 && info_rotation % 2 == 0) {
		return 1;
	}

	info = gameBoardInfo[y][x - 1];

	info_id = parseInfo(info, 0);
	info_rotation = parseInfo(info, 2);

	if (info_id == 5 && info_rotation % 2 == 0) {
		return 1;
	}

	return 0;

}

// choice : 0 for id, 1 for direction, 2 for rotation
int parseInfo(int info, int choice) { 
	switch (choice) {
	case 0:
		return (info / 100);
	case 1:
		return (info % 100 / 10);
	case 2:
		return (info % 10);
	default:
		return (info / 100);
	}
}

// ******** 이동블럭의 날개는 id 5, 이동방향id 0의 더미블럭으로 구현

void moveAll(Node* headNode) {
	Node* it = headNode;
	DWORD curTick = GetTickCount();
	while (it->nextNode != NULL) 
	{
		int con = 0;
		int colCheck = 0;
		if (it->obj.delay <= (curTick - it->lastUpdateTick)) 
		{
			int col;
			int objType = parseInfo(it->obj.objId, 0);
			int direction = parseInfo(it->obj.objId, 1);
			int rotation = parseInfo(it->obj.objId, 2);
			switch (objType)
			{
			case 3:
				col = detectCollisionInDirection(it->obj.x, it->obj.y, direction);
				if (gameBoardInfo[it->obj.y][it->obj.x] != it->obj.objId) {
					if (gameBoardInfo[it->obj.y][it->obj.x] == 900) {
						DiePlayer();
					}
					con = 1;
					it = it->nextNode;
					removeObjWithoutShowing(it->prevNode);
					break;
				}
				if (col != 0 && col != 1000) 
				{
					colCheck = 1;

					if (col == 900 && p.invincibility == 0)
						DiePlayer();
					else  if(col == 610 || col == 620 || col == 630 || col ==640)// 파이프블럭과 닿는부분
					{
						it = Pipeblock_Changeobj(it,col,direction);
						if ((it->obj.x + 1) * 2 == p.x, it->obj.y + 1 ==p.y)
							DiePlayer();
						colCheck = 0;
					}
					else if (col == 120) // 보라색
					{
						drawPurplePuzzle(1);
					}
				}
				else
				{
					it = thornMoveDirection(it, direction);
				}
				break;
			case 4:
				
				if (detectCollisionInDirection(it->obj.x, it->obj.y, direction) == 0)
				{
					int _x = (direction + 1) % 2 * (direction - 3) * (-1);
					int _y = direction % 2 * (direction - 2);
					mObj shoot = { 300 + direction * 10,it->obj.x + _x,it->obj.y + _y,100 };
					gameBoardInfo[shoot.y][shoot.x] = shoot.objId;
					SetCurrentCursorPos((shoot.x + 1) * 2, shoot.y + 1);
					switch (direction)
					{
					case 1:
						printf("△");
						break;
					case 2:
						printf("▷");
						break;
					case 3:
						printf("▽");
						break;
					case 4:
						printf("◁");
						break;
					}
					addObj(shoot, headNode);
				}
				break;
			case 5:
				if (detectCollisionForMBlock(it->obj.x + (direction + 1) % 2 * (direction - 3) * (-1), it->obj.y + direction % 2 * (direction - 2), rotation) % 10 == -1) {
					if (DetectCollisionForPlayer((p.x + (direction + 1) % 2 * (direction - 3) * (-2)) / 2, p.y + direction % 2 * (direction - 2)) == 0) {
						SetCurrentCursorPos(p.x, p.y);
						//캐릭터가 지나간자리를 공백으로만들기위해
						printf("  ");
						gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
						p.x += (direction + 1) % 2 * (direction - 3) * (-2);
						p.y += direction % 2 * (direction - 2);
					}
					else if (DetectCollisionForPlayer((p.x + (direction + 1) % 2 * (direction - 3) * (-2)) / 2, p.y + direction % 2 * (direction - 2)) == 1) {
						it->obj.objId = objType * 100 + (direction + 2) * 10 + rotation;
						if (parseInfo(it->obj.objId, 1) > 4) {
							it->obj.objId -= 40;
						}
						break;
					}
					else if (DetectCollisionForPlayer((p.x + (direction + 1) % 2 * (direction - 3) * (-2)) / 2, p.y + direction % 2 * (direction - 2)) == -1) {
						DiePlayer();
					}
				}
				if (detectCollisionForMBlock(it->obj.x+ (direction + 1) % 2 * (direction - 3) * (-1), it->obj.y+ direction % 2 * (direction - 2), rotation) <= 0) {
					mBlockMoveDirection(it, direction, rotation);
				}
				else {
					it->obj.objId = objType * 100 + (direction + 2) * 10 + rotation;
					if (parseInfo(it->obj.objId, 1) > 4) {
						it->obj.objId -= 40;
					}
					break;
				}
			case 10:
				if (gameBoardInfo[it->obj.y][it->obj.x] == 0) {
					gameBoardInfo[it->obj.y][it->obj.x] = 1000;
				}

				break;
			}


			it->lastUpdateTick = GetTickCount();
		}
		if (con == 1) {
			continue;
		}
		if ((it->obj.objId != 0 && parseInfo(it->obj.objId, 0) != 4) && ((it->obj.x < 1 || it->obj.x > gBoardWidth) || (it->obj.y < 1 || it->obj.y > gBoardHeight)))
		{
			colCheck = 1;
		}

		it = it->nextNode;
		if (colCheck == 1 && parseInfo(it->prevNode->obj.objId, 0) == 3)
		{
			removeObj(it->prevNode);
		}
	}
}

void addObj(mObj _obj, Node* listHeader) {
	Node* firstNode = listHeader->nextNode;
	Node* node = (Node*)malloc(sizeof(Node));
	node->obj = _obj;
	node->nextNode = firstNode;
	node->prevNode = listHeader;
	firstNode->prevNode = node;
	listHeader->nextNode = node;
	node->lastUpdateTick = GetTickCount();
}

void removeObj(Node* it) {
	Node* prev = it->prevNode;
	Node* next = it->nextNode;
	prev->nextNode = next;
	next->prevNode = prev;
	int x, y;
	x = it->obj.x;
	y = it->obj.y;
	gameBoardInfo[y][x] -= it->obj.objId;
	SetCurrentCursorPos((x + 1) * 2, y + 1);
	printf("  ");
	free(it);
}
void removeObjWithoutShowing(Node* it) {
	Node* prev = it->prevNode;
	Node* next = it->nextNode;
	prev->nextNode = next;
	next->prevNode = prev;
	int x, y;
	x = it->obj.x;
	y = it->obj.y;
	gameBoardInfo[y][x] -= it->obj.objId;
	free(it);
}
Node* Pipeblock_Changeobj(Node* it, int col, int direction)
{
	int x = 0, y = 0;
	switch (col)
	{
	case 610:
		if (direction == 3)
		{
			x = 2, y = 2;
			it->obj.objId = 320;
			direction = 2;
		}
		else
		{
			x = -2, y = -2;
			it->obj.objId = 310;
			direction = 1;
		}break;
	case 620:
		if (direction == 2)
		{
			x = 2, y = -2;
			it->obj.objId = 310;
			direction = 1;
		}
		else {
			x = -2, y = 2;
			it->obj.objId = 340;
			direction = 4;
		}
		break;
	case 630:
		if (direction == 1) {
			x = -2, y = -2;
			it->obj.objId = 340;
			direction = 4;
		}
		else {
			x = 2, y = 2;
			it->obj.objId = 330;
			direction = 3;
		}
		break;
	case 640:
		if (direction == 4)
		{
			x = -2, y = 2;
			it->obj.objId = 330;
			direction = 3;
		}
		else
		{

			x = 2, y = -2;
			it->obj.objId = 320;
			direction = 2;
			break;
		}
	}
		it = changeObjLot(it, x, y, direction);
		return it;
	}

Node* changeObjLot(Node* it, int x, int y,int direction) 
{


	SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
	printf("  ");
	// 현재 위치 공백
	gameBoardInfo[it->obj.y +y][it->obj.x+x] += it->obj.objId;
	gameBoardInfo[it->obj.y][it->obj.x] = 0;
	it->obj.x += x;
	it->obj.y += y;
	SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);

	switch (direction)
	{
	case 1:
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("△");
		break;
	case 2:
		printf("▷");
		break;
	case 3:
		printf("▽");
		break;
	case 4:
		printf("◁");
		break;
	}
	return it;
}
Node* thornMoveDirection(Node* it, int direction) 
{
	
	switch (direction) 
	{
	case 1:
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y);
		printf("△");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		
		printf("  ");
		gameBoardInfo[it->obj.y - 1][it->obj.x] += it->obj.objId;
		gameBoardInfo[it->obj.y][it->obj.x] -= it->obj.objId;
		it->obj.y -= 1;
		break;
	case 2:
		SetCurrentCursorPos((it->obj.x + 2) * 2, it->obj.y + 1);
		printf("▷");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("  ");
		gameBoardInfo[it->obj.y][it->obj.x + 1] += it->obj.objId;
		gameBoardInfo[it->obj.y][it->obj.x] -= it->obj.objId;
		it->obj.x += 1;
		break;
	case 3:
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 2);
		printf("▽");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("  ");
		gameBoardInfo[it->obj.y + 1][it->obj.x] += it->obj.objId;
		gameBoardInfo[it->obj.y][it->obj.x] -= it->obj.objId;
		it->obj.y += 1;
		break;
	case 4:
		SetCurrentCursorPos((it->obj.x) * 2, it->obj.y + 1);
		printf("◁");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("  ");
		gameBoardInfo[it->obj.y][it->obj.x - 1] += it->obj.objId;
		gameBoardInfo[it->obj.y][it->obj.x] -= it->obj.objId;
		it->obj.x -= 1;
		break;
	}

	return it;
}
Node* searchCoord(Node* mObjListHead, int x, int y) {
	Node* it = mObjListHead->nextNode;
	while (it->nextNode != NULL) {
		if (it->obj.x == x && it->obj.y == y) {
			return it;
		}
	}
	return NULL;
}
Node* mBlockMoveDirection(Node* it, int direction, int rotation) {
	int x = it->obj.x - 1;
	int y = it->obj.y - 1;

	int dx = (direction + 1) % 2 * (direction - 3) * (-1);
	int dy = direction % 2 * (direction - 2);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++){
			if (movingBlocks[rotation % 2][i][j] == 1) {
				gameBoardInfo[y + i][x + j] = 0;
				SetCurrentCursorPos((x + j + 1)*2, y + i + 1);
				printf("  ");
			}
		}
	}

	it->obj.x += dx;
	it->obj.y += dy;
	x = it->obj.x - 1;
	y = it->obj.y - 1;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (movingBlocks[rotation % 2][i][j] == 1) {
				gameBoardInfo[y + i][x + j] = 500;
				SetCurrentCursorPos((x + j + 1)*2, y + i + 1);
				printf("▤");
			}
		}
		gameBoardInfo[it->obj.y][it->obj.x] = it->obj.objId;
	}
	

	//이동하는 좌표에 투사체 삭제, 이동하는 좌표에 플레이어 밀어내기 추가필요
}

void freeAll(Node* headNode) {
	Node* it = headNode->nextNode;
	while (it->nextNode != NULL) {
		it = it->nextNode;
		removeObj(it->prevNode);
	}
}

int detectCollisionForMBlock(int x, int y, int rotation) {
	x--;
	y--;
	int check = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++){
			if (gameBoardInfo[y + i][x + j] != 0 && gameBoardInfo[y + i][x + j] != 900 && parseInfo(gameBoardInfo[y + i][x + j], 0) != 3 && parseInfo(gameBoardInfo[y+i][x+j],0) != 5 && movingBlocks[rotation % 2][i][j] == 1) {
				check = 1;
				break;
			}
			if (check != 1 && gameBoardInfo[y + i][x + j] == 900) {
				check = -1;
			}
		}
	}
	return check;
}

void StatusPrint()
{
	//화면아래에 게임보드인포출력
	SetCurrentCursorPos(0, gBoardHeight + 3); //(x,y)
	for (int i = 0; i < gBoardHeight; i++) {
		for (int j = 0; j < gBoardWidth; j++) {
			if (gameBoardInfo[i][j] == 0) {
				printf("000 ");
			}
			else {
				printf("%d ", gameBoardInfo[i][j]);
			}
		}
		printf("\n");
		//SetCurrentCursorPos(65, i + 1);
	}
	//플레이어 x,y출력
	SetCurrentCursorPos(gBoardWidth * 2 + 40, 0);
	printf("p.x:%d p.y:%d p.t_jump:%.1lf", p.x, p.y, p.t_jump);
	SetCurrentCursorPos(gBoardWidth * 2 + 40, 1);
	printf("아래gameBoardInfo[p.y][p.x / 2 - 1]:%d",
		gameBoardInfo[p.y][p.x / 2 - 1]);
	SetCurrentCursorPos(gBoardWidth * 2 + 40, 2);
	printf("오gameBoardInfo[p.y - 1][p.x / 2 - 2]:%d",
		gameBoardInfo[p.y - 1][p.x / 2 - 2]);
	SetCurrentCursorPos(gBoardWidth * 2 + 40, 3);
	printf("왼gameBoardInfo[p.y - 1][p.x / 2]:%d",
		gameBoardInfo[p.y - 1][p.x / 2]);
	SetCurrentCursorPos(gBoardWidth * 2 + 40, 4);
	printf("플레이어gameBoardInfo[p.y - 1][p.x / 2 - 1]:%d",
		gameBoardInfo[p.y - 1][p.x / 2 - 1]);
}
void PrintPlayer()
{
	SetCurrentCursorPos(p.x, p.y);
	printf("%s", "●");
	/*if (p.x % 2 == 0) {
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 900;
	}*/
	gameBoardInfo[p.y - 1][p.x / 2 - 1] = 900;
	Sleep(80);
}