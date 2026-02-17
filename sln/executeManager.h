/*===============================================================*/
//
//	executeManager.h(é¿çsâÊñ ä«óù)
// 
// 
// ---------------------------------- 2025/12/17 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _EXECUTE_MANAGER_H
#define _EXECUTE_MANAGER_H

#include "gameObject.h"
#include <string>

class ExecuteManager : public GameObject
{
private:

public:
	void Init() override;
	void Uninit() override
	{
		for (auto component : GetComponentVector())
		{
			component->Uninit();
		}
	}

	void Update() override
	{
		for (auto component : GetComponentVector())
		{
			component->Update();
		}
	}

	void Draw() override
	{
		for (auto component : GetComponentVector())
		{
			component->Draw();
		}
	}
};

#endif //_EXECUTE_MANAGER_H