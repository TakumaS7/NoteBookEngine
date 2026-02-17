/*===============================================================*/
//
//	plane.h(°ŠÇ—)
// 
// 
// ---------------------------------- 2025/07/17 »ìŽÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _PLANE_H
#define _PLANE_H

#include "gameObject.h"

class Plane : public GameObject
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

#endif //_PLANE_H