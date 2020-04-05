/*!
@file ProjectDraw.h
@brief �v���W�F�N�g�Ŏg�p����`��R���|�[�l���g����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void WaterDraw::OnDraw()
	{
		auto PtrGameObject = GetGameObject();
		//���b�V�����Ȃ���Ε`�悵�Ȃ�
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!MeshRes)",
				L"WaterDraw::OnDraw()"
			);
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//NULL�̃V�F�[�_���\�[�X�̏���
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

		auto PtrT = PtrGameObject->GetComponent<Transform>();
		//�J�����𓾂�
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();
		//�J�����̎擾
		Mat4x4 View, Proj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ShaderBuffer cb1;
		//�s��̐ݒ�(�]�u����)
		cb1.World = transpose(PtrT->GetWorldMatrix());
		cb1.View = transpose(View);
		cb1.Projection = transpose(Proj);

		////�G�~�b�V�u
		//cb1.Emissive = Col4(0, 0, 0, 1);
		////�f�t�B�[�Y�͂��ׂĒʂ�
		//cb1.Diffuse = Col4(1, 1, 1, 1);
		//���C�g�̐ݒ�
		auto PtrLight = PtrGameObject->OnGetDrawLight();
		cb1.LightDir = PtrLight.m_Directional;
		cb1.LightDir.w = 1.0f;
		//�e�N�X�`��
		auto shTex = GetTextureResource();
		if (shTex) {
			//�e�N�X�`��������
			cb1.ActiveFlg.x = 1;
		}
		else {
			//�`��R���|�[�l���g�ɂ̓e�N�X�`�����Ȃ�
			cb1.ActiveFlg.x = 0;
		}
		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(LINKCB::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�X�e�[�^�X�̃|�C���^
		//����������
		float blendfact[4] = { 0.0f,0.0f,0.0f,0.0f };
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), blendfact, D3D11_DEFAULT_SAMPLE_MASK);
		//�f�v�X�X�e���V���͎g�p����
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSWater::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSWater::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSWater::GetPtr()->GetInputLayout());

		//�e�N�X�`���ƃT���v���[
		if (shTex) {
			pID3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//�T���v���[��ݒ�
			//auto pSampler = RenderState->GetLinearWrap();
			//pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
			RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 0);
		}
		else {
			//�V�F�[�_�[���\�[�X���N���A
			pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			//�T���v���[���N���A
			RenderState->SetSamplerAllClear(pID3D11DeviceContext);
		}



		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = LINKCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		
		//
		ID3D11Buffer* pParamBuffer = PARAMCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(1, 1, &pParamBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(1, 1, &pParamBuffer);
		//
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();

	}

	void WaterDraw::UpdateParam(ShaderParam &SetParam)
	{
		auto pD3D11DeviceContext = App::GetApp()->GetDeviceResources()->GetD3DDeviceContext();
		pD3D11DeviceContext->UpdateSubresource(PARAMCB::GetPtr()->GetBuffer(), 0, nullptr, &SetParam, 0, 0);
	}

	void WaterDraw::SetTextures(UINT StartSlot, UINT NumViews, const wstring&Key)
	{
		//�f�o�C�X�̎擾
		auto pD3D11DeviceContext = App::GetApp()->GetDeviceResources()->GetD3DDeviceContext();
		auto Res = App::GetApp()->GetResource<TextureResource>(Key);
		pD3D11DeviceContext->PSSetShaderResources(StartSlot, NumViews, Res->GetShaderResourceView().GetAddressOf());
		auto pSampler = App::GetApp()->GetDeviceResources()->GetRenderState()->GetLinearClamp();

		pD3D11DeviceContext->PSSetSamplers(StartSlot, NumViews, &pSampler);
	}


	void NormalMapDraw::OnDraw()
	{
		//������
		auto PtrGameObject = GetGameObject();
		//���b�V���̊m�F
		auto MeshRes = GetMeshResource();
		if (!MeshRes)
		{
			throw BaseException(
				L"���b�V�����쐬����Ă܂���",
				L"if(!MeshRes)",
				L"NormalMap::OnDraw()");
		}

		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//NULL�̃V�F�[�_���\�[�X�̏���
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11ShaderResourceView* pNullMap[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

		//�R���X�^���g�o�b�t�@����
		auto TransComp = PtrGameObject->GetComponent<Transform>();
		//�擾
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();
		//�r���[�s��A�ˉe�s��̎擾
		Mat4x4 View, Projection;
		View = PtrCamera->GetViewMatrix();
		Projection = PtrCamera->GetProjMatrix();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ShaderBuffer CB;
		CB.World = transpose(TransComp->GetWorldMatrix());
		CB.View = transpose(View);
		CB.Projection = transpose(Projection);

		//���C�g
		auto PtrLight = PtrGameObject->OnGetDrawLight();
		CB.LightDir = PtrLight.m_Directional;
		CB.LightDir.w = 1.0f;

		//�e�N�X�`��
		auto shTex = GetTextureResource();
		auto mapTex = GetNormalMapTextureResource();
		if (shTex)
		{
			//�e�N�X�`��������
			CB.ActiveFlg.x = 1;
			//�}�b�s���O�e�N�X�`��
			if (mapTex)
			{
				CB.ActiveFlg.y = 1;
			}
			else
			{
				CB.ActiveFlg.y = 0;
			}
		}
		else
		{
			//�`��R���|�[�l���g�ɂ̓e�N�X�`�����Ȃ�
			//�e�N�X�`�����Ȃ���΃}�b�s���O���s��Ȃ�
			CB.ActiveFlg.x = 0;
			CB.ActiveFlg.y = 0;
		}

		pID3D11DeviceContext->UpdateSubresource(LINKCB::GetPtr()->GetBuffer(), 0, nullptr, &CB, 0, 0);

		//�X�g���C�h�E�I�t�Z�b�g
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//���_�p�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�̐ݒ�(�O�p�`)
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		//�X�e�[�^�X
		//�u�����h�X�e�[�g�̐ݒ�
		//������
		float blendstate[4] = { 0.0f,0.0f,0.0f,0.0f };
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), blendstate, D3D11_DEFAULT_SAMPLE_MASK);
		//�f�v�X�X�e���V��
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSNormalMap::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSNormalMap::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSNormalMap::GetPtr()->GetInputLayout());

		//�e�N�X�`���E�T���v���[
		if (shTex)
		{
			pID3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//�T���v���[
			RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 0);
			if (mapTex)
			{
				pID3D11DeviceContext->PSSetShaderResources(1, 1, mapTex->GetShaderResourceView().GetAddressOf());
				//�T���v���[
				RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 1);
			}
			else
			{
				pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNullMap);
			}
		}
		else
		{
			//�V�F�[�_�[���\�[�X���N���A
			pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNullMap);
			//�T���v���[���N���A
			RenderState->SetSamplerAllClear(pID3D11DeviceContext);
		}


		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = LINKCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//�ėp�p�����[�^�̐ݒ�

		//�����_�[�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//�����_�[�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();
	}

	void NormalMapDraw::SetNormalMapTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		m_NormalMapTextureResource = TextureRes;

	}

	void NormalMapDraw::SetNormalMapTextureResource(const wstring& TextureKey) {
		this->SetNormalMapTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<TextureResource> NormalMapDraw::GetNormalMapTextureResource() const {
		//�e�N�X�`�����Ȃ����null��Ԃ�
		auto shptr = m_NormalMapTextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;

	}

	//
	void POMapDraw::OnDraw()
	{
		//������
		auto PtrGameObject = GetGameObject();
		//���b�V���̊m�F
		auto MeshRes = GetMeshResource();
		if (!MeshRes)
		{
			throw BaseException(
				L"���b�V�����쐬����Ă܂���",
				L"if(!MeshRes)",
				L"NormalMap::OnDraw()");
		}

		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//NULL�̃V�F�[�_���\�[�X�̏���
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		//ID3D11ShaderResourceView* pNullMap[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

		//�R���X�^���g�o�b�t�@����
		auto TransComp = PtrGameObject->GetComponent<Transform>();
		//�擾
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();
		//�r���[�s��A�ˉe�s��̎擾
		Mat4x4 View, Projection;
		View = PtrCamera->GetViewMatrix();
		Projection = PtrCamera->GetProjMatrix();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ShaderBuffer CB;
		CB.World = transpose(TransComp->GetWorldMatrix());
		CB.View = transpose(View);
		CB.Projection = transpose(Projection);

		//���C�g
		auto PtrLight = PtrGameObject->OnGetDrawLight();
		CB.LightDir = PtrLight.m_Directional;
		CB.LightDir.w = 1.0f;

		//�J����
		CB.Camera =normalize(PtrCamera->GetAt()-PtrCamera->GetEye());
		
		//�e�N�X�`��
		auto shTex = GetTextureResource();
		//auto mapTex = GetHeightMapTextureResource();
		if (shTex)
		{
			//�e�N�X�`��������
			CB.ActiveFlg.x = 1;
			//�}�b�s���O�e�N�X�`��
			//if (mapTex)
			//{
			//	CB.ActiveFlg.y = 1;
			//}
			//else
			//{
			//	CB.ActiveFlg.y = 0;
			//}
		}
		else
		{
			//�`��R���|�[�l���g�ɂ̓e�N�X�`�����Ȃ�
			//�e�N�X�`�����Ȃ���΃}�b�s���O���s��Ȃ�
			CB.ActiveFlg.x = 0;
			CB.ActiveFlg.y = 0;
		}

		pID3D11DeviceContext->UpdateSubresource(LINKCB::GetPtr()->GetBuffer(), 0, nullptr, &CB, 0, 0);

		//�X�g���C�h�E�I�t�Z�b�g
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//���_�p�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�̐ݒ�(�O�p�`)
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�X�e�[�^�X
		//�u�����h�X�e�[�g�̐ݒ�
		//������
		float blendstate[4] = { 0.0f,0.0f,0.0f,0.0f };
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), blendstate, D3D11_DEFAULT_SAMPLE_MASK);
		//�f�v�X�X�e���V��
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSPOMap::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSPOMap::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSPOMap::GetPtr()->GetInputLayout());

		//�e�N�X�`���E�T���v���[
		if (shTex)
		{
			pID3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//�T���v���[
			RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 0);
			//if (mapTex)
			//{
			//	pID3D11DeviceContext->PSSetShaderResources(1, 1, mapTex->GetShaderResourceView().GetAddressOf());
			//	//�T���v���[
			//	RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 1);
			//}
			//else
			//{
			//	pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNullMap);
			//}
		}
		else
		{
			//�V�F�[�_�[���\�[�X���N���A
			pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			//pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNullMap);
			//�T���v���[���N���A
			RenderState->SetSamplerAllClear(pID3D11DeviceContext);
		}


		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = LINKCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//�ėp�p�����[�^�̐ݒ�
		ID3D11Buffer* pParamBuffer = PARAMCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(1, 1, &pParamBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(1, 1, &pParamBuffer);

		//�����_�[�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//�����_�[�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();
	}

	void POMapDraw::SetHeightMapTextureResource(const wstring&key)
	{
		this->SetHeightMapTextureResource(App::GetApp()->GetResource<TextureResource>(key));
	}

	void POMapDraw::SetHeightMapTextureResource(const shared_ptr<TextureResource>& TextureRes)
	{
		m_HeightMapTextureResource = TextureRes;
	}

	shared_ptr<TextureResource> POMapDraw::GetHeightMapTextureResource()const
	{
		auto TexRes = m_HeightMapTextureResource.lock();
		if (TexRes)
		{
			return TexRes;
		}
		return nullptr;
	}

	void TexBlendDraw::OnDraw()
	{
		auto ptrGameObject = GetGameObject();
		auto MeshRes = GetMeshResource();
		if (!MeshRes)
		{
			throw BaseException(
				L"���b�V�����ݒ肳��Ă��܂���",
				L"if(!MeshRes)",
				L"TexBlendDraw::OnDraw()"
			);
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		ID3D11ShaderResourceView* pMainNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11ShaderResourceView* pSubNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11ShaderResourceView* pMaskNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

		ID3D11SamplerState* pMainSamp[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pSubSamp[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pMaskSamp[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };

		//
		auto TransComp = ptrGameObject->GetComponent<Transform>();
		auto ptrCamera = ptrGameObject->OnGetDrawCamera();
		Mat4x4 View, Projection;
		View = ptrCamera->GetViewMatrix();
		Projection = ptrCamera->GetProjMatrix();

		//�R���X�^���g�o�b�t�@
		ShaderBuffer CB;
		CB.World = transpose(TransComp->GetWorldMatrix());
		CB.View = transpose(View);
		CB.Projection = transpose(Projection);

		auto ptrLight = ptrGameObject->OnGetDrawLight();
		CB.LightDir = ptrLight.m_Directional;
		CB.LightDir.w = 1.0f;

		auto mainTex = GetTextureResource();
		auto subTex = GetSubTextureResource();
		auto maskTex = GetMaskTextureResource();
		if (mainTex)
		{
			CB.ActiveFlg.x = 1;
			if (subTex&&maskTex)
			{
				CB.ActiveFlg.y = 1;
			}
			else
			{
				CB.ActiveFlg.y = 1;
			}
		}
		else
		{
			CB.ActiveFlg.x = 0;
			CB.ActiveFlg.y = 0;
		}

		pID3D11DeviceContext->UpdateSubresource(LINKCB::GetPtr()->GetBuffer(), 0, nullptr, &CB, 0, 0);


		//�X�g���C�h�E�I�t�Z�b�g
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//���_�p�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�̐ݒ�(�O�p�`)
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�X�e�[�^�X
		//�u�����h�X�e�[�g�̐ݒ�
		//������
		float blendstate[4] = { 0.0f,0.0f,0.0f,0.0f };
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), blendstate, D3D11_DEFAULT_SAMPLE_MASK);
		//�f�v�X�X�e���V��
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSTexBlend::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSTexBlend::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSTexBlend::GetPtr()->GetInputLayout());

		//�e�N�X�`���E�T���v���[
		if (mainTex)
		{
			pID3D11DeviceContext->PSSetShaderResources(0, 1, mainTex->GetShaderResourceView().GetAddressOf());
			//�T���v���[
			RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 0);
			if (subTex&&maskTex)
			{
				pID3D11DeviceContext->PSSetShaderResources(1, 1, subTex->GetShaderResourceView().GetAddressOf());
				pID3D11DeviceContext->PSSetShaderResources(2, 1, maskTex->GetShaderResourceView().GetAddressOf());
				//�T���v���[
				RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 1);
				RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 2);
			}
			else
			{
				pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSubNull);
				pID3D11DeviceContext->PSSetShaderResources(2, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pMaskNull);

				pID3D11DeviceContext->PSSetSamplers(1, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pSubSamp);
				pID3D11DeviceContext->PSSetSamplers(2, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pMaskSamp);
			}
		}
		else
		{
			//�V�F�[�_�[���\�[�X���N���A
			pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pMainNull);
			pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSubNull);
			pID3D11DeviceContext->PSSetShaderResources(2, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pMaskNull);
			//�T���v���[���N���A
			RenderState->SetSamplerAllClear(pID3D11DeviceContext);
		}


		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = LINKCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//�ėp�p�����[�^�̐ݒ�
		//ID3D11Buffer* pParamBuffer = PARAMCB::GetPtr()->GetBuffer();
		//pID3D11DeviceContext->VSSetConstantBuffers(1, 1, &pParamBuffer);
		//pID3D11DeviceContext->PSSetConstantBuffers(1, 1, &pParamBuffer);

		//�����_�[�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//�����_�[�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();

	}

	void TexBlendDraw::SetSubTextureResource(const wstring& key)
	{
		this->SetSubTextureResource(App::GetApp()->GetResource<TextureResource>(key));
	}

	void TexBlendDraw::SetSubTextureResource(const shared_ptr<TextureResource>& TextureRes)
	{
		m_SubTextureResource = TextureRes;
	}

	shared_ptr<TextureResource> TexBlendDraw::GetSubTextureResource()const
	{
		auto tex = m_SubTextureResource.lock();
		if (tex)
		{
			return tex;
		}
		return nullptr;
	}

	void TexBlendDraw::SetMaskTextureResource(const wstring& key)
	{
		this->SetMaskTextureResource(App::GetApp()->GetResource<TextureResource>(key));
	}

	void TexBlendDraw::SetMaskTextureResource(const shared_ptr<TextureResource>& TextureRes)
	{
		m_MaskTextureResource = TextureRes;
	}

	shared_ptr<TextureResource> TexBlendDraw::GetMaskTextureResource()const
	{
		auto tex = m_MaskTextureResource.lock();
		if (tex)
		{
			return tex;
		}
		return nullptr;
	}

}
//end basecross