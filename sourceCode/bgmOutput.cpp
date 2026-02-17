/*===============================================================*/
//
//	bgmOutput.cpp(BGMo—ÍŠÇ—)
// 
// 
// ---------------------------------- 2026/01/29 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#include "bgmOutput.h"

#include "sortingLayer.h"
#include "bgmOutputComponent.h"

void BgmOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<BgmOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
