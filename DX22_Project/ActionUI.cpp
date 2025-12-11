#include "ActionUI.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "UiSpriteRenderer.h"
#include "DebugLog.h"
#include <cassert>

void ActionUI::Awake()
{
	//位置・おおきさ
	constexpr Vector2 kSpinPos = { 920.f, 520.f };
	constexpr Vector2 kDashPos = { 1100.f, 520.f };
	constexpr Vector2 kSize = { 150.f, 150.f };
	constexpr Vector2 kSpinMojiPos = { 950.f, 660.f };
	constexpr Vector2 kDashMojiPos = { 1100.f,660.f };
	constexpr Vector2 kSpinMojiSize = {  90.f, 32.f };
	constexpr Vector2 kDashMojiSize = { 135.f, 32.f };

	//UV
	constexpr Vector2 kUVSize = { 1.f, 1.f };
	constexpr Vector2 kSpinMonoUVPos = { 1.f, 1.f };
	constexpr Vector2 kSpinUVPos = { 1.f, 1.f };
	constexpr Vector2 kDashMonoUVPos = { 1.f, 1.f };
	constexpr Vector2 kDashUVPos = { 1.f ,1.f };

	// 回転攻撃、モノクロ
	GameObject& spinMono = gameObject->CreateChild();
	spinMono.AddComponent<UiSpriteRenderer>(TextureTag::SpinMono, kSpinPos, kSize, kSpinMonoUVPos, kUVSize);

	// 回転攻撃、色付き
	GameObject& spin = gameObject->CreateChild();
	spin_ = &spin.AddComponent<UiSpriteRenderer>(TextureTag::Spin, kSpinPos, kSize, kSpinUVPos, kUVSize);

	// 回転攻撃、文字
	GameObject& spinMojiMono = gameObject->CreateChild();
	spinMojiMono.AddComponent<UiSpriteRenderer>(TextureTag::SpinMojiMono, kSpinMojiPos, kSpinMojiSize, kSpinUVPos, kUVSize);

	// 回転攻撃、文字、色付き
	GameObject& spinMoji = gameObject->CreateChild();
	spinMoji_= &spinMoji.AddComponent<UiSpriteRenderer>(TextureTag::SpinMoji, kSpinMojiPos, kSpinMojiSize, kSpinUVPos, kUVSize);

	// ダッシュ攻撃、モノクロ
	GameObject& dashMono = gameObject->CreateChild();
	dashMono.AddComponent<UiSpriteRenderer>(TextureTag::DashMono, kDashPos, kSize, kDashMonoUVPos, kUVSize);

	// ダッシュ、色付き
	GameObject& dash = gameObject->CreateChild();
	dash_ = &dash.AddComponent<UiSpriteRenderer>(TextureTag::Dash, kDashPos, kSize, kDashUVPos, kUVSize);

	// ダッシュ、文字
	GameObject& dashMojiMono = gameObject->CreateChild();
	dashMojiMono.AddComponent<UiSpriteRenderer>(TextureTag::DashMojiMono, kDashMojiPos, kDashMojiSize, kDashUVPos, kUVSize);

	// ダッシュ、文字、色付き
	GameObject& dashMoji = gameObject->CreateChild();
	dashMoji_ = &dashMoji.AddComponent<UiSpriteRenderer>(TextureTag::DashMoji, kDashMojiPos, kDashMojiSize, kDashUVPos, kUVSize);

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
		ct = 1.0f - state_->GetCoolDownRatio(ActionId::SpinAttack);
		spin_->SetOffset({ 920.f,  670 - (150 * ct) });
		spin_->SetSize({ 150.f , 150.f * ct });
		spin_->SetUVPos({ 1, 1 - ct });
		spin_->SetUVScale({ 1, ct });

		if (!state_->canUseAction[ToIndex(ActionId::SpinAttack)])
		{

			spinMoji_->SetOffset({ 950.f,  692 - (32 * ct) });
			spinMoji_->SetSize({ 90.f , 32.f * ct });
			spinMoji_->SetUVPos({ 1, 1 - ct });
			spinMoji_->SetUVScale({ 1, ct });
		}
		
	}
	
	//if (!state_->canUseAction[ToIndex(ActionId::DashAttack)] || 1)
	{
		ct = 1.0f - state_->GetCoolDownRatio(ActionId::DashAttack);
		dash_->SetOffset({ 1100.f,  670 - (150 * ct) });
		dash_->SetSize({ 150.f , 150.f * ct });
		dash_->SetUVPos({ 1, 1 - ct });
		dash_->SetUVScale({ 1, ct });

		if (!state_->canUseAction[ToIndex(ActionId::DashAttack)])
		{
			dashMoji_->SetOffset({ 1100.f,  692 - (32 * ct) });
			dashMoji_->SetSize({ 135.f , 32.f * ct });
			dashMoji_->SetUVPos({ 1, 1 - ct });
			dashMoji_->SetUVScale({ 1, ct });
		}
	}



	/*
	switch (action)
	{
	case ActionId::SpinAttack:
		ct = state_->GetCoolDownRatio(action);
		spin_->SetOffset({ 0, 200.f * (1 - ct) });
		spin_->SetSize({ 220.f , 200.f * ct });
		spin_->SetUVPos({ 1, 1 - ct });
		spin_->SetUVScale({ 1, ct });
		break;

	case ActionId::DashAttack:
		ct = state_->GetCoolDownRatio(action);
		dash_->SetOffset({ 0, 200.f * (1 - ct) });
		dash_->SetSize({ 220.f , 200.f * ct });
		dash_->SetUVPos({ 1, 1 - ct });
		dash_->SetUVScale({ 1, ct });
		break;

	default:
		break;
	}
	*/

}

void ActionUI::OnDestroy()
{
	state_ = nullptr;
	spin_  = nullptr;
	dash_  = nullptr;
	spinMoji_ = nullptr;
	dashMoji_ = nullptr;
}
