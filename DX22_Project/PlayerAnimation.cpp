#include "PlayerAnimation.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "Animator.h"
#include "ModelManager.h"
#include "Hammer.h"
#include "DebugLog.h"
#include <cassert>

/*
* @brief 初期化処理
*/
void PlayerAnimation::Start()
{
	state_ = gameObject->GetComponent<PlayerState>();
	anime_ = gameObject->GetComponent<Animator>();

	GameObject* hammerObj = gameObject->FindWithTag(Tag::Hammer);
	hammer_ = hammerObj ? hammerObj->GetComponent<Hammer>() : nullptr;
	assert(hammer_ && "PlayerAnimation で ハンマーが見つかりませんでした");

	//AddAllAnimations();
	InitAllAnimations();
	anime_->Play(allAnimations_[0][Type::Idle], false, 1.f);
	anime_->SetTimeSeconds(0.0f);

	assert(state_ && anime_ && "PlayerAnimation	で プレイヤーの <PlayerState> か <Animator> が nullptr です");
}

/*
* @brief 終了処理
*/
void PlayerAnimation::OnDestroy()
{
}

/*
* @brief 更新処理
*/
void PlayerAnimation::Update()
{
	grade_ = GetNowGrade();
	
	if (grade_ == -1)
	{
		return;
	}

	// ノックバック優先処理
	
	if (isKnockback_)
	{
		const float dt = 1.0f / 60.0f;
		knockbackTimer_ += dt;

		if (knockbackTimer_ >= knockbackDuration_)
		{
			isKnockback_ = false; currentType_ = Type::Move;
			anime_->Play(allAnimations_[0][Type::Move], false, 1.0f);
			anime_->SetTimeSeconds(4.8f);

			return;

		} // ノックバック中は他のアニメ判定を行わない
	}

	//移動を先に判定
	CheckMoveAnime();

	CheckNormalAttackAnime();
	CheckSpinAttackAnime();
	CheckDashAttackAnime();
	
}

void PlayerAnimation::CheckNockBackAnime(float duration)
{
	isKnockback_ = true;
	knockbackTimer_ = 0.0f;

	knockbackDuration_ = duration;

	anime_->Play(allAnimations_[grade_][Type::NockBack], false, 1.0f);
	if (currentType_ == Type::NormalAttack)
	{
		// 通常攻撃中に吹っ飛んだら、少しのけぞりが大きいところから
		anime_->SetTimeSeconds(1.8f);
	}
	else if (currentType_ == Type::DashAttack)
	{
		// ダッシュ攻撃の勢いがある時は、もっと激しくのけぞる
		anime_->SetTimeSeconds(1.5f);
	}
	else
	{
		anime_->SetTimeSeconds(1.3f);
	}

	currentType_ = Type::NockBack;

	state_->actionedThisFrame[ToIndex(ActionId::NormalAttack)] = false;
	state_->actionedThisFrame[ToIndex(ActionId::SpinAttack)] = false;
	state_->actionedThisFrame[ToIndex(ActionId::DashAttack)] = false;

}

void PlayerAnimation::Restart()
{
	anime_->Play(allAnimations_[0][Type::Move], false, 1.f);
	anime_->SetTimeSeconds(4.8f);
}

/*
* @brief 移動アニメーションの制御
*/
void PlayerAnimation::CheckMoveAnime()
{
	constexpr float kLoopStart = 0.5f;
	constexpr float kLoopEnd = 4.0f;

	if (currentType_ == Type::Move)
	{ 
		const float t = anime_->GetCurrentAnimeTime();

		//ループ範囲外に出たらループ開始位置に戻す
		if (t > kLoopEnd && state_->isMoving)
		{
			anime_->SetTimeSeconds(kLoopStart);
		}

		//移動停止したら終了位置まで再生
		if (state_->moveStoppedThisFrame)
		{
			anime_->SetTimeSeconds(4.8f);
			//currentType_ = Type::Idle;
			//anime_->Play(animation_[Type::Idle], true, 1.f);
		}
	}
	else
	{
		//移動開始したら移動アニメーションへ
		if (state_->moveStartedThisFrame)
		{
			currentType_ = Type::Move;
			//anime_->Play(animation_[Type::Move], false, 1.f);
			anime_->Play(allAnimations_[grade_][Type::Move], false, 1.f);
			anime_->SetTimeSeconds(0.3f);
		}
	}
}

