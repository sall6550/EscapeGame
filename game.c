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

//���� 0:���� 1:���� 2:�ٴ� 3:�÷��̾�
//������ ��:1 / ��:2 / ��:3 / ��:4
//��� ������Ʈ ������ ���ڸ����� �����Ͽ� ����

//*�߰�
//block.h�� enum direction���� ��:1 / ��:2 / ��:3 / ��:4 ���������ϱ�
//�̰� ���ø� ���Ұſ���
//�׸��� define�� �� ������� ���� ���� �־�����


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
Node* Pipeblock_Changeobj(Node* it,int col,int direction);// ��������
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
			
			//���θ޴��� = 0 / �������� Ŭ���� = 1
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
	//1 = ���ӽ��� / 0 = ��������

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
	printf("��");

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
					printf("��");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos(111, 44);
					printf("  ");
					SetCurrentCursorPos(108, 40);
					printf("��");
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
		//�÷��̾� �̵��Լ� ����־����ϴ�
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
				printf("��");
			}

		for (int x = 0; x < (gBoardWidth * 2) + 3; x += (gBoardWidth * 2) + 2)
			for (int y = 1; y < gBoardHeight + 1; y++)
			{
				SetCurrentCursorPos(x, y);
				printf("��");
			}

		for (int y = 1; y < gBoardHeight + 1; y++)
		{
			SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, y);
			printf("��");
		}
	}

	// ��������Ǳ׸���
	{
		int x, y;
		x = (gBoardWidth * 2) + 6, y = gBoardHeight - 1;
		for (int z = 1; z <= 3; z++)
		{
			for (int x = (gBoardWidth * 2) + 2; x <= (gBoardWidth * 2) + 30; x += 2)
			{
				SetCurrentCursorPos(x, y);
				if (x == (gBoardWidth * 2) + 2)
					printf("��");
				else if (x == (gBoardWidth * 2) + 30)
					printf("��");
				else
					printf("��");
			}
			y -= 6;
		}
		for (int y = gBoardHeight - 1; y >= gBoardHeight - 13; y--)
		{
			x = (gBoardWidth * 2) + 16;
			SetCurrentCursorPos(x, y);
			if (y == gBoardHeight - 13)
				printf("��");
			else if (y == gBoardHeight - 1)
				printf("��");
			else if (y == gBoardHeight - 7)
				printf("��");
			else
				printf("��");

		}
	}

	{
		SetCurrentCursorPos(0, 0);
		printf("��");
		SetCurrentCursorPos((gBoardWidth * 2) + 2, gBoardHeight + 1);
		printf("��");
		SetCurrentCursorPos((gBoardWidth * 2) + 2, 0);
		printf("��");
		SetCurrentCursorPos(0, gBoardHeight + 1);
		printf("��");
		SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, 0);
		printf("��");
		SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, gBoardHeight + 1);
		printf("��");
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
			printf("��");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

	if (p.isExtraLife)
	{
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) + 2, 7);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
		printf("�� ");
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

	//������ �����ð� ǥ��
	if (p.defyingGravity == 1 && p.invincibility == 0) {
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2)-6, 4);
		printf("                   ");
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 4);
		printf("�߷¹��� �����ð�:%d", DEFYING_GRAVITY_TIME-(int)duration1);
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 5);
		printf("                   ");
	}
	else if (p.defyingGravity == 0 && p.invincibility == 1) {
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 4);
		printf("                   ");
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 4);
		printf("  ���� �����ð�:%d", INVINCIBILITY_TIME - (int)duration2);
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 5);
		printf("                   ");
	}
	else if (p.defyingGravity == 1 && p.invincibility == 1) {
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 4);
		printf("                   ");
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 4);
		printf("�߷¹��� �����ð�:%d", DEFYING_GRAVITY_TIME - (int)duration1);
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 5);
		printf("                   ");
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 5);
		printf("  ���� �����ð�:%d", INVINCIBILITY_TIME - (int)duration2);
	}
	else if (p.defyingGravity == 0 && p.invincibility==0) {
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2)-6, 4);
		printf("                   ");
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) - 6, 5);
		printf("                   ");
	}
	//���̵�����Ʈ ǥ��
	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	for (int i = 1; i <= 3; i++)
	{
		if (i <= SideQuest)
		{
			printf("��");
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

	//1ȸ��Ȱ ������ ǥ��
	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) + 2, 6);
	if (p.isExtraLife)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
		printf("��");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	}
	else
	{
		printf("  ");
	}
}
int GamePause()
{
	//0 = ���� �޴��� / 1 = ���� �簳

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
			printf("��");
		}
	}
	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y++)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x += 30)
		{
			SetCurrentCursorPos(x, y);
			printf("��");
		}
	}

	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 - 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 - 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 + 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 + 5);
	printf("��");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 10, gBoardHeight / 2 - 3);
	printf("G A M E   P A U S E D");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 6, gBoardHeight / 2);
	printf("��  RESUME");

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
					printf("��");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 13, gBoardHeight / 2 + 2);
					printf("  ");
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 6, gBoardHeight / 2);
					printf("��");
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
			printf("��");
		}
	}
	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y++)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x += 30)
		{
			SetCurrentCursorPos(x, y);
			printf("��");
		}
	}

	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 - 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 - 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 + 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 + 5);
	printf("��");

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
			printf("��");
		}
	}
	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y++)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x += 30)
		{
			SetCurrentCursorPos(x, y);
			printf("��");
		}
	}

	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 - 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 - 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 + 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 + 5);
	printf("��");

	//�ð����� ���� Ŭ�����ϸ� ���� Ŭ���� �� ��ŭ �߰� ����, �ʰ� Ŭ�����ϸ� �ʰ� Ŭ���� �� ��ŭ ����
	int additional = time > StageClearTime ? (StageClearTime - time) * 100 : ((StageClearTime - time) / (float)StageClearTime) * 3000;
	//���ҵ� �ð��� 20000 �̻��ϰ�� ����
	additional = additional < -25000 ? -25000 : additional;
	//�� 1���� 2500 �߰�, �� 1�� �Ƴ������� 500��
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
			printf("��");
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
	//�÷��̾ �����̰���� ��ǥ x, y�� ���ڷ� ����
	//���� ������� �״� ������Ʈ�� �浹�ϸ� -1�� ������
	//��Ƶ� �����ӿ� ������ ����(����ϰų� �Դ� ������Ʈ��) 0�� ������
	//��Ƶ� ������ ������ �������� ������ 1�� ������

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
			case 1: //�߷¹���
				p.defyingGravity = 1;
				start1 = clock();
				break;
			case 2: //����
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
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� �������� 1ĭ �̵�
				p.x -= 2;
			}
			else if (rtn == -1)
				DiePlayer();

		}
		//������Ű ������
		//����:ĳ������ġ�� �� ���ʿ��ְ� &&
		//���Ӻ��������� ������ ���� 0�϶�
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.x += 2;
			}
			else if (rtn == -1)
				DiePlayer();
		}

		//�ٴ� Y��(�ٴڱ��� ĳ���Ͱ� �������� �ϱ����� �� ����)
		int floor_Y = gBoardHeight + 1;
		if (GetAsyncKeyState(VK_UP) & 0x8000
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//ĳ������ t_jump���� 0���θ�����
			if (rtn == 1)
				p.t_jump = 0;
			else if (rtn == -1)
				DiePlayer();
		}

		SetCurrentCursorPos(p.x, p.y);
		//ĳ���Ͱ� �������ڸ��� �������θ��������(����������)
		printf("  ");
		//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������(����������)
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;

		//����
		//ĳ���Ͱ� �� �ٴں��� ���� �ְ� 
		//&& ĳ���� t_jump �� 2���� ũ��
		//&& ĳ���� �Ʒ� ���Ӻ����������� 0�϶�

		//�Ʒ�
		if (p.y < floor_Y
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//ĳ���� �Ʒ��� ������
			if (rtn == 0)
				p.y += 1;
			else if (rtn == -1)
				DiePlayer();
		}

		//ĳ���� t_jump �� 2����������(up�� ������ t_jump�� 0�̵�����)
		if (p.t_jump < 2) {
			//����:ĳ�������� ��������������(���� �浹ó��)
			//&&ĳ���Ͱ� �� ���� �Ѿ���ʰ�
			if (p.y > 1)
			{
				rtn = DetectCollisionForPlayer(p.x / 2, p.y - 1);

				if (rtn == 0)
					p.y -= 1;
				//ĳ���� ���� �ø���
				//floor(�ݿø�)�� ���� ĳ���Ͱ� �� �����ϰ� �� ���ִµ� �����ӵ��� ������
				//p.y -= floor(p.t_jump);
			}
			//������ �׸��ϰ��ϱ����� t_jump�� ����
			p.t_jump += 0.6;

		}
	}
	//�߷¹��þ����� �Ծ�����
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
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� �������� 1ĭ �̵�
				p.x -= 2;
			}
			else if (rtn == -1)
				DiePlayer();

		}
		//������Ű ������
		//����:ĳ������ġ�� �� ���ʿ��ְ� &&
		//���Ӻ��������� ������ ���� 0�϶�
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.x += 2;
			}
			else if (rtn == -1)
				DiePlayer();
		}

		//�ٴ� Y��(�ٴڱ��� ĳ���Ͱ� �������� �ϱ����� �� ����)
		int floor_Y = gBoardHeight + 1;
		//���� Ű ������
		//����:ĳ���� t_jump���� 2���� Ŭ�� &&
		//ĳ���� �Ʒ� ���Ӻ��� �������� 2 �϶�
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y-1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.y -= 1;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y+1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.y += 1;
			}
		}
	}
	//���������� �Ծ�����
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
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� �������� 1ĭ �̵�
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
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.x += 2;
			}
		}

		//�ٴ� Y��(�ٴڱ��� ĳ���Ͱ� �������� �ϱ����� �� ����)
		int floor_Y = gBoardHeight + 1;
		if (GetAsyncKeyState(VK_UP) & 0x8000
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//ĳ������ t_jump���� 0���θ�����
			if (rtn == 1 || rtn == -1)
				p.t_jump = 0;
		}

		SetCurrentCursorPos(p.x, p.y);
		//ĳ���Ͱ� �������ڸ��� �������θ��������(����������)
		printf("  ");
		//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������(����������)
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;

		//����
		//ĳ���Ͱ� �� �ٴں��� ���� �ְ� 
		//&& ĳ���� t_jump �� 2���� ũ��
		//&& ĳ���� �Ʒ� ���Ӻ����������� 0�϶�

		//�Ʒ�
		if (p.y < floor_Y
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//ĳ���� �Ʒ��� ������
			if (rtn == 0)
				p.y += 1;
		}

		//ĳ���� t_jump �� 2����������(up�� ������ t_jump�� 0�̵�����)
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
	//�߷¹��þ����۰� ���������� �Ѵ� �Ծ�����
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
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� �������� 1ĭ �̵�
				p.x -= 2;
			}
		}
		//������Ű ������
		//����:ĳ������ġ�� �� ���ʿ��ְ� &&
		//���Ӻ��������� ������ ���� 0�϶�
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.x += 2;
			}
		}

		//�ٴ� Y��(�ٴڱ��� ĳ���Ͱ� �������� �ϱ����� �� ����)
		int floor_Y = gBoardHeight + 1;
		//���� Ű ������
		//����:ĳ���� t_jump���� 2���� Ŭ�� &&
		//ĳ���� �Ʒ� ���Ӻ��� �������� 2 �϶�
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y - 1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.y -= 1;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
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
		printf("��");
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 900;
	}
		
}


