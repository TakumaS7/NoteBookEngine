/*===============================================================*/
//
//	bgTextureOutput.h(”wŒi‰æ‘œo—ÍŠÇ—)
// 
// 
// ---------------------------------- 2025/12/11 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _BG_TEXTURE_OUTPUT_H
#define _BG_TEXTURE_OUTPUT_H

#include "gameObject.h"
#include <string>

class BgTextureOutput : public GameObject
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

#endif //_BG_TEXTURE_OUTPUT_H