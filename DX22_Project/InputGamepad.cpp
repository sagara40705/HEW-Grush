#include "InputGamepad.h"
#include <cstring> // std::memset
#include <cmath>   // std::abs

#pragma comment(lib, "Xinput.lib")

//================ Static メンバ定義 ==================

XINPUT_STATE InputGamepad::s_curr = {};
XINPUT_STATE InputGamepad::s_prev = {};
bool         InputGamepad::s_connected = false;

// デフォルトは XInput 推奨値を採用
SHORT InputGamepad::s_lStickDead = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;   // 7849
SHORT InputGamepad::s_rStickDead = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;  // 8689
BYTE  InputGamepad::s_lTriggerDead = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;     // 30
BYTE  InputGamepad::s_rTriggerDead = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;     // 30

//================ 内部ヘルパ ========================

// SHORT [-32768,32767] を [-1,1] に正規化。dead 内は 0。
float InputGamepad::NormalizeStick(SHORT v, SHORT dead)
{
    const SHORT av = static_cast<SHORT>(std::abs(static_cast<int>(v)));
    if (av <= dead) return 0.0f;

    const float sign = (v >= 0) ? 1.0f : -1.0f;
    const float range = 32767.0f - static_cast<float>(dead);
    float f = (static_cast<float>(av) - static_cast<float>(dead)) / range;
    if (f < 0.0f) f = 0.0f;
    if (f > 1.0f) f = 1.0f;
    return f * sign;
}

// BYTE [0,255] を [0,1] に正規化。dead 内は 0。
float InputGamepad::NormalizeTrigger(BYTE v, BYTE dead)
{
    if (v <= dead) return 0.0f;
    float f = (static_cast<float>(v) - static_cast<float>(dead)) /
        (255.0f - static_cast<float>(dead));
    if (f < 0.0f) f = 0.0f;
    if (f > 1.0f) f = 1.0f;
    return f;
}

//================ 公開 API ==========================

void InputGamepad::Init()
{
    s_connected = false;
    std::memset(&s_curr, 0, sizeof(XINPUT_STATE));
    std::memset(&s_prev, 0, sizeof(XINPUT_STATE));

    // とりあえず一度 Update しておく
    Update();
}

void InputGamepad::Uninit()
{
    // 現状は何も必要ない
}

void InputGamepad::Update()
{
    // 前フレーム退避
    s_prev = s_curr;

    // 常に「操作用パッド = 0 番」を見る
    XINPUT_STATE st{};
    DWORD result = XInputGetState(0, &st);
    if (result == ERROR_SUCCESS)
    {
        s_curr = st;
        s_connected = true;
    }
    else
    {
        // 未接続 or エラー → 入力なし扱い
        std::memset(&s_curr, 0, sizeof(XINPUT_STATE));
        s_connected = false;
    }
}

bool InputGamepad::IsConnected()
{
    return s_connected;
}

//================ ボタン ============================

bool InputGamepad::GetButton(WORD button)
{
    if (!s_connected) return false;
    return (s_curr.Gamepad.wButtons & button) != 0;
}

bool InputGamepad::GetButtonDown(WORD button)
{
    if (!s_connected) return false;

    WORD now = s_curr.Gamepad.wButtons;
    WORD prev = s_prev.Gamepad.wButtons;
    return ((now & button) != 0) && ((prev & button) == 0);
}

bool InputGamepad::GetButtonUp(WORD button)
{
    if (!s_connected) return false;

    WORD now = s_curr.Gamepad.wButtons;
    WORD prev = s_prev.Gamepad.wButtons;
    return ((now & button) == 0) && ((prev & button) != 0);
}

//================ スティック ========================

float InputGamepad::GetLStickX()
{
    if (!s_connected) return 0.0f;
    return NormalizeStick(s_curr.Gamepad.sThumbLX, s_lStickDead);
}

float InputGamepad::GetLStickY()
{
    if (!s_connected) return 0.0f;
    // 上方向を + にするか - にするかはゲーム側の座標系に合わせて変更可
    return NormalizeStick(s_curr.Gamepad.sThumbLY, s_lStickDead);
}

float InputGamepad::GetRStickX()
{
    if (!s_connected) return 0.0f;
    return NormalizeStick(s_curr.Gamepad.sThumbRX, s_rStickDead);
}

float InputGamepad::GetRStickY()
{
    if (!s_connected) return 0.0f;
    return NormalizeStick(s_curr.Gamepad.sThumbRY, s_rStickDead);
}

//================ トリガ =============================

float InputGamepad::GetLTrigger()
{
    if (!s_connected) return 0.0f;
    return NormalizeTrigger(s_curr.Gamepad.bLeftTrigger, s_lTriggerDead);
}

float InputGamepad::GetRTrigger()
{
    if (!s_connected) return 0.0f;
    return NormalizeTrigger(s_curr.Gamepad.bRightTrigger, s_rTriggerDead);
}

//================ ユーティリティ ====================

bool InputGamepad::AnyInputThisFrame()
{
    if (!s_connected) return false;

    const XINPUT_GAMEPAD& gp = s_curr.Gamepad;

    // 1) 何かボタンが押されている
    if (gp.wButtons != 0) return true;

    // 2) スティック
    if (NormalizeStick(gp.sThumbLX, s_lStickDead) != 0.0f) return true;
    if (NormalizeStick(gp.sThumbLY, s_lStickDead) != 0.0f) return true;
    if (NormalizeStick(gp.sThumbRX, s_rStickDead) != 0.0f) return true;
    if (NormalizeStick(gp.sThumbRY, s_rStickDead) != 0.0f) return true;

    // 3) トリガ
    if (NormalizeTrigger(gp.bLeftTrigger, s_lTriggerDead) != 0.0f) return true;
    if (NormalizeTrigger(gp.bRightTrigger, s_rTriggerDead) != 0.0f) return true;

    return false;
}

void InputGamepad::SetDeadZone(SHORT ls, SHORT rs, BYTE lt, BYTE rt)
{
    if (ls >= 0) s_lStickDead = ls;
    if (rs >= 0) s_rStickDead = rs;
    if (lt >= 0) s_lTriggerDead = lt;
    if (rt >= 0) s_rTriggerDead = rt;
}
