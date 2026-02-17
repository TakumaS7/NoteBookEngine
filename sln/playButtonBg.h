/*===============================================================*/
//
//	playButtonBg.h(ŽÀsƒ{ƒ^ƒ“”wŒiŠÇ—)
// 
// 
// ---------------------------------- 2025/10/12 »ìŽÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _PLAY_BUTTON_BG_H
#define _PLAY_BUTTON_BG_H

#include "gameObject.h"

class PlayButtonBg : public GameObject
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

#endif //_PLAY_BUTTON_BG_H