#include "ModelManager.h"
#include <cassert>

std::array<std::unique_ptr<Model>, static_cast<int>(ModelTag::Count)> ModelManager::s_models;

const char* ModelManager::kModelPath[static_cast<int>(ModelTag::Count)] =
{
    "Assets/‚Ü‚¯‚ñ®ÔŒ©‚©‚é‚Ñ•Pˆß‘•MMD/‚Ü‚¯‚ñ®ÔŒ©‚©‚é‚Ñ•Pˆß‘•ver1.0.pmx",
    "Assets/Model/MD_ActionkomuSwing_prototype.fbx",
    "Assets/Model/MD_REbenti_prototype.fbx",
    "Assets/Model/MD_UFO_prototype.fbx",
};

bool ModelManager::Register(ModelTag tag, const char* filePath)
{
    int index = static_cast<int>(tag);
    //assert((index >= 0 && index < static_cast<int>(ModeTag::Count)) && "a");

    auto model = std::make_unique<Model>();
    if (FAILED(model->Load(filePath, 0.08, Model::ZFlip)))
        return false;

    s_models[index] = std::move(model);
    return true;
}

bool ModelManager::RegisterAuto(ModelTag tag)
{
    return Register(tag, kModelPath[static_cast<int>(tag)]);
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
