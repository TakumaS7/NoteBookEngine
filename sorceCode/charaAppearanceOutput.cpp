/*===============================================================*/
//
//	charaAppearanceOutput.cpp(ƒLƒƒƒ‰“oêŠÇ—)
// 
// 
// ---------------------------------- 2026/02/05 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#include "charaAppearanceOutput.h"

#include "sortingLayer.h"
#include "charaAppearanceOutputComponent.h"

void CharaAppearanceOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<CharaAppearanceOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
