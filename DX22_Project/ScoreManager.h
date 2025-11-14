#pragma once

class ScoreManager
{
public:
    static ScoreManager& Instance()
    {
        static ScoreManager instance;
        return instance;
    }

    // ステージ開始時に呼ぶ（目標kgのセット）
    void Reset(int goalKg)
    {
        goalKg_ = goalKg;
        destroyedKg_ = 0;
    }

    // 壊したオブジェクトの kg を加算
    void AddDestroyedKg(int kg)
    {
        destroyedKg_ += kg;
        if (destroyedKg_ > goalKg_) destroyedKg_ = goalKg_;
    }

    // 「目標までの残り kg」を返す（0〜goalKg）
    int GetRemainingKg() const
    {
        int r = goalKg_ - destroyedKg_;
        if (r < 0) r = 0;
        return r;
    }

    int GetGoalKg()      const { return goalKg_; }
    int GetDestroyedKg() const { return destroyedKg_; }

private:
    int goalKg_ = 0;
    int destroyedKg_ = 0;

    ScoreManager() = default;
};
