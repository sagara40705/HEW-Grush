#pragma once
#include <Windows.h>
#undef max
#undef min

#include "Vector2.h"

class InputMouse
{
public:
	// 初期化
	static HRESULT Init();

	// 終了処理
	static void    Uninit();

	// 毎フレームの更新
	static void    Update();

    //==================== マウス状態 ====================
    static bool GetButton(int vk);
    static bool GetButtonDown(int vk);
    static bool GetButtonUp(int vk);

	static const Vector2& GetPosition();
	static const Vector2& GetVelocity();
private:
	// 現在 & 前フレームの状態
	static Vector2 s_currPos;
	static Vector2 s_prevPos;
	static BYTE s_currButton[2];
	static BYTE s_prevButton[2];

	// 内部ヘルパ
	static int ButtonIndex(int vk);
};