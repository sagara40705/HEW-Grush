#pragma once
#include <Windows.h>
#undef max
#undef min
#include <Xinput.h>

//============================================================
// XInput ゲームパッド生入力クラス（1P 専用）
//
// ・毎フレーム Update() を呼ぶ
// ・「操作用コントローラーは 1 機のみ」を前提に 0 番パッドだけ見る
// ・ボタン押下 / 押した瞬間 / 離した瞬間
// ・左右スティック / トリガを [-1,1], [0,1] で返す（デッドゾーン込み）
//
// 長押しリピートやコンボ判定などは、ここでは一切やらない。
//============================================================
class InputGamepad
{
public:
    // 初期化／終了
    static void Init();
    static void Uninit();

    // 毎フレームの更新
    static void Update();

    //==================== 接続状態 ====================

    // 操作用パッド（0 番）が接続されているか
    static bool IsConnected();

    //==================== ボタン ======================

    // 押されている間 true
    static bool GetButton(WORD button);

    // 押した瞬間だけ true（立ち上がりエッジ）
    static bool GetButtonDown(WORD button);

    // 離した瞬間だけ true（立ち下がりエッジ）
    static bool GetButtonUp(WORD button);

    //==================== スティック ==================

    // 左スティック X, Y [-1,1]
    static float GetLStickX();
    static float GetLStickY();

    // 右スティック X, Y [-1,1]
    static float GetRStickX();
    static float GetRStickY();

    //==================== トリガ ======================

    // 左トリガ [0,1]
    static float GetLTrigger();
    static float GetRTrigger();

    //==================== ユーティリティ =============

    // このフレーム「何らかの入力」があったか？
    //  - 何かボタンが押されている
    //  - スティックがデッドゾーンを超えた
    //  - トリガがデッドゾーンを超えた
    static bool AnyInputThisFrame();

    // デッドゾーン設定（必要なら変更）
    //  - ls, rs : 左右スティックの deadzone
    //  - lt, rt : 左右トリガの deadzone
    static void SetDeadZone(
        SHORT ls, SHORT rs,
        BYTE  lt, BYTE  rt);

private:
    // 操作用（0 番）パッドの現在/前フレーム状態
    static XINPUT_STATE s_curr;
    static XINPUT_STATE s_prev;
    static bool         s_connected;

    // デッドゾーン設定
    static SHORT s_lStickDead;
    static SHORT s_rStickDead;
    static BYTE  s_lTriggerDead;
    static BYTE  s_rTriggerDead;

    // 内部ヘルパ
    static float NormalizeStick(SHORT v, SHORT dead);
    static float NormalizeTrigger(BYTE v, BYTE dead);
};
