/*==============================================================================

   ƒ}ƒEƒX“ü—Íƒ‰ƒbƒp[ [input_mouse.cpp]
                                                         Author : Youhei Sato
--------------------------------------------------------------------------------

==============================================================================*/
#include "input_mouse.h"
#include <cstring>

// ‘OƒtƒŒ[ƒ€‚Ìƒ}ƒEƒXƒ{ƒ^ƒ“ó‘Ô
static bool gPrevMouseButton[3] = {};

// Œ»İƒtƒŒ[ƒ€‚Ìƒ}ƒEƒXó‘Ô
static Mouse_State gMouseState = {};


// ========== ‰Šú‰» / XV / I—¹ ==========

void InputMouse_Initialize(HWND hWnd)
{
    Mouse_Initialize(hWnd);
    std::memset(&gMouseState, 0, sizeof(gMouseState));
    std::memset(gPrevMouseButton, 0, sizeof(gPrevMouseButton));
}

void InputMouse_Update(void)
{
    // ‘OƒtƒŒ[ƒ€‚Ìƒ{ƒ^ƒ“ó‘Ô‚ğ•Û‘¶
    gPrevMouseButton[MOUSE_BUTTON_LEFT]   = gMouseState.leftButton;
    gPrevMouseButton[MOUSE_BUTTON_RIGHT]  = gMouseState.rightButton;
    gPrevMouseButton[MOUSE_BUTTON_MIDDLE] = gMouseState.middleButton;
	gPrevMouseButton[MOUSE_BUTTON_X1] = gMouseState.xButton1;
	gPrevMouseButton[MOUSE_BUTTON_X2] = gMouseState.xButton2;

    // Œ»İ‚Ìƒ}ƒEƒXó‘Ô‚ğæ“¾
    Mouse_GetState(&gMouseState);

    // ƒXƒNƒ[ƒ‹ƒzƒC[ƒ‹’l‚ÌƒŠƒZƒbƒg‚ğ©“®‰»
    Mouse_ResetScrollWheelValue();
}

void InputMouse_Finalize(void)
{
    Mouse_Finalize();
}


// ========== ÅĞ¶¨´¦Àí ==========

bool InputMouse_IsPress(MouseButton button)
{
    switch (button)
    {
    case MOUSE_BUTTON_LEFT:   return gMouseState.leftButton;
    case MOUSE_BUTTON_RIGHT:  return gMouseState.rightButton;
    case MOUSE_BUTTON_MIDDLE: return gMouseState.middleButton;
	case MOUSE_BUTTON_X1:     return gMouseState.xButton1;
	case MOUSE_BUTTON_X2:     return gMouseState.xButton2;

    default:                  return false;
    }
}

bool InputMouse_IsTrigger(MouseButton button)
{
    if (button < 0 || button >= MOUSE_BUTTON_MAX) return false;
    bool prev = gPrevMouseButton[button];
    bool curr = InputMouse_IsPress(button);
    return (!prev && curr);
}

bool InputMouse_IsRelease(MouseButton button)
{
    if (button < 0 || button >= MOUSE_BUTTON_MAX) return false;
    bool prev = gPrevMouseButton[button];
    bool curr = InputMouse_IsPress(button);
    return (prev && !curr);
}

int InputMouse_GetX(void)
{
    return gMouseState.x;
}

int InputMouse_GetY(void)
{
    return gMouseState.y;
}

int InputMouse_GetScrollWheel(void)
{
    return gMouseState.scrollWheelValue;
}

void InputMouse_SetMode(Mouse_PositionMode mode)
{
    Mouse_SetMode(mode);
}

void InputMouse_SetVisible(bool visible)
{
    Mouse_SetVisible(visible);
}