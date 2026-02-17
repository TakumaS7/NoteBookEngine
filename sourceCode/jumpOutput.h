/*===============================================================*/
//
//	jumpOutput.h(ジャンプ出力管理)
// 
// 
// ---------------------------------- 2026/02/14 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _JUMP_OUTPUT_H
#define _JUMP_OUTPUT_H

#include "gameObject.h"
#include <string>

class JumpOutput : public GameObject
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

#endif //_JUMP_OUTPUT_H