#include "StageMap.h"

void StageMap::Start()
{
	//ステージポイントの初期化

	//Stage1
	stagePoints[ToIndex(StageId::Stage1)].stageId = StageId::Stage1;
	stagePoints[ToIndex(StageId::Stage1)].position = { -5.0f,0.0f,0.0f };
	ConnectedStage stage1To2 = { Direction::Right,StageId::Stage2 };
	stagePoints[ToIndex(StageId::Stage1)].connectedStages.push_back(stage1To2);

	//Stage2
	stagePoints[ToIndex(StageId::Stage2)].stageId = StageId::Stage2;
	stagePoints[ToIndex(StageId::Stage2)].position = { 0.0f,0.0f,0.0f };
	ConnectedStage stage2To1 = { Direction::Left,StageId::Stage1 };
	ConnectedStage stage2To3a = { Direction::Up,StageId::Stage3a };
	ConnectedStage stage2To3b = { Direction::Right,StageId::Stage3b };
	ConnectedStage stage2To3c = { Direction::Down,StageId::Stage3c };
	stagePoints[ToIndex(StageId::Stage2)].connectedStages.push_back(stage2To1);
	stagePoints[ToIndex(StageId::Stage2)].connectedStages.push_back(stage2To3a);
	stagePoints[ToIndex(StageId::Stage2)].connectedStages.push_back(stage2To3b);
	stagePoints[ToIndex(StageId::Stage2)].connectedStages.push_back(stage2To3c);

	//Stage3a
	stagePoints[ToIndex(StageId::Stage3a)].stageId = StageId::Stage3a;
	stagePoints[ToIndex(StageId::Stage3a)].position = { 0.0f,5.0f,0.0f };
	ConnectedStage stage3aTo2 = { Direction::Left,StageId::Stage2 };
	ConnectedStage stage3aTo3b = { Direction::Down,StageId::Stage3b };
	stagePoints[ToIndex(StageId::Stage3a)].connectedStages.push_back(stage3aTo2);
	stagePoints[ToIndex(StageId::Stage3a)].connectedStages.push_back(stage3aTo3b);

	//Stage3b
	stagePoints[ToIndex(StageId::Stage3b)].stageId = StageId::Stage3b;
	stagePoints[ToIndex(StageId::Stage3b)].position = { 5.0f,0.0f,0.0f };
	ConnectedStage stage3bTo2 = { Direction::Left,StageId::Stage2 };
	ConnectedStage stage3bTo3a = { Direction::Up,StageId::Stage3a };
	ConnectedStage stage3bTo3c = { Direction::Down,StageId::Stage3c };
	stagePoints[ToIndex(StageId::Stage3b)].connectedStages.push_back(stage3bTo2);
	stagePoints[ToIndex(StageId::Stage3b)].connectedStages.push_back(stage3bTo3a);
	stagePoints[ToIndex(StageId::Stage3b)].connectedStages.push_back(stage3bTo3c);

	//Stage3c
	stagePoints[ToIndex(StageId::Stage3c)].stageId = StageId::Stage3c;
	stagePoints[ToIndex(StageId::Stage3c)].position = { 0.0f,-5.0f,0.0f };
	ConnectedStage stage3cTo2 = { Direction::Left,StageId::Stage2 };
	ConnectedStage stage3cTo3b = { Direction::Up,StageId::Stage3b };
	stagePoints[ToIndex(StageId::Stage3c)].connectedStages.push_back(stage3cTo2);
	stagePoints[ToIndex(StageId::Stage3c)].connectedStages.push_back(stage3cTo3b);
}

void StageMap::OnDestroy()
{
}
