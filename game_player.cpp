#include "game_player.h"
#include "shader.h"
#include "sprite.h"
#include "textureManager.h"
#include "animationFlip.h"
#include "direct3d.h"
#include "input_keyboard.h"
#include <DirectXMath.h>
#include "game_Player_Bullet.h"
using namespace DirectX;

static int g_TextureId_Player{ TEXTURE_INVALID_ID };
static XMFLOAT2 g_PlayerPosition{};

static float g_Player_MoveSpeed{};
static float g_Player_TextureSize{};

static float ShortSpeed{};

void GamePlayer_Initialize(float startX, float startY)
{
	g_TextureId_Player = Texture_Load(L"Player.png");
	g_PlayerPosition = { startX, startY };
	g_Player_MoveSpeed = 500.0f;
	g_Player_TextureSize = 128.0f;
	ShortSpeed = 0.5f;
}

void GamePlayer_Finalize()
{
	Texture_Finalize();
}

void GamePlayer_Update(float delta_time)
{
	//计算方向，防止斜着速度快
	XMFLOAT2 direction{ 0.0f, 0.0f};

	//读取键盘传入向量值
	InputKeyboard_Update(delta_time);
	if(InputKeyboard_IsPress(KK_W))
	{
		direction.y = -1.0f;

	}
	if (InputKeyboard_IsPress(KK_A))
	{
		direction.x = -1.0f;
	}
	if (InputKeyboard_IsPress(KK_D))
	{
		direction.x = 1.0f;
	}
	if (InputKeyboard_IsPress(KK_S))
	{
		direction.y = 1.0f;
	}

	//存为XMVECTOR velocity是direction方向向量
	XMVECTOR velocity = XMLoadFloat2(&direction);

	//这里是计算向量长度的平方 一步有多长 如果 1 1就是2了
	//计算是否为0 不为0就normalize 他计算是除一个数所以不能传0进去
	if (XMVectorGetX(XMVector2LengthSq(velocity)) != 0.0f) {
	
		//方向变为单位向量
		velocity = XMVector2Normalize(velocity);
		velocity *= g_Player_MoveSpeed * delta_time;

		XMStoreFloat2(&g_PlayerPosition,XMLoadFloat2(&g_PlayerPosition)+velocity);
		//把velocity的坐标值存储到Positon里面
		//读取出Position，然后加上移动量，再存储回去
	} 



	//不超过屏幕
	if(g_PlayerPosition.x>SCREEN_WIDTH- g_Player_TextureSize)
	{
		g_PlayerPosition.x = SCREEN_WIDTH- g_Player_TextureSize;
	}
	if(g_PlayerPosition.y>SCREEN_HEIGHT- g_Player_TextureSize)
	{
		g_PlayerPosition.y = SCREEN_HEIGHT- g_Player_TextureSize;
	}

	if (g_PlayerPosition.x <0)
	{
		g_PlayerPosition.x =0;
	}
	if (g_PlayerPosition.y < 0)
	{
		g_PlayerPosition.y = 0;
	}
	//不超过屏幕结束
	//这里clamp也可以 PlayerPositon.x=std::clamp(PlayerPositon.x,0,最大值（SCREEN_WIDTH- g_Player_TextureSize）)这种

	//发射子弹

	if (InputKeyboard_IsRepeat(KK_SPACE,1.0f,ShortSpeed)){
		GamePlayerBullet_Create(
			g_PlayerPosition.x+ g_Player_TextureSize * 0.5f,
			g_PlayerPosition.y+ g_Player_TextureSize * 0.5f);
	}

}

void GamePlayer_Draw()
{
	Sprite_Draw(g_TextureId_Player, g_PlayerPosition.x, g_PlayerPosition.y, g_Player_TextureSize, g_Player_TextureSize);
}
