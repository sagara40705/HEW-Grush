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
        if (!ui[i]) continue;

        // オフセットと幅
        ui[i]->SetOffset(kNumberPos[i]);
        Vector2 size = kNumberSize;
        size.y = 0.0f;               // 最初はゲージ空
        ui[i]->SetSize(size);

        // UV（X は固定・Y は 0 表示）
        Vector2 uvPos = kNumberUvPos[i];
        Vector2 uvSize = kNumberUVSize;
        uvPos.y = 1.0f;             // 一番下
        uvSize.y = 0.0f;             // 高さ 0
        ui[i]->SetUVPos(uvPos);
        ui[i]->SetUVScale(uvSize);
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

        // 0〜1 にクランプ
        float r = std::clamp(ctr[action], 0.0f, 1.0f);

        // 下側を起点に、割合 r で伸び縮みさせる
        constexpr float kGaugePixelHeight = 200.0f;   // GlassGauge と同じ高さ

        // 位置: X は固定、Y は (1-r) 分だけ上にずらす
        float baseX = kNumberPos[action].x;
        float baseY = kNumberPos[action].y;
        canvas->SetOffset({ baseX, baseY + kGaugePixelHeight * (1.0f - r) });

        // サイズ: 縦方向だけ割合を掛ける
        Vector2 size = kNumberSize;
        size.y *= r;
        canvas->SetSize(size);

        // UV: 下端を固定して縦方向をトリミング
        Vector2 uvPos = kNumberUvPos[action];
        Vector2 uvSize = kNumberUVSize;

        uvPos.y = 1.0f - r;  // 上側を持ち上げる
        uvSize.y = r;         // 高さを割合に

        canvas->SetUVPos(uvPos);
        canvas->SetUVScale(uvSize);
    }
}
