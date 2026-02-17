/*===============================================================*/
//
//	charaExitOutput.h(ƒLƒƒƒ‰‘ŞêŠÇ—)
// 
// 
// ---------------------------------- 2026/02/11 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _CHARA_EXIT_OUTPUT_H
#define _CHARA_EXIT_OUTPUT_H

#include "gameObject.h"
#include <string>

class CharaExitOutput : public GameObject
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

#endif //_CHARA_EXIT_OUTPUT_H