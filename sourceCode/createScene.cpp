/*===============================================================*/
//
//	createScene.cpp(作業用シーン管理)
// 
// 
// ---------------------------------- 2025/09/27 製作者：佐藤琢磨
//
/*===============================================================*/

#include "createScene.h"
#include "imGuiManager.h"

#include "createList.h"
#include "playButton.h"
#include "playButtonBg.h"
#include "projectSaveButton.h"
#include "projectLoad.h"

void CreateScene::Init()
{
	AddGameObject<CreateList>(OBJECT_2D);
	AddGameObject<PlayButton>(OBJECT_2D);
	AddGameObject<ProjectSaveButton>(OBJECT_2D);
	AddGameObject<PlayButtonBg>(OBJECT_2D);
	AddGameObject<ProjectLoad>(OBJECT_2D);

	Scene::Init();
}

void CreateScene::Update()
{
	Scene::Update();
}

void CreateScene::Draw()
{
	Scene::Draw();
	ImGuiManager::BegineFrame();
	ImGuiManager::DrawCreate();
	ImGuiManager::EndFrame();
}
