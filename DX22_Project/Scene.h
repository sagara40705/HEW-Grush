#ifndef __SCENE_H__
#define __SCENE_H__

#include <functional>
#include <vector>
#include "World.h"

//--------------------------------------------------
// 基底シーンクラス
//--------------------------------------------------
class Scene
{
public:
    using LoadTask = std::function<void()>;

    Scene();
    virtual ~Scene();

    //===== フレームワーク側から呼ばれる入口 =====
    void RootLoad();    // 同期ロード（全タスク完了まで回す）
    void RootUpdate();  // World 更新 → シーン固有 Update
    void RootDraw();    // シーン固有 Draw
    void RootUnload();  // Unload → World 破棄

    //===== ユーザーが派生クラスで実装するインターフェース =====
    virtual void Load() = 0;  // オブジェクト生成・リソース取得など
    virtual void Update() = 0;  // 毎フレーム更新
    virtual void Draw() = 0;  // 描画
    virtual void Unload() = 0;  // 解放

    //===== 非同期ロード用インターフェース =====
    // ロード処理を細かいタスクに分割して outTasks に追加する。
    // 既定実装:
    //   1) world.ClearAll()
    //   2) Load()
    //   3) world.Start()
    //
    // より細かく分割したいシーンはオーバーライドして、
    // std::function を複数 emplace_back していく。
    virtual void BuildLoadTasks(std::vector<LoadTask>& outTasks);
    
    // Optionから帰って来た時に呼ばれる関数
    virtual void OnReturnFromOption() {}

    // 1 回呼び出すごとに 1 タスクだけ実行。
    // すべてのタスクが完了すると true を返す。
    bool StepLoad();

    // 0.0f ～ 1.0f の進捗率
    float GetLoadProgress() const;

    // この Scene 専用の World へのアクセス
    World& GetWorld() { return world; }
    const World& GetWorld() const { return world; }

private:
    World world;

    bool onLoad;      // RootLoad / StepLoad が完了したか
    bool onUnload;    // RootUnload 済みか

    // 非同期ロード用
    bool                  loadTasksInitialized;
    std::vector<LoadTask> loadTasks;
    std::size_t           currentTaskIndex;

    void EnsureLoadTasksInitialized();
};

#endif // __SCENE_H__
