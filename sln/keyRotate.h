/*===============================================================*/
//
//	keyRotate.h(ƒL[“ü—Í‰ñ“]‘€ì)
// 
// 
// ---------------------------------- 2025/07/16 »ìŽÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _KEY_ROTATE_H
#define _KEY_ROTATE_H

#include "main.h"
#include "component.h"
#include "transform.h"
#include "input.h"

class KeyRotate : public Component
{
private:
	Transform* m_transform;

public:
	void Set(Transform* transform) { m_transform = transform; }

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		//‰ñ“]ˆ—
		if (Input::GetKeyPress(VK_LEFT)) {
			m_transform->rotation.y += 0.01f;
		}

		if (Input::GetKeyPress(VK_RIGHT)) {
			m_transform->rotation.y += -0.01f;
		}

		if (abs(m_transform->rotation.y) >= XM_2PI) {
			m_transform->rotation.y = 0.0f;
		}
	}

	void Draw() override {}
};

#endif //_KEY_ROTATE_H