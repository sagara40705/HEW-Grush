#pragma once
#include <cstdint>

//====================== サウンド共通型定義 ======================

// Cue / バス / 再生インスタンスなどの ID。
// 実装側では enum やハッシュなどに差し替えても良い。
using SoundCueId = std::int32_t;   // 再生したいキュー（SE/BGM）を指す ID
using SoundBusId = std::int32_t;   // カテゴリ / バス ID（必要なら使用）
using SoundHandle = std::int32_t;   // 再生中インスタンスを指すハンドル

constexpr SoundCueId   InvalidSoundCueId = -1;
constexpr SoundBusId   InvalidSoundBusId = -1;
constexpr SoundHandle  InvalidSoundHandle = -1;

// カテゴリ（バス）
// ミドルウェア側の Bus / Category にマッピングして使う想定。
enum class SoundCategory : std::uint8_t
{
    BGM = 0,
    SE,
    Voice,
    UI,
    Ambient,

    Count,          // 配列確保用の終端
};

