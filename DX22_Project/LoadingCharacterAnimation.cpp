#include "LoadingCharacterAnimation.h"
#include "GameObject.h"
#include "Transform.h"
#include <cmath>

void LoadingCharacterAnimation::Start()
{
	auto* transform = gameObject->GetComponent<Transform>();
	if (transform)
	{
		originalPos_ = transform->GetPosition();
	}
}

void LoadingCharacterAnimation::Update()
{
	constexpr float DT = 1.f / 60.f;
	animTime_ += DT;

	auto* transform = gameObject->GetComponent<Transform>();
	if (!transform) return;

	// ã‰º‚É—h‚ê‚éƒAƒjƒ[ƒVƒ‡ƒ“
	float bounce = std::sin(animTime_ * 2.0f) * 0.3f;
	Vector3 newPos = originalPos_;
	newPos.y += bounce;
	transform->SetPosition(newPos);

	// YŽ²Žü‚è‚É‰ñ“]
	Vector3 rotation = transform->GetRotation();
	rotation.y += 60.0f * DT;  // 1•b‚É60“x‰ñ“]
	transform->SetRotation(rotation);
}