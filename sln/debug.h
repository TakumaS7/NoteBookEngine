/*===============================================================*/
//
//	debug.h(デバッグシーン管理)
// 
// 
// ---------------------------------- 2025/10/13 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _DEBUG_H
#define _DEBUG_H

#include "gameObject.h"
#include <list>
#include "scene.h"



class Debug
{
private:
	std::list<GameObject*> m_gameObjects[LAYER_MAX];

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	template <typename T> //Tは型が送れる
	T* AddGameObject(const Layer& layer)
	{
		T* gameObject = new T;
		m_gameObjects[layer].push_back(gameObject);

		return gameObject;
	}

	void ProcessDelete();

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < LAYER_MAX; ++i)
		{
			for (auto gameObject : m_gameObjects[i]) {
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
		return m_gameObjects[OBJECT_3D];
	}

	std::list<GameObject*> GetAllGameObject2D()
	{
		return m_gameObjects[OBJECT_2D];
	}
};

#endif //_DEBUG_H