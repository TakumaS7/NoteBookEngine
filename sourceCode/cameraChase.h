/*===============================================================*/
//
//	cameraChase.h(追いカメラ処理)
// 
// 
// ---------------------------------- 2025/07/16 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CAMERA_CHASE_H
#define _CAMERA_CHASE_H

#include "component.h"
#include "transform.h"
#include "gameObject.h"

class CameraChase : public Component
{
private:
	Transform* m_transform;
	GameObject* m_target;
	Vector3 m_targetPosition = { 0.0f, 0.0f, 0.0f };

public:
	void Set(Transform* transform, GameObject* target) 
	{ 
		m_transform = transform; 
		m_target = target;
	}

	void Init() override {}
	void Uninit() override {}
	void Update() override 
	{
		//カメラがプレイヤーの座標を取得
		m_targetPosition = m_target->GetComponent<Transform>()->position;

		//プレイヤーについていく
		m_transform->position = m_targetPosition
			+ Vector3(-sinf(m_transform->rotation.y), 0.5f, -cosf(m_transform->rotation.y)) * 5.0f;
	}

	void Draw() override {}
};

#endif //_CAMERA_CHASE_H