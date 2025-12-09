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
	enum class Type : std::uint8_t
	{
		//Idle = ,			// 待機アニメーション
		Move = 0,			// 移動アニメーション
		//NormalAttack = ,	// 通常攻撃アニメーション
		//SpinAttack = ,		// 回転攻撃アニメーション
		Count = 1,			// アニメーションの総数
		None = 99			// 未指定状態
	};
	// 
	static constexpr size_t ToIndex(Type t) noexcept { return static_cast<size_t>(t); }
private:
	PlayerState* state_ = nullptr;	//プレイヤーの状態
	Animator* anime_ = nullptr;		//アニメーター
public:
	void Start() override;			// 開始処理
	void OnDestroy() override;		// 終了処理
	void Update()  override;		// 更新処理
private:
	Model::AnimeNo animation_[static_cast<int>(Type::Count)]; //アニメーションナンバーの格納先
	Type currentType_ = Type::None; // 現在のアニメーション
	Type nextType_	  = Type::None; // 現在のアニメーション
private:
	void CheckMoveAnime();

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
	* @param[from] no 現在のアニメーション Type
	* @param[from] no 次のアニメーション Type
	*/
	void ChangeState(Type from, Type to);

	/*
	* @brief 現在アニメーションの更新処理
	*/
	void UpdateCurrentState();
};