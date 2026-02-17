/*===============================================================*/
//
//	textBox.h(テキスト枠管理)
// 
// 
// ---------------------------------- 2025/10/13 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TEXT_BOX_H
#define _TEXT_BOX_H

#include "gameObject.h"
#include <string>

class TextBox : public GameObject
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

#endif //_TEXT_BOX_H