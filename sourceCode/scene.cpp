/*===============================================================*/
//
//	scene.cpp(シーン管理)
// 
// 
// ---------------------------------- 2025/07/19 製作者：佐藤琢磨
//
/*===============================================================*/

#include "scene.h"
#include "input.h"
#include "sortingLayer.h"
#include "imGuiManager.h"

void Scene::Init()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		for (auto gameObject : m_gameObject[i])
		{
			gameObject->Init(); //ポリモフィズム
		}
	}
}

void Scene::Uninit()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		for (auto gameObject : m_gameObject[i])
		{
			gameObject->Uninit(); //ポリモフィズム
			delete gameObject;
		}

		m_gameObject[i].clear();
	}
}

void Scene::Update()
{
	/* レイヤーソート */
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_gameObject[i].sort(
			[](GameObject* a, GameObject* b) {
				return	a->GetComponent<SortingLayer>()->GetSortingLayer() >
						b->GetComponent<SortingLayer>()->GetSortingLayer();
			});
	}

	for (int i = 0; i < LAYER_MAX; ++i)
	{
		for (auto gameObject : m_gameObject[i])
		{
			gameObject->Update();
		}
	}

	ProcessDelete();
}

void Scene::Draw()
{
	/* Zソート */
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_gameObject[i].sort(
			[](GameObject* a, GameObject* b) {
				return	a->GetComponent<SortingLayer>()->GetSortingLayer() <
					b->GetComponent<SortingLayer>()->GetSortingLayer();
			});
	}

	for (int i = 0; i < LAYER_MAX; ++i)
	{
		for (auto gameObject : m_gameObject[i])
		{
			gameObject->Draw();
		}
	}
}
