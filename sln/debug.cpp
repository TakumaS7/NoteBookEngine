/*===============================================================*/
//
//	debug.cpp(デバッグシーン管理)
// 
// 
// ---------------------------------- 2025/10/13 製作者：佐藤琢磨
//
/*===============================================================*/

#include "debug.h"
#include "input.h"
#include "manager.h"
#include "sortingLayer.h"
#include "mouse.h"

#include "textBox.h"
#include "bgTextureOutput.h"

#include "executeManager.h"

void Debug::Init()
{
	//AddGameObject<TextBox>(OBJECT_2D);
	//AddGameObject<BgTextureOutput>(OBJECT_2D);
	AddGameObject<ExecuteManager>(OBJECT_2D);


	for (int i = 0; i < LAYER_MAX; ++i)
	{
		for (auto gameObject : m_gameObjects[i])
		{
			gameObject->Init(); //ポリモフィズム
		}
	}
}

void Debug::Uninit()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		for (auto gameObject : m_gameObjects[i])
		{
			gameObject->Uninit(); //ポリモフィズム
			delete gameObject;
		}

		m_gameObjects[i].clear();
	}
}

void Debug::Update()
{
	/* レイヤーソート */
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_gameObjects[i].sort(
			[](GameObject* a, GameObject* b) {
				return	a->GetComponent<SortingLayer>()->GetSortingLayer() >
					b->GetComponent<SortingLayer>()->GetSortingLayer();
			});
	}

	for (int i = 0; i < LAYER_MAX; ++i)
	{
		for (auto gameObject : m_gameObjects[i])
		{
			gameObject->Update();
		}
	}

	ProcessDelete();
}

void Debug::Draw()
{
	/* Zソート */
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_gameObjects[i].sort(
			[](GameObject* a, GameObject* b) {
				return	a->GetComponent<SortingLayer>()->GetSortingLayer() <
					b->GetComponent<SortingLayer>()->GetSortingLayer();
			});
	}

	for (int i = 0; i < LAYER_MAX; ++i)
	{
		for (auto gameObject : m_gameObjects[i])
		{
			gameObject->Draw();
		}
	}
}



void Debug::ProcessDelete()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		for (auto it = m_gameObjects[i].begin(); it != m_gameObjects[i].end(); ) {
			GameObject* obj = *it;
			if (obj && obj->GetDestroy()) {
				if (Mouse::GetDragTarget() == obj)
				{
					Mouse::ClearDragTarget();
					Mouse::SetDragging(false);
				}

				obj->Uninit();
				delete obj;
				it = m_gameObjects[i].erase(it);
			}
			else {
				++it;
			}
		}
	}
}
