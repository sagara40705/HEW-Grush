#include "ModelManager.h"
#include <cassert>

std::array<std::unique_ptr<Model>, static_cast<int>(ModelTag::Count)> ModelManager::s_models;

const ModelLoadData ModelManager::kModelData[static_cast<int>(ModelTag::Count)] =
{
    {"Assets/Ç‹ÇØÇÒéÆê‘å©Ç©ÇÈÇ—ïPàﬂëïMMD/Ç‹ÇØÇÒéÆê‘å©Ç©ÇÈÇ—ïPàﬂëïver1.0.pmx", 0.08f, Model::Flip::ZFlip, Vector3(0,0,0), Vector3(0,0,0) },
    {"Assets/Model/MD_ActionkomuSwing_prototype.fbx",                         0.01f, Model::Flip::ZFlip, Vector3(0,0,0), Vector3(0,0,0) },
    {"Assets/Model/MD_REbenti_prototype.fbx",                                 0.01f, Model::Flip::ZFlip, Vector3(0,0,0), Vector3(0,0,0) },
    {"Assets/Model/MD_UFO_prototype.fbx",                                     0.01f, Model::Flip::ZFlip, Vector3(0,0,0), Vector3(0,0,0) },
    {"Assets/Model/MD_player_prototype.fbx",                                  0.01f, Model::Flip::ZFlip, Vector3(0,0,0), Vector3(0,0,0) },
    {"Assets/Model/playerchara_20251125.fbx",                                 0.01f, Model::Flip::ZFlip, Vector3(0,0,0), Vector3(0,0,0) }
};

bool ModelManager::Register(ModelTag tag, const ModelLoadData& data)
{
    int index = static_cast<int>(tag);
    //assert((index >= 0 && index < static_cast<int>(ModeTag::Count)) && "a");

    auto model = std::make_unique<Model>();
    if (FAILED(model->Load(data.pass, data.size, data.flip)))
        return false;

    s_models[index] = std::move(model);
    return true;
}

bool ModelManager::RegisterAuto(ModelTag tag)
{
    return Register(tag, kModelData[static_cast<int>(tag)]);
}

Model* ModelManager::Get(ModelTag tag)
{
    return s_models[static_cast<int>(tag)].get();
}

void ModelManager::Clear()
{
    for (auto& t : s_models)
    {
        t.reset();
    }
}
