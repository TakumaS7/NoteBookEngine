/*===============================================================*/
//
//	executeButton.h(Executeƒ{ƒ^ƒ“ŠÇ—)
// 
// 
// ---------------------------------- 2025/09/25 »ìŽÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _EXECUTE_BUTTON_H
#define _EXECUTE_BUTTON_H

#include "gameObject.h"

class ExecuteButton : public GameObject
{
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

#endif //_EXECUTE_BUTTON_H