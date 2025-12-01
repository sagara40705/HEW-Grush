#include "InputKeyboard.h"
#include <cstring> // std::memcpy, std::memset

//================ Static メンバ定義 ==================

BYTE InputKeyboard::s_now[256] = {};
BYTE InputKeyboard::s_prev[256] = {};

//================ 内部ヘルパ ========================

// 今フレームそのキーが押されているか
bool InputKeyboard::IsDownNow(int vk)
{
    if (vk < 0 || vk >= 256) return false;
    return (s_now[vk] & 0x80) != 0;
}

// 前フレームそのキーが押されていたか
bool InputKeyboard::WasDownPrev(int vk)
{
    if (vk < 0 || vk >= 256) return false;
    return (s_prev[vk] & 0x80) != 0;
}

//================ 公開 API ==========================

HRESULT InputKeyboard::Init()
{
    // 一旦ゼロクリア
    std::memset(s_now, 0, sizeof(s_now));
    std::memset(s_prev, 0, sizeof(s_prev));

    // 初回状態を取得
    if (!GetKeyboardState(s_now))
    {
        // 失敗したら両方 0 のままにしておく
        return E_FAIL;
    }

    // prev = now と揃えて、「トリガ無し」の状態から開始
    std::memcpy(s_prev, s_now, sizeof(s_prev));

    return S_OK;
}

void InputKeyboard::Uninit()
{
    // 現状は特に何もしない
}

void InputKeyboard::Update()
{
    // 1) 前フレーム退避
    std::memcpy(s_prev, s_now, sizeof(s_prev));

    // 2) 現在状態取得
    if (!GetKeyboardState(s_now))
    {
        // 失敗したら安全側に倒す（全部 OFF）
        std::memset(s_now, 0, sizeof(s_now));
    }
}

//================ 状態取得 ==========================

bool InputKeyboard::GetKey(int vk)
{
    return IsDownNow(vk);
}

bool InputKeyboard::GetKeyDown(int vk)
{
    return !WasDownPrev(vk) && IsDownNow(vk);
}

bool InputKeyboard::GetKeyUp(int vk)
{
    return WasDownPrev(vk) && !IsDownNow(vk);
}
