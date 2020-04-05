/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	void Water::OnCreate()
	{
		///Player�̕`��
		Vec3 color(1, 1, 1);
		Vec2 tipSize = Vec2(1.0f, 1.0f);

		vector<VertexPositionNormalTexture>vertices =
		{
			{Vec3(-1.0f,+1.0f,0.0f),color,Vec2(0		,0)},
			{Vec3(+1.0f,+1.0f,0.0f),color,Vec2(tipSize.x,0)},
			{Vec3(-1.0f,-1.0f,0.0f),color,Vec2(0		,tipSize.y)},
			{Vec3(+1.0f,-1.0f,0.0f),color,Vec2(tipSize.x,tipSize.y)},
		};

		vector<uint16_t> indices =
		{
			0,1,2,
			2,1,3,
		};

		auto drawComp = AddComponent<TexBlendDraw>();
		drawComp->CreateOriginalMesh<VertexPositionNormalTexture>(vertices, indices);
		drawComp->SetOriginalMeshUse(true);
		drawComp->SetTextureResource(L"MAINTEX_TX");
		drawComp->SetSubTextureResource(L"SUBTEX_TX");
		drawComp->SetMaskTextureResource(L"MASKTEX_TX");
		drawComp->SetSamplerState(SamplerState::LinearWrap);
		drawComp->SetDiffuse(Col4(0.5f));
	}

	void Water::OnUpdate()
	{
		//auto DeltaTime = App::GetApp()->GetElapsedTime();
		//auto DrawComp = GetComponent<WaterDraw>();
		//ShaderParam p;
		//m_key += DeltaTime *DeltaTime;
		//p.Param[0] = m_key;
		//p.Param[1] = 0.0f;
		//p.Param[2] = 0.2f;
		//DrawComp->UpdateParam(p);
	}

	void Block::OnCreate()
	{
		auto DrawComp = AddComponent<NormalMapDraw>();
		DrawComp->SetMeshResource(L"DEFAULT_CUBE");
		DrawComp->SetTextureResource(L"WALL_TX");
		DrawComp->SetNormalMapTextureResource(L"WALLMAP_TX");
		//DrawComp->SetSamplerState(SamplerState::LinearWrap);

	}

	void Plane::OnCreate()
	{
		///Player�̕`��
		Vec3 color(1, 1, 1);
		Vec2 tipSize = Vec2(1.0f, 1.0f);

		vector<VertexPositionNormalTexture>vertices =
		{
			{Vec3(-1.0f,0.0f,+1.0f),color,Vec2(0		,0)},
			{Vec3(+1.0f,0.0f,+1.0f),color,Vec2(tipSize.x,0)},
			{Vec3(-1.0f,0.0f,-1.0f),color,Vec2(0		,tipSize.y)},
			{Vec3(+1.0f,0.0f,-1.0f),color,Vec2(tipSize.x,tipSize.y)},
		};

		vector<uint16_t> indices =
		{
			0,1,2,
			2,1,3,
		};



		auto DrawComp = AddComponent<POMapDraw>();
		DrawComp->CreateOriginalMesh<VertexPositionNormalTexture>(vertices, indices);
		DrawComp->SetOriginalMeshUse(true);
		DrawComp->SetTextureResource(L"POMAP_TX");
		DrawComp->SetSamplerState(SamplerState::LinearWrap);

		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");
		ptrString->SetTextRect(Rect2D<float>(16.0f, 50.0f, 640.0f, 480.0f));
	}

	void Plane::OnUpdate()
	{
		DebugLog();
	}

	void Plane::DebugLog()
	{
		wstring begin(L"DebugLog\n");

		wstring CameraData(L"Camera\n");

		wstring EyePos(L"EyePos�F");
		auto eye = OnGetDrawCamera()->GetEye();
		EyePos += L"X�F" + Util::FloatToWStr(eye.x, 4, Util::Util::FloatModify::Fixed) + L",\t";
		EyePos += L"Y�F" + Util::FloatToWStr(eye.y, 4, Util::Util::FloatModify::Fixed) + L",\t";
		EyePos += L"Z�F" + Util::FloatToWStr(eye.z, 4, Util::Util::FloatModify::Fixed) + L",\n";

		wstring atPos(L"AtPos�F");
		auto at = OnGetDrawCamera()->GetAt();
		atPos += L"X�F" + Util::FloatToWStr(at.x, 4, Util::Util::FloatModify::Fixed) + L",\t";
		atPos += L"Y�F" + Util::FloatToWStr(at.y, 4, Util::Util::FloatModify::Fixed) + L",\t";
		atPos += L"Z�F" + Util::FloatToWStr(at.z, 4, Util::Util::FloatModify::Fixed) + L",\n";

		wstring UP(L"Up�F");
		auto Up = OnGetDrawCamera()->GetUp();
		UP += L"X�F" + Util::FloatToWStr(Up.x, 4, Util::Util::FloatModify::Fixed) + L",\t";
		UP += L"Y�F" + Util::FloatToWStr(Up.y, 4, Util::Util::FloatModify::Fixed) + L",\t";
		UP += L"Z�F" + Util::FloatToWStr(Up.z, 4, Util::Util::FloatModify::Fixed) + L",\n";

		wstring EX(L"EX�F");
		float As = OnGetDrawCamera()->GetAspect();
		float Fa = OnGetDrawCamera()->GetFar();
		float Fov = OnGetDrawCamera()->GetFovY();
		float Height = OnGetDrawCamera()->GetHeight();
		float Near = OnGetDrawCamera()->GetNear();

		EX += L"As" + Util::FloatToWStr(As, 4, Util::Util::FloatModify::Fixed) + L",\n";
		EX += L"Fa" + Util::FloatToWStr(Fa, 4, Util::Util::FloatModify::Fixed) + L",\n";
		EX += L"Fov" + Util::FloatToWStr(Fov, 4, Util::Util::FloatModify::Fixed) + L",\n";
		EX += L"Height" + Util::FloatToWStr(Height, 4, Util::Util::FloatModify::Fixed) + L",\n";
		EX += L"Near" + Util::FloatToWStr(Near, 4, Util::Util::FloatModify::Fixed) + L",\n";

		wstring masterStr(begin + EyePos + atPos+UP+EX);
		auto ptrString = this->GetComponent<StringSprite>();
		ptrString->SetText(masterStr);
	}
}
//end basecross
