#include "LoadingScene.h"
#include "SceneManager.h"  // ChangeSceneAlreadyLoaded を使う前提

LoadingScene::LoadingScene()
    : nextScene()
    , started(false)
    , progress(0.0f)
{
}

LoadingScene::~LoadingScene() = default;

void LoadingScene::SetNextScene(std::unique_ptr<Scene> next)
{
    nextScene = std::move(next);
    started = false;
    progress = 0.0f;
}

// このシーン自身のロード
void LoadingScene::Load()
{
    // ローディング画面用のオブジェクト生成などをここで行う。
    // 例:
    //   - カメラ
    //   - 背景スプライト
    //   - プログレスバー用 UI 等
    //
    // World& w = GetWorld();
    // auto* camera = w.CreateGameObject("LoadingCamera");
    // ...

    // 必須処理はないので、何も作らなくても動作自体はする。
}

// 毎フレーム呼ばれる
void LoadingScene::Update()
{
    // このシーン自身のアニメーション等はここに書く。
    // World の Update は Scene::RootUpdate 側で既に呼ばれている。

    if (!nextScene)
    {
        // 次シーンがセットされていない場合は何もしない
        return;
    }

    if (!started)
    {
        // 必要ならここで何か初期化。今回は特に何もしない。
        started = true;
    }

    bool finished = false;

    // 1 フレームに TasksPerFrame 個だけロードタスクを進める
    for (int i = 0; i < TasksPerFrame && !finished; ++i)
    {
        finished = nextScene->StepLoad();
    }

    // 進捗率を取得して保持（描画で使用する想定）
    progress = nextScene->GetLoadProgress();

    if (finished)
    {
        // 全タスク完了 → 本命シーンに切り替え
        //
        // ChangeSceneAlreadyLoaded は、
        //   - 既存スタックを ClearStack()
        //   - RootLoad を呼ばずに nextScene をアクティブにする
        // という実装にしておく。
        SceneManager::ChangeSceneAlreadyLoaded(std::move(nextScene));

        // この LoadingScene 自身は ClearStack の際に RootUnload されて破棄される想定。
    }
}

void LoadingScene::Draw()
{
    // ローディング画面の描画。
    // 実際には World 内 GameObject/Renderer を使って描画することを想定。
    //
    // 例：
    //   - 背景
    //   - progress を使ったプログレスバー
    //
    // ここではエンジン依存が強くなるので、中身は各自の実装に合わせて追加してください。
}

void LoadingScene::Unload()
{
    // ローディング画面用のリソース解放処理
    // nextScene は ChangeSceneAlreadyLoaded 時に move 済みなので基本的に nullptr のはず。
    nextScene.reset();
}
