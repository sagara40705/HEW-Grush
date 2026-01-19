#pragma once
#include "Component.h"
#include "Model.h"
#include <cstdint>

class PlayerState;

/*
* プレイヤーのアニメーション制御
*/
class PlayerAnimation : public MonoBehaviour
{
private:
	//アニメーションの種類
	enum Type
	{
		Idle = 0,			// 待機アニメーション
		Move = 1,			// 移動アニメーション
		NormalAttack = 2,	// 通常攻撃アニメーション
		SpinAttack = 3,		// 回転攻撃アニメーション
		DashAttack = 4,	    // 突進攻撃アニメーション
		LeanBack = 5,		// のけぞりアニメーション
		Count = 6,			// アニメーションの総数
		None = 99			// 未指定状態
	};

private:
	PlayerState* state_ = nullptr;	//プレイヤーの状態
	Animator* anime_ = nullptr;		//アニメーター
public:
	void Start() override;			// 開始処理
	void OnDestroy() override;		// 終了処理
	void Update()  override;		// 更新処理

	void CheckLeanBackAnime(float duration = 0.6f);	//のけぞりアニメーションのチェック
	
private:
	Model::AnimeNo animation_[Type::Count]; //アニメーションナンバーの格納先
	Type currentType_ = Type::None; // 現在のアニメーション
	Type nextType_	  = Type::None; // 次のアニメーション

	bool isKnockback_ = false;		//のけぞり中かどうか
	float knockbackTimer_ = 0.0f;	//のけぞり時間
	float knockbackDuration_ = 0.6f;//のけぞり継続時間

private:
	void CheckMoveAnime();
	void CheckNormalAttackAnim();
	void CheckSpinAttackAnime();
	void CheckDashAttackAnime();
	
private:
//========================================
//     内部処理
//========================================

	/*
	* @brief 全アニメーションをmodelに追加
	* @brief animation_にAnimeNoを格納
	*/
	void AddAllAnimations();
	
	/*
	* @brief プレイヤーのステートから次のアニメーションをSetする
	* @return 次のアニメーション Type
	*/
	PlayerAnimation::Type DecideNextTypeFromState() const;

	/*
	* @brief 次のアニメーションへ遷移する
	*/
	void ChangeState();

	/*
	* @brief 現在アニメーションの更新処理
	*/
	void UpdateCurrentState();
};