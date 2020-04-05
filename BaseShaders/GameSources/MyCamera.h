/*!
@file MyCamera.h
@brief �J����
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	MyCamera
	//--------------------------------------------------------------------------------------
	class MyCamera : public Camera {
		weak_ptr<GameObject> m_TargetObject;	
		float m_ToTargetLerp;	
		bsm::Vec3 m_TargetToAt;	
		float m_RadY;
		float m_RadXZ;
		float m_CameraUpDownSpeed;
		float m_CameraUnderRot;
		float m_ArmLen;
		float m_MaxArm;
		float m_MinArm;
		float m_RotSpeed;
		float m_ZoomSpeed;
		bool m_LRBaseMode;
		bool m_UDBaseMode;
	public:
		MyCamera();
		MyCamera(float ArmLen);
		virtual ~MyCamera();
		virtual void SetEye(const bsm::Vec3& Eye)override;
		virtual void SetEye(float x, float y, float z)override;
		shared_ptr<GameObject> GetTargetObject() const;
		void SetTargetObject(const shared_ptr<GameObject>& Obj);
		float GetToTargetLerp() const;
		void SetToTargetLerp(float f);
		float GetArmLengh() const;
		void UpdateArmLengh();
		float GetMaxArm() const;
		void SetMaxArm(float f);
		float GetMinArm() const;
		void SetMinArm(float f);
		float GetRotSpeed() const;
		void SetRotSpeed(float f);
		bsm::Vec3 GetTargetToAt() const;
		void SetTargetToAt(const bsm::Vec3& v);
		bool GetLRBaseMode() const;
		bool IsLRBaseMode() const;
		void SetLRBaseMode(bool b);
		bool GetUDBaseMode() const;
		bool IsUDBaseMode() const;
		void SetUDBaseMode(bool b);
		virtual void SetAt(const bsm::Vec3& At)override;
		virtual void SetAt(float x, float y, float z)override;
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void Debug();
	};

}
//end basecross
