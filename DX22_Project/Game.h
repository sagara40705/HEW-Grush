#pragma once
#include <cstdint>
#include <type_traits>
#include <memory>
#include "Scene.h"
#include "SceneManager.h"

struct GameState;

// ゲーム全体のエントリーポイント的クラス。
// シーン管理自体は SceneManager に委譲する。
class Game
{
private:
    static std::unique_ptr<GameState> s_state;
	static bool CheckTitleSound;
public:
    static GameState* GetState() { return s_state.get(); }
    static void OriginalInit();
    static void OriginalUninit();
public:
    static void Init();     // 初期化
    static void Uninit();   // 終了処理
    static void Update();   // 毎フレーム更新
    static void Draw();     // 毎フレーム描画

    // 既存コードで Game::SetNext<T>() を使っているなら、その互換ラッパ
    template<class T>
    static void SetNext()
    {
        SceneManager::ChangeScene<T>();
    }

    template<class T>
    static void PushScene()
    {
        SceneManager::PushScene<T>();
    }

    static void PopScene()
    {
        SceneManager::PopScene();
    }

private:
    static bool CheckTitleSound;
};
