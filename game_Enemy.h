#pragma once

#ifndef GAME_ENEMY_H
#define GAME_ENEMY_H

#include <DirectXMath.h>


void GameEnemy_Initialize();
void GameEnemy_Finalize();


void GameEnemy_Update(float delta_time);
void GameEnemy_Draw();

void GameEnemy_Create(float x, float y);


#endif // GAME_ENEMY_H