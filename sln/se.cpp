/*===============================================================*/
//
//	se.cpp(SEä«óù)
// 
// 
// ---------------------------------- 2026/02/15 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "se.h"

#include "sortingLayer.h"
#include "soundComponent.h"

void Se::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<SoundComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
