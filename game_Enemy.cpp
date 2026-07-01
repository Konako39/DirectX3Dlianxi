#include "game_Enemy.h"
#include <DirectXMath.h>
#include <cstdlib>
#include <ctime>
#include "direct3d.h"
#include "sprite.h"
#include "textureManager.h"
#include "game.h"
#include "Math.h"

//敌人出现计时器
static float EnemySpawnTimer{ 0.0f };
static constexpr float EnemySpawnTime{ 5.0f };

enum EnemyState {

	EnemyState_Spawn,
	EnemyState_Normal,
	EnemyState_Retuen,
	EnemyState_Dead,


};

struct EnemySpriteInfo {
	int textureId;
	float EnemySpeed;	
	float Enemy_WIDTH;
	float Enemy_HEIGHT;
};

struct Enemy
{
	EnemyType Type;
	float position_x;
	float position_y;

	float base_y;
	float create_time;

	bool isDead;

	EnemyState state;

};

void UpdateEnemyPosition(Enemy& enemy, float delta_time);
static constexpr int ENEMY_MAX{ 128 };
static constexpr int ENEMY_TYPE_MAX{ EnemyType_SpeedMonster + 1 };

static Enemy g_Enemies[ENEMY_MAX]{};
static int g_Enemy_Count{ 0 };

static EnemySpriteInfo g_EnemySpriteInfoes[ENEMY_TYPE_MAX]{
	{ TEXTURE_INVALID_ID, 100.0f, 192.0f, 192.0f }, // NormalMonster
	{ TEXTURE_INVALID_ID, 250.0f, 128.0f, 128.0f }, // SpeedMonster
};

static const wchar_t* ENEMY_TEXTURE_FILENAMES[ENEMY_TYPE_MAX]{
	L"Enemy.png",
	L"Enemy.png",
};


static const EnemySpriteInfo& GetEnemySpriteInfo(EnemyType type)
{
	return g_EnemySpriteInfoes[type];
}

void GameEnemy_Initialize()
{
	for (int i = 0; i < ENEMY_TYPE_MAX; ++i) {
		g_EnemySpriteInfoes[i].textureId = Texture_Load(ENEMY_TEXTURE_FILENAMES[i]);
	}

	g_Enemy_Count = 0;
	EnemySpawnTimer = 0.0f;

	std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void GameEnemy_Finalize()
{
	for (int i = 0; i < ENEMY_TYPE_MAX; ++i) {
		Texture_Release(g_EnemySpriteInfoes[i].textureId);
		g_EnemySpriteInfoes[i].textureId = TEXTURE_INVALID_ID;
	}
}

void GameEnemy_Update(float delta_time)
{
	EnemySpawnTimer += delta_time;
	if (EnemySpawnTimer >= EnemySpawnTime) {
		EnemySpawnTimer = 0.0f;

		const int randomType = std::rand() % ENEMY_TYPE_MAX;
		const EnemyType type = static_cast<EnemyType>(randomType);
		//这里传入敌人类型，可在这里随机生成不同类型的敌人,把这个int解释为EnemyType类型

		const EnemySpriteInfo& info = GetEnemySpriteInfo(type);

		const int spawnRange = static_cast<int>(SCREEN_HEIGHT - info.Enemy_HEIGHT);
		const float spawnY = spawnRange > 0 ? static_cast<float>(std::rand() % spawnRange) : 0.0f;
		//翻译一下就是随机出y坐标后，判断是否大于0，如果大于0就随机生成一个y坐标，如果小于0就直接为0
		//static_cast是强制类型转换，std::rand() % spawnRange是随机生成一个0到spawnRange之间的整数	

		GameEnemy_Create(type, static_cast<float>(SCREEN_WIDTH), spawnY);
		//传入类型，屏幕宽度，随机出来的Y
	}



	for (int i = 0; i < g_Enemy_Count; ++i) {
		const EnemySpriteInfo& info = GetEnemySpriteInfo(g_Enemies[i].Type);
		UpdateEnemyPosition(g_Enemies[i], delta_time);
	}


	//这里是总体敌人回收，只要超过屏幕了就回收
	for (int i = g_Enemy_Count - 1; i >= 0; --i) {
		const EnemySpriteInfo& info = GetEnemySpriteInfo(g_Enemies[i].Type);
		if (g_Enemies[i].position_x + info.Enemy_WIDTH <= 0.0f) {//这里+一个宽相当于把坐标点改到敌人贴图的右上角了
			g_Enemies[i].isDead = true;
		}
	}

	for (int i = g_Enemy_Count - 1; i >= 0; --i) {
		if (g_Enemies[i].isDead)
		{
			g_Enemies[i] = g_Enemies[--g_Enemy_Count];
		}
	}



}





void GameEnemy_Draw()
{
	for (int i = 0; i < g_Enemy_Count; ++i) {
		const EnemySpriteInfo& info = GetEnemySpriteInfo(g_Enemies[i].Type);

		Sprite_Draw(
				info.textureId,
				g_Enemies[i].position_x,
				g_Enemies[i].position_y,
				info.Enemy_WIDTH,
				info.Enemy_HEIGHT);

	}
}

void GameEnemy_Create(EnemyType type, float x, float y)
{
	if (g_Enemy_Count >= ENEMY_MAX) {
		return;
	}

	Enemy& r = g_Enemies[g_Enemy_Count];
	r.Type = type;
	r.position_x = x;
	r.position_y = y;
	r.base_y = y;
	r.isDead = false;
	r.create_time = Game_GetAccumulatedTime();

	r.state = EnemyState_Spawn;

	++g_Enemy_Count;
}

//这里定义每个敌人的移动方式
void UpdateEnemyPosition(Enemy& enemy, float delta_time)
{
	const EnemySpriteInfo& info = GetEnemySpriteInfo(enemy.Type);

	switch (enemy.Type)
	{
		case EnemyType_NormalMonster:
		enemy.position_x -= info.EnemySpeed * delta_time;
		break;

		case EnemyType_SpeedMonster:{
			float time = Game_GetAccumulatedTime() - enemy.create_time;

			switch (enemy.state) 
			{
				case EnemyState_Spawn:
				enemy.position_x -= info.EnemySpeed * delta_time;
				if (time >= 3.0f) {
					enemy.state = EnemyState_Normal;
				}
				break;

				case EnemyState_Normal:
					enemy.position_x -= info.EnemySpeed * delta_time;
					enemy.position_y = enemy.base_y + sinf(time * 2) * 200.0f;
					if (time >= 6.0f) {
						enemy.state = EnemyState_Retuen;
					}
					break;

				case EnemyState_Retuen:
					if (time >= 9.0f) {
						enemy.position_x -= info.EnemySpeed * delta_time;
					}
					else
						enemy.position_x += info.EnemySpeed*0.5f * delta_time;
					enemy.position_y = enemy.base_y + sinf(time * 2) * 200.0f;
					break;

				default:
				break;
			}
		}
			break;

	default:
		break;

	}
}