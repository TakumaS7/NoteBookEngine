/*===============================================================*/
//
//	buttonZoomInOut.h(ボタンが当たったときにズームイン・アウトする操作)
// 
// 
// ---------------------------------- 2025/12/18 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _BUUTON_ZOOM_IN_OUT_H
#define _BUUTON_ZOOM_IN_OUT_H

#include "main.h"
#include "gameObject.h"
#include "transform.h"
#include "component.h"
#include "mouseCollision.h"

class ButtonZoomInOut : public Component
{
private:
	GameObject* m_gameObject;						// ボタン自身
	bool m_buttonHit = false;						// ボタン自身の当たり判定情報
	float m_scaleSpeedX = 0.5f;						// スケール変更速度
	float m_scaleSpeedY = 0.2f;						// スケール変更速度
	Vector3 m_scale = { 0.0f, 0.0f, 0.0f };			// 元のトランスフォーム情報保管用

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	void Init() override
	{
		/* ボタンの初期状態を保管 */
		m_scale = m_gameObject->GetComponent<Transform>()->scale;
	}

	void Uninit() override {}
	void Update() override
	{
		/* マウスがボタンに当たっているかフラグの取得 */
		m_buttonHit = m_gameObject->GetComponent<MouseCollision>()->GetMouseHit();

		/* もしマウスがボタンに当たったら */
		if (m_buttonHit)
		{
			CreateButtonZoomInOut();
		}
		else
		{
			/* 初期状態へ戻す */
			Transform* transform = m_gameObject->GetComponent<Transform>();
			transform->scale = m_scale;
		}
	}

	void Draw() override {}

	void CreateButtonZoomInOut()
	{
		Transform* transform = m_gameObject->GetComponent<Transform>();

		if (transform == nullptr) { return; }

		transform->scale.x += m_scaleSpeedX;
		transform->scale.y += m_scaleSpeedY;

		/* 初期スケールの1.2倍になったら縮小処理へ */
		/* 初期スケールの1.2分の１になったら拡大処理へ */
		if (transform->scale.x >= m_scale.x * 1.2f)
		{
			m_scaleSpeedX *= -1.0f;
			m_scaleSpeedY *= -1.0f;
		}
		else if (transform->scale.x <= m_scale.x * (1.0f / 1.2f))
		{
			m_scaleSpeedX *= -1.0f;
			m_scaleSpeedY *= -1.0f;
		}
	}
};

#endif //_BUUTON_ZOOM_IN_OUT_H