#include "InputGamepad.h"
#include <cstring>
#include <cmath>

//==================================================
// static member
//==================================================

InputGamepad::Backend InputGamepad::s_backend = InputGamepad::Backend::None;
bool InputGamepad::s_connected = false;

InputGamepad::CommonState InputGamepad::s_curr = {};
InputGamepad::CommonState InputGamepad::s_prev = {};

SHORT InputGamepad::s_lStickDead = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
SHORT InputGamepad::s_rStickDead = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
BYTE  InputGamepad::s_lTriggerDead = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
BYTE  InputGamepad::s_rTriggerDead = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

HWND InputGamepad::s_hwnd = nullptr;
IDirectInput8* InputGamepad::s_di = nullptr;
IDirectInputDevice8* InputGamepad::s_diDevice = nullptr;
GUID                 InputGamepad::s_diGuid = GUID{};
bool                 InputGamepad::s_diFound = false;

//==================================================
// helper
//==================================================

float InputGamepad::NormalizeStick(SHORT v, SHORT dead)
{
    const int av = std::abs(static_cast<int>(v));
    if (av <= dead) return 0.0f;

    const float sign = (v >= 0) ? 1.0f : -1.0f;
    const float range = 32767.0f - static_cast<float>(dead);

    float f = (static_cast<float>(av) - static_cast<float>(dead)) / range;
    if (f < 0.0f) f = 0.0f;
    if (f > 1.0f) f = 1.0f;

    return f * sign;
}

float InputGamepad::NormalizeTrigger(BYTE v, BYTE dead)
{
    if (v <= dead) return 0.0f;

    float f = (static_cast<float>(v) - static_cast<float>(dead)) /
        (255.0f - static_cast<float>(dead));

    if (f < 0.0f) f = 0.0f;
    if (f > 1.0f) f = 1.0f;

    return f;
}

// DirectInput ÇÃ 0Å`65535 Ç XInput ïóÇÃ -32768Å`32767 Ç÷
SHORT InputGamepad::ToSignedAxis(LONG v)
{
    LONG centered = v - 32767;
    if (centered < -32768) centered = -32768;
    if (centered > 32767) centered = 32767;
    return static_cast<SHORT>(centered);
}

//==================================================
// DirectInput enum callback
//==================================================

BOOL CALLBACK InputGamepad::EnumGamepadsCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
    (void)context;
    s_diGuid = instance->guidInstance;
    s_diFound = true;
    return DIENUM_STOP;
}

//==================================================
// DirectInput init / uninit
//==================================================

bool InputGamepad::InitDirectInput(HWND hwnd)
{
    s_hwnd = hwnd;

    HINSTANCE hInst = GetModuleHandle(nullptr);
    HRESULT hr = DirectInput8Create(
        hInst,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        reinterpret_cast<void**>(&s_di),
        nullptr);

    if (FAILED(hr))
    {
        s_di = nullptr;
        return false;
    }

    return true;
}

void InputGamepad::UninitDirectInput()
{
    if (s_diDevice)
    {
        s_diDevice->Unacquire();
        s_diDevice->Release();
        s_diDevice = nullptr;
    }

    if (s_di)
    {
        s_di->Release();
        s_di = nullptr;
    }

    s_diFound = false;
    s_diGuid = GUID{};
}

bool InputGamepad::FindDirectInputGamepad()
{
    if (!s_di) return false;

    s_diFound = false;
    s_diGuid = GUID{};

    HRESULT hr = s_di->EnumDevices(
        DI8DEVCLASS_GAMECTRL,
        EnumGamepadsCallback,
        nullptr,
        DIEDFL_ATTACHEDONLY);

    if (FAILED(hr) || !s_diFound)
    {
        return false;
    }

    return true;
}

bool InputGamepad::CreateDirectInputDevice()
{
    if (!s_di || !s_diFound) return false;

    if (s_diDevice)
    {
        s_diDevice->Unacquire();
        s_diDevice->Release();
        s_diDevice = nullptr;
    }

    HRESULT hr = s_di->CreateDevice(s_diGuid, &s_diDevice, nullptr);
    if (FAILED(hr) || !s_diDevice) return false;

    hr = s_diDevice->SetDataFormat(&c_dfDIJoystick2);
    if (FAILED(hr)) return false;

    hr = s_diDevice->SetCooperativeLevel(
        s_hwnd,
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) return false;

    // é≤ÉåÉìÉWÇ 0Å`65535 Ç…ÇªÇÎÇ¶ÇÈ
    DIPROPRANGE diprg{};
    diprg.diph.dwSize = sizeof(DIPROPRANGE);
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    diprg.diph.dwHow = DIPH_BYOFFSET;
    diprg.lMin = 0;
    diprg.lMax = 65535;

    const DWORD offsets[] =
    {
        DIJOFS_X, DIJOFS_Y, DIJOFS_Z,
        DIJOFS_RX, DIJOFS_RY, DIJOFS_RZ
    };

    for (DWORD ofs : offsets)
    {
        diprg.diph.dwObj = ofs;
        s_diDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
    }

    s_diDevice->Acquire();
    return true;
}

