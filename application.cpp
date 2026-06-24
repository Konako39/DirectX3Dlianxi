#include <Windows.h>
#include "application.h"
#include "direct3d.h"
#include "shader.h"
#include "sprite.h"
#include "textureManager.h"
#include "animationFlip.h"
#include "input_keyboard.h"
#include "input_mouse.h"
#include "game.h"
#include "game_player.h"


bool Application_Initialize(HWND hWnd)
{

	//Direct3D初始化 得先初始化D3D，不然其他初始化不了
	if (!Direct3D_Initialize(hWnd)) {
		return false;
	}
	InputKeyboard_Initialize();
	InputMouse_Initialize(hWnd);
	FlipAnimation_Initialize();
	//Shader初始化 需要着色器来画东西
	if (!Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext())) {
		return false;
	}

	//Texture初始化 需要D3D设备才能把png上传到显卡
	Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());

	if (!Sprite_Initialize()) {
		return false;
	}

	Game_Initialize();
	GamePlayer_Initialize();

	InputMouse_SetVisible(true);//是否显示光标

	return true;
}


void Application_Finalize()
{
	Game_Finalize();
	GamePlayer_Finalize();	
	FlipAnimation_Finalize();
	Sprite_Finalize();
	Shader_Finalize();
	Direct3D_Finalize();
	//先Shader释放再D3D
}

void Application_Update(float delta_time)
{
	InputKeyboard_Update(delta_time);
	InputMouse_Update();

	FlipBookAnimation_Update(delta_time);
	Game_Update(delta_time);
	GamePlayer_Update(delta_time);


}

void Application_Draw(){
	Direct3D_DrawBegin();

	GamePlayer_Draw();

	Direct3D_Present();
}
