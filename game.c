#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "block.h"
#include "defines.h"
#include "player.h"
#include "objects.h"
#pragma warning(disable:4996)


int gBoardHeight;
int gBoardWidth;
int speed = 500 / 15;


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
void ClearConsole();


//MAP
int gameBoardInfo[MAX_HEIGHT][MAX_WIDTH] = { 0 };
int SideQuest = 0;
void InitStageInfo();
int LoadStage(Node* mObjListHead);
void DrawGameBoard();


//PLAYER
Player p;
void InitPlayer();
void ControlCharacter(int key);
int DetectCollisionForPlayer(int x, int y);
int MovePlayer();


//BLOCKMANAGE
int UserBlockID[MAXUSERBLOCK] = { 0 };
int StageNumber = 1;
int CurrentUserBlock;
int page = 1;
int bX, bY;
int blockid = 0;
int MODE = 0, prevblockid = -1;
int prevbX=0, prevbY = 0;
int collosion_redraw = 0;
void ShowRedBlock(char blockInfo[4][4]);
void BlockBuild(int key);
void UserBlockManage();
void BlockAllocator();
void ShowBlock(char blockinfo[4][4]);
void DeleteBlock(char blockinfo[4][4]);
void DeleteAllBlock();
void MakeBlock(char blockInfo[4][4]);
int DetectCollisionForBlock(int x, int y, char blockInfo[4][4]);

//MovingObjects
int detectCollisionInDirection(int x, int y, int direction);
int detectCollision(int x, int y);
int parseInfo(int info, int choice);
int detectCollisionMovingBlocks(int x, int y);
void moveAll(Node* headNode);
Node* moveDirection(Node* it, int direction);
void addObj(mObj obj, Node* listHeader);
void removeObj(Node* it);
void freeAll(Node* headNode);



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
		BlockAllocator();
		while (1)
		{
			
			
			if (ShowGame() == 0)
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
					MODE = 1;
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
}
void UpdateGameUI()
{
	int statusBoardWidth = 28;
	int statusBoardHeight = gBoardHeight;

	//���̵�����Ʈ ǥ��
	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 5);
	for (int i = 1; i <= 3; i++)
	{
		if (i <= SideQuest)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
			printf("��");
		}
	}

	//1ȸ��Ȱ ������ ǥ��
	if (p.isExtraLife)
	{
		SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) + 2, 7);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
		printf("�� ");
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

	if (colID == 3 /*// colID == �Ѿ�*/)
		return -1;
	else if (colID == 0 || colID == 7 || colID == 8)
		return 0;
	else
		return 1;
}
int MovePlayer()
{
	int rtn;
	//����Ű ������
	//����:ĳ������ġ�� �� ���ʿ��ְ� && 
	//���Ӻ��������� ĳ���� ���� ���� 0 �϶�
	if (GetAsyncKeyState(VK_LEFT) & 0x8000 &&
		p.x > 2 && 
		DetectCollisionForPlayer(p.x / 2 - 1, p.y) != 1)
	{
		rtn = DetectCollisionForPlayer(p.x / 2 - 1, p.y);

		SetCurrentCursorPos(p.x, p.y);
		//ĳ���Ͱ� �������ڸ��� �������θ��������
		printf("  ");
		//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
		//ĳ���� �������� 1ĭ �̵�
		p.x -= 2;

	}
	//������Ű ������
	//����:ĳ������ġ�� �� ���ʿ��ְ� &&
	//���Ӻ��������� ������ ���� 0�϶�
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
		p.x < gBoardWidth * 2 && 
		DetectCollisionForPlayer(p.x / 2 + 1, p.y) != 1) 
	{
		rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

		SetCurrentCursorPos(p.x, p.y);
		//ĳ���Ͱ� �������ڸ��� �������θ��������
		printf("  ");
		//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
		//ĳ���� ���������� 1ĭ �̵�
		p.x += 2;
	}

	//�ٴ� Y��(�ٴڱ��� ĳ���Ͱ� �������� �ϱ����� �� ����)
	int floor_Y = gBoardHeight + 1;
	//���� Ű ������
	//����:ĳ���� t_jump���� 2���� Ŭ�� &&
	//ĳ���� �Ʒ� ���Ӻ��� �������� 2 �϶�
	if (GetAsyncKeyState(VK_UP) & 0x8000
		&& p.t_jump >= 2
		&& gameBoardInfo[p.y][p.x / 2 - 1] != 0) {
		//ĳ������ t_jump���� 0���θ�����
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


	if (p.y < floor_Y
		&& p.t_jump >= 2
		//�Ʒ�
		&& gameBoardInfo[p.y][p.x / 2 - 1] == 0) 
	{
		rtn = DetectCollisionForPlayer(p.x / 2 - 1, p.y);

		p.y += 1;//ĳ���� �Ʒ��� ������
	}

	//ĳ���� t_jump �� 2����������(up�� ������ t_jump�� 0�̵�����)
	if (p.t_jump < 2) {
		//����:ĳ�������� ��������������(���� �浹ó��)
		//&&ĳ���Ͱ� �� ���� �Ѿ���ʰ�
		if (gameBoardInfo[p.y - 2][p.x / 2 - 1] == 0
			&& p.y > 1) {
			p.y -= 1;//ĳ���� ���� �ø���
			//floor(�ݿø�)�� ���� ĳ���Ͱ� �� �����ϰ� �� ���ִµ� �����ӵ��� ������
			//p.y -= floor(p.t_jump);
		}
		//������ �׸��ϰ��ϱ����� t_jump�� ����
		p.t_jump += 0.6;

	}
}


