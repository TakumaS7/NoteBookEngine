/*===============================================================*/
//
//	deleteObjectButtonComponent.h(オブジェクトを削除するボタン操作)
// 
// 
// ---------------------------------- 2025/09/26 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _DELETE_OBJECT_BUTTON_COMPONENT
#define _DELETE_OBJECT_BUTTON_COMPONENT

#include "main.h"
#include "gameObject.h"
#include "component.h"
#include "mouseCollision.h"
#include "mouse.h"
#include "fade.h"
#include "manager.h"
#include "scene.h"
#include "createList.h"
#include "objectDragDrop.h"

class DeleteObjectButtonComponent : public Component
{
private:
	GameObject* m_gameObject;			// ボタン自身
	GameObject* m_deleteObject;			// 消す対象オブジェクト
	bool m_buttonHit = false;			// ボタン自身の当たり判定情報

public:
	void Set(GameObject* gameObject, GameObject* deleteObject) {
		m_gameObject = gameObject;
		m_deleteObject = deleteObject;
	}

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		auto listObjects = Manager::GetScene()->GetAllGameObject2D();

		for (auto object : listObjects)
		{
			if (!object->GetComponent<ObjectDragDrop>()) { continue; }

			object->GetComponent<ObjectDragDrop>()->SetDraggable(true);
		}

		/* マウスがボタンに当たっているかフラグの取得 */
		m_buttonHit = m_gameObject->GetComponent<MouseCollision>()->GetMouseHit();

		/* 左クリック処理 + ドラッグして無いとき */
		if (m_buttonHit && Mouse::GetLeftClickTrigger() && !Mouse::GetDragging())
		{
			/* もし削除対象がドラッグ中なら解除 */
			if (Mouse::GetDragTarget() == m_deleteObject)
			{
				Mouse::ClearDragTarget();
				Mouse::SetDragging(false);
			}

			m_deleteObject->SetDestroy();
			m_gameObject->SetDestroy();

			Manager::GetScene()->GetGameObject<CreateList>()->SetSequence();

			/* ボタン自身もドラッグ状態をクリア */
			if (Mouse::GetDragTarget() == m_gameObject)
			{
				Mouse::ClearDragTarget();
				Mouse::SetDragging(false);
			}

			/* ドラッグ可能状態を全オブジェクトに戻す */
			auto listObjects = Manager::GetScene()->GetAllGameObject2D();
			for (auto object : listObjects)
			{
				if (!object->GetComponent<ObjectDragDrop>()) { continue; }
				object->GetComponent<ObjectDragDrop>()->SetDraggable(true);
			}
		}
	}

	void Draw() override {}
};

#endif //_DELETE_OBJECT_BUTTON_COMPONENT