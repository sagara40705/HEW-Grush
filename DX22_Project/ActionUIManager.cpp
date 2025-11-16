#include "ActionUIManager.h"
#include "CanvasUI.h"
#include "Game.h"
#include "PlayerControll.h"

ActionUIManager::ActionUIManager(CanvasUI* spin, CanvasUI* dash)
{
    ui[Action::Spining] = spin;
    ui[Action::Dash] = dash;
}

void ActionUIManager::Start()
{
    // === PlayerControll‚Ìæ“¾ ===
    for (auto& o : Game::GetWorld().objects)
    {
        if (!o->active) continue;
        if (!o->CompareTag(Tag::Player)) continue;
        pc = o->TryGetComponent<PlayerControll>();
        if (pc) break;
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
    //Ct‚ÌŠ„‡‚ğæ“¾
    float ctr[Action::Count];
    ctr[Action::Spining] = pc->GetSpinCtRatio();
    ctr[Action::Dash] = pc->GetDashCtRatio();

    //UIî•ñ‚ğXV
    for (int action = 0; action < Action::Count; ++action)
    {
        ui[action]->SetSize({ 0.5 , ctr[action] });

    }
}
