/*===============================================================*/
//
//	keyJump.h(キー入力ジャンプ操作)
// 
// 
// ---------------------------------- 2025/07/21 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _KEY_JUMP_H
#define _KEY_JUMP_H

#include "main.h"
#include "component.h"
#include "rigidbody.h"
#include "input.h"
#include "aabb.h"

class KeyJump : public Component
{
private:
	GameObject* m_gameObject;

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		if (Input::GetKeyTrigger(VK_SPACE))
		{
			if (m_gameObject->GetComponent<Rigidbody>()->GetOnGraund())
			{
				m_gameObject->GetComponent<Rigidbody>()->AddForce({ 0.0f, 0.3f, 0.0f });
			}
		}
	}

	void Draw() override {}
};

#endif //_KEY_JUMP_H