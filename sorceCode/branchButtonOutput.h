/*===============================================================*/
//
//	branchButtonOutput.h(分岐ボタン出力管理)
// 
// 
// ---------------------------------- 2026/02/12 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _BRANCH_BUTTON_OUTPUT_H
#define _BRANCH_BUTTON_OUTPUT_H

#include "gameObject.h"
#include <string>

class BranchButtonOutput : public GameObject
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

#endif //_BRANCH_BUTTON_OUTPUT_H