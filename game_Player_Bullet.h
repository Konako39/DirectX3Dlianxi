#pragma once
#ifndef GAME_PLAYER_BULLET_H
#define GAME_PLAYER_BULLET_H

#include <DirectXMath.h>

#include"collision.h"

void GamePlayerBullet_Initialize();
void GamePlayerBullet_Finalize();


void GamePlayerBullet_Update(float delta_time);
void GamePlayerBullet_Draw();

void GamePlayerBullet_Create(bool strong,float x,float y);

CollisionCircle GamePlayerBullet_GetCollision(int index);


#endif // GAME_PLAYER_H