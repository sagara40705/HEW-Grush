#include "ActionUI.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "UiSpriteRenderer.h"
#include "DebugLog.h"
#include <cassert>
void ActionUI::Awake()
{
	//文字位置・おおきさ
	constexpr Vector2 kSpinMojiPos = { 930.f, 670.f };
	constexpr Vector2 kDashMojiPos = { 1090.f,670.f };
	constexpr Vector2 kMojiSize = { 135.f, 32.f };
	//UVサイズ（文字共通）
	constexpr Vector2 kUVFontSize = { 0.25f, 1.f };

	//位置・大きさ
	constexpr Vector2 kPos[ToIndex(ActionDigits::Count)] =
	{ {920.f,530.f},{920.f,530.f},{920.f,530.f},{920.f,530.f},      //スピンアイコン座標
	  {1090.f,530.f},{1090.f,530.f},{1091.f,529.f},{1089.f,530.f} }; //ダッシュアイコン座標
	constexpr Vector2 kSize = { 150.f, 150.f };

	// 1マスあたりのUVサイズ
	constexpr float uW = 1.f / 8.f; // 0.125
	constexpr float uH = 1.f / 2.f; // 0.5

	// 左上を (0,0) とした場合のアイコン位置
	constexpr Vector2 kUVPos[ToIndex(ActionDigits::Count)] = {
		{0 * uW, 0.0f}, {2 * uW, 0.0f}, {4 * uW, 0.0f}, {6 * uW, 0.0f}, // Spin系の4枚
		{0 * uW, 0.5f}, {2 * uW, 0.5f}, {4 * uW, 0.5f}, {6 * uW, 0.5f}  // Dash系の4枚
	};
	constexpr Vector2 kUVSize = { uW, uH };

	//アクションアイコンの生成
	for (size_t digits = 0; digits < ToIndex(ActionDigits::Count); ++digits)
	{
		GameObject& go = gameObject->CreateChild();
		ActionUi_[digits] = &(go.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Action_Icon, kPos[digits], kSize, kUVPos[digits], kUVSize));
	}
	//光らせる系の初期状態は透明にしておく
	ActionUi_[ToIndex(ActionDigits::DashFlash)]->SetColor({ 1.f, 1.f, 1.f, 0.f });
	ActionUi_[ToIndex(ActionDigits::DashFront)]->SetColor({ 1.f, 1.f, 1.f, 0.f });
	ActionUi_[ToIndex(ActionDigits::SpinFlash)]->SetColor({ 1.f, 1.f, 1.f, 0.f });
	ActionUi_[ToIndex(ActionDigits::SpinFront)]->SetColor({ 1.f, 1.f, 1.f, 0.f });

	// 回転攻撃、文字
	GameObject& spinMojiMono = gameObject->CreateChild();
	spinMojiMono.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Action_TextIcon, kSpinMojiPos, kMojiSize, Vector2({ 0.5f,0.f }), kUVFontSize);

	// 回転攻撃、文字、色付き
	GameObject& spinMoji = gameObject->CreateChild();
	spinMoji_ = &spinMoji.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Action_TextIcon, kSpinMojiPos, kMojiSize, Vector2({ 0.75f,0.f }), kUVFontSize);

	// ダッシュ、文字
	GameObject& dashMojiMono = gameObject->CreateChild();
	dashMojiMono.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Action_TextIcon, kDashMojiPos, kMojiSize, Vector2({ 0.0f,0.f }), kUVFontSize);

	// ダッシュ、文字、色付き
	GameObject& dashMoji = gameObject->CreateChild();
	dashMoji_ = &dashMoji.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Action_TextIcon, kDashMojiPos, kMojiSize, Vector2({ 0.25f,0.f }), kUVFontSize);

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
	float uW = 0.125f; // 1/8
	float uH = 0.5f; // 1行の高さ

	// ---回転攻撃
	if (!state_->canUseAction[ToIndex(ActionId::SpinAttack)])
	{
		ct = 1.0f - state_->GetCoolDownRatio(ActionId::SpinAttack);
		a = static_cast<int>(ct);
		float SpinX = 4 * uW; // 本来表示したいアイコンのX座標
		float SpinY = 0.5f;	  // 上の行の底辺

		// ---回転攻撃の表示更新
		ActionUi_[ToIndex(ActionDigits::Spin)]->SetOffset({ 920.f,  680.f - (150.f * ct) });
		ActionUi_[ToIndex(ActionDigits::Spin)]->SetSize({ 150.f , 150.f * ct });
		ActionUi_[ToIndex(ActionDigits::Spin)]->SetUVPos({ SpinX, SpinY - (uH * ct) });
		ActionUi_[ToIndex(ActionDigits::Spin)]->SetUVScale({ 0.125f, uH * ct });
		if (a <= 0)
			spinMoji_->SetColor({ 1.f, 1.f, 1.f, 0.f });
	}
	else
		spinMoji_->SetColor({ 1.f, 1.f, 1.f, 1.f });


	// ---たまった瞬間光らせる処理(回転攻撃)
	if (glowSpinTimer > 0.f)
	{
		glowSpinTimer -= 1.0f / 60.0f;

		float t = glowSpinTimer / kGlowDuration; // 1.0 → 0.0
		float alpha = std::sin(t * 3.1415f); // 0→1→0 のカーブ

		ActionUi_[ToIndex(ActionDigits::SpinFlash)]->SetColor({ 2.f,1.2f,2.f, alpha }); // アルファでフェード
		ActionUi_[ToIndex(ActionDigits::SpinFront)]->SetColor({ 2.f,1.2f,2.f, alpha }); // アルファでフェード
		ActionUi_[ToIndex(ActionDigits::SpinFlash)]->SetVisible(true);
	}
	else
	{
		ActionUi_[ToIndex(ActionDigits::SpinFlash)]->SetVisible(false); // 消す
	}

	bool isSpinReady = state_->canUseAction[ToIndex(ActionId::SpinAttack)];

	if (isSpinReady && !wasSpinReady)
	{
		glowSpinTimer = kGlowDuration;
	}
	wasSpinReady = isSpinReady;


	// ---ダッシュ攻撃
	if (!state_->canUseAction[ToIndex(ActionId::DashAttack)])
	{
		ct = 1.0f - state_->GetCoolDownRatio(ActionId::DashAttack);
		a = static_cast<int>(ct);
		float DashX = 4 * uW; // 本来表示したいアイコンのX座標
		float DashY = 1.f; // 上の行の底辺

		// ---回転攻撃の表示更新
		ActionUi_[ToIndex(ActionDigits::Dash)]->SetOffset({ 1091.f,  679.f - (150.f * ct) });
		ActionUi_[ToIndex(ActionDigits::Dash)]->SetSize({ 150.f , 150.f * ct });
		ActionUi_[ToIndex(ActionDigits::Dash)]->SetUVPos({ DashX, DashY - (uH * ct) });
		ActionUi_[ToIndex(ActionDigits::Dash)]->SetUVScale({ 0.125f, uH * ct });
		if (a <= 0)
			dashMoji_->SetColor({ 1.f, 1.f, 1.f, 0.f });
	}
	else
		dashMoji_->SetColor({ 1.f, 1.f, 1.f, 1.f });


	// ---たまった瞬間光らせる処理(ダッシュ攻撃)
	if (glowDashTimer > 0.f)
	{
		glowDashTimer -= 1.0f / 60.0f;

		float t = glowDashTimer / kGlowDuration; // 1.0 → 0.0
		float alpha = std::sin(t * 3.1415f); // 0→1→0 のカーブ

		ActionUi_[ToIndex(ActionDigits::DashFlash)]->SetColor({ 1.f,1.2f,1.f, alpha }); // アルファでフェード
		ActionUi_[ToIndex(ActionDigits::DashFront)]->SetColor({ 1.f,1.2f,1.f, alpha }); // アルファでフェード
		ActionUi_[ToIndex(ActionDigits::DashFlash)]->SetVisible(true);
	}
	else
	{
		ActionUi_[ToIndex(ActionDigits::DashFlash)]->SetVisible(false); // 消す
	}

	bool isDashReady = state_->canUseAction[ToIndex(ActionId::DashAttack)];

	if (isDashReady && !wasDashReady)
	{
		glowDashTimer = kGlowDuration;
	}
	wasDashReady = isDashReady;

}

void ActionUI::OnDestroy()
{

	spinMoji_ = nullptr;
	dashMoji_ = nullptr;
	for (size_t digits = 0; digits < ToIndex(ActionDigits::Count); ++digits)
	{
		ActionUi_[digits] = nullptr;
	}
}
