#pragma once
#include "Component.h"
#include "Model.h"
#include <cstdint>

class PlayerState;
class Hammer;

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
		DashAttack = 4,		// 突進攻撃アニメーション
		NockBack = 5,		// ノックバックアニメーション
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
	void CheckNockBackAnime(float duration );	//ノックバックアニメーションのチェック
	void Restart();
private:
	Model::AnimeNo animation_[Type::Count]; //アニメーションナンバーの格納先
	Type currentType_ = Type::None; // 現在のアニメーション
	Type nextType_	  = Type::None; // 現在のアニメーション

	bool isKnockback_ = false;		//ノックバック中かどうか
	float knockbackTimer_ = 0.0f;	//ノックバック時間
	float knockbackDuration_ = 0.6f;//ノックバック継続時間
private:
	void CheckMoveAnime();
	void CheckNormalAttackAnime();
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
	void InitAllAnimations();
	
	/*
	* @brief プレイヤーのステートから次のアニメーションをSetする
	* @return 次のアニメーション Type
	*/
	PlayerAnimation::Type DecideNextTypeFromState() const;

private:
	// 全用
	Model::AnimeNo allAnimations_[3][Type::Count]{}; 
	Hammer* hammer_ = nullptr; // ハンマーのグレードによってアニメーションを変えるために必要
	int GetNowGrade() const;
	int grade_ = 0; // ハンマーのグレード

private:
};