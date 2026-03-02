/*===============================================================*/
//
//	speakTextHiddenOutput.h(テキストフレーム非表示管理)
// 
// 
// ---------------------------------- 2026/03/02 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SPEAK_TEXT_HIDDEN_OUTPUT_H
#define _SPEAK_TEXT_HIDDEN_OUTPUT_H

#include "gameObject.h"

class SpeakTextHiddenOutput : public GameObject
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

#endif //_SPEAK_TEXT_HIDDEN_OUTPUT_H