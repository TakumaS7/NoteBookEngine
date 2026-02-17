/*===============================================================*/
//
//	createGameStop.h(çÏê¨ÉQÅ[ÉÄí‚é~ä«óù)
// 
// 
// ---------------------------------- 2026/02/12 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _CREATE_GAME_STOP_H
#define _CREATE_GAME_STOP_H

#include "gameObject.h"
#include <string>

class CreateGameStop : public GameObject
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

#endif //_CREATE_GAME_STOP_H