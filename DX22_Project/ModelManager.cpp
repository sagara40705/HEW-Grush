#include "ModelManager.h"
#include <cassert>

std::array<std::unique_ptr<Model>, static_cast<size_t>(ModelTag::Count)> ModelManager::s_models{};
std::array<float, static_cast<size_t>(ModelTag::Count)> ModelManager::modelBounds = {
    0.7f, 0.5f, 0.8f, 0.7f, 0.9f, 0.9f, 0.9f, 0.f,  /*0 - 7*/
    1.8f, 1.8f, 1.6f, 1.4f, /*8 - 11*/
	2.4f, 3.f, 3.f,  /*12 - 14*/
    0.f,   //15
    0.f,0.f,0.f,   /*16 - 18*/
	0.f, 0.f  /*19*/
};

const ModelLoadData ModelManager::kModelData[static_cast<size_t>(ModelTag::Count)] =
{
    {"Assets/Model/Grade_1/MD_actionkomuSwing_251221_test2.fbx",              0.01f, Model::Flip::ZFlip}, //1
    {"Assets/Model/Grade_1/MD_akican_251208.fbx",                             0.01f, Model::Flip::ZFlip}, //2
    {"Assets/Model/Grade_1/MD_desktop_260121.fbx",                            0.2f,  Model::Flip::ZFlip}, //3
    {"Assets/Model/Grade_1/MD_gomibukuro_260126.fbx",                         0.01f, Model::Flip::ZFlip}, //4
    {"Assets/Model/Grade_1/MD_isu_251208.fbx",                                0.01f, Model::Flip::ZFlip}, //5
    {"Assets/Model/Grade_1/MD_kuukseizixyoukii_20251205.fbx",                 0.15f, Model::Flip::ZFlip}, //6
    {"Assets/Model/Grade_1/MD_ningixyou_20251205.fbx",                        0.15f, Model::Flip::ZFlip}, //7
    {"Assets/Model/Grade_1/MD_SagaraDoll_260126.fbx",                         0.01f, Model::Flip::ZFlip}, //8

    {"Assets/Model/MD_REbenti_prototype.fbx",                                 0.01f, Model::Flip::ZFlip}, //9
    {"Assets/Model/Grade_2/MD_aircon_251208.fbx",                             0.01f, Model::Flip::ZFlip}, //10
    {"Assets/Model/Grade_2/MD_jihanki_251208.fbx",                            0.006f,Model::Flip::ZFlip}, //11
    {"Assets/Model/Grade_2/MD_reizouko_20251205.fbx",                         0.5f,  Model::Flip::ZFlip}, //12
     
    {"Assets/Model/Grade_3/MD_UFO_260126.fbx",                                0.01f, Model::Flip::ZFlip}, //13
    {"Assets/Model/Grade_3/MD_eisei_251208.fbx",                              0.015f,Model::Flip::ZFlip}, //14
    {"Assets/Model/Grade_3/MD_star_251231.fbx",                               0.015f,Model::Flip::ZFlip}, //15

    {"Assets/Model/Player/playerchara_20260114.fbx",                          0.01f, Model::Flip::ZFlip}, //18

    {"Assets/Model/Hammer/Grade_1/new_hammer_grade1_2.fbx",                   0.08f, Model::Flip::ZFlip}, //20
    {"Assets/Model/Hammer/Grade_2/hammer_grade2.fbx",                         0.08f, Model::Flip::ZFlip}, //21
    {"Assets/Model/Hammer/Grade_3/hammer_grade3.fbx",                         0.08f, Model::Flip::ZFlip}, //22

    {"Assets/Model/Stage/MD_fence_260118.fbx",                                0.04f, Model::Flip::ZFlip}, //23
	{"Assets/Model/Stage/warp.fbx",                                           0.5f, Model::Flip::ZFlip}, //24
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
    if (!m) return Model::ANIME_NONE; // –¢ƒ[ƒh
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