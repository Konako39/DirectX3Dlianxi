#pragma once
#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

void GamePlayer_Initialize();
void GamePlayer_Finalize();


void GamePlayer_Update(float delta_time);
void GamePlayer_Draw();

#endif // GAME_PLAYER_H