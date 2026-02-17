/*===============================================================*/
//
//	cameraAutoRotate.h(ƒJƒƒ‰Ž©“®‰ñ“]ˆ—)
// 
// 
// ---------------------------------- 2025/12/05 »ìŽÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _CAMERA_AUTO_ROTATE_H
#define _CAMERA_AUTO_ROTATE_H

#include "main.h"
#include "component.h"
#include "renderer.h"
#include "gameObject.h"
#include "transform.h"
#include "vector3.h"

class CameraAutoRotate : public Component
{
private:
	Transform* m_transform;

public:
	void Set(Transform* transform)
	{
		m_transform = transform;
	}

	void Init() override {}
	void Uninit() override {}
	void Update() override 
	{
		m_transform->rotation.y += 0.005f;

		if (m_transform->rotation.y >= 3.14159265358979f * 2.0f) {
			m_transform->rotation.y = 0.0f;
		}
	}
	void Draw() override {}
};

#endif //_CAMERA_AUTO_ROTATE_H