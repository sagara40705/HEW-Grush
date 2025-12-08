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
		Idle = 0,			// 待機アニメーション
		Move = 1,			// 移動アニメーション
		NormalAttack = 2,	// 通常攻撃アニメーション
		SpinAttack = 3,		// 回転攻撃アニメーション
		Count = 4,			// アニメーションの総数
		None = 99			// 未指定状態
	};
	// 
	static constexpr size_t ToIndex(Type t) noexcept { return static_cast<size_t>(t); }
private:
	PlayerState* state = nullptr;	//プレイヤーの状態
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
//========================================
//     内部処理
//========================================

	/*
	* @brief 全アニメーションをmodelに追加
	* @brief animation_にAnimeNoを格納
	*/
	void AddAllAnimations();


};