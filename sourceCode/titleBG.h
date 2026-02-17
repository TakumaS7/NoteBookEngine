/*===============================================================*/
//
//	titleBG.h(ƒ^ƒCƒgƒ‹”wŒiŠÇ—)
// 
// 
// ---------------------------------- 2025/07/20 »ìŽÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _TITLE_BG_H
#define _TITLE_BG_H

#include "gameObject.h"

class TitleBG : public GameObject
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

#endif //_TITLE_BG_H