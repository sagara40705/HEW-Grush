#include "SelectPlayerControl.h"
#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "InputKeyboard.h"
#include "DebugLog.h"
#include "GameState.h"

#include <assert.h>
#include <algorithm>

namespace
{
    // World::Update が fixedDt=1/60 で回っている前提
    constexpr float kFixedDt = 1.0f / 60.0f;
}

void SelectPlayerControl::Start()
{
    GameObject* stage = gameObject->FindWithTag(Tag::StageMap);
    assert(stage && "SelectPlayerControl::Start stage is null");
    stageMap = stage->GetComponent<StageMap>();
    assert(stageMap && "SelectPlayerControl::Start stageMap is null");

    currentStagePoint = stageMap->GetStagePoint(StageId::Stage1);
    nextStagePoint = nullptr;

    tf = gameObject->GetComponent<Transform>();
    assert(tf && "SelectPlayerControl::Start tf is null");

    if (currentStagePoint)
    {
        tf->SetPosition(currentStagePoint->position);
    }

    isMoving = false;
    moveElapsed = 0.0f;
    moveTargetPoint = nullptr;

    currentState = Idle;
}

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

void SelectPlayerControl::OnDestroy()
{
    nextStagePoint = nullptr;
    moveTargetPoint = nullptr;
    currentStagePoint = nullptr;
    stageMap = nullptr;
    tf = nullptr;
}

void SelectPlayerControl::UpdateIdle()
{
    // 外部（クリック等）で目標がセットされている場合は移動開始
    if (nextStagePoint && currentStagePoint && nextStagePoint != currentStagePoint)
    {
        BeginMove(nextStagePoint);
        currentState = Moving;
        return;
    }

    // 入力による移動確認（隣接点へ）
    Direction inputDirection = InputMove();

    if (inputDirection != Direction::Invalid)
    {
        DebugLog::OutputValue("Input Direction dir = %d", static_cast<int>(inputDirection));

        if (CanMove(inputDirection))
        {
            BeginMove(nextStagePoint);
            currentState = Moving;
            return;
        }
    }

    // 決定
    if (InputKeyboard::GetKeyDown(VK_SPACE))
    {
        SetSelectedStage(currentStagePoint->stageId);
        SceneManager::ChangeSceneAsync<SceneGame>();
        return;
    }

	if (InputKeyboard::GetKeyDown('T'))
	{
		// タイトルへ戻る
		SceneManager::ChangeSceneAsync<SceneTitle>();
		return;
	}
}

Direction SelectPlayerControl::InputMove()
{
    if (InputKeyboard::GetKeyDown(VK_LEFT))  return Direction::Left;
    if (InputKeyboard::GetKeyDown(VK_RIGHT)) return Direction::Right;
    if (InputKeyboard::GetKeyDown(VK_UP))    return Direction::Up;
    if (InputKeyboard::GetKeyDown(VK_DOWN))  return Direction::Down;

    return Direction::Invalid;
}

bool SelectPlayerControl::CanMove(Direction direction)
{
    if (!currentStagePoint || !stageMap) return false;

    const std::vector<ConnectedStage>& connectedStages = currentStagePoint->connectedStages;

    for (const ConnectedStage& connectedStage : connectedStages)
    {
        if (connectedStage.direction == direction)
        {
            nextStagePoint = stageMap->GetStagePoint(connectedStage.stageId);
            return (nextStagePoint != nullptr);
        }
    }

    return false;
}

void SelectPlayerControl::BeginMove(StagePoint* goal)
{
    if (!goal || !tf)
    {
        isMoving = false;
        moveTargetPoint = nullptr;
        moveElapsed = 0.0f;
        return;
    }

    moveTargetPoint = goal;

    // 現在位置から目標位置までをそのまま線形補間（斜めも許可）
    moveStartPos = tf->GetPosition();
    moveEndPos = goal->position;

    moveElapsed = 0.0f;
    isMoving = true;
}

void SelectPlayerControl::MoveToStagePoint()
{
    if (!tf || !currentStagePoint)
    {
        currentState = Idle;
        nextStagePoint = nullptr;
        isMoving = false;
        moveTargetPoint = nullptr;
        return;
    }

    // 目標がないなら終了
    if (!nextStagePoint)
    {
        currentState = Idle;
        isMoving = false;
        moveTargetPoint = nullptr;
        return;
    }

    // すでに到達済み
    if (nextStagePoint == currentStagePoint)
    {
        currentState = Idle;
        nextStagePoint = nullptr;
        isMoving = false;
        moveTargetPoint = nullptr;
        return;
    }

    // 途中で目標が変わった / まだ移動開始していない -> ここで開始
    if (!isMoving || moveTargetPoint != nextStagePoint)
    {
        BeginMove(nextStagePoint);
    }

    // 線形補間
    const float denom = std::max(1e-6f, moveDuration);
    moveElapsed += kFixedDt;
    const float t = std::clamp(moveElapsed / denom, 0.0f, 1.0f);

    tf->SetPosition(Vector3::Lerp(moveStartPos, moveEndPos, t));

    // 完了（完了まで currentState は Moving のまま）
    if (t >= 1.0f)
    {
        tf->SetPosition(moveEndPos);

        currentStagePoint = nextStagePoint;
        DebugLog::OutputValue("Player move stage point id = %d", static_cast<int>(currentStagePoint->stageId));

        nextStagePoint = nullptr;
        isMoving = false;
        moveTargetPoint = nullptr;
        currentState = Idle;
    }
}
