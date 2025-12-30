#pragma once
#include "GameState.h"
#include <vector>

//方向の列挙型
enum Direction
{
	Up = 0,
	Down = 1,
	Left = 2,
	Right = 3,
	Invalid = 4
};

//繋がっているステージの情報
struct ConnectedStage
{
	Direction direction = Invalid;	//方向
	StageId stageId;				//繋がっているステージID
};

//SelectSceneで使用するステージポイントの情報
struct StagePoint
{
	StageId stageId;							 //ステージID
	std::vector<ConnectedStage> connectedStages; //繋がっているステージの情報
};