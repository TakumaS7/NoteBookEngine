/*===============================================================*/
//
//	bgm.h(BGMä«óù)
// 
// 
// ---------------------------------- 2026/01/29 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _BGM_H
#define _BGM_H

#include "gameObject.h"
#include <string>

class Bgm : public GameObject
{
private:
	std::wstring m_bgmName{};

public:
	void Set(const std::wstring& bgmName) { m_bgmName = bgmName; }

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

#endif //_BGM_H