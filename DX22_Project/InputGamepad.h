#pragma once
#include <Windows.h>
#undef max
#undef min

#include <Xinput.h>
#include <dinput.h>

// ƒ‰ƒCƒuƒ‰ƒŠ
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class InputGamepad
{
public:
    static void Init(HWND hwnd);
    static void Uninit();
    static void Update();

    static bool IsConnected();

    static bool GetButton(WORD button);
    static bool GetButtonDown(WORD button);
    static bool GetButtonUp(WORD button);

    static float GetLStickX();
    static float GetLStickY();
    static float GetRStickX();
    static float GetRStickY();

    static float GetLTrigger();
    static float GetRTrigger();

    static bool AnyInputThisFrame();

    static void SetDeadZone(SHORT ls, SHORT rs, BYTE lt, BYTE rt);

private:
    enum class Backend
    {
        None,
        XInput,
        DirectInput
    };

    struct CommonState
    {
        WORD  buttons = 0;
        SHORT lx = 0;
        SHORT ly = 0;
        SHORT rx = 0;
        SHORT ry = 0;
        BYTE  lt = 0;
        BYTE  rt = 0;
    };

private:
    static Backend s_backend;
    static bool    s_connected;

    static CommonState s_curr;
    static CommonState s_prev;

    static SHORT s_lStickDead;
    static SHORT s_rStickDead;
    static BYTE  s_lTriggerDead;
    static BYTE  s_rTriggerDead;

private:
    // XInput
    static bool UpdateXInput();

    // DirectInput
    static HWND s_hwnd;
    static IDirectInput8* s_di;
    static IDirectInputDevice8* s_diDevice;
    static GUID                  s_diGuid;
    static bool                  s_diFound;

    static bool InitDirectInput(HWND hwnd);
    static void UninitDirectInput();
    static bool FindDirectInputGamepad();
    static bool CreateDirectInputDevice();

    static BOOL CALLBACK EnumGamepadsCallback(const DIDEVICEINSTANCE* instance, VOID* context);
    static bool UpdateDirectInput();

    // helper
    static float NormalizeStick(SHORT v, SHORT dead);
    static float NormalizeTrigger(BYTE v, BYTE dead);
    static SHORT ToSignedAxis(LONG v);
    static WORD  MapDirectInputButtons(const DIJOYSTATE2& js);
};