void PlayerAnimation::CheckNormalAttackAnime()
{
	if (isKnockback_) return; // ノックバック中は無視！

	if (currentType_ == Type::NormalAttack)
	{
		const float t = anime_->GetCurrentAnimeTime();

		//アニメーション終了したら待機アニメーションへ
		if (t > 1.5f)
		{
			currentType_ = Type::Move;
			anime_->Play(allAnimations_[grade_][Type::Move], false, 1.f);
			anime_->SetTimeSeconds(4.8f);
		}
	}
	else
	{
		if (state_->actionedThisFrame[ToIndex(ActionId::NormalAttack)])
		{
			currentType_ = Type::NormalAttack;
			anime_->Play(allAnimations_[grade_][Type::NormalAttack], false, 0.6f);
			anime_->SetTimeSeconds(1.3f);
		}

	}
}

void PlayerAnimation::CheckSpinAttackAnime()
{
	if (isKnockback_) return; // ノックバック中は無視！

	if (currentType_ == Type::SpinAttack)
	{
		const float t = anime_->GetCurrentAnimeTime();

		//アニメーション終了したら待機アニメーションへ
		if (t > 2.1f)
		{
			currentType_ = Type::Move;
			anime_->Play(allAnimations_[grade_][Type::Move], false, 1.0f);
			anime_->SetTimeSeconds(4.8f);
		}
	}
	else
	{
		//ダッシュ攻撃アクションを実行したらダッシュ攻撃アニメーションへ
		if (state_->actionedThisFrame[ToIndex(ActionId::SpinAttack)])
		{
			currentType_ = Type::SpinAttack;
			anime_->Play(allAnimations_[grade_][Type::SpinAttack], false, 1.0f);
			anime_->SetTimeSeconds(1.3f);
		}

	}

}

void PlayerAnimation::CheckDashAttackAnime()
{
	if (isKnockback_) return; // ノックバック中は無視！

	if (currentType_ == Type::DashAttack)
	{
		const float t = anime_->GetCurrentAnimeTime();

		//アニメーション終了したら待機アニメーションへ
		if (t > 3.0)
		{
			currentType_ = Type::Move;
			anime_->Play(allAnimations_[grade_][Type::Move], false, 1.f);
			anime_->SetTimeSeconds(4.8f);
		}
	}
	else
	{
		//ダッシュ攻撃アクションを実行したらダッシュ攻撃アニメーションへ
		if (state_->actionedThisFrame[ToIndex(ActionId::DashAttack)])
		{
			currentType_ = Type::DashAttack;
			anime_->Play(allAnimations_[grade_][Type::DashAttack], false, 1.0f);
			anime_->SetTimeSeconds(1.5f);
		}
	}
}

/*
* @brief 全アニメーションをmodelに追加
* @brief animation_にAnimeNoを格納
*/
void PlayerAnimation::AddAllAnimations()
{
	Model* model = anime_->GetModel();
	if (!model) return;

	//待機
	animation_[Type::Idle] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_taiki.fbx");
	//移動
	animation_[Type::Move] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_hashiru.fbx");
	// 通常攻撃
	animation_[Type::NormalAttack] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_rotateattack.fbx");
	// 回転攻撃
	animation_[Type::SpinAttack] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_rotateattack.fbx");
	//突進攻撃
	animation_[Type::DashAttack] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_dashattack.fbx");
	//ノックバック
	animation_[Type::NockBack] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_taiki.fbx");
}

void PlayerAnimation::InitAllAnimations()
{
	constexpr ModelTag modelTag[3] = { ModelTag::Player_1, ModelTag::Player_2, ModelTag::Player_3 };

	for (int i = 0; i < 3; ++i)
	{
		Model* model = ModelManager::Get(modelTag[i]);
		if (!model) continue;

		//待機
		allAnimations_[i][Type::Idle] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_taiki.fbx");
		//移動
		allAnimations_[i][Type::Move] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_hashiru.fbx");
		// 通常攻撃
		allAnimations_[i][Type::NormalAttack] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_rotateattack.fbx");
		// 回転攻撃
		allAnimations_[i][Type::SpinAttack] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_rotateattack.fbx");
		//突進攻撃
		allAnimations_[i][Type::DashAttack] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_dashattack.fbx");
		//ノックバック
		allAnimations_[i][Type::NockBack] = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_taiki.fbx");
	}
}

/*
* @brief プレイヤーのステートから次のアニメーションをSetする
*/
PlayerAnimation::Type PlayerAnimation::DecideNextTypeFromState() const
{
	return Type::None;
}



int PlayerAnimation::GetNowGrade() const
{
	//TODO: プレイヤーの現在のグレードを返す処理を実装
	if (!hammer_)
	{
		return -1;
	}

	HammerGrade grade = hammer_->GetGrade();

	return static_cast<int>(grade);
}




