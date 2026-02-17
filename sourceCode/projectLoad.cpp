/*===============================================================*/
//
//	projectLoad.cpp(プロジェクトロード管理)
// 
// 
// ---------------------------------- 2026/02/16 製作者：佐藤琢磨
//
/*===============================================================*/
//
//	プロジェクトは自動でロードされる
//
/*===============================================================*/

#include "projectLoad.h"
#include "projectLoadComponent.h"
#include "sortingLayer.h"

void ProjectLoad::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<ProjectLoadComponent>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
