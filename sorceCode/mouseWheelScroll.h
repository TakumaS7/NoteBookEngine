/*===============================================================*/
//
//	mouseWheelScroll.h(マウスホイールスクロール操作)
// 
// 
// ---------------------------------- 2025/10/06 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _MOUSE_WHEEL_SCROLL_H
#define _MOUSE_WHEEL_SCROLL_H

#include "component.h"
#include "transform.h"
#include "mouse.h"
#include "time.h"

class MouseWheelScroll : public Component
{
private:
	int m_scroll = 0;				// ホイール入力値取得
	Transform* m_transform;			// 自分のトランスフォーム
	float m_scrollStep = 80.0f;		// ホイール1回転あたりのスクロール量
	float m_scrollMin = -500.0f;	// 上限
	float m_scrollMax = 500.0f;		// 下限

public:
	void Set(Transform* transform) { m_transform = transform; }

	void SetScrollMax(float scrollMax) { m_scrollMax = scrollMax; }
	void SetScrollMin(float scrollMin) { m_scrollMin = scrollMin; }
	float GetScrollMin() const { return m_scrollMin; }
	float GetScrollMax() const { return m_scrollMax; }

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		if (!m_transform) { return; }

		/* ホイールの回転量取得 */
		m_scroll = Mouse::GetScrollWheelValue();

		if (m_scroll != 0)
		{
			/* ブラウザのように瞬間的に位置を動かす */
			float move = (m_scroll / 120.0f) * m_scrollStep;
			m_transform->position.y += move;	// 通常スクロール方向に合わせて反転

			/* 範囲制限 */
			if (m_transform->position.y < m_scrollMin)
			{
				m_transform->position.y = m_scrollMin;
			}
			else if (m_transform->position.y > m_scrollMax)
			{
				m_transform->position.y = m_scrollMax;
			}

			/* ホイール入力をリセット */
			Mouse::ResetScrollWheelValue();
		}
	}

	void Draw() override {}
};

#endif //_MOUSE_WHEEL_SCROLL_H