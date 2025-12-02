#include "ModelManager.h"
#include <cassert>

std::array<std::unique_ptr<Model>, static_cast<size_t>(ModelTag::Count)> ModelManager::s_models{};
std::array<float, static_cast<size_t>(ModelTag::Count)> ModelManager::modelBounds{};

const ModelLoadData ModelManager::kModelData[static_cast<size_t>(ModelTag::Count)] =
{
    {"Assets/Ç‹ÇØÇÒéÆê‘å©Ç©ÇÈÇ—ïPàﬂëïMMD/Ç‹ÇØÇÒéÆê‘å©Ç©ÇÈÇ—ïPàﬂëïver1.0.pmx", 0.08f, Model::Flip::ZFlip},
    {"Assets/Model/MD_ActionkomuSwing_prototype.fbx",                         0.01f, Model::Flip::ZFlip},
    {"Assets/Model/MD_REbenti_prototype.fbx",                                 0.01f, Model::Flip::ZFlip},
    {"Assets/Model/MD_UFO_prototype.fbx",                                     0.01f, Model::Flip::ZFlip},
    {"Assets/Model/MD_player_prototype.fbx",                                  0.01f, Model::Flip::ZFlip},
    {"Assets/Model/playerchara_20251202/space_suit_12.fbx",                   0.01f, Model::Flip::ZFlip}
};

bool ModelManager::Load(ModelTag tag)
{
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

void ModelManager::Clear()
{
    for (auto& t : s_models)
    {
        t.reset();
    }
}