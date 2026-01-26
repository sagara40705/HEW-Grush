// PrefabSet.h （ゲーム固有）

#pragma once
#include <cstdint>

// このゲームで使う Prefab の種類
enum class PrefabId : std::uint8_t
{
    Swing,
    Akican,
    Desktop,
    Gomibukuro,
    Isu,
    Kuukseizixyoukii,
    Ningixyou,
    SagaraDoll,
    Testp,

    Benti,
    Aircon,
    Jihanki,
    Reizouko,
    AirPurifier,
    fridge,

    Ufo,
    Eisei,
    Star,

    ProtPlay,
    GlassShard,
    Count
};

// 全 Prefab を登録する（呼び出しタイミングはゲーム初期化時など）
void RegisterAllPrefabs();
