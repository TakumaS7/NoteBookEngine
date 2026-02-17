/*===============================================================*/
//
//	objectDragDrop.h(クリエイトシーンでのオブジェクト操作)
// 
// 
// ---------------------------------- 2025/09/28 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _OBJECT_DRAGDROP_H
#define _OBJECT_DRAGDROP_H

#include "gameObject.h"
#include "component.h"
#include "mouseCollision.h"
#include "mouse.h"
#include "transform.h"
#include "manager.h"
#include "scene.h"
#include "createList.h"
#include <algorithm>

class ObjectDragDrop : public Component
{
private:
	GameObject* m_gameObject{};						// 自身
	Transform* m_transform{};						// 自身のtransform
	bool m_buttonHit = false;						// ボタン自身の当たり判定情報
	bool m_draggable = false;						// ドラッグ可能か
	Vector3 m_mousePosition = { 0.0f, 0.0f, 0.0f };	// マウスのポジション
	Vector3 m_offset{ 0.0f, 0.0f, 0.0f };			// マウスとオブジェクトの差分
	
public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	void SetDraggable(const bool& draggable) { m_draggable = draggable; }

	void Init() override {}
	void Uninit() override {}
	void Update() override
	{
		if (!m_draggable) { return; }

		/* transformの取得 */
		m_transform = m_gameObject->GetComponent<Transform>();

		/* マウスがリストオブジェクトに当たっているかフラグの取得 */
		m_buttonHit = m_gameObject->GetComponent<MouseCollision>()->GetMouseHit();
		
		/* マウスの座標取得 */
		m_mousePosition = m_gameObject->GetComponent<MouseCollision>()->GetMousePosition();

		/* オブジェクトドラッグ開始 */
		if (m_buttonHit && Mouse::GetLeftClick())
		{
			/* 既にドラッグ中でないとき */
			if (!Mouse::GetDragging() && Mouse::GetDragTarget() == nullptr) {
				m_offset = m_mousePosition - m_transform->position;
				Mouse::SetDragging(true);
				Mouse::SetDragTarget(m_gameObject);	// 自分を掴んだと登録
			}
		}

		/* ドラッグ中はマウスに追従 */
		if (Mouse::GetDragging() && Mouse::GetDragTarget() == m_gameObject) {
			m_transform->position = m_mousePosition - m_offset;
		}

		/* ドラッグ中の処理 */
		if (Mouse::GetLeftClick() && Mouse::GetDragging() && Mouse::GetDragTarget() == m_gameObject)
		{
			GameObject* hitObject = CheckHitOther();

			if (hitObject == nullptr) { return; }

			float hitObjectPositionY = hitObject->GetComponent<Transform>()->position.y;	// 当たったリストオブジェクトの座標Y
			float myPositionY = m_gameObject->GetComponent<Transform>()->position.y;	// 自分自身の座標Y


			CreateList* createList = Manager::GetScene()->GetGameObject<CreateList>();	// リスト情報を取得

			if (createList == nullptr) { return; }

			auto& listObjects = createList->GetListObjects();	// 全てのリストオブジェクトを取得

			/* 自分と当たったオブジェクトのイテレーターを取得 */
			std::list<ObjectEntry>::iterator myIterator = std::find_if(listObjects.begin(), listObjects.end(),
				[&](const ObjectEntry& entry){
					return entry.listObject == m_gameObject;
			});

			std::list<ObjectEntry>::iterator hitObjectIterator = std::find_if(listObjects.begin(), listObjects.end(),
				[&](const ObjectEntry& entry) {
					return entry.listObject == hitObject;
				});


			/* リストオブジェクトの移動、配置処理 */
			bool HitObjectIteratorFront = std::distance(listObjects.begin(), myIterator) > std::distance(listObjects.begin(), hitObjectIterator);	// ture→当たったオブジェクトが自身よりも前

			if (HitObjectIteratorFront == true)	// 当たったオブジェクトのイテレーターが自分自身よりも前（リストでいう上）であるとき
			{
				HitObjectSwapDown(myPositionY, hitObjectPositionY, myIterator, hitObjectIterator);
			}
			else if (HitObjectIteratorFront == false)	// 当たったオブジェクトのイテレーターが自分自身よりも後ろ（リストでいう下）であるとき
			{
				HitObjectSwapUp(myPositionY, hitObjectPositionY, myIterator, hitObjectIterator);
			}
		}


		/* クリックを離したらドラッグ終了 */
		if (!Mouse::GetLeftClick() && Mouse::GetDragging() && Mouse::GetDragTarget() == m_gameObject) 
		{
			Mouse::SetDragging(false);
			Mouse::ClearDragTarget();
		}

	}

	void Draw() override {}

	GameObject* CheckHitOther()
	{
		auto gameObjects = Manager::GetScene()->GetAllGameObject2D();

		for (auto gameObject : gameObjects)
		{
			/* 自分は除外 */
			if (gameObject == m_gameObject) { continue; }

			/* objectDragDropを持っているかの確認 */
			if (gameObject->GetComponent<ObjectDragDrop>() == nullptr) { continue; }

			Transform* gameObjectTransform = gameObject->GetComponent<Transform>();
			if (!gameObjectTransform) { continue; }

			/* 当たり判定処理 */
			if (fabs(gameObjectTransform->position.x - m_transform->position.x) < fabs(gameObjectTransform->scale.x + m_transform->scale.x) * 0.5f &&
				fabs(gameObjectTransform->position.y - m_transform->position.y) < fabs(gameObjectTransform->scale.y + m_transform->scale.y) * 0.5f)
			{
				return gameObject;	// 当たったオブジェクトを返す
			}
		}
		return nullptr;
	}

	void HitObjectSwapDown(float& myPositionY, float& hitObjectPositionY, std::list<ObjectEntry>::iterator& myIterator, std::list<ObjectEntry>::iterator& hitObjectIterator)
	{
		/* イテレーターが自分自身よりも前（リストでいう上）であるとき */
		/* もし自分自身の座標が、当たったリストオブジェクトの半分よりも上に移動したとき */
		/* 自分と当たったリストオブジェクトの位置を入れ替える */
		if (myPositionY > hitObjectPositionY)
		{
			std::iter_swap(myIterator, hitObjectIterator);		// イテレーターの前後関係を変更（myIteratorを前に）
		}
	}

	void HitObjectSwapUp(float& myPositionY, float& hitObjectPositionY, std::list<ObjectEntry>::iterator& myIterator, std::list<ObjectEntry>::iterator& hitObjectIterator)
	{
		/* イテレーターが自分自身よりも後ろ（リストでいう下）であるとき */
		/* もし自分自身の座標が、当たったリストオブジェクトの半分よりも下に移動したとき */
		/* 自分と当たったリストオブジェクトの位置を入れ替える */
		if (myPositionY < hitObjectPositionY)
		{
			std::iter_swap(myIterator, hitObjectIterator);		// イテレーターの前後関係を変更（myIteratorを前に）
		}
	}
};

#endif //_OBJECT_DRAGDROP_H