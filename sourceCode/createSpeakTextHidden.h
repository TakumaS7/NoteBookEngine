/*===============================================================*/
//
//	createSpeakTextHidden.h(作成テキストフレーム非表示管理)
// 
// 
// ---------------------------------- 2026/03/01 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_SPEAK_TEXT_HIDDEN_H
#define _CREATE_SPEAK_TEXT_HIDDEN_H

#include "gameObject.h"

class CreateSpeakTextHidden : public GameObject
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

#endif //_CREATE_SPEAK_TEXT_HIDDEN_H