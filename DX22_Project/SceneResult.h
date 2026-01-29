#pragma once
#include "Scene.h"
#include "Component.h"
#include <cstdint>

class SceneResult : public Scene
{
public:
    SceneResult() = default;
    ~SceneResult() override = default;
    //TimerUI* timerUI_;
   // GameObject* gameObject = nullptr;  // 所有者
    void Load() override;
    void Update() override;
    void Draw() override;
    void Unload() override;

    // 非同期ロード用
    void BuildLoadTasks(std::vector<LoadTask>& outTasks) override;

private:
    void CreateCanvas();
    void CreateBace();
    void CreateTimeUI();
	void CreateGaugeUI();
	void CreateRankUI();
    void CreateJobUI();
    void CreateNextText();

    bool m_CheckResultSE;
};

