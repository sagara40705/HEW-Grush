#pragma once
#include "Component.h"
#include "StagePoint.h"
#include <array>

// StageSelect全体の管理
class StageMap : public Component
{
private:
	//ステージポイントの情報
	std::array<StagePoint, ToIndex(StageId::Count)> stagePoints;
public:
	void Start() override;
	void OnDestroy() override;
};
