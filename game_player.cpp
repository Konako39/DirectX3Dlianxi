#include "game_player.h"
#include "shader.h"
#include "sprite.h"
#include "textureManager.h"
#include "animationFlip.h"
#include "direct3d.h"
#include "input_keyboard.h"

static int g_TextureId_Player{ TEXTURE_INVALID_ID };
static float g_Player_PositionX{ 10.0f};
static float g_Player_PositionY{ 10.0f};

void GamePlayer_Initialize()
{
	Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
	g_TextureId_Player = Texture_Load(L"Player.png");
}

void GamePlayer_Finalize()
{
	Texture_Finalize();
}

void GamePlayer_Update(float delta_time)
{
	InputKeyboard_Update(delta_time);
	if(InputKeyboard_IsPress(KK_W))
	{
		g_Player_PositionY -= 10.0f;
	}
	if (InputKeyboard_IsPress(KK_A))
	{
		g_Player_PositionX -= 10.0f;
	}
	if (InputKeyboard_IsPress(KK_D))
	{
		g_Player_PositionX += 10.0f;
	}
	if (InputKeyboard_IsPress(KK_S))
	{
		g_Player_PositionY += 10.0f;
	}

	if(g_Player_PositionX>SCREEN_WIDTH-128)
	{
		g_Player_PositionX = SCREEN_WIDTH-128;
	}
	if(g_Player_PositionY>SCREEN_HEIGHT-128)
	{
		g_Player_PositionY = SCREEN_HEIGHT-128;
	}

	if (g_Player_PositionX <0)
	{
		g_Player_PositionX=0;
	}
	if (g_Player_PositionY < 0)
	{
		g_Player_PositionY = 0;
	}
}

void GamePlayer_Draw()
{
	Sprite_Draw(g_TextureId_Player, g_Player_PositionX, g_Player_PositionY,128,128);
}
