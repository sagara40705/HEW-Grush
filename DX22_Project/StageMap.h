#pragma once
#include "Component.h"
#include "StagePoint.h"
#include "TextureManager.h"
#include <array>

enum class Stagelevel : std::uint8_t
{
	Easy,
	level1,
	level2,
	level3,
};
// StageSelect全体の管理
class StageMap : public Component
{
private:
	//ステージポイントの情報
	std::array<StagePoint, ToIndex(StageId::Count)> stagePoints;
public:
	void Start() override;
	void OnDestroy() override;
	void Update();
	
	StagePoint* GetStagePoint(StageId stageId) { return &stagePoints[ToIndex(stageId)]; }
};
