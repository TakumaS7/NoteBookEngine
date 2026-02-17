/*===============================================================*/
//
//	cameraView.h(ƒJƒƒ‰‰æ–Êˆ—)
// 
// 
// ---------------------------------- 2025/07/16 »ìŽÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _CAMERA_VIEW_H
#define _CAMERA_VIEW_H

#include "main.h"
#include "component.h"
#include "renderer.h"
#include "gameObject.h"
#include "transform.h"
#include "vector3.h"

class CameraView : public Component
{
private:
	Transform* m_transform;
	GameObject* m_target;
	XMMATRIX m_Projection;
	XMMATRIX m_View;
	Vector3 targetPosition = { 0.0f, 0.0f, 0.0f };

public:
	void Set(Transform* transform, GameObject* target)
	{
		m_transform = transform;
		m_target = target;
	}

	XMMATRIX& GetViewMatrix() { return m_View; }

	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override 
	{
		m_Projection = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

		Renderer::SetProjectionMatrix(m_Projection);

		targetPosition = m_target->GetComponent<Transform>()->position;
		XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_View = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&m_transform->position), XMLoadFloat3((XMFLOAT3*)&targetPosition), XMLoadFloat3(&up));

		Renderer::SetViewMatrix(m_View);
	}
};

#endif //_CAMERA_VIEW_H