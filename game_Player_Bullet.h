#pragma once
#ifndef GAME_PLAYER_BULLET_H
#define GAME_PLAYER_BULLET_H

#include <DirectXMath.h>


void GamePlayerBullet_Initialize();
void GamePlayerBullet_Finalize();


void GamePlayerBullet_Update(float delta_time);
void GamePlayerBullet_Draw();

void GamePlayerBullet_Create(float x,float y);


#endif // GAME_PLAYER_H