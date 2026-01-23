#pragma once
#include "Scene.h"
#include "Component.h"
#include <cstdint>

class Result : public Scene
{
public:
    Result() = default;
    ~Result() override = default;
    //TimerUI* timerUI_;
   // GameObject* gameObject = nullptr;  // 所有者
    void Load() override;
    void Update() override;
    void Draw() override;
    void Unload() override;

    // 非同期ロード用
    void BuildLoadTasks(std::vector<LoadTask>& outTasks) override;

    void TimerUIDeaw();
    void GaugeDraw();
    void CanvasDraw();
    void RankDraw();

private:
    void CreateCanvas();
    void CreateBace();
    void CreateTimeUI();
	void CreateGaugeUI();
	void CreateRankUI();
    void CreateJobUI();
    void CreateNextText();
};

