﻿/*!
@file MyCamera.cpp
@brief �J��������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	MyCamera�J�����i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MyCamera::MyCamera() :
		Camera(),
		m_ToTargetLerp(1.0f),
		m_TargetToAt(0, 0, 0),
		m_RadY(0.5f),
		m_RadXZ(0),
		m_CameraUpDownSpeed(0.5f),
		m_CameraUnderRot(0.1f),
		m_ArmLen(5.0f),
		m_MaxArm(20.0f),
		m_MinArm(2.0f),
		m_RotSpeed(1.0f),
		m_ZoomSpeed(0.1f),
		m_LRBaseMode(true),
		m_UDBaseMode(true)
	{}

	MyCamera::MyCamera(float ArmLen) :
		Camera(),
		m_ToTargetLerp(1.0f),
		m_TargetToAt(0, 0, 0),
		m_RadY(0.5f),
		m_RadXZ(0),
		m_CameraUpDownSpeed(0.5f),
		m_CameraUnderRot(0.1f),
		m_ArmLen(5.0f),
		m_MaxArm(20.0f),
		m_MinArm(2.0f),
		m_RotSpeed(1.0f),
		m_ZoomSpeed(0.1f),
		m_LRBaseMode(true),
		m_UDBaseMode(true)
	{
		m_ArmLen = ArmLen;
		auto eye = GetEye();
		eye.y = m_ArmLen;
		SetEye(eye);
	}

	MyCamera::~MyCamera() {}
	//�A�N�Z�T

	void MyCamera::SetEye(const bsm::Vec3& Eye) {
		Camera::SetEye(Eye);
		UpdateArmLengh();
	}
	void MyCamera::SetEye(float x, float y, float z) {
		Camera::SetEye(x, y, z);
		UpdateArmLengh();
	}


	shared_ptr<GameObject> MyCamera::GetTargetObject() const {
		if (!m_TargetObject.expired()) {
			return m_TargetObject.lock();
		}
		return nullptr;
	}

	void MyCamera::SetTargetObject(const shared_ptr<GameObject>& Obj) {
		m_TargetObject = Obj;
	}

	float MyCamera::GetToTargetLerp() const {
		return m_ToTargetLerp;
	}
	void MyCamera::SetToTargetLerp(float f) {
		m_ToTargetLerp = f;
	}

	float MyCamera::GetArmLengh() const {
		return m_ArmLen;
	}

	void MyCamera::UpdateArmLengh() {
		auto vec = GetEye() - GetAt();
		m_ArmLen = bsm::length(vec);
		if (m_ArmLen >= m_MaxArm) {
			m_ArmLen = m_MaxArm;
		}
		if (m_ArmLen <= m_MinArm) {
			m_ArmLen = m_MinArm;
		}
	}

	float MyCamera::GetMaxArm() const {
		return m_MaxArm;

	}
	void MyCamera::SetMaxArm(float f) {
		m_MaxArm = f;
	}
	float MyCamera::GetMinArm() const {
		return m_MinArm;
	}
	void MyCamera::SetMinArm(float f) {
		m_MinArm = f;
	}

	float MyCamera::GetRotSpeed() const {
		return m_RotSpeed;

	}
	void MyCamera::SetRotSpeed(float f) {
		m_RotSpeed = abs(f);
	}

	bsm::Vec3 MyCamera::GetTargetToAt() const {
		return m_TargetToAt;

	}
	void MyCamera::SetTargetToAt(const bsm::Vec3& v) {
		m_TargetToAt = v;
	}

	bool MyCamera::GetLRBaseMode() const {
		return m_LRBaseMode;

	}
	bool MyCamera::IsLRBaseMode() const {
		return m_LRBaseMode;

	}
	void MyCamera::SetLRBaseMode(bool b) {
		m_LRBaseMode = b;
	}
	bool MyCamera::GetUDBaseMode() const {
		return m_UDBaseMode;

	}
	bool MyCamera::IsUDBaseMode() const {
		return m_UDBaseMode;
	}
	void MyCamera::SetUDBaseMode(bool b) {
		m_UDBaseMode = b;

	}


	void MyCamera::SetAt(const bsm::Vec3& At) {
		Camera::SetAt(At);
		Vec3 armVec = GetEye() - GetAt();
		armVec.normalize();
		armVec *= m_ArmLen;
		Vec3 newEye = GetAt() + armVec;
		Camera::SetEye(newEye);
	}
	void MyCamera::SetAt(float x, float y, float z) {
		Camera::SetAt(x, y, z);
		Vec3 armVec = GetEye() - GetAt();
		armVec.normalize();
		armVec *= m_ArmLen;
		Vec3 newEye = GetAt() + armVec;
		Camera::SetEye(newEye);

	}

	void MyCamera::OnCreate()
	{

	}

	void MyCamera::OnUpdate() {
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto keyData = App::GetApp()->GetInputDevice().GetKeyState();
		float elapsedTime = App::GetApp()->GetElapsedTime();
		Vec3 newEye = GetEye();
		Vec3 newAt = GetAt();
		bsm::Vec3 armVec = newEye - newAt;
		armVec.normalize();
		float fThumbRY = 0.0f;
		float fThumbRX = 0.0f;
		WORD wButtons = 0;
		if (cntlVec[0].bConnected) {
			fThumbRY = cntlVec[0].fThumbRY;
			fThumbRX = cntlVec[0].fThumbRX;
			wButtons = cntlVec[0].wButtons;
		}

		if (fThumbRY >= 0.1f || keyData.m_bPushKeyTbl[VK_UP]) {
			if (IsUDBaseMode()) {
				m_RadY += m_CameraUpDownSpeed * elapsedTime;
			}
			else {
				m_RadY -= m_CameraUpDownSpeed * elapsedTime;
			}
		}
		else if (fThumbRY <= -0.1f || keyData.m_bPushKeyTbl[VK_DOWN]) {
			if (IsUDBaseMode()) {
				m_RadY -= m_CameraUpDownSpeed * elapsedTime;
			}
			else {
				m_RadY += m_CameraUpDownSpeed * elapsedTime;
			}
		}
		if (m_RadY > XM_PI * 4 / 9.0f) {
			m_RadY = XM_PI * 4 / 9.0f;
		}
		else if (m_RadY <= m_CameraUnderRot) {
			m_RadY = m_CameraUnderRot;
		}
		armVec.y = sin(m_RadY);
		if (fThumbRX != 0 || keyData.m_bPushKeyTbl[VK_LEFT] || keyData.m_bPushKeyTbl[VK_RIGHT]) {
			if (fThumbRX != 0) {
				if (IsLRBaseMode()) {
					m_RadXZ += -fThumbRX * elapsedTime * m_RotSpeed;
				}
				else {
					m_RadXZ += fThumbRX * elapsedTime * m_RotSpeed;
				}
			}
			else if (keyData.m_bPushKeyTbl[VK_LEFT]) {
				if (IsLRBaseMode()) {
					m_RadXZ += elapsedTime * m_RotSpeed;
				}
				else {
					m_RadXZ -= elapsedTime * m_RotSpeed;
				}
			}
			else if (keyData.m_bPushKeyTbl[VK_RIGHT]) {
				if (IsLRBaseMode()) {
					m_RadXZ -= elapsedTime * m_RotSpeed;
				}
				else {
					m_RadXZ += elapsedTime * m_RotSpeed;
				}

			}
			if (abs(m_RadXZ) >= XM_2PI) {
				m_RadXZ = 0;
			}
		}
		Quat qtXZ;
		qtXZ.rotation(m_RadXZ, bsm::Vec3(0, 1.0f, 0));
		qtXZ.normalize();
		Mat4x4 Mat;
		Mat.strTransformation(
			bsm::Vec3(1.0f, 1.0f, 1.0f),
			bsm::Vec3(0.0f, 0.0f, -1.0f),
			qtXZ
		);

		Vec3 posXZ = Mat.transInMatrix();
		armVec.x = posXZ.x;
		armVec.z = posXZ.z;
		armVec.normalize();

		auto ptrTarget = GetTargetObject();
		if (ptrTarget) {
			Vec3 toAt = ptrTarget->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			toAt += m_TargetToAt;
			newAt = Lerp::CalculateLerp(GetAt(), toAt, 0, 1.0f, 1.0, Lerp::Linear);
		}
		if (wButtons & XINPUT_GAMEPAD_DPAD_DOWN || keyData.m_bPushKeyTbl[VK_NEXT]) {
			m_ArmLen += m_ZoomSpeed;
			if (m_ArmLen >= m_MaxArm) {
				m_ArmLen = m_MaxArm;
			}
		}
		else if (wButtons & XINPUT_GAMEPAD_DPAD_UP || keyData.m_bPushKeyTbl[VK_PRIOR]) {
			m_ArmLen -= m_ZoomSpeed;
			if (m_ArmLen <= m_MinArm) {
				m_ArmLen = m_MinArm;
			}
		}
		Vec3 toEye = newAt + armVec * m_ArmLen;
		newEye = Lerp::CalculateLerp(GetEye(), toEye, 0, 1.0f, m_ToTargetLerp, Lerp::Linear);

		SetAt(newAt);
		SetEye(newEye);
		UpdateArmLengh();
		Camera::OnUpdate();
	}

	void MyCamera::Debug()
	{

	}

}
//end basecross