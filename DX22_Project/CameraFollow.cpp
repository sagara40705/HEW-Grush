#// === File: CameraFollow.cpp ===
#include "CameraFollow.h"
#include "GameObject.h"
#include "Transform.h"
#include "PlayerAction.h"
#include "Game.h"
#include "GameObject.h"
#include <DirectXMath.h>
#include <cassert>

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

void CameraFollow::Start()
{
    self_ = gameObject->TryGetComponent<Transform>();
    assert(self_ && "CameraFollow requires Transform");
    assert(target_ && "CameraFollow target Transform is null");

    // === PlayerControllの取得 ===
    for (auto& o : Game::GetWorld().objects)
    {
        if (!o->active) continue;
        if (!o->CompareTag(Tag::Player)) continue;
        pa_ = o->TryGetComponent<PlayerAction>();
        if (pa_) break;
    }

    WrapAngle_();
    RecomputeTransform_(); // 初期反映
}

void CameraFollow::Update()
{
    if (!self_ || !target_) return;

    ApplyKeyboardInput_();
    RecomputeTransform_();
}

void CameraFollow::OnDestroy()
{
    self_ = nullptr;
    target_ = nullptr;
}

void CameraFollow::ApplyKeyboardInput_()
{
    if (pa_->IsDashing()) return;

    // あなたのTime管理があるなら置換（例：Time::deltaTime）
    constexpr float DT = 1.f / 60.f;
    float d = 0.f;

    // --- まず Rスティックの入力をチェック（入力があればパッド優先） ---
    float stickX = 0.0f;
    const bool padActive = GetRightStickX(stickX);
    if (padActive)
    {
        // 右スティック右：時計回り、左：反時計回り
        // yawSpeedDeg を「最大倒し時の角速度」として使用
        d += cfg_.yawSpeedDeg * DT * stickX;
    }
    else
    {
#ifdef _WIN32
        // パッド入力がないときだけキーボードを使用
        // 右＝時計回り、左＝反時計回り（上から見下ろしたとき）
        if (KeyHeld(VK_RIGHT)) d += cfg_.yawSpeedDeg * DT;
        if (KeyHeld(VK_LEFT))  d -= cfg_.yawSpeedDeg * DT;
#endif
    }

    if (d != 0.f) AddAngleDegrees(d);
}

void CameraFollow::RecomputeTransform_()
{
    // --- 原点 = プレイヤー位置 + (0, height, 0) ---
    const XMFLOAT3 tp = target_->GetPosition();
    const XMVECTOR targetPos = XMLoadFloat3(&tp);
    const XMVECTOR origin = XMVectorAdd(targetPos, XMVectorSet(0.f, cfg_.height, 0.f, 0.f));

    // --- 原点中心の水平円（半径=distance）を angleDeg で回転（右＝時計回り） ---
    const float r = cfg_.distance;
    const float th = XMConvertToRadians(cfg_.angleDeg);
    const float x = r * std::cos(th);
    const float z = -r * std::sin(th); // 時計回り

    const XMVECTOR eye = XMVectorAdd(origin, XMVectorSet(x, 0.f, z, 0.f));

    // 位置
    XMFLOAT3 eyeF; XMStoreFloat3(&eyeF, eye);
    self_->SetPosition(eyeF);

    // --- 向き：Transformは「+Z が前方」、回転は SetRotation(度)→内部で RollPitchYaw(pitch,yaw,roll) を使用 ---
    //          よって +Z を (target - eye) に合わせるEulerを求める
    XMVECTOR dir = XMVectorSubtract(targetPos, eye);      // = target - eye
    dir = XMVector3Normalize(dir);

    XMFLOAT3 d; XMStoreFloat3(&d, dir);
    const float yaw = std::atan2(d.x, d.z);                                 // +Z基準の水平角
    const float pitch = std::atan2(d.y, std::sqrt(d.x * d.x + d.z * d.z));    // 上下角

    self_->SetRotation(Vector3{
        XMConvertToDegrees(pitch) + 30,   // X = pitch[deg]
        XMConvertToDegrees(yaw),     // Y = yaw[deg]
        0.0f                         // Z = roll
        });

}