/*===============================================================*/
//
//	charaTexture.h(ƒLƒƒƒ‰‰æ‘œŠÇ—)
// 
// 
// ---------------------------------- 2026/01/06 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _CHARA_TEXTURE_H
#define _CHARA_TEXTURE_H

#include "gameObject.h"
#include <string>

class CharaTexture : public GameObject
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

#endif //_CHARA_TEXTURE_H