#pragma once
#include "Component.h"
#include "StageMap.h"
#include "Vector3.h"

class SelectPlayerControl : public MonoBehaviour
{
private:
    enum State
    {
        Idle,
        Moving,
    };

private:
    State currentState = Idle;

    StageMap* stageMap = nullptr;
    StagePoint* currentStagePoint = nullptr;
    Transform* tf = nullptr;
	UiSpriteRenderer* ui = nullptr;

    // 目標（外部/入力でセットされる）
    StagePoint* nextStagePoint = nullptr;

    // Lerp移動
    bool isMoving = false;
    float moveElapsed = 0.0f;
    float moveDuration = 0.25f; // 秒
    Vector3 moveStartPos{};
    Vector3 moveEndPos{};
    StagePoint* moveTargetPoint = nullptr;
	Vector2 moveStartUiPos{};
	Vector2 moveEndUiPos{};

public:
    void Start() override;
    void Update() override;
    void OnDestroy() override;

private:
    void UpdateIdle();
    Direction InputMove();
	Direction prevLStickDir = Direction::Invalid;
    bool CanMove(Direction direction);

    // nextStagePoint まで単純に線形補間（斜めOK）
    void MoveToStagePoint();

    void BeginMove(StagePoint* goal);
};
