
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {
			//�N���A����F��ݒ�
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);

			wstring Dir;
			App::GetApp()->GetAssetsDirectory(Dir);
			App::GetApp()->RegisterTexture(L"WATER_TX", Dir + L"/Final/" + L"Water.jpg");
			App::GetApp()->RegisterTexture(L"WALL_TX", Dir + L"wall.jpg");
			App::GetApp()->RegisterTexture(L"WALLMAP_TX", Dir + L"wall_normal.png");

			App::GetApp()->RegisterTexture(L"POMAP_TX", Dir + L"/Final/" + L"MainTexture.jpg");
			App::GetApp()->RegisterTexture(L"POHEIGHTMAP_TX", Dir + L"/Final/" + L"DisplacementMap.png");

			App::GetApp()->RegisterTexture(L"MAINTEX_TX", Dir + L"/Final/" + L"Tex2.png");
			App::GetApp()->RegisterTexture(L"SUBTEX_TX", Dir + L"/Final/" + L"Tex1.png");
			App::GetApp()->RegisterTexture(L"MASKTEX_TX", Dir + L"/Final/" + L"TexMask.png");

			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���


			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
	}

}
//end basecross
