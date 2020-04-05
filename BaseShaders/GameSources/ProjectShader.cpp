/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	IMPLEMENT_DX11_CONSTANT_BUFFER(LINKCB)
		IMPLEMENT_DX11_CONSTANT_BUFFER(PARAMCB)

		//���ʃV�F�[�_
		IMPLEMENT_DX11_VERTEX_SHADER(VSWater, App::GetApp()->GetShadersPath() + L"WaterVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSWater, App::GetApp()->GetShadersPath() + L"WaterPS.cso")

		//�m�[�}���}�b�s���O�V�F�[�_
		IMPLEMENT_DX11_VERTEX_SHADER(VSNormalMap, App::GetApp()->GetShadersPath() + L"NormalMapVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSNormalMap, App::GetApp()->GetShadersPath() + L"NormalMapPS.cso")

		//�����}�b�s���O�V�F�[�_
		IMPLEMENT_DX11_VERTEX_SHADER(VSPOMap, App::GetApp()->GetShadersPath() + L"ParallaxOcclusionMappingVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSPOMap, App::GetApp()->GetShadersPath() + L"ParallaxOcclusionMappingPS.cso")

		//
		IMPLEMENT_DX11_VERTEX_SHADER(VSTexBlend, App::GetApp()->GetShadersPath() + L"TextureBlendVS.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSTexBlend, App::GetApp()->GetShadersPath() + L"TextureBlendPS.cso")
}
//end basecross


