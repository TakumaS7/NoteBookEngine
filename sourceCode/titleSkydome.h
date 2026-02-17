/*===============================================================*/
//
//	titleSkydome.h(タイトルスカイドーム管理)
// 
// 
// ---------------------------------- 2025/12/10 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TITLE_SKYDOME_H
#define _TITLE_SKYDOME_H

#include "gameObject.h"

class TitleSkydome : public GameObject
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

#endif //_TITLE_SKYDOME_H