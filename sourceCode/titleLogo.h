/*===============================================================*/
//
//	titleLogo.h(タイトルロゴ管理)
// 
// 
// ---------------------------------- 2025/07/20 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TITLE_LOGO_H
#define _TITLE_LOGO_H

#include "gameObject.h"

class TitleLogo : public GameObject
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

#endif //_TITLE_LOGO_H