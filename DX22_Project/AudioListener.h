#pragma once
#include "Component.h"

//=========================================================
// AudioListener
//   - カメラ等にアタッチして、サウンド用リスナー位置を更新。
//   - 有効な AudioListener は常に 1 つだけとする（最後に OnEnable されたもの）。
//=========================================================
class AudioListener : public MonoBehaviour
{
public:
    AudioListener() = default;
    ~AudioListener() override = default;

    void OnEnable() override;
    void OnDisable() override;
    void LateUpdate() override;

private:
    static AudioListener* s_active;
};

