/*===============================================================*/
//
//	projectLoad.h(プロジェクトロード管理)
// 
// 
// ---------------------------------- 2026/02/16 製作者：佐藤琢磨
//
/*===============================================================*/
//
//	プロジェクトは自動でロードされる
//
/*===============================================================*/

#ifndef _PROJECT_LOAD_H
#define _PROJECT_LOAD_H

#include "gameObject.h"

class ProjectLoad : public GameObject
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

#endif //_PROJECT_LOAD_H