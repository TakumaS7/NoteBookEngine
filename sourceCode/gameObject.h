/*===============================================================*/
//
//	gameObject.h(ゲームオブジェクト処理)
// 
// 
// ---------------------------------- 2025/05/14 製作者：佐藤琢磨
//
/*===============================================================*/
#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include "main.h"
#include "transform.h"
#include "component.h"
#include <vector>

class GameObject
{
private:
	bool m_Destroy = false;

	std::vector<Component*> m_components;

public:

	Transform transform;

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetDestroy() { m_Destroy = true; }
	bool GetDestroy() const { return m_Destroy; }

	template<typename T>
	T* AddComponent()
	{
		T* component = new T;
		m_components.emplace_back(component);

		return component;
	}

	template<typename T>
	T* GetComponent()
	{
		for (auto component : m_components) {
			T* find = dynamic_cast<T*>(component);	//dynamicだと確認してくれる

			if (find != nullptr) {
				return find;
			}
		}
		return nullptr;
	}

	std::vector<Component*> GetComponentVector()
	{
		return m_components;
	}
};

#endif //_GAME_OBJECT_H
