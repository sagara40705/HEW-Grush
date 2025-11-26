#include "CameraControl.h"
#include "Input.h"
#include "Game.h"
#include "Transform.h"
#include "PlayerAction.h"
#include <cassert>
#include "DebugLog.h"

#ifdef _WIN32
#include <Windows.h>
static inline bool KeyHeld(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }
#else
static inline bool KeyHeld(int) { return false; }
#endif
// --- XInput: Rスティックでカメラ回転 ---
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")
using namespace DirectX;
// Rスティック X 入力取得（-1～1）。実際に入力があれば true
namespace
{
    constexpr SHORT RIGHT_STICK_DEAD = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

    bool GetRightStickX(float& outX)
    {
        XINPUT_STATE st{};
        if (XInputGetState(0, &st) != ERROR_SUCCESS)
        {
            outX = 0.0f;
            return false; // コントローラ未接続
        }

        const SHORT sx = st.Gamepad.sThumbRX;
        if (std::abs(sx) <= RIGHT_STICK_DEAD)
        {
            outX = 0.0f;
            return false; // デッドゾーン内 → 入力なし扱い
        }

        outX = static_cast<float>(sx) / 32767.0f; // -1～1
        // 1.0 を超える場合は正規化（安全のため）
        if (outX > 1.0f)  outX = 1.0f;
        if (outX < -1.0f) outX = -1.0f;
        return true;
    }
}


void CameraControl::Start()
{
    // === PlayerControllの取得 ===
    for (auto& o : Game::GetWorld().objects)
    {
        if (!o->active) continue;
        if (!o->CompareTag(Tag::Player)) continue;
        playerTf_ = o->TryGetComponent<Transform>();
        playerAc_ = o->TryGetComponent<PlayerAction>();
        if (playerTf_) break;
    }

    selfTf_ = gameObject->TryGetComponent<Transform>();

    assert(selfTf_ && "CameraFollow requires Transform");
    assert(playerTf_ && "CameraFollow target Transform is null");
    assert(playerAc_ && "CameraFollow target Ac is null");
}

void CameraControl::OnDestroy()
{
    playerTf_ = nullptr;
    selfTf_ = nullptr;
    playerAc_ = nullptr;
}

void CameraControl::Update()
{
    FollowPlayer();
    ApplyInput_();
}


void CameraControl::FollowPlayer()
{
    selfTf_->SetPosition(playerTf_->GetWorldPosition());
}

void CameraControl::ApplyInput_()
{
    //プレイヤーアクション中は制限
    if (playerAc_->IsAction()) return;

    // 仮のDTを使用
    constexpr float DT = 1.f / 60.f;
    float d = 0.f;

    // --- まず Rスティックの入力をチェック（入力があればパッド優先） ---
    float stickX = 0.0f;
    const bool padActive = GetRightStickX(stickX);
    if (padActive)
    {
        // 右スティック右：時計回り、左：反時計回り
        // yawSpeedDeg を「最大倒し時の角速度」として使用
        d += kYawSpeedDeg * DT * stickX;
    }
    else
    {
#ifdef _WIN32
        // パッド入力がないときだけキーボードを使用
        // 右＝時計回り、左＝反時計回り（上から見下ろしたとき）
        if (KeyHeld(VK_RIGHT)) d += kYawSpeedDeg * DT;
        if (KeyHeld(VK_LEFT))  d -= kYawSpeedDeg * DT;
#endif
    }

    // 現在角度に加算
    if (d != 0.f)
    {
        angleDeg += d;
        WrapAngle_();
        selfTf_->AddRotate(Vector3(0.0f, d, 0.0f)); // Y=Yaw
        Debug::LogValue("Caemra-AddRotate", d);
    }
}
