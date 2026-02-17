/*===============================================================*/
//
//	setDeleteButton.h(デリートボタンの設置)
// 
// 
// ---------------------------------- 2025/10/04 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SET_DELETE_BUTTON_H
#define _SET_DELETE_BUTTON_H

#include "component.h"
#include "gameObject.h"
#include "manager.h"
#include "scene.h"
#include "deleteSpeakText.h"
#include "transform.h"

class SetDeleteButton : public Component
{
private:
	GameObject* m_deleteObject{};			// 消す対象オブジェクト
	DeleteSpeakText* m_deleteButton{};		// オブジェクトを消すボタン

public:
	void Set(GameObject* deleteObject) { m_deleteObject = deleteObject; }

	void Init() override 
	{
		m_deleteButton = Manager::GetScene()->AddGameObject<DeleteSpeakText>(OBJECT_2D);
		m_deleteButton->Set(m_deleteObject);
		m_deleteButton->Init();
	}

	void Uninit() override {}
	void Update() override 
	{
		/* それぞれのトランスフォームを取得 */
		Transform* objTransform = m_deleteObject->GetComponent<Transform>();
		Transform* buttonTransform = m_deleteButton->GetComponent<Transform>();

		if (objTransform && buttonTransform)
		{
			Vector3& objPosition = objTransform->position;
			Vector3& objScale = objTransform->scale;

			Vector3& buttonPosition = buttonTransform->position;
			Vector3& buttonScale = buttonTransform->scale;

			/* ボタンを対象オブジェクトの右上に配置 */
			buttonPosition.x = objPosition.x + (objScale.x * 0.95f) - (buttonScale.x * 0.5f);	// 右端にそろえる
			buttonPosition.y = objPosition.y - (objScale.y * 0.9f) + (buttonScale.y * 0.5f);	// 上にそろえる
		}
	}

	void Draw() override {}
};

#endif //_SET_DELETE_BUTTON_H