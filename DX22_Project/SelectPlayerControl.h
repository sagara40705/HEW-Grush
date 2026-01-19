#pragma once
#include "Component.h"
#include "StageMap.h"

//ステージセレクトsceneにおけるプレイヤーの管理
class SelectPlayerControl : public MonoBehaviour
{
private:
	enum State
	{
		Idle,	   //待機状態
		Moving,	   //移動状態
	};
	State currentState = Idle; //現在の状態
private:
	StageMap* stageMap = nullptr; //ステージマップのポインタ
	StagePoint* currentStagePoint = nullptr; //現在いるステージポイントのポインタ
	Transform* tf = nullptr; //Transformコンポーネントのポインタ
private:
	// 1区間（1回の十字移動）分の補間
	bool  isSegmentMoving = false;
	float segmentElapsed = 0.0f;	// 1区間の経過時間（秒）
	float segmentDuration = 0.18f;	// 1区間の移動時間（秒）
	Vector3 segmentStartPos{};
	Vector3 segmentEndPos{};
	StagePoint* segmentTargetPoint = nullptr;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	// 状態更新関数
	void UpdateIdle();
	// 入力を受け付ける
	Direction InputMove();
	// 移動できるか確認
	bool CanMove(Direction direction);

	StagePoint* nextStagePoint = nullptr; //次に移動するステージポイントのポインタ
private:
	void MoveToStagePoint(); //線形補間でステージポイントへ移動
private:
	// 現在地点から指定方向へ繋がっている隣接 StagePoint を取得（なければ nullptr）
	StagePoint* GetConnectedPoint(Direction direction) const;
	// 目標へ近づく方向を優先度（上→右→左→下）で判定して、次の隣接点を返す
	StagePoint* DecideNextHopTowardGoal(StagePoint* goal) const;
	// 1区間移動を開始
	void BeginSegment(StagePoint* to);
};