#include "SelectPlayerControl.h"
#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "InputKeyboard.h"
#include "DebugLog.h"
#include "GameState.h"
#include <assert.h>

namespace
{
	// World::Update が fixedDt=1/60 で回っているため、ここも同じ前提にする
	constexpr float kFixedDt = 1.0f / 60.0f;
	constexpr float kAxisEps = 1e-3f;
}


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

	// Transformコンポーネントの取得
	tf = gameObject->GetComponent<Transform>();
	assert(tf && "SelectPlayerControl::Start tf is null");

	// 移動補間初期化
	isSegmentMoving = false;
	segmentElapsed = 0.0f;
	segmentTargetPoint = nullptr;

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
	DebugLog::OutputValue("Input Direction dir = %d", static_cast<int>(inputDirection));

	// 移動可能か確認
	if (CanMove(inputDirection))
	{
		currentState = Moving;
		return;
	}

	// 決定ボタンの確認
	if (InputKeyboard::GetKeyDown(VK_SPACE))
	{
		// ゲームシーンへ移動
		SetSelectedStage(currentStagePoint->stageId);
		SceneManager::ChangeSceneAsync<SceneGame>();
		return;
	}
}

// 入力による移動
Direction SelectPlayerControl::InputMove()
{
	if (InputKeyboard::GetKeyDown(VK_LEFT)) return Direction::Left;
	if (InputKeyboard::GetKeyDown(VK_RIGHT)) return Direction::Right;
	if (InputKeyboard::GetKeyDown(VK_UP)) return Direction::Up;
	if (InputKeyboard::GetKeyDown(VK_DOWN)) return Direction::Down;

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

// 指定方向へ繋がっている隣接 StagePoint を取得
StagePoint* SelectPlayerControl::GetConnectedPoint(Direction direction) const
{
	if (!currentStagePoint || !stageMap) return nullptr;

	const std::vector<ConnectedStage>& connectedStages = currentStagePoint->connectedStages;
	for (const ConnectedStage& c : connectedStages)
	{
		if (c.direction == direction)
		{
			return stageMap->GetStagePoint(c.stageId);
		}
	}
	return nullptr;
}

// 目標へ近づく方向を優先度（上→右→左→下）で判定して、次の隣接点を返す
StagePoint* SelectPlayerControl::DecideNextHopTowardGoal(StagePoint* goal) const
{
	if (!currentStagePoint || !goal) return nullptr;

	const Vector3 cur = currentStagePoint->position;
	const Vector3 dst = goal->position;

	const float dx = dst.x - cur.x;
	const float dz = dst.z - cur.z;

	const bool wantUp = (dz > kAxisEps);
	const bool wantDown = (dz < -kAxisEps);
	const bool wantRight = (dx > kAxisEps);
	const bool wantLeft = (dx < -kAxisEps);

	// 優先度：上 → 右 → 左 → 下
	if (wantUp)
	{
		if (StagePoint* p = GetConnectedPoint(Direction::Up)) return p;
	}
	if (wantRight)
	{
		if (StagePoint* p = GetConnectedPoint(Direction::Right)) return p;
	}
	if (wantLeft)
	{
		if (StagePoint* p = GetConnectedPoint(Direction::Left)) return p;
	}
	if (wantDown)
	{
		if (StagePoint* p = GetConnectedPoint(Direction::Down)) return p;
	}

	// 目標方向に進めない（接続がない）
	return nullptr;
}

// 1区間移動を開始
void SelectPlayerControl::BeginSegment(StagePoint* to)
{
	assert(tf);
	assert(currentStagePoint);
	assert(to);

	segmentTargetPoint = to;

	segmentStartPos = currentStagePoint->position;
	segmentEndPos = to->position;

	segmentElapsed = 0.0f;
	isSegmentMoving = true;

	// 現在位置がズレていた時のため、開始点へ寄せる
	tf->SetPosition(segmentStartPos);
}

// 線形補間でステージポイントへ移動
void SelectPlayerControl::MoveToStagePoint()
{
	// 目標がないなら終了
	if (!nextStagePoint || !currentStagePoint || !tf)
	{
		currentState = Idle;
		isSegmentMoving = false;
		segmentTargetPoint = nullptr;
		nextStagePoint = nullptr;
		return;
	}

	// すでに到達済み
	if (currentStagePoint == nextStagePoint)
	{
		currentState = Idle;
		isSegmentMoving = false;
		segmentTargetPoint = nullptr;
		nextStagePoint = nullptr;
		return;
	}

	// セグメント未開始なら、次の1手（十字方向）を決めて開始
	if (!isSegmentMoving)
	{
		StagePoint* hop = DecideNextHopTowardGoal(nextStagePoint);
		if (!hop)
		{
			// 進めない場合は安全に停止（要件外：迂回はしない）
			DebugLog::OutputValue("SelectPlayerControl: cannot move toward goal. goal=%d", static_cast<int>(nextStagePoint->stageId));
			currentState = Idle;
			nextStagePoint = nullptr;
			return;
		}

		BeginSegment(hop);
	}

	// 線形補間でセグメント移動
	segmentElapsed += kFixedDt;
	const float t = (segmentDuration <= 1e-6f) ? 1.0f : (segmentElapsed / segmentDuration);

	tf->SetPosition(Vector3::Lerp(segmentStartPos, segmentEndPos, t));

	// セグメント完了
	if (t >= 1.0f)
	{
		tf->SetPosition(segmentEndPos);
		currentStagePoint = segmentTargetPoint;

		DebugLog::OutputValue("Player move stage point id = %d", static_cast<int>(currentStagePoint->stageId));

		// セグメントリセット
		isSegmentMoving = false;
		segmentTargetPoint = nullptr;

		// ゴール到達まで Moving のまま
		if (currentStagePoint == nextStagePoint)
		{
			nextStagePoint = nullptr;
			currentState = Idle;
		}
		else
		{
			currentState = Moving;
		}
	}
}