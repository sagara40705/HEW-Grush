#pragma once
#include "Component.h"

class LoadingScene;

class LoadingGauge : public MonoBehaviour
{
private:
	LoadingScene* loading_ = nullptr;
	UiSpriteRenderer* ui_ = nullptr;
public:
	LoadingGauge(LoadingScene* loading);
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};