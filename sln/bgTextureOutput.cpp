/*===============================================================*/
//
//	bgTextureOutput.cpp(”wŒi‰æ‘œo—ÍŠÇ—)
// 
// 
// ---------------------------------- 2025/12/11 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#include "bgTextureOutput.h"

#include "sortingLayer.h"
#include "bgTextureOutputComponent.h"

void BgTextureOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<BgTextureOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
