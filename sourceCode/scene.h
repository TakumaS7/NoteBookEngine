/*===============================================================*/
//
//	scene.h(シーン管理)
// 
// 
// ---------------------------------- 2025/07/19 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SCENE_H
#define _SCENE_H

#include <list>
#include "gameObject.h"
#include "mouse.h"

enum Layer
{
	CAMERA,
	OBJECT_3D,
	OBJECT_2D,
	LAYER_MAX
};

class Scene
{
protected:
	std::list<GameObject*> m_gameObject[LAYER_MAX];

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	template <typename T> //Tは型が送れる
	T* AddGameObject(const Layer& layer)
	{
		T* gameObject = new T;
		m_gameObject[layer].push_back(gameObject);

		return gameObject;
	}

	void ProcessDelete()
	{
		for (int i = 0; i < LAYER_MAX; ++i)
		{
			for (auto it = m_gameObject[i].begin(); it != m_gameObject[i].end(); ) {
				GameObject* obj = *it;
				if (obj && obj->GetDestroy()) {
					if (Mouse::GetDragTarget() == obj)
					{
						Mouse::ClearDragTarget();
						Mouse::SetDragging(false);
					}

					obj->Uninit();
					delete obj;
					it = m_gameObject[i].erase(it);
				}
				else {
					++it;
				}
			}
		}
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < LAYER_MAX; ++i)
		{
			for (auto gameObject : m_gameObject[i]) {
				T* find = dynamic_cast<T*>(gameObject);	//dynamicだと確認してくれる

				if (find != nullptr) {
					return find;
				}
			}
		}

		return nullptr;
	}

	std::list<GameObject*> GetAllGameObject()
	{
		return m_gameObject[OBJECT_3D];
	}

	std::list<GameObject*> GetAllGameObject2D()
	{
		return m_gameObject[OBJECT_2D];
	}
};

#endif //_SCENE_H