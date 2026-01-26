// PrefabSet.h （ゲーム固有）

#pragma once
#include <cstdint>

// このゲームで使う Prefab の種類
enum class PrefabId : std::uint8_t
{
    // Grade_1
    Swing,
    Akican,
    Desktop,
    Gomibukuro,
    Isu,
    Kuukseizixyoukii,
    Ningixyou,
    SagaraDoll,

    // Grade_2
    Benti,
    Aircon,
    Jihanki,
    Reizouko,

	// Grade_3
    Ufo,
    Eisei,

	// その他
    ProtPlay,
    GlassShard,

    StageWall,

    Count
};

// 全 Prefab を登録する（呼び出しタイミングはゲーム初期化時など）
void RegisterAllPrefabs();
