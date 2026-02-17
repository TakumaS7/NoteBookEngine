/*===============================================================*/
//
//	keyMove.h(ƒL[“ü—Í‘€ì)
// 
// 
// ---------------------------------- 2025/07/15 »ìŽÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _KEY_MOVE_H
#define _KEY_MOVE_H

#include "component.h"
#include "manager.h"
#include "input.h"
#include "transform.h"
#include "rigidbody.h"

class KeyMove : public Component
{
private:
	GameObject* m_gameObject;
	GameObject* m_camera;

public:
	void Set(GameObject* gameObject, GameObject* camera) 
	{ 
		m_gameObject = gameObject;
		m_camera = camera; 
	}

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		Vector3 rotation = m_camera->GetComponent<Transform>()->rotation;

		if (Input::GetKeyPress('A'))
		{
			m_gameObject->GetComponent<Rigidbody>()->AddForce(m_camera->GetComponent<Transform>()->GetRight() * -0.05f);
			m_gameObject->GetComponent<Transform>()->rotation.y = rotation.y - XM_PIDIV2;
		}

		if (Input::GetKeyPress('D'))
		{
			m_gameObject->GetComponent<Rigidbody>()->AddForce(m_camera->GetComponent<Transform>()->GetRight() * 0.05f);
			m_gameObject->GetComponent<Transform>()->rotation.y = rotation.y + XM_PIDIV2;
		}

		if (Input::GetKeyPress('W'))
		{
			m_gameObject->GetComponent<Rigidbody>()->AddForce(m_camera->GetComponent<Transform>()->GetForward() * 0.05f);
			m_gameObject->GetComponent<Transform>()->rotation.y = rotation.y;
		}

		if (Input::GetKeyPress('S'))
		{
			m_gameObject->GetComponent<Rigidbody>()->AddForce(m_camera->GetComponent<Transform>()->GetForward() * -0.05f);
			m_gameObject->GetComponent<Transform>()->rotation.y = rotation.y + XM_PI;
		}
	}

	void Draw() override {}
};

#endif //_KEY_MOVE_H