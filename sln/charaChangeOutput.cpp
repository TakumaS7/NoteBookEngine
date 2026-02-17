/*===============================================================*/
//
//	charaChangeOutput.cpp(ƒLƒƒƒ‰ó‘Ô•ÏXŠÇ—)
// 
// 
// ---------------------------------- 2026/02/08 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#include "charaChangeOutput.h"

#include "sortingLayer.h"
#include "charaChangeOutputComponent.h"

void CharaChangeOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<CharaChangeOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