//MAP
//�������� ������ �ʱ�ȭ����� �� ������
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
					gameBoardInfo[y][x + 1] = 500; // <-����ȵ�. �����ʿ�
					gameBoardInfo[y][x - 1] = 500;
					break;
				case 3:
				case 1:
					gameBoardInfo[y + 1][x] = 500; // <-����ȵ�. �����ʿ�
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
					printf("��");
					break;
				case 1:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("��");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("��");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				}
			case 2:
				printf("��");
				break;
			case 3:
				switch (ten)
				{
				case UP:
					printf("��");
					break;
				case DOWN:
					printf("��");
					break;
				case RIGHT:
					printf("��");
					break;
				case LEFT:
					printf("��");
					break;
				}
				break;
			case 4:
				printf("��");
				//���� ���� �ʿ�
				break;
			case 5:
				printf("��");
				//����, �Ÿ� ���� �ʿ�

				break;
			case 6:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
				printf("��");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				//���� ���� �ʿ�
				break;
			case 7:
				switch (ten)
				{
				case 1:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
					printf("��");    //�߷¹���
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("��");    //����
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 3:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
					printf("��");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				}
				//���̵�����Ʈ ���� �ʿ�
				break;
				//������ ���� ���� �ʿ�
				break;
			case 8:
				switch (ten)
				{
				case 1:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
					printf("��");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					p.x = x * 2;
					p.y = y;
					StartPosition.X = p.x;
					StartPosition.Y = p.y;
					SetCurrentCursorPos(x * 2, y);
					printf("��");
					break;
				case 3:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
					printf("��");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				}
				//���̵�����Ʈ ���� �ʿ�
				break;
			case 9:
				printf("  ");
				//Ŭ��������Ʈ
				break;
			case 10:
				printf("  ");
				//�̵��� ��ȯ��
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
					printf("��");
					break;
				case 1:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("��");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("��");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				}
			case 2:
				printf("��");
				break;
			case 3:
				switch (ten)
				{
				case UP:
					printf("��");
					break;
				case DOWN:
					printf("��");
					break;
				case RIGHT:
					printf("��");
					break;
				case LEFT:
					printf("��");
					break;
				}
				break;
			case 4:
				printf("��");
				//���� ���� �ʿ�
				break;
			case 5:
				printf("��");
				//����, �Ÿ� ���� �ʿ�

				break;
			case 6:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
				printf("��");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				//���� ���� �ʿ�
				break;
			case 7:
				switch (ten)
				{
				case 1:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
					printf("��");    //�߷¹���
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PURPLE);
					printf("��");    //����
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 3:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
					printf("��");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				}
				//���̵�����Ʈ ���� �ʿ�
				break;
				//������ ���� ���� �ʿ�
				break;
			case 8:
				switch (ten)
				{
				case 1:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
					printf("��");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
					break;
				case 2:
					p.x = x * 2;
					p.y = y;
					StartPosition.X = p.x;
					StartPosition.Y = p.y;
					SetCurrentCursorPos(x * 2, y);
					printf("��");
					break;
				case 3:
					SetCurrentCursorPos(x * 2, y);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
					printf("��");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				}
				//���̵�����Ʈ ���� �ʿ�
				break;
			case 9:
				printf("  ");
				//Ŭ��������Ʈ
				break;
			case 10:
				printf("  ");
				//�̵��� ��ȯ��
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


	// ȭ�鿡 ���� ������ ������� ���
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
void BlockAllocator() // �ʱ���Ҵ���.
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
	// �浹�� �߻��ϸ� ������ ��ġ�� ��������� �����ϼ� �ְ� �浹�� ����
	// �浹�� ��ġ �Ұ���
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
				printf("��");
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
	//1 �⺻�� 2~5 �������� 6~ ?
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
int detectCollision(int x, int y) { // �̵��� �浹üũ���� (�̵����� �߽� ���� ������ �κ��� �Ϲݺ��� ������ 100 ����)
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

int detectCollisionMovingBlocks(int x, int y) { // �ش� ��ǥ�� �̵����� �߽��� ������ �� �����κ��� �����ϴ��� �˻�, �� �Լ� ������ ���Ǹ� ���� gameboard �׵θ� ��ĭ���� �Ϲ� ������ ä���ִ°��� ���ƺ���
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

// ******** �̵����� ������ id 5, �̵�����id 0�� ���̺����� ����

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
					else  if(col == 610 || col == 620 || col == 630 || col ==640)// ���������� ��ºκ�
					{
						it = Pipeblock_Changeobj(it,col,direction);
						if ((it->obj.x + 1) * 2 == p.x, it->obj.y + 1 ==p.y)
							DiePlayer();
						colCheck = 0;
					}
					else if (col == 120) // �����
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
						printf("��");
						break;
					case 2:
						printf("��");
						break;
					case 3:
						printf("��");
						break;
					case 4:
						printf("��");
						break;
					}
					addObj(shoot, headNode);
				}
				break;
			case 5:
				if (detectCollisionForMBlock(it->obj.x + (direction + 1) % 2 * (direction - 3) * (-1), it->obj.y + direction % 2 * (direction - 2), rotation) % 10 == -1) {
					if (DetectCollisionForPlayer((p.x + (direction + 1) % 2 * (direction - 3) * (-2)) / 2, p.y + direction % 2 * (direction - 2)) == 0) {
						SetCurrentCursorPos(p.x, p.y);
						//ĳ���Ͱ� �������ڸ��� �������θ��������
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
	// ���� ��ġ ����
	gameBoardInfo[it->obj.y +y][it->obj.x+x] += it->obj.objId;
	gameBoardInfo[it->obj.y][it->obj.x] = 0;
	it->obj.x += x;
	it->obj.y += y;
	SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);

	switch (direction)
	{
	case 1:
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("��");
		break;
	case 2:
		printf("��");
		break;
	case 3:
		printf("��");
		break;
	case 4:
		printf("��");
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
		printf("��");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		
		printf("  ");
		gameBoardInfo[it->obj.y - 1][it->obj.x] += it->obj.objId;
		gameBoardInfo[it->obj.y][it->obj.x] -= it->obj.objId;
		it->obj.y -= 1;
		break;
	case 2:
		SetCurrentCursorPos((it->obj.x + 2) * 2, it->obj.y + 1);
		printf("��");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("  ");
		gameBoardInfo[it->obj.y][it->obj.x + 1] += it->obj.objId;
		gameBoardInfo[it->obj.y][it->obj.x] -= it->obj.objId;
		it->obj.x += 1;
		break;
	case 3:
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 2);
		printf("��");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("  ");
		gameBoardInfo[it->obj.y + 1][it->obj.x] += it->obj.objId;
		gameBoardInfo[it->obj.y][it->obj.x] -= it->obj.objId;
		it->obj.y += 1;
		break;
	case 4:
		SetCurrentCursorPos((it->obj.x) * 2, it->obj.y + 1);
		printf("��");
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
				printf("��");
			}
		}
		gameBoardInfo[it->obj.y][it->obj.x] = it->obj.objId;
	}
	

	//�̵��ϴ� ��ǥ�� ����ü ����, �̵��ϴ� ��ǥ�� �÷��̾� �о�� �߰��ʿ�
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
	//ȭ��Ʒ��� ���Ӻ����������
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
	//�÷��̾� x,y���
	SetCurrentCursorPos(gBoardWidth * 2 + 40, 0);
	printf("p.x:%d p.y:%d p.t_jump:%.1lf", p.x, p.y, p.t_jump);
	SetCurrentCursorPos(gBoardWidth * 2 + 40, 1);
	printf("�Ʒ�gameBoardInfo[p.y][p.x / 2 - 1]:%d",
		gameBoardInfo[p.y][p.x / 2 - 1]);
	SetCurrentCursorPos(gBoardWidth * 2 + 40, 2);
	printf("��gameBoardInfo[p.y - 1][p.x / 2 - 2]:%d",
		gameBoardInfo[p.y - 1][p.x / 2 - 2]);
	SetCurrentCursorPos(gBoardWidth * 2 + 40, 3);
	printf("��gameBoardInfo[p.y - 1][p.x / 2]:%d",
		gameBoardInfo[p.y - 1][p.x / 2]);
	SetCurrentCursorPos(gBoardWidth * 2 + 40, 4);
	printf("�÷��̾�gameBoardInfo[p.y - 1][p.x / 2 - 1]:%d",
		gameBoardInfo[p.y - 1][p.x / 2 - 1]);
}
void PrintPlayer()
{
	SetCurrentCursorPos(p.x, p.y);
	printf("%s", "��");
	/*if (p.x % 2 == 0) {
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 900;
	}*/
	gameBoardInfo[p.y - 1][p.x / 2 - 1] = 900;
	Sleep(80);
}