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

    Benti,
    Aircon,
    Jihanki,
    Reizouko,

    Ufo,
    Eisei,

    ProtPlay,
    GlassShard,
    Count
};

// 全 Prefab を登録する（呼び出しタイミングはゲーム初期化時など）
void RegisterAllPrefabs();