//MAP
//�������� ������ �ʱ�ȭ����� �� ������
void InitStageInfo()
{
	SideQuest = 0;
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

	fscanf(fp, "%d %d", &width, &height);
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
				obj.delay = gameBoardInfo[y][x]%10*100;
				addObj(obj, mObjListHead);
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
				printf("��");
				break;
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
				printf("��");
				//���� ���� �ʿ�
				break;
			case 7:
				printf("��");
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
					SetCurrentCursorPos(x * 2, y);
					printf("��");
					break;
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

			//if (gameBoardInfo[y][x] == 0)
			//{
			//	printf("  ");
			//}
			//else if (gameBoardInfo[y][x] == 1) {
			//	printf("��");
			//}
			//else if (gameBoardInfo[y][x] == 2) { //�ٴ�
			//	printf("==");
			//}
			//else if (gameBoardInfo[y][x] == 3) { 
			//	printf("��");
			//}
			//else { printf("  "); }
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
				printf("��");
				break;
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
				printf("��");
				//���� ���� �ʿ�
				break;
			case 7:
				printf("��");
				//������ ���� ���� �ʿ�
				break;
			case 8:
				switch (ten)
				{
				case 1:
					SetCurrentCursorPos(x * 2, y);
					printf("��");
					break;
				case 2:
					p.x = x * 2;
					p.y = y;
					SetCurrentCursorPos(x * 2, y);
					printf("��");
					break;
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
			ShowBlock(blockModel[UserBlockID[i]],7);
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

		CurrentUserBlock = 0;
		for (int i = 0; i < MAXUSERBLOCK; i++) {
			if (i < 7)
			{
				UserBlockID[i] = i * 4;
				CurrentUserBlock++;
			}
			else
				UserBlockID[i] = -1;
		}
}
void BlockBuild(int key)
{
	// �浹�� �߻��ϸ� ������ ��ġ�� ��������� �����ϼ� �ְ� �浹�� ����
	// �浹�� ��ġ �Ұ���
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
				MakeBlock(blockModel[prevblockid]);
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
			collosion_redraw = 0;
		}
	}
	SetCurrentCursorPos(bX, bY);
	if ((DetectCollisionForBlock(bX, bY, blockModel[UserBlockID[blockid]])))
	{
		ShowBlock(blockModel[UserBlockID[blockid]], 12);
		collosion_redraw = 1;
	}
	else
		ShowBlock(blockModel[UserBlockID[blockid]],7);
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
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
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
void MakeBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	curPos.X = curPos.X - 2;
	curPos.Y--;
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			if (blockInfo[y][x] == 1)
				gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 100;
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


