/*===============================================================*/
//
//	square.h(³•ûŒ`ŠÇ—)
// 
// 
// ---------------------------------- 2025/07/20 »ìŽÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _SQUARE_H
#define _SQUARE_H

#include "gameObject.h"

class Square : public GameObject
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

#endif //_SQUARE_H