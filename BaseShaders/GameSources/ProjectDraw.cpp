/*!
@file ProjectDraw.h
@brief プロジェクトで使用する描画コンポーネント実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void WaterDraw::OnDraw()
	{
		auto PtrGameObject = GetGameObject();
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"WaterDraw::OnDraw()"
			);
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//NULLのシェーダリソースの準備
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

		auto PtrT = PtrGameObject->GetComponent<Transform>();
		//カメラを得る
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();
		//カメラの取得
		Mat4x4 View, Proj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//コンスタントバッファの設定
		ShaderBuffer cb1;
		//行列の設定(転置する)
		cb1.World = transpose(PtrT->GetWorldMatrix());
		cb1.View = transpose(View);
		cb1.Projection = transpose(Proj);

		////エミッシブ
		//cb1.Emissive = Col4(0, 0, 0, 1);
		////デフィーズはすべて通す
		//cb1.Diffuse = Col4(1, 1, 1, 1);
		//ライトの設定
		auto PtrLight = PtrGameObject->OnGetDrawLight();
		cb1.LightDir = PtrLight.m_Directional;
		cb1.LightDir.w = 1.0f;
		//テクスチャ
		auto shTex = GetTextureResource();
		if (shTex) {
			//テクスチャがある
			cb1.ActiveFlg.x = 1;
		}
		else {
			//描画コンポーネントにはテクスチャがない
			cb1.ActiveFlg.x = 0;
		}
		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(LINKCB::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//
		//ストライドとオフセット
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//ステータスのポインタ
		//半透明処理
		float blendfact[4] = { 0.0f,0.0f,0.0f,0.0f };
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), blendfact, D3D11_DEFAULT_SAMPLE_MASK);
		//デプスステンシルは使用する
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSWater::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSWater::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSWater::GetPtr()->GetInputLayout());

		//テクスチャとサンプラー
		if (shTex) {
			pID3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//サンプラーを設定
			//auto pSampler = RenderState->GetLinearWrap();
			//pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
			RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 0);
		}
		else {
			//シェーダーリソースもクリア
			pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			//サンプラーもクリア
			RenderState->SetSamplerAllClear(pID3D11DeviceContext);
		}



		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = LINKCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		
		//
		ID3D11Buffer* pParamBuffer = PARAMCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(1, 1, &pParamBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(1, 1, &pParamBuffer);
		//
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();

	}

	void WaterDraw::UpdateParam(ShaderParam &SetParam)
	{
		auto pD3D11DeviceContext = App::GetApp()->GetDeviceResources()->GetD3DDeviceContext();
		pD3D11DeviceContext->UpdateSubresource(PARAMCB::GetPtr()->GetBuffer(), 0, nullptr, &SetParam, 0, 0);
	}

	void WaterDraw::SetTextures(UINT StartSlot, UINT NumViews, const wstring&Key)
	{
		//デバイスの取得
		auto pD3D11DeviceContext = App::GetApp()->GetDeviceResources()->GetD3DDeviceContext();
		auto Res = App::GetApp()->GetResource<TextureResource>(Key);
		pD3D11DeviceContext->PSSetShaderResources(StartSlot, NumViews, Res->GetShaderResourceView().GetAddressOf());
		auto pSampler = App::GetApp()->GetDeviceResources()->GetRenderState()->GetLinearClamp();

		pD3D11DeviceContext->PSSetSamplers(StartSlot, NumViews, &pSampler);
	}


	void NormalMapDraw::OnDraw()
	{
		//下準備
		auto PtrGameObject = GetGameObject();
		//メッシュの確認
		auto MeshRes = GetMeshResource();
		if (!MeshRes)
		{
			throw BaseException(
				L"メッシュが作成されてません",
				L"if(!MeshRes)",
				L"NormalMap::OnDraw()");
		}

		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//NULLのシェーダリソースの準備
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11ShaderResourceView* pNullMap[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

		//コンスタントバッファ準備
		auto TransComp = PtrGameObject->GetComponent<Transform>();
		//取得
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();
		//ビュー行列、射影行列の取得
		Mat4x4 View, Projection;
		View = PtrCamera->GetViewMatrix();
		Projection = PtrCamera->GetProjMatrix();

		//コンスタントバッファの設定
		ShaderBuffer CB;
		CB.World = transpose(TransComp->GetWorldMatrix());
		CB.View = transpose(View);
		CB.Projection = transpose(Projection);

		//ライト
		auto PtrLight = PtrGameObject->OnGetDrawLight();
		CB.LightDir = PtrLight.m_Directional;
		CB.LightDir.w = 1.0f;

		//テクスチャ
		auto shTex = GetTextureResource();
		auto mapTex = GetNormalMapTextureResource();
		if (shTex)
		{
			//テクスチャがある
			CB.ActiveFlg.x = 1;
			//マッピングテクスチャ
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
			//描画コンポーネントにはテクスチャがない
			//テクスチャがなければマッピングも行わない
			CB.ActiveFlg.x = 0;
			CB.ActiveFlg.y = 0;
		}

		pID3D11DeviceContext->UpdateSubresource(LINKCB::GetPtr()->GetBuffer(), 0, nullptr, &CB, 0, 0);

		//ストライド・オフセット
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点パッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法の設定(三角形)
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		//ステータス
		//ブレンドステートの設定
		//半透明
		float blendstate[4] = { 0.0f,0.0f,0.0f,0.0f };
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), blendstate, D3D11_DEFAULT_SAMPLE_MASK);
		//デプスステンシル
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSNormalMap::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSNormalMap::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSNormalMap::GetPtr()->GetInputLayout());

		//テクスチャ・サンプラー
		if (shTex)
		{
			pID3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//サンプラー
			RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 0);
			if (mapTex)
			{
				pID3D11DeviceContext->PSSetShaderResources(1, 1, mapTex->GetShaderResourceView().GetAddressOf());
				//サンプラー
				RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 1);
			}
			else
			{
				pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNullMap);
			}
		}
		else
		{
			//シェーダーリソースもクリア
			pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNullMap);
			//サンプラーもクリア
			RenderState->SetSamplerAllClear(pID3D11DeviceContext);
		}


		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = LINKCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//汎用パラメータの設定

		//レンダーステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//レンダーステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}

	void NormalMapDraw::SetNormalMapTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		m_NormalMapTextureResource = TextureRes;

	}

	void NormalMapDraw::SetNormalMapTextureResource(const wstring& TextureKey) {
		this->SetNormalMapTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<TextureResource> NormalMapDraw::GetNormalMapTextureResource() const {
		//テクスチャがなければnullを返す
		auto shptr = m_NormalMapTextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;

	}

	//
	void POMapDraw::OnDraw()
	{
		//下準備
		auto PtrGameObject = GetGameObject();
		//メッシュの確認
		auto MeshRes = GetMeshResource();
		if (!MeshRes)
		{
			throw BaseException(
				L"メッシュが作成されてません",
				L"if(!MeshRes)",
				L"NormalMap::OnDraw()");
		}

		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//NULLのシェーダリソースの準備
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		//ID3D11ShaderResourceView* pNullMap[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

		//コンスタントバッファ準備
		auto TransComp = PtrGameObject->GetComponent<Transform>();
		//取得
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();
		//ビュー行列、射影行列の取得
		Mat4x4 View, Projection;
		View = PtrCamera->GetViewMatrix();
		Projection = PtrCamera->GetProjMatrix();

		//コンスタントバッファの設定
		ShaderBuffer CB;
		CB.World = transpose(TransComp->GetWorldMatrix());
		CB.View = transpose(View);
		CB.Projection = transpose(Projection);

		//ライト
		auto PtrLight = PtrGameObject->OnGetDrawLight();
		CB.LightDir = PtrLight.m_Directional;
		CB.LightDir.w = 1.0f;

		//カメラ
		CB.Camera =normalize(PtrCamera->GetAt()-PtrCamera->GetEye());
		
		//テクスチャ
		auto shTex = GetTextureResource();
		//auto mapTex = GetHeightMapTextureResource();
		if (shTex)
		{
			//テクスチャがある
			CB.ActiveFlg.x = 1;
			//マッピングテクスチャ
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
			//描画コンポーネントにはテクスチャがない
			//テクスチャがなければマッピングも行わない
			CB.ActiveFlg.x = 0;
			CB.ActiveFlg.y = 0;
		}

		pID3D11DeviceContext->UpdateSubresource(LINKCB::GetPtr()->GetBuffer(), 0, nullptr, &CB, 0, 0);

		//ストライド・オフセット
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点パッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法の設定(三角形)
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//ステータス
		//ブレンドステートの設定
		//半透明
		float blendstate[4] = { 0.0f,0.0f,0.0f,0.0f };
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), blendstate, D3D11_DEFAULT_SAMPLE_MASK);
		//デプスステンシル
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSPOMap::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSPOMap::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPOMap::GetPtr()->GetInputLayout());

		//テクスチャ・サンプラー
		if (shTex)
		{
			pID3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//サンプラー
			RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 0);
			//if (mapTex)
			//{
			//	pID3D11DeviceContext->PSSetShaderResources(1, 1, mapTex->GetShaderResourceView().GetAddressOf());
			//	//サンプラー
			//	RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 1);
			//}
			//else
			//{
			//	pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNullMap);
			//}
		}
		else
		{
			//シェーダーリソースもクリア
			pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			//pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNullMap);
			//サンプラーもクリア
			RenderState->SetSamplerAllClear(pID3D11DeviceContext);
		}


		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = LINKCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//汎用パラメータの設定
		ID3D11Buffer* pParamBuffer = PARAMCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(1, 1, &pParamBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(1, 1, &pParamBuffer);

		//レンダーステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//レンダーステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//後始末
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
				L"メッシュが設定されていません",
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

		//コンスタントバッファ
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


		//ストライド・オフセット
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点パッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法の設定(三角形)
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//ステータス
		//ブレンドステートの設定
		//半透明
		float blendstate[4] = { 0.0f,0.0f,0.0f,0.0f };
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), blendstate, D3D11_DEFAULT_SAMPLE_MASK);
		//デプスステンシル
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSTexBlend::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSTexBlend::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSTexBlend::GetPtr()->GetInputLayout());

		//テクスチャ・サンプラー
		if (mainTex)
		{
			pID3D11DeviceContext->PSSetShaderResources(0, 1, mainTex->GetShaderResourceView().GetAddressOf());
			//サンプラー
			RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 0);
			if (subTex&&maskTex)
			{
				pID3D11DeviceContext->PSSetShaderResources(1, 1, subTex->GetShaderResourceView().GetAddressOf());
				pID3D11DeviceContext->PSSetShaderResources(2, 1, maskTex->GetShaderResourceView().GetAddressOf());
				//サンプラー
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
			//シェーダーリソースもクリア
			pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pMainNull);
			pID3D11DeviceContext->PSSetShaderResources(1, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSubNull);
			pID3D11DeviceContext->PSSetShaderResources(2, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pMaskNull);
			//サンプラーもクリア
			RenderState->SetSamplerAllClear(pID3D11DeviceContext);
		}


		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = LINKCB::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//汎用パラメータの設定
		//ID3D11Buffer* pParamBuffer = PARAMCB::GetPtr()->GetBuffer();
		//pID3D11DeviceContext->VSSetConstantBuffers(1, 1, &pParamBuffer);
		//pID3D11DeviceContext->PSSetConstantBuffers(1, 1, &pParamBuffer);

		//レンダーステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//レンダーステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//後始末
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