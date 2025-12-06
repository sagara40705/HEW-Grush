// === File: CameraShake.cpp ===
#include "CameraShake.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "DebugLog.h"
#include <cassert>
#include <cstdlib> 
#include <cmath>

constexpr size_t kShakeTypeCount = static_cast<size_t>(ShakeType::COUNT);

const ShakeParam kShakeParam[kShakeTypeCount] =
{
    { 0.2f, 0.5f, 0.f , Vector3(1,1,1), Vector3(0,0,0), 30.f },
    { 0.3f, 0.f, 0.f , Vector3(0,0,0), Vector3(0,0,0), 30.f },
    { 0.4f, 0.f, 0.f , Vector3(0,0,0), Vector3(0,0,0), 30.f },
};

const Vector3 kBaseLocalPos = Camera::kInitPos;
const Vector3 kBaseLocalRot = Camera::kInitRotate;

void CameraShake::Play(ShakeType type)
{
    const size_t idx = ToIndex(type);
    if (idx >= kShakeTypeCount) return; // safety

    param_ = kShakeParam[ToIndex(type)];
    timer_ = 0.0f;
    play_ = true;
    DebugLog::OutputValue("CameraShake::Play, type", static_cast<int>(type));
}

void CameraShake::Stop()
{
    if (!play_ && !hasOffset_) return;

    ClearOffset_();
    play_ = false;
    timer_ = 0.0f;
    DebugLog::Output("CameraShake::Stop");
}

void CameraShake::Start()
{
    tf_ = gameObject->GetComponent<Transform>();

    assert(tf_ && "CameraShake::Start(),Transformgがnullptr");
}

void CameraShake::OnDisable()
{
    Stop();
}

void CameraShake::OnDestroy()
{
    Stop();
    tf_ = nullptr;
}

void CameraShake::LateUpdate()
{
    if (!play_) return;

    // シンプルに 60FPS 固定で進める
    constexpr float DT = 1.0f / 60.0f;
    timer_ += DT;

    if (param_.duration <= 0.0f || timer_ >= param_.duration)
    {
        // 最終フレームを過ぎたのでオフセットを消して停止
        Stop();
        return;
    }

    StepShake_();
}

void CameraShake::ClearOffset_()
{
    if (!tf_ || !hasOffset_)
    {
        hasOffset_ = false;
        return;
    }

    // 直前フレームで加えたオフセットを打ち消す
    tf_->SetPosition(kBaseLocalPos);
    tf_->SetRotation(kBaseLocalRot);

    hasOffset_ = false;
}

void CameraShake::StepShake_()
{
    if (!tf_)
    {
        Stop();
        return;
    }


    // まず前フレーム分の揺れを打ち消す
    if (hasOffset_)
    {
        tf_->SetPosition(kBaseLocalPos);
        tf_->SetRotation(kBaseLocalRot) ;
    }

    // 減衰係数 0〜1（時間が経つほど 0 に近づく）
    float t = timer_ / param_.duration;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // 二乗でイージングっぽく減衰
    float damper = 1.0f - t;
    damper *= damper;

    // 揺れが不要ならこれ以上何もしない
    if (damper <= 0.0f ||
        (param_.positionAmplitude <= 0.0f && param_.rotationAmplitude <= 0.0f))
    {
        hasOffset_ = false;
        return;
    }

    // 簡易ランダム揺れ
    // frequency は「どの程度ガタガタするか」の調整用
    float freqScale = param_.frequency;
    if (freqScale < 0.0f) freqScale = 0.0f;
    freqScale /= 30.0f; // 30 を基準値に

    Vector3 randP(
        RandomRange_(-1.0f, 1.0f),
        RandomRange_(-1.0f, 1.0f),
        RandomRange_(-1.0f, 1.0f)
    );

    Vector3 randR(
        RandomRange_(-1.0f, 1.0f),
        RandomRange_(-1.0f, 1.0f),
        RandomRange_(-1.0f, 1.0f)
    );

    Vector3 posOffset = randP * (param_.positionAmplitude * damper * freqScale);
    posOffset = posOffset * param_.positionAxisMask; // 各軸をマスク

    Vector3 rotOffset = randR * (param_.rotationAmplitude * damper * freqScale);
    rotOffset = rotOffset * param_.rotationAxisMask;

    // 新しいオフセットを適用
    tf_->SetPosition(kBaseLocalPos + posOffset);
    tf_->SetRotation(kBaseLocalRot + rotOffset);

    hasOffset_ = true;
}

float CameraShake::RandomRange_(float min, float max)
{
    float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return min + (max - min) * r;
}
