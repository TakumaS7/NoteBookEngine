/*===============================================================*/
//
//	playButton.h(実行ボタン管理)
// 
// 
// ---------------------------------- 2025/10/12 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _PLAY_BUTTON_H
#define _PLAY_BUTTON_H

#include "gameObject.h"

class PlayButton : public GameObject
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

#endif //_PLAY_BUTTON_H