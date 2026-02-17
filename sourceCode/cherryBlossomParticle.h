/*===============================================================*/
//
//	cherryBlossomParticle.h(桜パーティクル管理)
// 
// 
// ---------------------------------- 2025/12/07 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CHERRY_BLOSSOM_PARTICLE_H
#define _CHERRY_BLOSSOM_PARTICLE_H

#include "gameObject.h"

class CherryBlossomParticle : public GameObject
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

#endif //_CHERRY_BLOSSOM_PARTICLE_H