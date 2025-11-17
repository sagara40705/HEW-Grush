#include "ActionUIManager.h"
#include "Game.h"
#include "PlayerControll.h"

ActionUIManager::ActionUIManager(CanvasUI* spin, CanvasUI* dash)
{
    ui[Action::Spining] = spin;
    ui[Action::Dash] = dash;
}

void ActionUIManager::Start()
{
    // === PlayerControllの取得 ===
    for (auto& o : Game::GetWorld().objects)
    {
        if (!o->active) continue;
        if (!o->CompareTag(Tag::Player)) continue;
        pc = o->TryGetComponent<PlayerControll>();
        if (pc) break;
    }
    // UI 初期設定（位置と横幅だけ決めて、縦は 0 にしておく）
    for (int i = 0; i < Action::Count; ++i)
    {
        CanvasUI* canvas = ui[i];
        if (!canvas) continue;

        // 配置位置
        canvas->SetOffset(kNumberPos[i]);

        // サイズ：横幅は一定、縦は最初 0（空のゲージ）
        Vector2 size = kNumberSize;
        size.y = 0.0f;
        canvas->SetSize(size);

        // UV：X は全体を使い、Y は「下端から高さ 0」の状態にしておく
        canvas->SetUVPos({ 0.0f, 1.0f }); // 下端
        canvas->SetUVScale({ 1.0f, 0.0f }); // 高さ 0
    }
}

void ActionUIManager::OnDestroy()
{
    for (int i = 0; i < Action::Count; ++i)
    {
        ui[i] = nullptr;
    }
    pc = nullptr;
}

void ActionUIManager::Update()
{
    //Ctの割合を取得
    float ctr[Action::Count];
    ctr[Action::Spining] = pc->GetSpinCtRatio();
    ctr[Action::Dash] = pc->GetDashCtRatio();

    //UI情報を更新
    for (int action = 0; action < Action::Count; ++action)
    {
        CanvasUI* canvas = ui[action];
        if (!canvas) continue;

        float r = std::clamp(ctr[action], 0.0f, 1.0f);   // 0〜1 にクランプ

        // ゲージの「ピクセル高さ」（見た目上どれくらい伸ばすか）
        constexpr float kGaugePixelHeight = 140.5f;

        // 下端基準で伸び縮みさせる
        const float baseX = kNumberPos[action].x;
        const float baseY = kNumberPos[action].y;

        // オフセット：上側を切っていくので、比率に応じて下から持ち上げる
        canvas->SetOffset({ baseX, baseY + kGaugePixelHeight * (1.0f - r) });

        // サイズ：縦方向だけ割合を掛ける
        Vector2 size = kNumberSize;
        size.y *= r;
        canvas->SetSize(size);

        // UV：テクスチャ全体を使う前提で、Y だけをトリミング
        //  下端はそのまま固定し、上側を (1 - r) だけ削るイメージ
        Vector2 uvPos = { 0.0f, 1.0f - r }; // 上側を下げる
        Vector2 uvSize = { 1.0f, r };        // 高さは割合 r

        canvas->SetUVPos(uvPos);
        canvas->SetUVScale(uvSize);
    }
}
