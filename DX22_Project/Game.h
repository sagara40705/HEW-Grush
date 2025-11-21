#pragma once
#include "Scene.h"
#include <cstdint>
#include <vector>
#include <memory>

class Game
{
public:
    // ゲーム全体で共有する World
    static World world;

private:
    static std::vector<std::unique_ptr<Scene>> s_scenes;
    static std::unique_ptr<Scene> s_nextScene;

public:
    // 最初のシーンを渡して初期化
    // 例: Game::Init(new SceneGame());
    static void Init();
    static void Uninit();
    // 毎フレーム呼ぶ
    static void Update();
    static void Draw();
    // World へのアクセサ（W の代わりに使う想定）
    static World& GetWorld();

public:
    static void LoadScene();
    static void PreLoadNext();
    static void StackScene();
    static void PopScene();
private:
    static void SceneStackClear();
public:
    template<class T>
    static void SetNext()
    {
        static_assert(std::is_base_of_v<Scene, T>, "T must derive from Scene");
        if (s_nextScene)
        {
            s_nextScene->RootUnload();
        }
        s_nextScene = std::make_unique<T>();
    }

};