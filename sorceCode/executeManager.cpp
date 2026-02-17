/*===============================================================*/
//
//	executeManager.h(Às‰æ–ÊŠÇ—)
// 
// 
// ---------------------------------- 2025/12/17 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#include "executeManager.h"

#include "sortingLayer.h"
#include "executeManagerComponent.h"

void ExecuteManager::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<ExecuteManagerComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
