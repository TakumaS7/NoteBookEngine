/*===============================================================*/
//
//	mouseCollsion.h(マウスの当たり判定操作)
// 
// 
// ---------------------------------- 2025/09/26 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _BUTTON_PUSH_H
#define _BUTTON_PUSH_H

#include "main.h"
#include "component.h"
#include "gameObject.h"
#include "transform.h"
#include "mouse.h"

class MouseCollision : public Component
{
private:
	GameObject* m_gameObject{};							// ボタンの自身
	Transform* m_transform{};							// ボタンのtransform情報
	Vector3 m_buttonPosition = { 0.0f, 0.0f, 0.0f };	// ボタンの座標
	Vector3 m_buttonScale = { 0.0f, 0.0f, 0.0f };		// ボタンのサイズ
	Vector3 m_mousePosition = { 0.0f, 0.0f, 0.0f };		// マウスの座標
	bool m_mouseHit = false;							// マウスがボタンに当たってるか
	bool m_onImGui = false;								// ImGuiの上かどうか

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }
	bool GetMouseHit() const { return m_mouseHit; }
	void SetMouseHit(bool hit) { m_mouseHit = hit; }
	Vector3 GetMousePosition() const { return m_mousePosition; }
	void SetOnImGui(const bool& onImGui) { m_onImGui = onImGui; }

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		/* transform情報取得 */
		m_transform = m_gameObject->GetComponent<Transform>();

		/* ボタンの座標・大きさ取得 */
		m_buttonPosition = m_transform->position;
		m_buttonScale = m_transform->scale;

		/* マウスの座標取得 */
		m_mousePosition = Mouse::GetMousePosition();

		/* 当たり判定（ボタンがマウスに当たっていたらマウスヒット判定ON） */
		if (m_mousePosition.x >= m_buttonPosition.x - m_buttonScale.x &&
			m_mousePosition.x <= m_buttonPosition.x + m_buttonScale.x &&
			m_mousePosition.y >= m_buttonPosition.y - m_buttonScale.y &&
			m_mousePosition.y <= m_buttonPosition.y + m_buttonScale.y &&
			m_onImGui == false)
		{
			m_mouseHit = true;
		}
		else
		{
			m_mouseHit = false;
		}
	}

	void Draw() override {}
};

#endif //_BUTTON_PUSH_H