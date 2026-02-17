/*===============================================================*/
//
//	player.h(プレイヤー管理)
// 
// 
// ---------------------------------- 2025/07/15 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _PLAYER_TWO_H
#define _PLAYER_TWO_H

#include "gameObject.h"

class Player : public GameObject
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

#endif //_PLAYER_TWO_H