#pragma once
#include "Component.h"
#include "Time/Timer.h"
#include "Model.h"

class TitlePlyerContoroll : public MonoBehaviour
{
private:
	Transform* hammerTf_ = nullptr;
	Transform* pivotTf_ = nullptr;
	Animator* anime_ = nullptr;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	bool isAnimation_ = false;
	Model::AnimeNo idleAnimeNo_;
	Model::AnimeNo spinAnimeNo_;
	Timer animationTimer_;
	static constexpr float kAnimationTime = 0.9f;
private:
	void AddAnimation();
private:
	void UpdateInput();
	void UpdateAnimation();
};

