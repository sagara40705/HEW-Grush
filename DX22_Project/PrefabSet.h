// PrefabSet.h （ゲーム固有）

#pragma once
#include <cstdint>

// このゲームで使う Prefab の種類
enum class PrefabId : std::uint8_t
{
    Swing,
    Benti,
    Ufo,
    ProtPlay,
    Count
};

// 全 Prefab を登録する（呼び出しタイミングはゲーム初期化時など）
void RegisterAllPrefabs();
