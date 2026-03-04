#include "StageMap.h"
#include "GameState.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
namespace
{
	// ポイントごとに位置
	Vector3 stagePositions[ToIndex(StageId::Count)] =
	{
		{ -5.f, 0.f,  0.f }, // Stage1
		{ -1.f, 0.f,  0.f },  // Stage2
		{  4.f, 0.f,  3.f },  // Stage3a
		{  4.f, 0.f,  0.f },  // Stage3b
		{  4.f, 0.f, -3.f }  // Stage3c
	};

	Vector2 stageUIPositions[ToIndex(StageId::Count)] =
	{
		{ 180.f, 360.f }, // Stage1
		{ 540.f, 360.f }, // Stage2
		{ 1000.f, 140.f }, // Stage3a
		{ 1000.f, 360.f }, // Stage3b
		{ 1000.f, 580.f }  // Stage3c
	};
}


void StageMap::Start()
{
	//ステージポイントの初期化
	//Stage1
	stagePoints[ToIndex(StageId::Stage1)].stageId = StageId::Stage1;
	stagePoints[ToIndex(StageId::Stage1)].position = stagePositions[ToIndex(StageId::Stage1)];
	stagePoints[ToIndex(StageId::Stage1)].uiPosition = stageUIPositions[ToIndex(StageId::Stage1)];
	ConnectedStage stage1To2 = { Direction::Right,StageId::Stage2 };
	stagePoints[ToIndex(StageId::Stage1)].connectedStages.push_back(stage1To2);
	
	
	//Stage2
	stagePoints[ToIndex(StageId::Stage2)].stageId = StageId::Stage2;
	stagePoints[ToIndex(StageId::Stage2)].position = stagePositions[ToIndex(StageId::Stage2)];
	stagePoints[ToIndex(StageId::Stage2)].uiPosition = stageUIPositions[ToIndex(StageId::Stage2)];
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
	stagePoints[ToIndex(StageId::Stage3a)].position = stagePositions[ToIndex(StageId::Stage3a)];
	stagePoints[ToIndex(StageId::Stage3a)].uiPosition = stageUIPositions[ToIndex(StageId::Stage3a)];
	ConnectedStage stage3aTo2 = { Direction::Left,StageId::Stage2 };
	ConnectedStage stage3aTo3b = { Direction::Down,StageId::Stage3b };
	stagePoints[ToIndex(StageId::Stage3a)].connectedStages.push_back(stage3aTo2);
	stagePoints[ToIndex(StageId::Stage3a)].connectedStages.push_back(stage3aTo3b);

	//Stage3b
	stagePoints[ToIndex(StageId::Stage3b)].stageId = StageId::Stage3b;
	stagePoints[ToIndex(StageId::Stage3b)].position = stagePositions[ToIndex(StageId::Stage3b)];
	stagePoints[ToIndex(StageId::Stage3b)].uiPosition = stageUIPositions[ToIndex(StageId::Stage3b)];
	ConnectedStage stage3bTo2 = { Direction::Left,StageId::Stage2 };
	ConnectedStage stage3bTo3a = { Direction::Up,StageId::Stage3a };
	ConnectedStage stage3bTo3c = { Direction::Down,StageId::Stage3c };
	stagePoints[ToIndex(StageId::Stage3b)].connectedStages.push_back(stage3bTo2);
	stagePoints[ToIndex(StageId::Stage3b)].connectedStages.push_back(stage3bTo3a);
	stagePoints[ToIndex(StageId::Stage3b)].connectedStages.push_back(stage3bTo3c);

	//Stage3c
	stagePoints[ToIndex(StageId::Stage3c)].stageId = StageId::Stage3c;
	stagePoints[ToIndex(StageId::Stage3c)].position = stagePositions[ToIndex(StageId::Stage3c)];
	stagePoints[ToIndex(StageId::Stage3c)].uiPosition = stageUIPositions[ToIndex(StageId::Stage3c)];
	ConnectedStage stage3cTo2 = { Direction::Left,StageId::Stage2 };
	ConnectedStage stage3cTo3b = { Direction::Up,StageId::Stage3b };
	stagePoints[ToIndex(StageId::Stage3c)].connectedStages.push_back(stage3cTo2);
	stagePoints[ToIndex(StageId::Stage3c)].connectedStages.push_back(stage3cTo3b);
}

void StageMap::OnDestroy()
{
}

void StageMap::Update()
{
	
	
}
