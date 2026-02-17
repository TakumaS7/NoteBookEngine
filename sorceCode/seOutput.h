/*===============================================================*/
//
//	seOutput.h(SEèoóÕä«óù)
// 
// 
// ---------------------------------- 2026/02/15 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _SE_OUTPUT_H
#define _SE_OUTPUT_H

#include "gameObject.h"
#include <string>

class SeOutput : public GameObject
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

#endif //_SE_OUTPUT_H