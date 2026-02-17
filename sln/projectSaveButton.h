/*===============================================================*/
//
//	projectSaveButton.h(プロジェクトセーブボタン管理)
// 
// 
// ---------------------------------- 2026/02/15 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _PROJECT_SAVE_BUTTON_H
#define _PROJECT_SAVE_BUTTON_H

#include "gameObject.h"

class ProjectSaveButton : public GameObject
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

#endif //_PROJECT_SAVE_BUTTON_H