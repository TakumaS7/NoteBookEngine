/*===============================================================*/
//
//	bgm.cpp(BGMä«óù)
// 
// 
// ---------------------------------- 2026/01/29 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "bgm.h"

#include "sortingLayer.h"
#include "soundComponent.h"

void Bgm::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<SoundComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
