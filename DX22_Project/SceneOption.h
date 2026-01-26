#pragma once
#include "Scene.h"

class SceneOption : public Scene
{
public:
    SceneOption() = default;
    ~SceneOption() override = default;

    void Load() override;
    void Update() override;
    void Draw() override;
    void Unload() override;
private:
	void TexturesLoad();
	void CheckCanvas();
    void CreateBG();
};