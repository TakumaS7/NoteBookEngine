/*===============================================================*/
//
//	charaChangeOutput.h(ƒLƒƒƒ‰ó‘Ô•ÏXŠÇ—)
// 
// 
// ---------------------------------- 2026/02/08 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _CHARA_CHANGE_OUTPUT_H
#define _CHARA_CHANGE_OUTPUT_H

#include "gameObject.h"
#include <string>

class CharaChangeOutput : public GameObject
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

#endif //_CHARA_CHANGE_OUTPUT_H