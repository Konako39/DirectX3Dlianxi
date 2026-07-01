#pragma once
#ifndef GAME_H
#define GAME_H

void Game_Initialize();
void Game_Finalize();
void Game_Update(float delta_time);
void Game_Draw();
float Game_GetAccumulatedTime();

#endif // GAME_H	