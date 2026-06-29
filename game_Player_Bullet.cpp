#include "game_Player_Bullet.h"
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "textureManager.h"

static int g_TextureId_Bullet{ TEXTURE_INVALID_ID };
struct Bullet
{
	float position_x, position_y;
};
static constexpr float BULLET_WIDTH{ 64.0f };
static constexpr float BULLET_HEIGHT{ 64.0f };

static constexpr int BULLET_MAX{128};
//constexpr在编译阶段就完成
static Bullet g_Bullets[BULLET_MAX]{};
static int g_BulletFireCount = 0;
static float BulletSpeed{ 0 };

//屏幕边缘 子弹移动最大值
static constexpr float Bullet_Move_LIMIT{ SCREEN_WIDTH - BULLET_WIDTH*0.5f };

void GamePlayerBullet_Initialize()
{
	g_TextureId_Bullet = Texture_Load(L"bullet.png");
	g_BulletFireCount = 0;
	BulletSpeed = 800.0f;
}

void GamePlayerBullet_Finalize()
{
}

void GamePlayerBullet_Update(float delta_time)
{
	//子弹一直往右走
	for (int i = 0; i < g_BulletFireCount; ++i) {
		g_Bullets[i].position_x += BulletSpeed * delta_time;
	}
	//出画面了回收 destory
	for (int i = g_BulletFireCount - 1; i >= 0; --i) {
		if (g_Bullets[i].position_x >= Bullet_Move_LIMIT) {
			g_Bullets[i] = g_Bullets[--g_BulletFireCount];
		}
	}
}


void GamePlayerBullet_Create(float x, float y)
{
	if (g_BulletFireCount >= BULLET_MAX) {
		return;
	}

	Bullet& r = g_Bullets[g_BulletFireCount];
	r.position_x = x;
	r.position_y = y;
	++g_BulletFireCount;
}

void GamePlayerBullet_Draw()
{
	for (int i = 0; i < g_BulletFireCount; ++i) {
		Sprite_Draw(
			g_TextureId_Bullet,
			g_Bullets[i].position_x,
			g_Bullets[i].position_y,
			BULLET_WIDTH,
			BULLET_HEIGHT);
	}
}