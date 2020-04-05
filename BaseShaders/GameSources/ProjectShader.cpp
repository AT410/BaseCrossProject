/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	IMPLEMENT_DX11_CONSTANT_BUFFER(LINKCB)
		IMPLEMENT_DX11_CONSTANT_BUFFER(PARAMCB)

		//水面シェーダ
		IMPLEMENT_DX11_VERTEX_SHADER(VSWater, App::GetApp()->GetShadersPath() + L"WaterVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSWater, App::GetApp()->GetShadersPath() + L"WaterPS.cso")

		//ノーマルマッピングシェーダ
		IMPLEMENT_DX11_VERTEX_SHADER(VSNormalMap, App::GetApp()->GetShadersPath() + L"NormalMapVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSNormalMap, App::GetApp()->GetShadersPath() + L"NormalMapPS.cso")

		//視差マッピングシェーダ
		IMPLEMENT_DX11_VERTEX_SHADER(VSPOMap, App::GetApp()->GetShadersPath() + L"ParallaxOcclusionMappingVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSPOMap, App::GetApp()->GetShadersPath() + L"ParallaxOcclusionMappingPS.cso")

		//
		IMPLEMENT_DX11_VERTEX_SHADER(VSTexBlend, App::GetApp()->GetShadersPath() + L"TextureBlendVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSTexBlend, App::GetApp()->GetShadersPath() + L"TextureBlendPS.cso")
}
//end basecross


