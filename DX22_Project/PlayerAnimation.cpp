#include "PlayerAnimation.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "Animator.h"
#include "ModelManager.h"
#include <cassert>

/*
* @brief 初期化処理
*/
void PlayerAnimation::Start()
{
	state_ = gameObject->GetComponent<PlayerState>();
	anime_ = gameObject->GetComponent<Animator>();
	AddAllAnimations();
	anime_->Play(animation_[Type::Move], false, 1.f);
	anime_->SetTimeSeconds(4.8f);

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
	// ノックバック優先処理
	
	if (isKnockback_)
	{
		const float dt = 1.0f / 60.0f;
		knockbackTimer_ += dt;

		if (knockbackTimer_ >= knockbackDuration_)
		{
			isKnockback_ = false; currentType_ = Type::Move;
			anime_->Play(animation_[Type::Move], false, 1.0f);

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

	anime_->Play(animation_[Type::NockBack], false, 1.0f);
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
			anime_->Play(animation_[Type::Move], false, 1.f);
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
		if (t > 2.5f)
		{
			currentType_ = Type::Move;
			anime_->Play(animation_[Type::Move], false, 1.f);
			anime_->SetTimeSeconds(4.8f);
		}
	}
	else
	{
		if (state_->actionedThisFrame[ToIndex(ActionId::NormalAttack)])
		{
			currentType_ = Type::NormalAttack;
			anime_->Play(animation_[Type::NormalAttack], false, 4.0f);
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
			anime_->Play(animation_[Type::Move], false, 1.0f);
			anime_->SetTimeSeconds(4.8f);
		}
	}
	else
	{
		//ダッシュ攻撃アクションを実行したらダッシュ攻撃アニメーションへ
		if (state_->actionedThisFrame[ToIndex(ActionId::SpinAttack)])
		{
			currentType_ = Type::SpinAttack;
			anime_->Play(animation_[Type::SpinAttack], false, 1.0f);
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
			anime_->Play(animation_[Type::Move], false, 1.f);
			anime_->SetTimeSeconds(4.8f);
		}
	}
	else
	{
		//ダッシュ攻撃アクションを実行したらダッシュ攻撃アニメーションへ
		if (state_->actionedThisFrame[ToIndex(ActionId::DashAttack)])
		{
			currentType_ = Type::DashAttack;
			anime_->Play(animation_[Type::DashAttack], false, 1.0f);
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
	animation_[Type::Idle] = model->AddAnimation("Assets/Animation/taiki/body_taiki.fbx");
	//移動
	animation_[Type::Move] = model->AddAnimation("Assets/Animation/hashiru/body_hashiru.fbx");
	// 通常攻撃
	animation_[Type::NormalAttack] = model->AddAnimation("Assets/Animation/rotateattack/animation_rotateattack2.fbx");
	// 回転攻撃
	animation_[Type::SpinAttack] = model->AddAnimation("Assets/Animation/rotateattack/body_rotateattack.fbx");
	//突進攻撃
	animation_[Type::DashAttack] = model->AddAnimation("Assets/Animation/dashattack/body_dashattack.fbx");
	//ノックバック
	animation_[Type::NockBack] = model->AddAnimation("Assets/Animation/knockback/body_knockback.fbx");
}

/*
* @brief プレイヤーのステートから次のアニメーションをSetする
*/
PlayerAnimation::Type PlayerAnimation::DecideNextTypeFromState() const
{
	return Type::None;
}

/*
* @brief 次のアニメーションへ遷移する
* @param[from] no 現在のアニメーション Type
* @param[from] no 次のアニメーション Type
*/
void PlayerAnimation::ChangeState()
{

}

/*
* @brief 現在アニメーションの更新処理
*/
void PlayerAnimation::UpdateCurrentState()
{

}

void PlayerAnimation::AddHammerAllAnimations()
{
	GameObject* hummer = gameObject->FindChildWithTag(Tag::Hammer);
	if (!hummer) return;
	
	hummerAnime_ = hummer->GetComponent<Animator>();

	Model* model = ModelManager::Get(ModelTag::Hammer_1);

	//待機
	hummerAnimation_[Type::Idle] = model->AddAnimation("Assets/Animation/taiki/hammer_taiki.fbx");
	//移動
	hummerAnimation_[Type::Move] = model->AddAnimation("Assets/Animation/hashiru/hammer_hashiru.fbx");
	// 通常攻撃
	hummerAnimation_[Type::NormalAttack] = model->AddAnimation("Assets/Animation/rotateattack/hammer_rotateattack.fbx");
	// 回転攻撃
	hummerAnimation_[Type::SpinAttack] = model->AddAnimation("Assets/Animation/rotateattack/hammer_rotateattack.fbx");
	//突進攻撃
	hummerAnimation_[Type::DashAttack] = model->AddAnimation("Assets/Animation/dashattack/hammer_dashattack.fbx");
	//ノックバック
	hummerAnimation_[Type::NockBack] = model->AddAnimation("Assets/Animation/knockback/hammer_knockback.fbx");
}


