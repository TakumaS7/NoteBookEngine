/*===============================================================*/
//
//	bgTexture.h(”wŒi‰æ‘œŠÇ—)
// 
// 
// ---------------------------------- 2025/12/04 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _BG_TEXTURE_H
#define _BG_TEXTURE_H

#include "gameObject.h"
#include <string>

class BgTexture : public GameObject
{
private:
	std::wstring m_textName{};

public:
	void Set(const std::wstring& textName) { m_textName = textName; }

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

#endif //_BG_TEXTURE_H