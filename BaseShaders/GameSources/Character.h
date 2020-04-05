/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Water :public GameObject
	{
	public:
		Water(const shared_ptr<Stage>&StagePtr)
			:GameObject(StagePtr),m_key(0.0f)
		{}

		void OnCreate()override;
		void OnUpdate()override;

	private:
		float m_key;
	};

	//Block(�m�[�}���}�b�s���O)
	class Block :public GameObject
	{
	public:
		Block(const shared_ptr<Stage>&StagePtr)
			:GameObject(StagePtr)
		{

		}
		void OnCreate()override;
	};

	class Plane : public GameObject
	{
	public: 
		Plane(const shared_ptr<Stage>&StagePtr)
			:GameObject(StagePtr)
		{}
		void OnCreate()override;
		void OnUpdate()override;

		void DebugLog();
	};
}
//end basecross
