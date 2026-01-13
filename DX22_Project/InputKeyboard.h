#pragma once
#include <Windows.h>
#undef max
#undef min

class InputKeyboard
{
public:
    // 初期化
    //  - 内部の now/prev バッファをクリア
    //  - 初回の GetKeyboardState を呼んで now/prev を揃える
    static HRESULT Init();

    // 終了処理
    //  - 現状は何もしない（将来 RawInput 等を導入するならここで解放）
    static void    Uninit();

    // 毎フレームの更新
    //  - prev ← now をコピー
    //  - GetKeyboardState() で now を取得
    static void    Update();

    //==================== キー状態 ====================

    // 押されている間 true
    //  (Unity: Input.GetKey)
    static bool GetKey(int vk);

    // 押した瞬間だけ true（立ち上がりエッジ）
    //  (Unity: Input.GetKeyDown)
    static bool GetKeyDown(int vk);

    // 離した瞬間だけ true（立ち下がりエッジ）
    //  (Unity: Input.GetKeyUp)
    static bool GetKeyUp(int vk);

    // 生のバッファが必要な場合のためのアクセサ（任意）
    static const BYTE* GetNowBuffer() { return s_now; }
    static const BYTE* GetPrevBuffer() { return s_prev; }

private:
    // 現在 & 前フレームのキー状態（GetKeyboardState の内容そのまま）
    static BYTE s_now[256];
    static BYTE s_prev[256];

    // 内部ヘルパ
    static bool IsDownNow(int vk);
    static bool WasDownPrev(int vk);
};
