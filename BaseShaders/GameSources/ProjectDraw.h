/*!
@file ProjectDraw.h
@brief �v���W�F�N�g�Ŏg�p����`��R���|�[�l���g
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class WaterDraw :public SmBaseDraw
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �R���X�g���N�^
		@param[in]  GameObjectPtr   �Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit WaterDraw(const shared_ptr<GameObject>& GameObjectPtr)
			:SmBaseDraw(GameObjectPtr) {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WaterDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnCreate����
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnUpdate�����i��֐��j
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnDraw����
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnDraw����
		@return �Ȃ�
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
		@brief  �R���X�g���N�^
		@param[in]  GameObjectPtr   �Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit NormalMapDraw(const shared_ptr<GameObject>& GameObjectPtr)
			:SmBaseDraw(GameObjectPtr) {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~NormalMapDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnCreate����
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnUpdate�����i��֐��j
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnDraw����
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �m�[�}���}�b�s���O�p�̃e�N�X�`��
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void NormalMapDraw::SetNormalMapTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void NormalMapDraw::SetNormalMapTextureResource(const wstring& TextureKey);
		shared_ptr<TextureResource> NormalMapDraw::GetNormalMapTextureResource() const;
	private:
		///�e�N�X�`�����\�[�X
		weak_ptr<TextureResource> m_NormalMapTextureResource;
	};


	class POMapDraw :public SmBaseDraw
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �R���X�g���N�^
		@param[in]  GameObjectPtr   �Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit POMapDraw(const shared_ptr<GameObject>& GameObjectPtr)
			:SmBaseDraw(GameObjectPtr) {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~POMapDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnCreate����
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnUpdate�����i��֐��j
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnDraw����
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �m�[�}���}�b�s���O�p�̃e�N�X�`��
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetHeightMapTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void SetHeightMapTextureResource(const wstring& TextureKey);
		shared_ptr<TextureResource> GetHeightMapTextureResource() const;
	private:
		///�e�N�X�`�����\�[�X
		weak_ptr<TextureResource> m_HeightMapTextureResource;
	};

	//

	class TexBlendDraw :public SmBaseDraw
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �R���X�g���N�^
		@param[in]  GameObjectPtr   �Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit TexBlendDraw(const shared_ptr<GameObject>& GameObjectPtr)
			:SmBaseDraw(GameObjectPtr) {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TexBlendDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnCreate����
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnUpdate�����i��֐��j
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief  OnDraw����
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �m�[�}���}�b�s���O�p�̃e�N�X�`��
		@return �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetSubTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void SetMaskTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void SetSubTextureResource(const wstring& TextureKey);
		void SetMaskTextureResource(const wstring& TextureKey);
		shared_ptr<TextureResource> GetSubTextureResource() const;
		shared_ptr<TextureResource> GetMaskTextureResource() const;
	private:
		///�e�N�X�`�����\�[�X
		weak_ptr<TextureResource> m_SubTextureResource;
		weak_ptr<TextureResource> m_MaskTextureResource;
	};


}
//end basecross