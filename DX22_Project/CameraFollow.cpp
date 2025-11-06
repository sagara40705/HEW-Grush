#include "CameraFollow.h"
#include "GameObject.h"
#include "Transform.h"
#include <DirectXMath.h>
using namespace DirectX;

void CameraFollow::Start()
{
    self_ = Parent->TryGetComponent<Transform>();
    assert(self_ && "CameraFollow requires a Transform component");
    assert(target_ && "CameraFollow target Transform is null");
}

void CameraFollow::Update()
{
    if (!self_ || !target_) return;

    // プレイヤーの位置を取得
    const XMFLOAT3 targetPos = target_->GetPosition();

    // 追従位置 = プレイヤー位置 + オフセット
    self_->SetPosition({ targetPos.x + OFFSET.x, targetPos.y + OFFSET.y, targetPos.z + OFFSET.z });
}

void CameraFollow::OnDestroy()
{
    self_ = nullptr;
    target_ = nullptr;
}
