// SceneStory.cpp
// SceneStory.cpp
#include "SceneStory.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "SceneManager.h"
#include "InputKeyboard.h"
#include "SceneStageSelect.h"

void SceneStory::Load()
{
    // SceneStory に必要なロード処理を全部ここで実行する
    // (あなたの既存構造に合わせて BuildLoadTasks を呼び出す)
    std::vector<std::function<void()>> tasks;
    BuildLoadTasks(tasks);

    // まずは同期で全部実行（コンパイルと動作確認を優先）
    for (auto& t : tasks)
    {
        t();
    }
}

void SceneStory::Draw()
{
    // 多くの構成では World が描画を持っている
    // もし GetWorld().Draw() が存在しない場合は、あなたの他SceneのDrawと同じ形に合わせてください
    GetWorld().Draw();
}

void SceneStory::Unload()
{
    // ここもエンジン側の作法があるはずなので、まずは空でOK
    // 必要なら World の GameObject をクリアする処理に差し替え
    // GetWorld().Clear(); などがあるなら呼ぶ
}

void SceneStory::BuildLoadTasks(std::vector<std::function<void()>>& outTasks)
{
    // ストーリー用テクスチャをロード
    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro1); });
    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro2); });
    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro3); });
    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro4); });

    // UI生成
    outTasks.emplace_back([this] { CreateIntro(); });
}

void SceneStory::CreateIntro()
{
    World& w = GetWorld();

    GameObject& obj = w.CreateGameObject(Vector3(0.f, 0.f, 0.f));

    // 画面サイズは既存の Title_Fadein と同じに合わせる想定
    auto& r = obj.AddComponent<UiSpriteRenderer>(
        m_introTags[0],
        Vector2(0.0f, 0.0f),
        Vector2(1500.0f, 900.0f)
    );

    m_introObj = &obj;
    m_introRenderer = &r;

    m_index = 0;
    m_timer = 0.0f;
}

void SceneStory::Update()
{
    // dt取得があるならそれを使って下さい。無い場合は固定でOK
    const float DT = 1.0f / 60.0f;

    // 入力スキップ：スペースで次へ
    if (InputKeyboard::GetKeyDown(VK_SPACE))
    {
        NextOrFinish();
        return;
    }

    m_timer += DT;
    if (m_timer < m_interval)
        return;

    m_timer = 0.0f;
    NextOrFinish();
}

void SceneStory::NextOrFinish()
{
    m_index++;

    // 次の画像があるなら差し替え
    if (m_index < (int)m_introTags.size())
    {
        if (m_introRenderer)
        {
            m_introRenderer->SetTexture(m_introTags[m_index]);
        }
        return;
    }

    // 全部終わったら StageSelect へ
    SceneManager::ChangeSceneAsync<SceneStageSelect>();
}