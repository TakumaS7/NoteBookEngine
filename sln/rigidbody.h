/*===============================================================*/
//
//	rigidbody.h(リジッドボディ)
// 
// 
// ---------------------------------- 2025/07/19 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

#include "component.h"
#include "gameObject.h"
#include "transform.h"
#include "vector3.h"

class Rigidbody : public Component
{
private:
	GameObject* m_gameObject;
	float m_gravityAccel = 0.0098f;	// 重力加速度
	Vector3 m_force = { 0.0f, 0.0f, 0.0f };
	Vector3 m_velocity = { 0.0f, 0.0f, 0.0f };
	bool m_useGravity = false;
	float m_friction = 0.2f;	// 簡易摩擦係数
	bool m_onGround;	// 地面に乗ったかのフラグ

	Vector3 m_constraintsPosition = { 0.0f, 0.0f, 0.0f };	// RigidbodyConstraintsで座標固定に使用
	Vector3 m_constraintsRotation = { 0.0f, 0.0f, 0.0f };	// RigidbodyConstraintsで回転固定に使用
	bool m_freezePositionX = false;
	bool m_freezePositionY = false;
	bool m_freezePositionZ = false;
	bool m_freezeRotationX = false;
	bool m_freezeRotationY = false;
	bool m_freezeRotationZ = false;

public:

	enum RigidbodyConstraints
	{
		NONE,				//　フリーズしない
		FREEZE_POSITION_X,	//　座標Xだけフリーズ
		FREEZE_POSITION_Y,	//　座標Yだけフリーズ
		FREEZE_POSITION_Z,	//　座標Zだけフリーズ
		FREEZE_POSITION,	//　座標全てフリーズ
		FREEZE_ROTATION_X,	//　X軸回転だけフリーズ
		FREEZE_ROTATION_Y,	//　Y軸回転だけフリーズ
		FREEZE_ROTATION_Z,	//　Z軸回転だけフリーズ
		FREEZE_ROTATION,	//　回転全てフリーズ
		FREEZE_ALL,			//　座標も回転も全てフリーズ
	};

	RigidbodyConstraints m_rigidbodyConstraints1 = RigidbodyConstraints::NONE;
	RigidbodyConstraints m_rigidbodyConstraints2 = RigidbodyConstraints::NONE;

	void Set(GameObject* gameObject, const bool& useGravity, const RigidbodyConstraints& rbConstraintsPosition, const RigidbodyConstraints& rbConstraintsRotation)
	{
		m_gameObject = gameObject;
		m_useGravity = useGravity;

		m_rigidbodyConstraints1 = rbConstraintsPosition;
		m_rigidbodyConstraints2 = rbConstraintsRotation;

		m_constraintsPosition = gameObject->GetComponent<Transform>()->position;
		m_constraintsRotation = gameObject->GetComponent<Transform>()->rotation;
	}

	void Set(GameObject* gameObject, const bool& useGravity, const RigidbodyConstraints& rbConstraints)
	{
		m_gameObject = gameObject;
		m_useGravity = useGravity;

		m_rigidbodyConstraints1 = rbConstraints;
	}

	void AddForce(Vector3 force) { m_force += force; }

	Vector3 GetForce() { return m_force; }

	bool GetUseGravity() { return m_useGravity; }
	void SetUseGravity(bool use) { m_useGravity = use; }

	void SetOnGround(bool onGround) { m_onGround = onGround; }
	bool GetOnGraund() { return m_onGround; }

	void SetVelocity(Vector3 velocity) { m_velocity = velocity; }
	Vector3 GetVelocity() { return m_velocity; }

	void Init() override {}

	void Uninit() override {}
	void Update() override 
	{
		m_velocity += m_force;

		if (m_useGravity)
		{
			m_velocity.y += -m_gravityAccel;
		}
		
		if (m_onGround)
		{
			if (fabs(m_velocity.x) < 0.0001f) { m_velocity.x = 0.0f; }
			if (fabs(m_velocity.y) < 0.0001f) { m_velocity.y = 0.0f; }
			if (fabs(m_velocity.z) < 0.0001f) { m_velocity.z = 0.0f; }
		}

		m_velocity.x *= (1.0f - m_friction);
		m_velocity.z *= (1.0f - m_friction);

		m_gameObject->GetComponent<Transform>()->position += m_velocity;

		m_force = { 0.0f, 0.0f, 0.0f };


		/* RigidbodyConstraints処理 */
		switch (m_rigidbodyConstraints1)
		{
		case Rigidbody::NONE:
			break;
		case Rigidbody::FREEZE_POSITION_X:
			m_gameObject->GetComponent<Transform>()->position.x = m_constraintsPosition.x;
			break;
		case Rigidbody::FREEZE_POSITION_Y:
			m_gameObject->GetComponent<Transform>()->position.y = m_constraintsPosition.y;
			break;
		case Rigidbody::FREEZE_POSITION_Z:
			m_gameObject->GetComponent<Transform>()->position.z = m_constraintsPosition.z;
			break;
		case Rigidbody::FREEZE_POSITION:
			m_gameObject->GetComponent<Transform>()->position = m_constraintsPosition;
			break;
		case Rigidbody::FREEZE_ALL:
			m_gameObject->GetComponent<Transform>()->position = m_constraintsPosition;
			m_gameObject->GetComponent<Transform>()->rotation = m_constraintsRotation;
			break;
		default:
			break;
		}

		switch (m_rigidbodyConstraints2)
		{
		case Rigidbody::NONE:
			break;
		case Rigidbody::FREEZE_ROTATION_X:
			m_gameObject->GetComponent<Transform>()->rotation.x = m_constraintsRotation.x;
			break;
		case Rigidbody::FREEZE_ROTATION_Y:
			m_gameObject->GetComponent<Transform>()->rotation.y = m_constraintsRotation.y;
			break;
		case Rigidbody::FREEZE_ROTATION_Z:
			m_gameObject->GetComponent<Transform>()->rotation.z = m_constraintsRotation.z;
			break;
		case Rigidbody::FREEZE_ROTATION:
			m_gameObject->GetComponent<Transform>()->rotation = m_constraintsRotation;
			break;
		default:
			break;
		}
	}
	void Draw() override {}
};

#endif //_RIGIDBODY_H