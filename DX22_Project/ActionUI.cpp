#include "ActionUI.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "UiSpriteRenderer.h"
#include "DebugLog.h"
#include <cassert>

void ActionUI::Awake()
{
	//位置・おおきさ
	constexpr Vector2 kSpinPos      = { 850.f, 500.f };
	constexpr Vector2 kDashPos      = { 1050.f,500.f };
	constexpr Vector2 kSpinMojiPos  = { 900.f, 675.f };
	constexpr Vector2 kDashMojiPos  = { 1050.f,675.f };
	constexpr Vector2 kSize         = { 200.f, 200.f };
	constexpr Vector2 kSpinMojiSize = { 100.f, 35.f };
	constexpr Vector2 kDashMojiSize = { 150.f, 36.f };

	//UV
	constexpr Vector2 kUVSize        = { 1.f, 1.f };
	constexpr Vector2 kSpinMonoUVPos = { 1.f, 1.f };
	constexpr Vector2 kSpinUVPos     = { 1.f, 1.f };
	constexpr Vector2 kDashMonoUVPos = { 1.f, 1.f };
	constexpr Vector2 kDashUVPos     = { 1.f ,1.f };


	// 回転攻撃、モノクロ
	GameObject& spinMono = gameObject->CreateChild();
	spinMono.AddComponent<UiSpriteRenderer>(TextureTag::SpinMono, kSpinPos, kSize, kSpinMonoUVPos, kUVSize);

	// 回転攻撃、色付き
	GameObject& spin = gameObject->CreateChild();
	spin_ = &spin.AddComponent<UiSpriteRenderer>(TextureTag::Spin, kSpinPos, kSize, kSpinUVPos, kUVSize);

	// 回転攻撃, 文字
	GameObject& spinMoji = gameObject->CreateChild();
	spinMoji.AddComponent<UiSpriteRenderer>(TextureTag::SpinMoji, kSpinMojiPos, kSpinMojiSize, kSpinUVPos, kUVSize);

	// ダッシュ攻撃、モノクロ
	GameObject& dashMono = gameObject->CreateChild();
	dashMono.AddComponent<UiSpriteRenderer>(TextureTag::DashMono, kDashPos, kSize, kDashMonoUVPos, kUVSize);

	// ダッシュ、色付き
	GameObject& dash = gameObject->CreateChild();
	dash_ = &dash.AddComponent<UiSpriteRenderer>(TextureTag::Dash, kDashPos, kSize, kDashUVPos, kUVSize);

	// ダッシュ、文字
	GameObject& dashMoji = gameObject->CreateChild();
	dashMoji.AddComponent<UiSpriteRenderer>(TextureTag::DashMoji, kDashMojiPos, kDashMojiSize, kDashUVPos, kUVSize);
}

void ActionUI::Start()
{
	GameObject* player = gameObject->FindWithTag(Tag::Player);
	state_ = player->GetComponent<PlayerState>();
	assert(state_ && "ActionUI::Start()内でstate_がnullptrです");
}

void ActionUI::Update()
{


	//現在使ってるアクションを取得
	//ActionId action = state_->usingAction;
	float ct;

	{
		// スピン攻撃
		
		ct = 1.0f - state_->GetCoolDownRatio(ActionId::SpinAttack); // 0→1 に増える
		spin_->SetOffset({ 850.f,  700 - (200 * ct) });
		spin_->SetSize({ 200.f , 200.f * ct });
		spin_->SetUVPos({ 1, 1 - ct });
		spin_->SetUVScale({ 1, ct });
	}

	{
		ct = 1.0f - state_->GetCoolDownRatio(ActionId::DashAttack); // 0→1 に増える
		dash_->SetOffset({ 1050.f,  700 - (200 * ct) });
		dash_->SetSize({ 200.f , 200.f * ct });
		dash_->SetUVPos({ 1, 1 - ct });
		dash_->SetUVScale({ 1, ct });
	}


}

void ActionUI::OnDestroy()
{
	state_ = nullptr;
	spin_  = nullptr;
	dash_  = nullptr;
}
