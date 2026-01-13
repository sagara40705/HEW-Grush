// InputMouse.cpp
#include "InputMouse.h"

#include <Windows.h>
#include <cstring>

// 静的メンバ定義
Vector2 InputMouse::s_currPos = { 0.0f, 0.0f };
Vector2 InputMouse::s_prevPos = { 0.0f, 0.0f };
BYTE    InputMouse::s_currButton[2] = { 0, 0 };
BYTE    InputMouse::s_prevButton[2] = { 0, 0 };

int InputMouse::ButtonIndex(int vk)
{
    switch (vk)
    {
    case VK_LBUTTON: return 0;
    case VK_RBUTTON: return 1;
    default:         return -1; // 未対応
    }
}

// 初期化
HRESULT InputMouse::Init()
{
    // ボタン状態リセット
    s_currButton[0] = s_currButton[1] = 0;
    s_prevButton[0] = s_prevButton[1] = 0;

    // 現在のマウス位置を取得して、前/今をそろえておく
    POINT p = { 0, 0 };
    if (GetCursorPos(&p))
    {
        // ゲームが単一ウインドウ前提ならこれで十分
        HWND hWnd = GetActiveWindow();
        if (hWnd)
        {
            ScreenToClient(hWnd, &p);
        }
        s_currPos.x = static_cast<float>(p.x);
        s_currPos.y = static_cast<float>(p.y);
        s_prevPos = s_currPos;
    }
    else
    {
        s_currPos = { 0.0f, 0.0f };
        s_prevPos = s_currPos;
    }

    return S_OK;
}

// 終了処理（特に何もすることはないが形だけ）
void InputMouse::Uninit()
{
    // 必要ならここで何か解放
}

// 毎フレーム更新
void InputMouse::Update()
{
    // 前フレームの状態を保存
    s_prevPos = s_currPos;
    std::memcpy(s_prevButton, s_currButton, sizeof(s_currButton));

    // 位置更新
    POINT p = { 0, 0 };
    if (GetCursorPos(&p))
    {
        HWND hWnd = GetActiveWindow();
        if (hWnd)
        {
            ScreenToClient(hWnd, &p);
        }
        s_currPos.x = static_cast<float>(p.x);
        s_currPos.y = static_cast<float>(p.y);
    }

    // ボタン状態更新（押されていれば 1, そうでなければ 0）
    s_currButton[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
    s_currButton[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 1 : 0;
}

//==================== マウス状態参照 ====================

bool InputMouse::GetButton(int vk)
{
    const int idx = ButtonIndex(vk);
    if (idx < 0) return false;
    return s_currButton[idx] != 0;
}

bool InputMouse::GetButtonDown(int vk)
{
    const int idx = ButtonIndex(vk);
    if (idx < 0) return false;
    return (s_currButton[idx] != 0) && (s_prevButton[idx] == 0);
}

bool InputMouse::GetButtonUp(int vk)
{
    const int idx = ButtonIndex(vk);
    if (idx < 0) return false;
    return (s_currButton[idx] == 0) && (s_prevButton[idx] != 0);
}

const Vector2& InputMouse::GetPosition()
{
    return s_currPos;
}

const Vector2& InputMouse::GetVelocity()
{
    // 毎回差分から計算して返す（一時バッファに保持）
    static Vector2 s_velocity = Vector2::zero();
    s_velocity.x = s_currPos.x - s_prevPos.x;
    s_velocity.y = s_currPos.y - s_prevPos.y;
    return s_velocity;
}
