#include "JumpSki.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "DebugLog.h"

//初期化処理
void JampSki::Start()
{
	// エフェクト設定・再生
	// エフェクトが勝手にループ再生します
}

// 終了処理
void JampSki::OnDestroy()
{
	// エフェクトの後片付け
}

// ギミック
void JampSki::OnTriggerEnter(const TriggerInfo& info)
{
	/* 参考ファイル ： Rigidbdoy */

	//プレイヤーと当たった時だけ処理

	//プレイヤーのRigidbodyを取得

	//上方向に力を加える

	//DebugLog::Output("ジャンプ台起動");
}
