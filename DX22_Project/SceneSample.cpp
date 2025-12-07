// SceneTitle.cpp
#include "SceneSmaple.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "ModelManager.h"

void SceneSmaple::Load()
{
	ModelManager::Load(ModelTag::Player);
	Model* model = ModelManager::Get(ModelTag::Player);

	World& w = GetWorld();

	Model::AnimeNo jumpAnim = model->AddAnimation("Assets/Model/animation_taiki1.fbx");
	model->Play(jumpAnim, true, 1.0f);

}

void SceneSmaple::Unload()
{
	TextureManager::Clear();
}

void SceneSmaple::Update()
{
	Model* model = ModelManager::Get(ModelTag::Player);
	model->Step(1.f / 60.f);
}

void SceneSmaple::Draw()
{
	Model* model = ModelManager::Get(ModelTag::Player);

	// ���_�V�F�[�_�[�ɓn���ϊ��s��̕ϐ���錾 
	DirectX::XMFLOAT4X4 fWVP[3];    // World,View,Projection�̗�  
	DirectX::XMMATRIX world, view, proj; // �e�ϊ��s��̊i�[�� 

	// �쐬�����s����e�ϐ��֊i�[ 
	//���[���h���W�p
	world = DirectX::XMMatrixRotationY(0.0f);
	//�r���[���W�p

	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 2.0f, -3.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);

	//�v���W�F�N�V�������W�p
	proj = DirectX::XMMatrixPerspectiveFovLH(70.0f, 16.0f / 9.0f, 0.1f, 10.0f);

	// �v�Z�p�̃f�[�^����ǂݎ��p�̃f�[�^�ɕϊ� 
	DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&fWVP[1], DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&fWVP[2], DirectX::XMMatrixTranspose(proj));


	// �V�F�[�_�[�֕ϊ��s���ݒ� 
	ShaderList::SetWVP(fWVP); // SetWVP�֐��̈����ɂ�XMFLOAT4X4�^�ŗv�f���R�̔z��̃A�h���X��n�� 


	//�{�[���s����V�F�[�_�[�ɐݒ�
	//200��Model�N���X�̍ő�{�[�����ɍ��킹��
	for (unsigned int i = 0; i < model->GetMeshNum(); ++i)
	{
		const Model::Mesh* mesh = model->GetMesh(i);
		if (!mesh) continue;

		// �{�[���s����V�F�[�_�[�ɐݒ�
		DirectX::XMFLOAT4X4 bones[200]; // Model::MAX_BONE ���g���Ă�OK
		for (int boneIdx = 0; boneIdx < mesh->bones.size(); ++boneIdx)
		{
			const Model::Bone& bone = mesh->bones[boneIdx];
			DirectX::XMStoreFloat4x4(
				&bones[boneIdx],
				DirectX::XMMatrixTranspose(
					bone.invOffset * model->GetBone(bone.index)
				)
			);
		}

		ShaderList::SetBones(bones);

		// ���b�V���`��i�K�v�Ȃ炱���Łj
		// m_pModel->Draw(i);
	}


	// ���f���Ɏg�p���钸�_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[��ݒ� 
	model->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	model->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));


	// �}�e���A���ʂɃ��b�V����\�� 
	for (unsigned int i = 0; i < model->GetMeshNum(); ++i) {
		// ���f���̃��b�V�����擾 
		const Model::Mesh* mesh = model->GetMesh(i);
		// ���b�V���Ɋ��蓖�Ă��Ă���}�e���A�����擾 
		Model::Material material = *model->GetMaterial(mesh->materialID);
		// �V�F�[�_�[�փ}�e���A����ݒ� 
		ShaderList::SetMaterial(material);
		// ���f���̕`�� 
		model->Draw(i);
	}
}


