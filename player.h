#pragma once

typedef struct Player 
{
	int x, y;		//�÷��̾� ��ǥ
	float t_jump;

	int isDead;
	int isExtraLife;
	//�߷¹���
	int defyingGravity;
	//����
	int invincibility;
}Player;
