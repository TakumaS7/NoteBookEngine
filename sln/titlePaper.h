/*===============================================================*/
//
//	titlePaper.h(タイトルの紙管理)
// 
// 
// ---------------------------------- 2025/12/05 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TITLE_PAPER_H
#define _TITLE_PAPER_H

#include "gameObject.h"

class TitlePaper : public GameObject
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

#endif //_TITLE_PAPER_H