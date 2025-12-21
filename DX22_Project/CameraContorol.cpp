#include "CameraContorol.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputKeyboard.h"
#include "InputGamepad.h"
#include "ShaderList.h"

namespace
{
    // スティックのデッドゾーン（これ以下なら無視）
    constexpr float kStickDeadZone = 0.2f;

    // 回転速度（度 / フレーム想定。Time::DeltaTime があるなら掛けて調整）
    constexpr float kGamepadYawSpeed = 2.5f;
    constexpr float kKeyboardYawSpeed = 3.0f;
}

void CameraControl::Start()
{
	GameObject* palyer = gameObject->FindWithTag(Tag::Player);

	playerTf_ = palyer->transform;
	selfTf_ = gameObject->transform;
}

void CameraControl::OnDestroy()
{
	playerTf_ = nullptr;
	selfTf_ = nullptr;
}

void CameraControl::Update()
{
    //ShaderList::SetCameraPos(playerTf_->GetWorldPosition());
    RotateByInput();
	FollowPlayer();
}

void CameraControl::FollowPlayer()
{
	selfTf_->SetPosition(playerTf_->GetWorldPosition());
}
void CameraControl::RotateByInput()
{
    if (!selfTf_) return;

    float yawDelta = 0.0f;

    //==================== 1) ゲームパッド（右スティック） ====================
    // 右スティック X [-1,1] を Y 回転にマッピング
    float rstickX = InputGamepad::GetRStickX();

    if (std::fabs(rstickX) > kStickDeadZone)
    {
        yawDelta += rstickX * kGamepadYawSpeed;
    }

    //==================== 2) キーボード（LEFT / RIGHT） =====================
    // 判定を後にすることで、同時入力時はキーボードを優先
    if (InputKeyboard::GetKey(VK_LEFT))
    {
        yawDelta -= kKeyboardYawSpeed;
    }
    if (InputKeyboard::GetKey(VK_RIGHT))
    {
        yawDelta += kKeyboardYawSpeed;
    }

    // 実際の回転反映
    if (yawDelta != 0.0f)
    {
        Vector3 rot = selfTf_->GetRotation(); // (pitch, yaw, roll) [度]
        rot.y += yawDelta;
        selfTf_->SetRotation(rot);
    }
}
