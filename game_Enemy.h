#pragma once

#ifndef GAME_ENEMY_H
#define GAME_ENEMY_H

#include <DirectXMath.h>


void GameEnemy_Initialize();
void GameEnemy_Finalize();

enum EnemyType
{
	EnemyType_NormalMonster,
	EnemyType_SpeedMonster,
};


void GameEnemy_Update(float delta_time);
void GameEnemy_Draw();

void GameEnemy_Create(EnemyType type,float x, float y);


#endif // GAME_ENEMY_H