/*===============================================================*/
//
//	playButtonComponent.h(クリエイトシーンへ移行するボタン操作)
// 
// 
// ---------------------------------- 2025/09/26 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _PLAY_BUTTON_COMPONENT_H
#define _PLAY_BUTTON_COMPONENT_H

#include "main.h"
#include "gameObject.h"
#include "component.h"
#include "mouseCollision.h"
#include "mouse.h"
#include "fade.h"

class PlayButtonComponent : public Component
{
private:
	GameObject* m_gameObject;			// ボタン自身
	bool m_buttonHit = false;			// ボタン自身の当たり判定情報
	bool m_createDebugWindow = false;	// ボタンを押したらデバッグウィンドウ生成

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		m_createDebugWindow = false;

		/* マウスがボタンに当たっているかフラグの取得 */
		m_buttonHit = m_gameObject->GetComponent<MouseCollision>()->GetMouseHit();

		/* 左クリック処理 */
		if (m_buttonHit && Mouse::GetLeftClickTrigger())
		{
			m_createDebugWindow = true;
		}

		if (m_createDebugWindow) {
			CreateDebugWindow();
		}
	}

	void Draw() override {}
};

#endif //_PLAY_BUTTON_COMPONENT_H