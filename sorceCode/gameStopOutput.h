/*===============================================================*/
//
//	gameStopOutput.h(ÉQÅ[ÉÄí‚é~èoóÕä«óù)
// 
// 
// ---------------------------------- 2026/02/12 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _GAME_STOP_OUTPUT_H
#define _GAME_STOP_OUTPUT_H

#include "gameObject.h"
#include <string>

class GameStopOutput : public GameObject
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

#endif //_GAME_STOP_OUTPUT_H