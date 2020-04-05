
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {
			//クリアする色を設定
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

			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる


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
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
	}

}
//end basecross
