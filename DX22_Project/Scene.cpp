#include "Scene.h"
#include "SceneManager.h"

//====================== ctor / dtor ======================

Scene::Scene()
    : onLoad(false)
    , onUnload(false)
    , loadTasksInitialized(false)
    , currentTaskIndex(0)
{
}

Scene::~Scene() = default;

//====================== Root 系 ==========================

// 同期ロード：全タスクが終わるまで StepLoad を回す
void Scene::RootLoad()
{
    if (onLoad) return;

    // 新しくタスクを構築し直す
    loadTasksInitialized = false;
    loadTasks.clear();
    currentTaskIndex = 0;

    // 全タスク完了まで同期で回す
    while (!StepLoad())
    {
        // 何もしない（StepLoad 内で 1 タスクずつ進める）
    }
}

void Scene::RootUpdate()
{
    // World の更新 → シーン独自の更新
    world.Update();
    Update();
}

void Scene::RootDraw()
{
    // 描画はシーン側に任せる
    Draw();
    world.Draw();
}

void Scene::RootUnload()
{
    if (onUnload) return;

    onUnload = true;

    // ユーザー実装の解放処理
    Unload();

    // World の中身を全破棄
    world.ClearAll();

    // 再ロードできるようにフラグを戻しておく（必要に応じて）
    onLoad = false;
    loadTasksInitialized = false;
    loadTasks.clear();
    currentTaskIndex = 0;
}

//====================== 非同期ロード =====================

// 既定のタスク構築:
//   1) world.ClearAll()
//   2) Load()
//   3) world.Start()
void Scene::BuildLoadTasks(std::vector<LoadTask>& outTasks)
{
    outTasks.emplace_back([this]
        {
            world.ClearAll();
        });

    outTasks.emplace_back([this]
        {
            this->Load();
        });

    outTasks.emplace_back([this]
        {
            world.Start();
        });
}


void Scene::EnsureLoadTasksInitialized()
{
    if (loadTasksInitialized) return;

    loadTasks.clear();
    BuildLoadTasks(loadTasks);
    currentTaskIndex = 0;
    loadTasksInitialized = true;
}

// 1 回呼ぶごとに 1 タスクだけ実行
bool Scene::StepLoad()
{
    // 初回呼び出し時にタスク構築
    EnsureLoadTasksInitialized();

    if (currentTaskIndex >= loadTasks.size())
    {
        onLoad = true;
        return true; // すでに完了
    }

    // 今回分のタスクを実行
    LoadTask& task = loadTasks[currentTaskIndex];
    if (task)
    {
        task();
    }
    ++currentTaskIndex;

    if (currentTaskIndex >= loadTasks.size())
    {
        // 全タスク完了
        onLoad = true;
        return true;
    }

    return false;
}

float Scene::GetLoadProgress() const
{
    if (!loadTasksInitialized || loadTasks.empty())
        return 0.0f;

    float total = static_cast<float>(loadTasks.size());
    float done = static_cast<float>(currentTaskIndex);
    if (done > total) done = total;

    return done / total;
}
