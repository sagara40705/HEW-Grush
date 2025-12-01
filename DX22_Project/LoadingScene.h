#pragma once
#include "Scene.h"
#include <memory>

// ローディング画面用シーン
class LoadingScene : public Scene
{
public:
    LoadingScene();
    ~LoadingScene() override;

    // このシーン自身のリソース読み込み
    void Load() override;

    // ローディング演出 + 次シーンの StepLoad
    void Update() override;

    // ローディング画面の描画
    void Draw() override;

    // このシーン自身の解放
    void Unload() override;

    // 次に遷移するシーンを渡す
    void SetNextScene(std::unique_ptr<Scene> next);

    // 0.0 ～ 1.0 の進捗率（描画用）
    float GetProgress() const { return progress; }

private:
    std::unique_ptr<Scene> nextScene;   // ロード対象の本命シーン
    bool                   started;     // 非同期ロード開始済みか
    float                  progress;    // 進捗率（次シーンの GetLoadProgress を反映）

    static constexpr int TasksPerFrame = 1; // 1 フレームに何タスク進めるか
};
