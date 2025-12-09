#include "ModelManager.h"
#include <cassert>

std::array<std::unique_ptr<Model>, static_cast<size_t>(ModelTag::Count)> ModelManager::s_models{};
std::array<float, static_cast<size_t>(ModelTag::Count)> ModelManager::modelBounds = {
    0.f,
    0.62f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    2.6f,2.6f,
    1.f, 1.f,
    0.7f,0.f,0.f
};

const ModelLoadData ModelManager::kModelData[static_cast<size_t>(ModelTag::Count)] =
{
    {"Assets/Ç‹ÇØÇÒéÆê‘å©Ç©ÇÈÇ—ïPàﬂëïMMD/Ç‹ÇØÇÒéÆê‘å©Ç©ÇÈÇ—ïPàﬂëïver1.0.pmx", 0.08f, Model::Flip::ZFlip},

    {"Assets/Model/MD_ActionkomuSwing_prototype.fbx",                         0.01f, Model::Flip::ZFlip},
    {"Assets/Model/Grade_1/MD_akican_251208.fbx",                             0.01f, Model::Flip::ZFlip},
    {"Assets/Model/Grade_1/MD_desktop_20251205.fbx",                          0.2f, Model::Flip::ZFlip},
    {"Assets/Model/Grade_1/MD_gomibukuro_251208.fbx",                         0.01f, Model::Flip::ZFlip},
    {"Assets/Model/Grade_1/MD_isu_251208.fbx",                                0.01f, Model::Flip::ZFlip},
    {"Assets/Model/Grade_1/MD_kuukseizixyoukii_20251205.fbx",                 0.15f, Model::Flip::ZFlip},
    {"Assets/Model/Grade_1/MD_ningixyou_20251205.fbx",                        0.15f, Model::Flip::ZFlip},

    {"Assets/Model/MD_REbenti_prototype.fbx",                                 0.01f, Model::Flip::ZFlip},
    {"Assets/Model/Grade_2/MD_aircon_251208.fbx",                             0.01f, Model::Flip::ZFlip},
    {"Assets/Model/Grade_2/MD_jihanki_251208.fb",                             0.15f, Model::Flip::ZFlip},
    {"Assets/Model/Grade_2/MD_reizouko_20251205.fbx",                         0.5f, Model::Flip::ZFlip},

    {"Assets/Model/MD_UFO_prototype.fbx",                                     0.01f, Model::Flip::ZFlip},
    {"Assets/Model/Grade_3/MD_eiseii_20251202.fbx",                           0.4f, Model::Flip::ZFlip},

    {"Assets/Model/MD_player_prototype.fbx",                                  0.01f, Model::Flip::ZFlip},
    {"Assets/Model/playerchara_20251202/space_suit_12.fbx",                   0.01f, Model::Flip::ZFlip},
    {"Assets/Model/new_hammer_grade1.fbx",                                    0.08f, Model::Flip::ZFlip },
};

bool ModelManager::Load(ModelTag tag)
{
    if (s_models[static_cast<size_t>(tag)]) return false;

    int index = static_cast<size_t>(tag);
    const ModelLoadData& data = kModelData[static_cast<size_t>(tag)];

    assert(index < s_models.size());

    auto model = std::make_unique<Model>();
    if (FAILED(model->Load(data.pass, data.size, data.flip)))
        return false;

    s_models[index] = std::move(model);
    return true;
}

void ModelManager::Unload(ModelTag tag)
{
    s_models[static_cast<size_t>(tag)].reset();
}

Model* ModelManager::Get(ModelTag tag)
{
    return s_models[static_cast<size_t>(tag)].get();
}

float ModelManager::GetBounds(ModelTag tag)
{
    return modelBounds[static_cast<size_t>(tag)];
}

Model::AnimeNo ModelManager::AddAnimation(ModelTag tag, const char* file)
{
    const size_t idx = ToIndex(tag);
    if (idx >= s_models.size()) return Model::ANIME_NONE;

    Model* m = s_models[idx].get();
    if (!m) return Model::ANIME_NONE; // ñ¢ÉçÅ[Éh
    if (!file || file[0] == '\0') return Model::ANIME_NONE;

    return m->AddAnimation(file);
}

void ModelManager::Clear()
{
    for (auto& t : s_models)
    {
        t.reset();
    }
}