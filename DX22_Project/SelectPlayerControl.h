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
};