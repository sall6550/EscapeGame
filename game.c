#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "block.h"
#include "defines.h"
#pragma warning(disable:4996)


int gBoardHeight;
int gBoardWidth;
int speed = 500 / 15;

//���� �̱���
//0:���� 1:��ϡ� 2:������ϡ� 3:���á� 4:���á� 5:���â� 6:���â� 7:����ü�߻��Ϣ� 8:�����̴� ��Ϣ� 9:��������Ϧ� 10:��������Ϧ�
//11:��12:�� 13:���̵�����Ʈ�� 14:�߷¹��� �����ۢ� 15:�� ���� �����ۢ� 16:1ȸ ��Ȱ �����ۢ�

//���� 0:���� 1:���� 2:�ٴ� 3:�÷��̾�
//������ ��:1 / ��:2 / ��:3 / ��:4
//��� ������Ʈ ������ ���ڸ����� �����Ͽ� ����

//*�߰�
//block.h�� enum direction���� ��:1 / ��:2 / ��:3 / ��:4 ���������ϱ�
//�̰� ���ø� ���Ұſ���
//�׸��� define�� �� ������� ���� ���� �־�����



int gameBoardInfo[MAX_HEIGHT][MAX_WIDTH] = { 0 };
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

void Player();

//MAP
int LoadStage();
void DrawGameBoard();

//BLOCKMANAGE
int UserBlockID[MAXUSERBLOCK] = { 0 };
int StageNumber = 1;
int CurrentUserBlock;
int page=1;

void UserBlockManage();
void BlockAllocator();
void ShowBlock(char blockinfo[4][4]);
void DeleteAllBlock();

//MovingObjects
int detectCollisionInDirection(int x, int y, int direction);
int detectCollision(int x, int y);
int parseInfo(int info, int choice);
int detectCollisionMovingBlocks(int x, int y);


//M A I N
int main()
{
	srand(time(NULL));
	RemoveCursor();

	int control;

	while (1)
	{
		if (ShowMainMenu() == 0)
			break;
		

		while (1)
		{
			BlockAllocator();
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
	//1 = ���ӽ��� / 0 = ��������

	SetCurrentCursorPos(50, 10);
	printf("ESCAPE FROM COLLEGE");

	SetCurrentCursorPos(54, 20);
	printf("GAME START");

	SetCurrentCursorPos(57, 22);
	printf("EXIT");

	SetCurrentCursorPos(50, 20);
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
					SetCurrentCursorPos(50, 20);
					printf("  ");
					SetCurrentCursorPos(53, 22);
					printf("��");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos(53, 22);
					printf("  ");
					SetCurrentCursorPos(50, 20);
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
	DWORD startMsTime = GetTickCount();

	LoadStage();
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

			SetCurrentCursorPos((gBoardWidth * 2) + ((gBoardWidth + 8) / 2), 3);
			printf("      ");

			SetCurrentCursorPos((gBoardWidth * 2) + ((gBoardWidth + 8) / 2), 3);
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

		//DrawGameBoard();
		Player();
	}
}
void ShowResult()
{

}
void DrawGameUI()
{
	int statusBoardWidth = gBoardWidth + 8;
	int statusBoardHeight = gBoardHeight;

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

	// ��������Ǳ׸���
	int x, y;
	x = (gBoardWidth * 2) + 6, y = gBoardHeight - 11;
	for (int z = 1; z <= 3; z++) {
		for (int x = (gBoardWidth * 2) + 2; x <= (gBoardWidth * 2) + 30; x += 2)
		{
			SetCurrentCursorPos(x, y);
			if (x == (gBoardWidth * 2) + 2)
				printf("��");
			else if (x == (gBoardWidth * 2) + 30 && y == gBoardHeight + 1)
				printf("��");
			else if (x == (gBoardWidth * 2) + 30)
				printf("��");
			else
				printf("��");
		}
		y += 6;
	}
	for (int y = 7; y < gBoardHeight ; y++) {
		x = (gBoardWidth * 2) + 16;
		SetCurrentCursorPos(x, y);
		if (y == gBoardHeight - 11)
			printf("��");
		else if (y == 19)
			printf("��");
		else if ( y == 13)
			printf("��");
		else
			printf("��");

	}

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

	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 2);
	printf("TIME");
	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 3);
	printf("%02d:%02d", 0, 0);

}
void UpdateGameUI()
{

}
int GamePause()
{
	//0 = ���� �޴��� / 1 = ���� �簳

	int select = 1;

	for (int y = gBoardHeight / 2 - 4; y <= gBoardHeight / 2 + 4; y++)
	{
		for (int x = (gBoardWidth * 2) / 2 - 15; x <= (gBoardWidth * 2) / 2 + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf(" ");
		}
	}

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
	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("  ");
		}
	}
}


//PLAYER
void Player()
{

}


//MAP
int LoadStage()
{
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
				printf("��");
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


//BLOCK MANAGE
void UserBlockManage()
{
	int x, y;
	x = (gBoardWidth * 2) + 6, y = gBoardHeight - 11;
	

	// ȭ�鿡 ���� ������ ������� ���
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
	SetCurrentCursorPos(gBoardWidth * 2 + 4, gBoardHeight);
	printf(" page : %d/%d n:next m:prev", page,5);
}
void BlockAllocator() // �ʱ���Ҵ���.
{
	if (StageNumber == 1)
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
}
void ShowBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("��");
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