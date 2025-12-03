#include "PlayerAction.h"
#include "PlayerState.h"
#include "GameObject.h"
#include "DebugLog.h"
#include <cassert>

void PlayerAction::Start()
{
	state_ = gameObject->GetComponent<PlayerState>();
	assert(state_ && "state_がnullptrです");
	if (!state_) return;
	for (size_t action = 0; action < ToIndex(ActionId::Count); ++action)
	{
		state_->canUseAction[action] = true;
	}
}

void PlayerAction::Update()
{
	InputWantAction();
	CheckCanAction();
	UpdateAction();
	UpdateActionCT();
}

void PlayerAction::OnDestroy()
{
	state_ = nullptr;
}

// ダミー実装
void PlayerAction::InputWantAction()
{
	if (!state_->canAction) return;

	state_->wantUseAction[ToIndex(ActionId::NormalAttack)] = false;
	state_->wantUseAction[ToIndex(ActionId::SpinAttack)] = false;
	state_->wantUseAction[ToIndex(ActionId::DashAttack)] = false;
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
			//使用アクションをセット
			state_->usingAction = ToId(action);
			DebugLog::OutputValue("アクション実行 id", action);
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

	//使用中アクションごとに更新 *現在は空
	switch (action)
	{
	case ActionId::NormalAttack: UpdateNormalAction();	break;

	case ActionId::SpinAttack:	 UpdateSpinAction();	break;

	case ActionId::DashAttack:	 UpdateDashAction();	break;

	default: assert(false && "state_->usingActionが範囲外です");
		break;
	}

	// アクション時間を超えたら終了 + CoolTime設定
	if (state_->actionTimer[ToIndex(action)]
		>= state_->kActionTime[ToIndex(action)])
	{
		DebugLog::OutputValue("アクション終了 id", action);
		state_->cooltime[ToIndex(action)] = state_->kActionCT[ToIndex(action)];
		state_->usingAction = ActionId::None;
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
			DebugLog::OutputValue("アクションCT終了 id", action);
		}
	}
}

// ダミー実装
void PlayerAction::UpdateNormalAction()
{

}

// ダミー実装
void PlayerAction::UpdateSpinAction()
{
}

// ダミー実装
void PlayerAction::UpdateDashAction()
{
}