//==================================================
// public
//==================================================

void InputGamepad::Init(HWND hwnd)
{
    s_backend = Backend::None;
    s_connected = false;
    std::memset(&s_curr, 0, sizeof(s_curr));
    std::memset(&s_prev, 0, sizeof(s_prev));

    InitDirectInput(hwnd);
    Update();
}

void InputGamepad::Uninit()
{
    UninitDirectInput();

    s_backend = Backend::None;
    s_connected = false;
    std::memset(&s_curr, 0, sizeof(s_curr));
    std::memset(&s_prev, 0, sizeof(s_prev));
}

void InputGamepad::Update()
{
    s_prev = s_curr;
    s_curr = {};

    // 1. Xbox / XInput ÇóDêÊ
    if (UpdateXInput())
    {
        s_backend = Backend::XInput;
        s_connected = true;
        return;
    }

    // 2. Ç»ÇØÇÍÇŒ DirectInput
    if (UpdateDirectInput())
    {
        s_backend = Backend::DirectInput;
        s_connected = true;
        return;
    }

    s_backend = Backend::None;
    s_connected = false;
}

bool InputGamepad::IsConnected()
{
    return s_connected;
}

//==================================================
// XInput
//==================================================

bool InputGamepad::UpdateXInput()
{
    XINPUT_STATE st{};
    DWORD result = XInputGetState(0, &st);
    if (result != ERROR_SUCCESS)
    {
        return false;
    }

    s_curr.buttons = st.Gamepad.wButtons;
    s_curr.lx = st.Gamepad.sThumbLX;
    s_curr.ly = st.Gamepad.sThumbLY;
    s_curr.rx = st.Gamepad.sThumbRX;
    s_curr.ry = st.Gamepad.sThumbRY;
    s_curr.lt = st.Gamepad.bLeftTrigger;
    s_curr.rt = st.Gamepad.bRightTrigger;

    return true;
}

//==================================================
// DirectInput
//==================================================

WORD InputGamepad::MapDirectInputButtons(const DIJOYSTATE2& js)
{
    WORD buttons = 0;

    auto pressed = [&](int index) -> bool
        {
            if (index < 0 || index >= 128) return false;
            return (js.rgbButtons[index] & 0x80) != 0;
        };

    // DualSense Ç≈ÇÊÇ≠Ç†ÇÈï¿Ç—
    // 0:Å† 1:Å~ 2:Åõ 3:Å¢
    if (pressed(1)) buttons |= XINPUT_GAMEPAD_A; // Å~
    if (pressed(2)) buttons |= XINPUT_GAMEPAD_B; // Åõ
    if (pressed(0)) buttons |= XINPUT_GAMEPAD_X; // Å†
    if (pressed(3)) buttons |= XINPUT_GAMEPAD_Y; // Å¢

    if (pressed(4))  buttons |= XINPUT_GAMEPAD_LEFT_SHOULDER;
    if (pressed(5))  buttons |= XINPUT_GAMEPAD_RIGHT_SHOULDER;
    if (pressed(10)) buttons |= XINPUT_GAMEPAD_LEFT_THUMB;
    if (pressed(11)) buttons |= XINPUT_GAMEPAD_RIGHT_THUMB;
    if (pressed(8))  buttons |= XINPUT_GAMEPAD_BACK;   // Create / Share
    if (pressed(9))  buttons |= XINPUT_GAMEPAD_START;  // Options

    // è\éöÉLÅ[ÇÕ POV
    DWORD pov = js.rgdwPOV[0];
    if (LOWORD(pov) != 0xFFFF)
    {
        if (pov == 0 || pov == 4500 || pov == 31500)
            buttons |= XINPUT_GAMEPAD_DPAD_UP;
        if (pov == 9000 || pov == 4500 || pov == 13500)
            buttons |= XINPUT_GAMEPAD_DPAD_RIGHT;
        if (pov == 18000 || pov == 13500 || pov == 22500)
            buttons |= XINPUT_GAMEPAD_DPAD_DOWN;
        if (pov == 27000 || pov == 22500 || pov == 31500)
            buttons |= XINPUT_GAMEPAD_DPAD_LEFT;
    }

    return buttons;
}

