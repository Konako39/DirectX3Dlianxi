#include "game_Enemy.h"
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "textureManager.h"


static int g_TextureId_Enemy{ TEXTURE_INVALID_ID };

static int g_Enemy_Count = 0;

//敌人出现计时
float EnemySpawnTimer{ 0 };
float EnemySpawnTime{ 5.0f };


struct Enemy
{
	float position_x, position_y;
};

static constexpr int Enemy_MAX{ 128 };
static Enemy g_Enemies[Enemy_MAX]{};

static constexpr float Enemy_WIDTH{ 192.0f };
static constexpr float Enemy_HEIGHT{ 192.0f };


//敌人移动速度
static float EnemySpeed{ 0 };

//屏幕边缘 敌人移动最大值
static constexpr float Enemy_Move_LIMIT{ 0 - Enemy_WIDTH };



void GameEnemy_Initialize()
{
	g_TextureId_Enemy = Texture_Load(L"enemy.png");
	EnemySpeed = 100.0f;
	g_Enemy_Count = 0;
}

void GameEnemy_Finalize()
{


}

void GameEnemy_Update(float delta_time)
{
	EnemySpawnTimer += delta_time;
	if (EnemySpawnTimer >= EnemySpawnTime) {
		EnemySpawnTimer = 0;
		GameEnemy_Create(SCREEN_WIDTH, rand() % (SCREEN_HEIGHT - (int)Enemy_HEIGHT));
	}

	//敌人一直往右走
	for (int i = 0; i < g_Enemy_Count; ++i) {
		g_Enemies[i].position_x -= EnemySpeed * delta_time;
	}
	//出画面了回收 destory
	for (int i = g_Enemy_Count - 1; i >= 0; --i) {
		if (g_Enemies[i].position_x <= Enemy_Move_LIMIT) {
			g_Enemies[i] = g_Enemies[--g_Enemy_Count];
		}
	}
}

void GameEnemy_Draw()
{
	for (int i = 0; i < g_Enemy_Count; ++i) {
		Sprite_Draw(
			g_TextureId_Enemy,
			g_Enemies[i].position_x,
			g_Enemies[i].position_y,
			Enemy_WIDTH,
			Enemy_HEIGHT
		);
	}
}

void GameEnemy_Create(float x, float y)
{
	if (g_Enemy_Count >= Enemy_MAX) {
		return;
	}

	Enemy& r = g_Enemies[g_Enemy_Count];
	r.position_x = x;
	r.position_y = y;
	++g_Enemy_Count;
}
