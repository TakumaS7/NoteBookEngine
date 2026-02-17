/*===============================================================*/
//
//	se.h(SEä«óù)
// 
// 
// ---------------------------------- 2026/02/15 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _SE_H
#define _SE_H

#include "gameObject.h"
#include <string>

class Se : public GameObject
{
private:
	std::wstring m_seName{};

public:
	void Set(const std::wstring& seName) { m_seName = seName; }

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

#endif //_SE_H