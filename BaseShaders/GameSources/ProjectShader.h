/*!
@file ProjectShader.h
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//�R���X�^���g�o�b�t�@
	struct ShaderBuffer
	{
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Projection;
		Vec4 Emissive;
		Vec4 Diffuse;
		Vec4 LightDir;
		Vec4 Camera;
		XMUINT4 ActiveFlg;
		ShaderBuffer()
		{
			memset(this, 0, sizeof(ShaderBuffer));
			ActiveFlg = XMUINT4(0,0,0,0);
		}
	};

	DECLARE_DX11_CONSTANT_BUFFER(LINKCB,ShaderBuffer)

	struct ShaderParam
	{
		//---
		//�ėp�p�����[�^
		//Param[0]�AUV�X�N���[���pU���W�̈ړ���
		//Param[1]�AUV�X�N���[���pV���W�̈ړ���
		//Param[2]�A�I�u�W�F�N�g�̃��l
		//---
		float Param[4] = { 0.0f,0.0f,0.0f,0.0f };

		ShaderParam()
		{
			memset(this, 0, sizeof(ShaderParam));
		}
	};
	DECLARE_DX11_CONSTANT_BUFFER(PARAMCB, ShaderParam)

	//���ʃV�F�[�_

	DECLARE_DX11_VERTEX_SHADER(VSWater,VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSWater)

	//�m�[�}���}�b�s���O�V�F�[�_
	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƃe�N�X�`����Tangent��Binormal�������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureTangentBinormalLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƃe�N�X�`����Tangent��Binormal�������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTextureTangentBinormal
	{
		VertexPositionNormalTextureTangentBinormal()
		{ }

		VertexPositionNormalTextureTangentBinormal(bsm::Vec3 const& pos, bsm::Vec3 const& norm, bsm::Vec2 const& tex,const bsm::Vec3 tangent,const bsm::Vec3 binormal)
			: position(pos),
			normal(norm),
			textureCoordinate(tex),
			tangent(tangent),
			binormal(binormal)
		{ }

		VertexPositionNormalTextureTangentBinormal(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR tex, FXMVECTOR tang, FXMVECTOR binorm)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat3(&normal, norm);
			XMStoreFloat2(&textureCoordinate, tex);
			XMStoreFloat3(&tangent, tang);
			XMStoreFloat3(&binormal, binorm);
		}

		bsm::Vec3 position;
		bsm::Vec3 normal;
		bsm::Vec2 textureCoordinate;
		bsm::Vec3 tangent;
		bsm::Vec3 binormal;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() {
			return VertexPositionNormalTextureTangentBinormalLayout;
		}
		static UINT GetNumElements() {
			return  ARRAYSIZE(VertexPositionNormalTextureTangentBinormalLayout);
		}
	};

	DECLARE_DX11_VERTEX_SHADER(VSNormalMap, VertexPositionNormalTextureTangentBinormal)
	DECLARE_DX11_PIXEL_SHADER(PSNormalMap)

	//�����}�b�s���O
	DECLARE_DX11_VERTEX_SHADER(VSPOMap, VertexPositionNormalTextureTangentBinormal)
	DECLARE_DX11_PIXEL_SHADER(PSPOMap)

	//�e�N�X�`���u�����h
	DECLARE_DX11_VERTEX_SHADER(VSTexBlend, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSTexBlend)
}
//end basecross

