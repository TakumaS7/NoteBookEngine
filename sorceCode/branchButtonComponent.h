/*===============================================================*/
//
//	branchButtonComponent.h(分岐ボタン操作)
// 
// 
// ---------------------------------- 2026/02/12 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _BRANCH_BUTTON_COMPONENT_H
#define _BRANCH_BUTTON_COMPONENT_H

#include "main.h"
#include "gameObject.h"
#include "component.h"
#include "mouseCollision.h"
#include "mouse.h"
#include "fade.h"

class BranchButtonComponent : public Component
{
private:
	GameObject* m_gameObject;	// ボタン自身
	bool m_buttonHit = false;	// ボタン自身の当たり判定情報
	bool m_buttonPush = false;	// ボタンを押したらtrue

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	/* ボタンを押したらtrue */
	bool ButtonPush() const { return m_buttonPush == true; }

	void SetButtonPush(bool push) { m_buttonPush = push; }

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		m_buttonPush = false;

		/* マウスがボタンに当たっているかフラグの取得 */
		m_buttonHit = m_gameObject->GetComponent<MouseCollision>()->GetMouseHit();

		/* 左クリック処理 */
		if (m_buttonHit && Mouse::GetLeftClickTrigger())
		{
			m_buttonPush = true;
		}
	}

	void Draw() override {}
};

#endif //_BRANCH_BUTTON_COMPONENT_H