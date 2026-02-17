/*===============================================================*/
//
//	branchButtonOutput.cpp(分岐ボタン出力管理)
// 
// 
// ---------------------------------- 2026/02/12 製作者：佐藤琢磨
//
/*===============================================================*/

#include "branchButtonOutput.h"

#include "sortingLayer.h"
#include "branchButtonOutputComponent.h"

void BranchButtonOutput::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<BranchButtonOutputComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
