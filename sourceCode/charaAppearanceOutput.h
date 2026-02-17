/*===============================================================*/
//
//	charaAppearanceOutput.h(ÉLÉÉÉâìoèÍä«óù)
// 
// 
// ---------------------------------- 2026/02/05 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _CHARA_APPEARANCE_OUTPUT_H
#define _CHARA_APPEARANCE_OUTPUT_H

#include "gameObject.h"
#include <string>

class CharaAppearanceOutput : public GameObject
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

#endif //_CHARA_APPEARANCE_OUTPUT_H