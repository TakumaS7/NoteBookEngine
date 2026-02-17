/*===============================================================*/
//
//	mouseDragScroll.h(マウスホイールスクロール操作)
// 
// 
// ---------------------------------- 2025/10/06 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _MOUSE_DRAG_SCROLL_H
#define _MOUSE_DRAG_SCROLL_H

#include "component.h"
#include "gameObject.h"
#include "transform.h"
#include "mouse.h"
#include "time.h"
#include "mouseWheelScroll.h"

class MouseDragScroll : public Component
{
private:
	GameObject* m_gameObject;						// 自分のトランスフォーム
	static constexpr float m_scrollPower = 10.0f;	// ホイール1回転あたりのスクロール量
	float m_scrollMin = -500.0f;					// 上限
	float m_scrollMax = 500.0f;						// 下限

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		if (!m_gameObject) { return; }

		/* ドラッグしてないときはreturn */
		if (!Mouse::GetDragging()) { return; }

		/* スクロールの上限下限を保持 */
		if (MouseWheelScroll* mouseWheelScroll = m_gameObject->GetComponent<MouseWheelScroll>()) {
			float scrollMin = mouseWheelScroll->GetScrollMin();
			float scrollMax = mouseWheelScroll->GetScrollMax();

			m_scrollMin = scrollMin;
			m_scrollMax = scrollMax;
		}

		/* ドラッグ対象のトランスフォームの保持 */
		Transform* transform = m_gameObject->GetComponent<Transform>();

		if (!transform) { return; }

		if (Mouse::GetMousePosition().y >= SCREEN_HEIGHT - 50.0f)	// ドラッグ中にマウスが下にいるとき
		{
			transform->position.y += -m_scrollPower;	// 下へスクロール
		}
		else if (Mouse::GetMousePosition().y <= 50.0f)
		{
			transform->position.y += m_scrollPower;	// 上へスクロール
		}

		/* 範囲制限 */
		if (transform->position.y < m_scrollMin)
		{
			transform->position.y = m_scrollMin;
		}
		else if (transform->position.y > m_scrollMax)
		{
			transform->position.y = m_scrollMax;
		}
	}

	void Draw() override {}
};

#endif //_MOUSE_DRAG_SCROLL_H