//OBJECTS
int detectCollision(int x, int y) { // �̵��� �浹üũ���� (�̵����� �߽� ���� ������ �κ��� �Ϲݺ��� ������ 100 ����)
	if (detectCollisionMovingBlocks(x, y) == 1) {
		return 100;
	}
	return(gameBoardInfo[y][x]);
}

int detectCollisionInDirection(int x, int y, int direction) {
	switch (direction) {
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

int parseInfo(int info, int choice) { // choice : 0 for id, 1 for direction, 2 for rotation
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

void moveAll(Node* headNode) {
	Node* it = headNode;
	DWORD curTick = GetTickCount();
	while (it->nextNode != NULL) {
		int colCheck = 0;
		if (it->obj.delay <= (curTick - it->lastUpdateTick)) {
			int objType = parseInfo(it->obj.objId, 0);
			int direction = parseInfo(it->obj.objId, 1);
			int rotation = parseInfo(it->obj.objId, 2);
			switch (objType) {
			case 3:
				if (detectCollisionInDirection(it->obj.x, it->obj.y, direction) != 0) { //�Ѿ� �浹ó�� �κ� - �÷��̾� �浹ó�� �߰��ʿ�
					colCheck = 1;
				}
				else {
					it = moveDirection(it, direction);
				}
				break;
			case 4:
				if (detectCollisionInDirection(it->obj.x, it->obj.y, direction) == 0) {
					int _x = (direction + 1) % 2 * (direction - 3) * (-1);
					int _y = direction % 2 * (direction - 2);
					mObj shoot = { 300 + direction * 10,it->obj.x + _x,it->obj.y + _y,100 };
					gameBoardInfo[shoot.y][shoot.x] = shoot.objId;
					SetCurrentCursorPos((shoot.x + 1) * 2, shoot.y + 1);
					switch (direction) {
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
			}
			it->lastUpdateTick = GetTickCount();
		}
		if ((it->obj.objId != 0) && ((it->obj.x <= 1 || it->obj.x > gBoardWidth) || (it->obj.y <= 1 || it->obj.y > gBoardHeight)))
		{
			colCheck = 1;
		}

		it = it->nextNode;
		if (colCheck == 1) {
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
	gameBoardInfo[y][x] = 0;
	SetCurrentCursorPos((x + 1) * 2, y + 1);
	printf("  ");
	free(it);
}

Node* moveDirection(Node* it, int direction) 
{
	
	switch (direction) 
	{
	case 1:
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y);
		printf("��");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("  ");
		gameBoardInfo[it->obj.y - 1][it->obj.x] = gameBoardInfo[it->obj.y][it->obj.x];
		gameBoardInfo[it->obj.y][it->obj.x] = 0;
		it->obj.y -= 1;
		break;
	case 2:
		SetCurrentCursorPos((it->obj.x + 2) * 2, it->obj.y + 1);
		printf("��");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("  ");
		gameBoardInfo[it->obj.y][it->obj.x + 1] = gameBoardInfo[it->obj.y][it->obj.x];
		gameBoardInfo[it->obj.y][it->obj.x] = 0;
		it->obj.x += 1;
		break;
	case 3:
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 2);
		printf("��");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("  ");
		gameBoardInfo[it->obj.y + 1][it->obj.x] = gameBoardInfo[it->obj.y][it->obj.x];
		gameBoardInfo[it->obj.y][it->obj.x] = 0;
		it->obj.y += 1;
		break;
	case 4:
		SetCurrentCursorPos((it->obj.x) * 2, it->obj.y + 1);
		printf("��");
		SetCurrentCursorPos((it->obj.x + 1) * 2, it->obj.y + 1);
		printf("  ");
		gameBoardInfo[it->obj.y][it->obj.x - 1] = gameBoardInfo[it->obj.y][it->obj.x];
		gameBoardInfo[it->obj.y][it->obj.x] = 0;
		it->obj.x -= 1;
		break;
	}

	return it;
}

void freeAll(Node* headNode) {
	Node* it = headNode->nextNode;
	while (it->nextNode != NULL) {
		it = it->nextNode;
		removeObj(it->prevNode);
	}
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