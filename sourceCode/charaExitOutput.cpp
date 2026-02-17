/*===============================================================*/
//
//	charaExitOutput.cpp(ƒLƒƒƒ‰‘ŞêŠÇ—)
// 
// 
// ---------------------------------- 2026/02/11 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#include "charaExitOutput.h"

#include "sortingLayer.h"
#include "charaExitOutputComponent.h"

void CharaExitOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<CharaExitOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
