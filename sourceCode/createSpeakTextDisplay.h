/*===============================================================*/
//
//	createSpeakTextDisplay.h(作成テキストフレーム表示管理)
// 
// 
// ---------------------------------- 2026/03/02 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_SPEAK_TEXT_DISPLAY_H
#define _CREATE_SPEAK_TEXT_DISPLAY_H

#include "gameObject.h"

class CreateSpeakTextDisplay : public GameObject
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

#endif //_CREATE_SPEAK_TEXT_DISPLAY_H