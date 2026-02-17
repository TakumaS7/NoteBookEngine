/*===============================================================*/
//
//	seOutput.cpp(SEo—ÍŠÇ—)
// 
// 
// ---------------------------------- 2026/02/15 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#include "seOutput.h"

#include "sortingLayer.h"
#include "seOutputComponent.h"

void SeOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<SeOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
