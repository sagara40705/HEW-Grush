#pragma once
#include "Component.h"
#include "StageMap.h"

//ステージセレクトsceneにおけるプレイヤーの管理
class SelectPlayerControl : public MonoBehaviour
{
private:
	StageMap* stageMap = nullptr; //ステージマップのポインタ
	StagePoint* currentStagePoint = nullptr; //現在いるステージポイントのポインタ
	Direction moveDirection = Direction::Invalid; //移動方向
};