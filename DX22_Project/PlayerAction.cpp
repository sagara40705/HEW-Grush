#include "PlayerAction.h"
#include "Transform.h"
#include "PlayerState.h"
#include "Hammer.h"
#include "GameObject.h"
#include "DebugLog.h"
#include "InputKeyboard.h"   
#include "InputGamepad.h"  
#include "BoxCollider.h"
#include <cassert>

constexpr float kActionTime[ToIndex(ActionId::Count)] = { 0.4f, 1.f, 2.f };
constexpr float kCoolDown[ToIndex(ActionId::Count)]  = { 0.1f, 5.f, 5.f };

void PlayerAction::Start()
{
	tf_ = gameObject->GetComponent<Transform>();
	state_ = gameObject->GetComponent<PlayerState>();
	for (GameObject* go : gameObject->GetChildren())
	{
		if (!go) continue;
		if (go->CompareTag(Tag::Hammer))
		{
			hammer_ = go->GetComponent<Hammer>();
			break;
		}
	}

	assert(state_ && tf_ && hammer_ && "参照のどれかがnullptrです");
	if (!state_ || !tf_ || !hammer_) return;

	for (size_t action = 0; action < ToIndex(ActionId::Count); ++action)
	{
		state_->canUseAction[action] = true;
	}
	SetHammerActive(false);
}

void PlayerAction::Update()
{
	assert(state_ && tf_ && hammer_ && "参照のどれかがnullptrです");
	if (!state_ || !tf_ || !hammer_) return;

	if (!state_) return;
	if (!state_->canControl) return;

	InputWantAction();
	CheckCanAction();
	UpdateAction();
	UpdateActionCT();
}

void PlayerAction::OnDestroy()
{
	hammer_ = nullptr;
	state_ = nullptr;
	tf_ = nullptr;
}

// ダミー実装
void PlayerAction::InputWantAction()
{
	// そもそもアクション禁止 + 他アクション中 なら入力を見ない
	if (!state_->canAction) return;
	if (state_->usingAction != ActionId::None) return;

	// まず今フレームの「やりたいアクション」を全部クリア
	for (size_t i = 0; i < ToIndex(ActionId::Count); ++i)
	{
		state_->wantUseAction[i] = false;
	}

	//==================== NormalAttack (Eキー / Xボタン) ====================
	if (InputKeyboard::GetKeyDown('E') ||
		InputGamepad::GetButtonDown(XINPUT_GAMEPAD_X))
	{
		state_->wantUseAction[ToIndex(ActionId::NormalAttack)] = true;
		DebugLog::Output("NormalAttack 入力受付");
	}

	//==================== SpinAttack (Rキー / R1ボタン) =====================
	if (InputKeyboard::GetKeyDown('R') ||
		InputGamepad::GetButtonDown(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		state_->wantUseAction[ToIndex(ActionId::SpinAttack)] = true;
		DebugLog::Output("SpinAttack 入力受付");
	}

	//==================== DashAttack (Fキー / L1ボタン) =====================
	if (InputKeyboard::GetKeyDown('F') ||
		InputGamepad::GetButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		state_->wantUseAction[ToIndex(ActionId::DashAttack)] = true;
		DebugLog::Output("DashAttack 入力受付");
	}
}

void PlayerAction::CheckCanAction()
{
	//他アクション中は発動できない
	if (!state_->canAction) return;
	if (state_->usingAction != ActionId::None) return;

	for (size_t action = 0; action < ToIndex(ActionId::Count); ++action)
	{
		// 入力がある + 使用可能なら発動
		if (state_->wantUseAction[action] && state_->canUseAction[action])
		{
			//使用アクションをセット + Hammerを有効化
			SetHammerActive(true);
			state_->usingAction = ToId(action);
			DebugLog::Output("アクション実行");
			//一応タイマーリセット
			state_->actionTimer[action] = 0.f;
			// 使用不可にしておく
			state_->canUseAction[action] = false;
			state_->wantUseAction[action] = false;
			return;
		}
	}
}

void PlayerAction::UpdateAction()
{
	//参照回数が多いため最初に抜き取る
	const ActionId action = state_->usingAction;

	// 念のため範囲チェック + タイマー加算
	if (action >= ActionId::Count) return;
	constexpr float DT = 1.f / 60.f;
	state_->actionTimer[ToIndex(action)] += DT;

	// アクション時間を超えたら終了 + CoolTime設定
	if (state_->actionTimer[ToIndex(action)]
		>= kActionTime[ToIndex(action)])
	{
		DebugLog::Output("アクション終了");
		SetHammerActive(false);
		hammer_->ResetPos();
		state_->cooltime[ToIndex(action)] = kCoolDown[ToIndex(action)];
		state_->usingAction = ActionId::None;
		return;
	}

	//使用中アクションごとに更新 *現在は空
	switch (action)
	{
	case ActionId::NormalAttack: UpdateNormalAttack();	break;

	case ActionId::SpinAttack:	 UpdateSpinAttack();	break;

	case ActionId::DashAttack:	 UpdateDashAttack();	break;

	default: assert(false && "state_->usingActionが範囲外です");
		break;
	}
}

void PlayerAction::UpdateActionCT()
{
	for (size_t action = 0; action < ToIndex(ActionId::Count); ++action)
	{
		//使用可能状態 + 使用中は動かさない
		if (state_->canUseAction[action] 
			|| action == ToIndex(state_->usingAction)) continue;

		constexpr float DT = 1.f / 60.f;

		// 経過時間を減算 、 0 になったら使用可能
		if ((state_->cooltime[action] -= DT) <= 0.0f)
		{
			state_->cooltime[action] = 0.0f;
			state_->canUseAction[action] = true;
			DebugLog::Output("アクションCT終了");
		}
	}
}

void PlayerAction::StopNormalAttack()
{
	if (state_->usingAction == ActionId::NormalAttack)
	{
		DebugLog::Output("通所攻撃強制終了");
		SetHammerActive(false);
		state_->cooltime[ToIndex(ActionId::NormalAttack)] = state_->kActionCT[ToIndex(ActionId::NormalAttack)];
		state_->usingAction = ActionId::None;
	}
}

//攻撃中 = 有効化, その他 = 無効
void PlayerAction::SetHammerActive(bool v)
{
	hammer_->gameObject->GetComponent<BoxCollider>()->SetEnabled(v);
}

// ダミー実装
void PlayerAction::UpdateNormalAttack()
{
	const float ratio = state_->actionTimer[ToIndex(ActionId::NormalAttack)] / kActionTime[ToIndex(ActionId::NormalAttack)];

	hammer_->UpdateNormalAttack(ratio);
}

// ダミー実装
void PlayerAction::UpdateSpinAttack()
{
	constexpr Vector3 delta = { 0.f, 18.f, 0.f };

	tf_->AddRotate(delta);
}

// ダミー実装
void PlayerAction::UpdateDashAttack()
{
}
