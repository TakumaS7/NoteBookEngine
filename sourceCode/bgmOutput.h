/*===============================================================*/
//
//	bgmOutput.h(BGMèoóÕä«óù)
// 
// 
// ---------------------------------- 2026/01/29 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _BGM_OUTPUT_H
#define _BGM_OUTPUT_H

#include "gameObject.h"
#include <string>

class BgmOutput : public GameObject
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

#endif //_BGM_OUTPUT_H