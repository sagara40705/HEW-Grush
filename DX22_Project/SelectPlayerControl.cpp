#include "SelectPlayerControl.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include <assert.h>

void SelectPlayerControl::Start()
{
	// ステージマップの取得
	GameObject* stage = gameObject->FindWithTag(Tag::StageMap);
	assert(stage && "SelectPlayerControl::Start stage is null");
	stageMap = stage->GetComponent<StageMap>();
	assert(stageMap && "SelectPlayerControl::Start stageMap is null");

	// ステージポイントの初期化
	currentStagePoint = stageMap->GetStagePoint(StageId::Stage1);
	nextStagePoint = nullptr;

	currentState = Idle;
}

// 更新
void SelectPlayerControl::Update()
{
	switch (currentState)
	{
	case SelectPlayerControl::Idle:
		UpdateIdle();
		break;
	case SelectPlayerControl::Moving:
		MoveToStagePoint();
		break;
	default:
		assert(false && "SelectPlayerControl::Update invalid state");
		break;
	}
}

// 破棄
void SelectPlayerControl::OnDestroy()
{
	nextStagePoint = nullptr;
	currentStagePoint = nullptr;
	stageMap = nullptr;
}

// 待機状態の更新
void SelectPlayerControl::UpdateIdle()
{
	// 入力による移動確認
	Direction inputDirection = InputMove();

	// 入力なし
	if (inputDirection == Direction::Invalid) return; 

	// 移動可能か確認
	if (CanMove(inputDirection))
	{
		currentState = Moving;
		return;
	}

	// 決定ボタンの確認
}

// 入力による移動
Direction SelectPlayerControl::InputMove()
{


	return Direction::Invalid;
}

// 移動できるか確認
bool SelectPlayerControl::CanMove(Direction direction)
{
	// 現在のステージポイントの繋がっているステージ情報を取得
	std::vector<ConnectedStage> connectedStages = currentStagePoint->connectedStages;

	// 繋がっているステージを確認
	for (const ConnectedStage& connectedStage : connectedStages)
	{
		// 指定された方向に繋がっているステージがあるか確認
		if (connectedStage.direction == direction)
		{
			// 次のステージポイントを設定
			StageId nextStageId = connectedStage.stageId;

			// ステージマップから次のステージポイントを取得
			// nextStagePointにセット
			nextStagePoint = stageMap->GetStagePoint(nextStageId);

			return true; // 移動可能
		}
	}

	return false; // 指定された方向に繋がっているステージがない
}

// ステージポイントへ移動
void SelectPlayerControl::MoveToStagePoint()
{
	// 線形補間で移動処理を書く
}
