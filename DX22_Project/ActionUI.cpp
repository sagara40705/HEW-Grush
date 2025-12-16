#include "ActionUI.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "UiSpriteRenderer.h"
#include "DebugLog.h"
#include <cassert>

void ActionUI::Awake()
{
	//位置・おおきさ
	constexpr Vector2 kSpinPos = { 920.f, 530.f };
	constexpr Vector2 kDashPos = { 1090.f, 530.f };
	constexpr Vector2 kSize = { 150.f, 150.f };
	constexpr Vector2 kSpinMojiPos = { 950.f, 670.f };
	constexpr Vector2 kDashMojiPos = { 1090.f,670.f };
	constexpr Vector2 kSpinMojiSize = {  90.f, 32.f };
	constexpr Vector2 kDashMojiSize = { 135.f, 32.f };

	constexpr Vector2 kSpinFlashPos = { 820.f, 430.f };
	constexpr Vector2 kFlashSize = { 200.f, 200.f };

	//UV
	constexpr Vector2 kUVSize = { 1.f, 1.f };
	constexpr Vector2 kSpinMonoUVPos = { 1.f, 1.f };
	constexpr Vector2 kSpinUVPos = { 1.f, 1.f };
	constexpr Vector2 kDashMonoUVPos = { 1.f, 1.f };
	constexpr Vector2 kDashUVPos = { 1.f ,1.f };


	// 回転攻撃、モノクロ
	GameObject& spinMono = gameObject->CreateChild();
	spinMono.AddComponent<UiSpriteRenderer>(TextureTag::SpinMono, kSpinPos, kSize, kSpinMonoUVPos, kUVSize);

	// 回転攻撃、色付き(光らせる用)
	GameObject& spinF = gameObject->CreateChild();
	spFlash_ = &spinF.AddComponent<UiSpriteRenderer>(TextureTag::SpinFlash, kSpinPos, kFlashSize, kSpinUVPos, kUVSize);
	spFlash_->SetColor({ 1.f, 1.f, 1.f, 0.f });

	// 回転攻撃、色付き
	GameObject& spin = gameObject->CreateChild();
	spin_ = &spin.AddComponent<UiSpriteRenderer>(TextureTag::Spin, kSpinPos, kSize, kSpinUVPos, kUVSize);

	// 回転攻撃、色付き(光らせる用)
	GameObject& spinFr = gameObject->CreateChild();
	spFront_ = &spinFr.AddComponent<UiSpriteRenderer>(TextureTag::SpinFront, kSpinPos, kSize, kSpinUVPos, kUVSize);
	spFront_->SetColor({ 1.f, 1.f, 1.f, 0.f });

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
	constexpr float kGlowDuration = 0.3f; // 0.3秒だけ光る
	float ct;
	int a;
	
	// ---回転攻撃
	if (!state_->canUseAction[ToIndex(ActionId::SpinAttack)])
	{
		ct = 1.0f - state_->GetCoolDownRatio(ActionId::SpinAttack);
		a = static_cast<int>(ct);
		spin_->SetOffset({ 920.f,  680 - (150 * ct) });
		spin_->SetSize({ 150.f , 150.f * ct });
		spin_->SetUVPos({ 1, 1 - ct });
		spin_->SetUVScale({ 1, ct });

		if (a <= 0)
		spinMoji_->SetSize({ 0.f , 0.f });
	}
	else
	spinMoji_->SetSize({ 90.f , 32.f });


	// ---たまった瞬間光らせる処理(回転攻撃)
	if (glowTimer > 0.f)
	{
		glowTimer -= 1.0f / 60.0f;

		float t = glowTimer / kGlowDuration; // 1.0 → 0.0
		float alpha = std::sin(t * 3.1415f); // 0→1→0 のカーブ

		spFlash_->SetColor({ 2.f,1.2f,2.f, alpha }); // アルファでフェード
		spFront_->SetColor({ 2.f,1.2f,2.f, alpha }); // アルファでフェード
		spFlash_->SetVisible(true);
	}
	else
	{
		spFlash_->SetVisible(false); // 消す
	}

	bool isSpinReady = state_->canUseAction[ToIndex(ActionId::SpinAttack)];

	if (isSpinReady && !wasSpinReady)
	{
		glowTimer = kGlowDuration; 
	}
	wasSpinReady = isSpinReady;


	// ---ダッシュ攻撃
	if (!state_->canUseAction[ToIndex(ActionId::DashAttack)])
	{
		ct = 1.0f - state_->GetCoolDownRatio(ActionId::DashAttack);
		a = static_cast<int>(ct);
		dash_->SetOffset({ 1090.f,  680 - (150 * ct) });
		dash_->SetSize({ 150.f , 150.f * ct });
		dash_->SetUVPos({ 1, 1 - ct });
		dash_->SetUVScale({ 1, ct });

		if (a <= 0)
		dashMoji_->SetSize({ 0.f , 0.f });
	}
	else
	dashMoji_->SetSize({ 135.f , 32.f });

}

void ActionUI::OnDestroy()
{
	state_ = nullptr;
	spin_  = nullptr;
	dash_  = nullptr;
	spFlash_ = nullptr;
	//daFlash_ = nullptr;
	spinMoji_ = nullptr;
	dashMoji_ = nullptr;
}
