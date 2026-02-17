/*===============================================================*/
//
//	jumpOutput.cpp(ジャンプ出力管理)
// 
// 
// ---------------------------------- 2026/02/14 製作者：佐藤琢磨
//
/*===============================================================*/

#include "jumpOutput.h"

#include "sortingLayer.h"
#include "jumpOutputComponent.h"

void JumpOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<JumpOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