bool InputGamepad::UpdateDirectInput()
{
    if (!s_di) return false;

    if (!s_diDevice)
    {
        if (!FindDirectInputGamepad()) return false;
        if (!CreateDirectInputDevice()) return false;
    }

    HRESULT hr = s_diDevice->Poll();
    if (FAILED(hr))
    {
        hr = s_diDevice->Acquire();
        while (hr == DIERR_INPUTLOST)
        {
            hr = s_diDevice->Acquire();
        }

        if (FAILED(hr))
        {
            s_diDevice->Release();
            s_diDevice = nullptr;
            return false;
        }

        hr = s_diDevice->Poll();
        if (FAILED(hr))
        {
            return false;
        }
    }

    DIJOYSTATE2 js{};
    hr = s_diDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
    if (FAILED(hr))
    {
        s_diDevice->Release();
        s_diDevice = nullptr;
        return false;
    }

    s_curr.buttons = MapDirectInputButtons(js);

    // ç∂ÉXÉeÉBÉbÉN
    s_curr.lx = ToSignedAxis(js.lX);
    s_curr.ly = -ToSignedAxis(js.lY);

    // âEÉXÉeÉBÉbÉN
    // DualSense ÇÕä¬ã´Ç…ÇÊÇ¡Çƒ Z/Rz Ç‹ÇΩÇÕ Rx/Ry ÇégÇ§Ç±Ç∆Ç™Ç†ÇÈ
    // Ç‹Ç∏ Rx/Ry ÇóDêÊÇµÅAñ≥îΩâûÇ»ÇÁ Z/Rz Ç÷êÿÇËë÷Ç¶ÇƒÇ≠ÇæÇ≥Ç¢
    s_curr.rx = ToSignedAxis(js.lRx);
    s_curr.ry = -ToSignedAxis(js.lRy);

    // DirectInput Ç≈ÇÕÉgÉäÉKÇ™ì∆óßÇµÇ»Ç¢èÍçáÇ™ëΩÇ¢ÇÃÇ≈àÍíU 0
    s_curr.lt = 0;
    s_curr.rt = 0;

    return true;
}

//==================================================
// button
//==================================================

bool InputGamepad::GetButton(WORD button)
{
    if (!s_connected) return false;
    return (s_curr.buttons & button) != 0;
}

bool InputGamepad::GetButtonDown(WORD button)
{
    if (!s_connected) return false;
    return ((s_curr.buttons & button) != 0) && ((s_prev.buttons & button) == 0);
}

bool InputGamepad::GetButtonUp(WORD button)
{
    if (!s_connected) return false;
    return ((s_curr.buttons & button) == 0) && ((s_prev.buttons & button) != 0);
}

//==================================================
// stick
//==================================================

float InputGamepad::GetLStickX()
{
    if (!s_connected) return 0.0f;
    return NormalizeStick(s_curr.lx, s_lStickDead);
}

float InputGamepad::GetLStickY()
{
    if (!s_connected) return 0.0f;
    return NormalizeStick(s_curr.ly, s_lStickDead);
}

float InputGamepad::GetRStickX()
{
    if (!s_connected) return 0.0f;
    return NormalizeStick(s_curr.rx, s_rStickDead);
}

float InputGamepad::GetRStickY()
{
    if (!s_connected) return 0.0f;
    return NormalizeStick(s_curr.ry, s_rStickDead);
}

//==================================================
// trigger
//==================================================

float InputGamepad::GetLTrigger()
{
    if (!s_connected) return 0.0f;
    return NormalizeTrigger(s_curr.lt, s_lTriggerDead);
}

float InputGamepad::GetRTrigger()
{
    if (!s_connected) return 0.0f;
    return NormalizeTrigger(s_curr.rt, s_rTriggerDead);
}

//==================================================
// utility
//==================================================

bool InputGamepad::AnyInputThisFrame()
{
    if (!s_connected) return false;

    if (s_curr.buttons != 0) return true;

    if (NormalizeStick(s_curr.lx, s_lStickDead) != 0.0f) return true;
    if (NormalizeStick(s_curr.ly, s_lStickDead) != 0.0f) return true;
    if (NormalizeStick(s_curr.rx, s_rStickDead) != 0.0f) return true;
    if (NormalizeStick(s_curr.ry, s_rStickDead) != 0.0f) return true;
    if (NormalizeTrigger(s_curr.lt, s_lTriggerDead) != 0.0f) return true;
    if (NormalizeTrigger(s_curr.rt, s_rTriggerDead) != 0.0f) return true;

    return false;
}

void InputGamepad::SetDeadZone(SHORT ls, SHORT rs, BYTE lt, BYTE rt)
{
    if (ls >= 0) s_lStickDead = ls;
    if (rs >= 0) s_rStickDead = rs;
    s_lTriggerDead = lt;
    s_rTriggerDead = rt;
}