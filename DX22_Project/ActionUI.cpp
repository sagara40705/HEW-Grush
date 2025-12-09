#include "ActionUI.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "UiSpriteRenderer.h"
#include "DebugLog.h"
#include <cassert>

void ActionUI::Awake()
{
	//位置・おおきさ
	constexpr Vector2 kSpinPos = { 0.f, 0.f };
	constexpr Vector2 kDashPos = { 0.f, 0.f };
	constexpr Vector2 kSize = { 200.f, 200.f };

	//UV
	constexpr Vector2 kUVSize = { 0.5f, 0.5f };
	constexpr Vector2 kSpinMonoUVPos = { 0.f, 0.f };
	constexpr Vector2 kSpinUVPos = { 0.5f, 0.f };
	constexpr Vector2 kDashMonoUVPos = { 0.f, 0.5f };
	constexpr Vector2 kDashUVPos = { 0.5f ,0.5f };

	// 回転攻撃、モノクロ
	GameObject& spinMono = gameObject->CreateChild();
	spinMono.AddComponent<UiSpriteRenderer>(TextureTag::Star/*変更してください*/, kSpinPos, kSize, kSpinMonoUVPos, kUVSize);

	// 回転攻撃、色付き
	GameObject& spin = gameObject->CreateChild();
	spin_ = &spin.AddComponent<UiSpriteRenderer>(TextureTag::Star/*変更してください*/, kSpinPos, kSize, kSpinUVPos, kUVSize);

	// ダッシュ攻撃、モノクロ
	GameObject& dashMono = gameObject->CreateChild();
	dashMono.AddComponent<UiSpriteRenderer>(TextureTag::Star/*変更してください*/, kDashPos, kSize, kDashMonoUVPos, kUVSize);

	// ダッシュ、色付き
	GameObject& dash = gameObject->CreateChild();
	dash_ = &dash.AddComponent<UiSpriteRenderer>(TextureTag::Star/*変更してください*/, kDashPos, kSize, kDashUVPos, kUVSize);
}

void ActionUI::Start()
{
	GameObject* player = gameObject->FindWithTag(Tag::Player);
	state_ = player->GetComponent<PlayerState>();
	assert(state_ && "ActionUI::Start()内でstate_がnullptrです");
}

void ActionUI::Update()
{
	
}

void ActionUI::OnDestroy()
{
	state_ = nullptr;
}
