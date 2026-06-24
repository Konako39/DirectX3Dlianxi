/*ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

 WindowsAPIの勉強
 基礎
 
													Author : SongLi
													Date : 2025/05/27
													
ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー*/
#include <Windows.h> 
//windows.h是windows api的头文件，包含了windows api的函数声明和一些常量定义等
#include<SDKDDKVer.h>
//SDKDDKVer.h是windows sdk的头文件，包含了windows sdk的版本信息等
#define WIN32_LEAN_AND_MEAN 
//WIN32_LEAN_AND_MEAN是一个宏定义，告诉windows.h不要包含一些不必要的头文件，从而减少编译时间和可执行文件的大小
#include"direct3d.h"
#include"shader.h"
#include"sprite.h"
#include"textureManager.h"
#include"application.h"
#include"mouse.h"
#include"keyboard.h"


static int g_TextureId0{ TEXTURE_INVALID_ID };
static int g_TextureId1{ TEXTURE_INVALID_ID };
static int g_TextureId2{ TEXTURE_INVALID_ID };

static constexpr char WINDOW_CLASS[] = "GameWindow"; //窗口的类名，注册窗口时需要用到 
static constexpr char TITLE[] = "Direct3D";    //标题

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//WndProc是窗口过程函数，处理窗口消息的回调函数，参数分别是窗口句柄、消息类型、消息参数1、消息参数2
//就是windows把用户的操作传回函数
HDC hdc;
PAINTSTRUCT ps;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE,
	_In_ LPSTR, _In_ int nCmdShow)
	//hInstance: 当前实例的句柄
		//应用上，拿到这个句柄，就可以向windows注册窗口
	//hPrevInstance: 以前实例的句柄，Win32不使用，始终为NULL
	
	//lpCmdLine: 命令行参数,比如Game.exe -width 1920 -height 1080 -server 127.0.0.1
		//就取得到"-width 1920 -height 1080 -server。
		//应用上，比如拿到-dev.那么就开启开发者模式之类的
		
	//nCmdShow: 显示窗口的方式
{
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	//初始化COM

	// DPIスケーリング対応（Per-Monitor v2）
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);


	WNDCLASSEX wcex{}; //定义一个WNDCLASSEX结构体变量wcex，用于注册窗口类
	//下面是对wcex结构体成员的赋值，设置窗口类的属性和行为
    wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style = CS_HREDRAW | CS_VREDRAW; //窗口风格，水平和垂直重绘
    wcex.lpfnWndProc = WndProc;
	//窗口收到信息后，传给WndProc函数处理
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex.lpszMenuName = nullptr; // メニューは作らない 
	wcex.lpszClassName = WINDOW_CLASS;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	RegisterClassEx(&wcex);
	//在这里注册了这个窗口，windows已经知道窗口属性了
	/*メインウィンドウの作成 */
	//HWND表示窗口的句柄，ID，代表这个窗口的唯一标识符

	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // 设置窗口的初始大小
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX), FALSE);
	//调整窗口大小，使客户区为800x600，考虑到窗口边框和标题栏的大小

	const int WINDOW_WIDTH{window_rect.right - window_rect.left };
	const int WINDOW_HEIGHT{window_rect.bottom - window_rect.top };

	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// 计算窗口位置，使其居中显示
	int window_x = (desktop_width - WINDOW_WIDTH) / 2;
	int window_y = (desktop_height - WINDOW_HEIGHT) / 2;

	////////创建窗口！////////
	HWND hWnd = CreateWindow(
		WINDOW_CLASS, 
		TITLE, 
		WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
		window_x, window_y, 
		//生成位置在屏幕中心
		WINDOW_WIDTH,WINDOW_HEIGHT,
		nullptr, nullptr, hInstance, nullptr);


	//Direct3Dの初期化
	if (Application_Initialize(hWnd)) {

	//初期化Direct3D（在Application_Initialize中）
	//传入窗口句柄hWnd，让Direct3D知道在哪个窗口上绘制图形
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/*メッセージループ 创建一个MSG类来存储，不停得到消息*/
	MSG msg;
	//PeekMessage函数从消息队列中检索消息，如果有消息就返回非零值，否则返回零
	do {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { // ウィンドウメッセージが来ていたら 
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else { 
			//Game循环
			Application_Update(0.016f);
			Application_Draw();
		}
	} while (msg.message != WM_QUIT);
	
	}
	Application_Finalize();

	return 0;
}

///////////////////////////////////////
//窗口事件开始
//////////////////////////////////////

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	//RECTは矩形区域构造体，包含left top right bottom


	switch (message)
	{
		case WM_DESTROY: // ウィンドウの破棄メッセージ 
		Direct3D_Finalize();
		PostQuitMessage(0); // WM_QUITメッセージの送信 
		break;

		case WM_CLOSE:
			if(MessageBox(hWnd,"ウィンドウを閉じますか？",
				"確認",MB_OKCANCEL | MB_ICONQUESTION) == IDOK) { 
				DestroyWindow(hWnd);
				
			}
			break;

		case WM_ACTIVATEAPP:
			Mouse_ProcessMessage(message, wParam, lParam);
			Keyboard_ProcessMessage(message, wParam, lParam);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE) {
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			}
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard_ProcessMessage(message, wParam, lParam);
			break;
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			Mouse_ProcessMessage(message, wParam, lParam);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		
	}

	return 0;
}