/*!
@file ProjectDraw.h
@brief プロジェクトで使用する描画コンポーネント
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class WaterDraw :public SmBaseDraw
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief  コンストラクタ
		@param[in]  GameObjectPtr   ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit WaterDraw(const shared_ptr<GameObject>& GameObjectPtr)
			:SmBaseDraw(GameObjectPtr) {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief  デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WaterDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnCreate処理
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnUpdate処理（空関数）
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnDraw処理
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnDraw処理
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextures(UINT StartSlot,UINT NumTextuers,const wstring& Key);

		void UpdateParam(ShaderParam& param);

	private:
		float time;
		unsigned int count;

	};


	class NormalMapDraw :public SmBaseDraw
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief  コンストラクタ
		@param[in]  GameObjectPtr   ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit NormalMapDraw(const shared_ptr<GameObject>& GameObjectPtr)
			:SmBaseDraw(GameObjectPtr) {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief  デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~NormalMapDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnCreate処理
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnUpdate処理（空関数）
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnDraw処理
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  ノーマルマッピング用のテクスチャ
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		void NormalMapDraw::SetNormalMapTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void NormalMapDraw::SetNormalMapTextureResource(const wstring& TextureKey);
		shared_ptr<TextureResource> NormalMapDraw::GetNormalMapTextureResource() const;
	private:
		///テクスチャリソース
		weak_ptr<TextureResource> m_NormalMapTextureResource;
	};


	class POMapDraw :public SmBaseDraw
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief  コンストラクタ
		@param[in]  GameObjectPtr   ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit POMapDraw(const shared_ptr<GameObject>& GameObjectPtr)
			:SmBaseDraw(GameObjectPtr) {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief  デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~POMapDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnCreate処理
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnUpdate処理（空関数）
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnDraw処理
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  ノーマルマッピング用のテクスチャ
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		void SetHeightMapTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void SetHeightMapTextureResource(const wstring& TextureKey);
		shared_ptr<TextureResource> GetHeightMapTextureResource() const;
	private:
		///テクスチャリソース
		weak_ptr<TextureResource> m_HeightMapTextureResource;
	};

	//

	class TexBlendDraw :public SmBaseDraw
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief  コンストラクタ
		@param[in]  GameObjectPtr   ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit TexBlendDraw(const shared_ptr<GameObject>& GameObjectPtr)
			:SmBaseDraw(GameObjectPtr) {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief  デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TexBlendDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnCreate処理
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnUpdate処理（空関数）
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnDraw処理
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  ノーマルマッピング用のテクスチャ
		@return なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSubTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void SetMaskTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void SetSubTextureResource(const wstring& TextureKey);
		void SetMaskTextureResource(const wstring& TextureKey);
		shared_ptr<TextureResource> GetSubTextureResource() const;
		shared_ptr<TextureResource> GetMaskTextureResource() const;
	private:
		///テクスチャリソース
		weak_ptr<TextureResource> m_SubTextureResource;
		weak_ptr<TextureResource> m_MaskTextureResource;
	};


}
//end basecross