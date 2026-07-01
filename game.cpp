#include "game.h"
#include "game_player.h"
#include"game_Player_Bullet.h"
#include"game_Enemy.h"

#include "direct3d.h"
#include "sprite.h"


void Game_Initialize()
{
	GamePlayer_Initialize(SCREEN_WIDTH/2-64, SCREEN_HEIGHT/2-64);
	GamePlayerBullet_Initialize();
	GameEnemy_Initialize();
}

void Game_Finalize()
{
	GamePlayer_Finalize();
	GamePlayerBullet_Finalize();
	GameEnemy_Finalize();
}

void Game_Update(float delta_time)
{
	GamePlayer_Update(delta_time);
	GamePlayerBullet_Update(delta_time);
	GameEnemy_Update(delta_time);
}

void Game_Draw()
{
	//后画的在上面

	GameEnemy_Draw();

	GamePlayerBullet_Draw();

	GamePlayer_Draw();
}
