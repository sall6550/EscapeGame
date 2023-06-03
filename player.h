#pragma once

typedef struct Player 
{
	int x, y;		//플레이어 좌표
	float t_jump;

	int isDead;
	int isExtraLife;
	//중력무시
	int defyingGravity;
	//무적
	int invincibility;
}Player;
