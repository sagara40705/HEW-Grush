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
    void TimerUIDeaw();
	void GaugeDraw();
    // 非同期ロード用
    void BuildLoadTasks(std::vector<LoadTask>& outTasks) override;
private:
   /* UiSpriteRenderer* uiMinute_[ToIndex(TimerDigits::Count)]{};
    UiSpriteRenderer* uiColon_ = nullptr;
    UiSpriteRenderer* uiSecond_[ToIndex(TimerDigits::Count)]{};*/
};


