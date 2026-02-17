/*===============================================================*/
//
//	gameStopOutput.cpp(ÉQÅ[ÉÄí‚é~èoóÕä«óù)
// 
// 
// ---------------------------------- 2026/02/12 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "gameStopOutput.h"

#include "sortingLayer.h"
#include "gameStopOutputComponent.h"

void GameStopOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<GameStopOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
