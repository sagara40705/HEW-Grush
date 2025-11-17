#pragma once
#include "World.h"
#include "Scene.h"
class Game
{
public:
    // ゲーム全体で共有する World
    static World world;

private:
    // 現在のシーン
    static Scene* s_currentScene;
    // 次フレームに切り替えるために予約されたシーン
    static Scene* s_nextScene;

public:
    // 最初のシーンを渡して初期化
    // 例: Game::Init(new SceneGame());
    static void Init(Scene* firstScene);

    static void Uninit();

    // 毎フレーム呼ぶ
    static void Update();
    static void Draw();

    // シーン切り替え予約
    // 例: Game::RequestSceneChange(new DebugScene());
    static void RequestSceneChange(Scene* nextScene);

    // World へのアクセサ（W の代わりに使う想定）
    static World& GetWorld() { return world; }

private:
    // Update の最後で呼ぶ：予約があれば実際に切り替える
    static void ApplySceneChange();
};