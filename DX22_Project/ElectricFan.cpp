#include "ElectricFan.h"
#include "GameObject.h"
#include "BoxCollider.h"

void ElectricFan::Awake()
{
	//風を作りだす。
	GameObject& wind = gameObject->CreateChild();
	BoxCollider& windCol = wind.AddComponent<BoxCollider>(); windCol.isTrigger = true;
	wind.AddComponent<Wind>();
}

void ElectricFan::OnDestroy()
{
}

void Wind::Start()
{
	// エフェクト設定・再生
	// エフェクトが勝手にループ再生します
}

void Wind::OnDestroy()
{
	// エフェクトの後片付け
}

void Wind::OnTriggerStay(const TriggerInfo& info)
{
	//プレイヤーと当たった時だけ処理

	//プレイヤーのRigidbodyを取得

	//風の方向に力を加える

	//DebugLog::Output("風を当てています");
}
