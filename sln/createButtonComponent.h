/*===============================================================*/
//
//	createButtonComponent.h(クリエイトシーンへ移行するボタン操作)
// 
// 
// ---------------------------------- 2025/09/26 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_BUTTON_COMPONENT_H
#define _CREATE_BUTTON_COMPONENT_H

#include "main.h"
#include "gameObject.h"
#include "component.h"
#include "mouseCollision.h"
#include "mouse.h"
#include "fade.h"
#include "imGuiManager.h"

class CreateButtonComponent : public Component
{
private:
	GameObject* m_gameObject;						// ボタン自身
	bool m_buttonHit = false;						// ボタン自身の当たり判定情報

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	void Init() override {}
	void Uninit() override {}
	void Update() override 
	{
		/* マウスがボタンに当たっているかフラグの取得 */
		m_buttonHit = m_gameObject->GetComponent<MouseCollision>()->GetMouseHit();

		/* 左クリック処理 */
		if (m_buttonHit && Mouse::GetLeftClickTrigger())
		{
			ImGuiManager::OpenToProjectSelect();
		}
	}

	void Draw() override {}
};

#endif //_CREATE_BUTTON_COMPONENT_H