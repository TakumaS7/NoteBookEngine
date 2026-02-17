/*===============================================================*/
//
//	title.h(タイトル処理)
// 
// 
// ---------------------------------- 2025/07/20 製作者：佐藤琢磨
//
/*===============================================================*/

#include "title.h"
#include "input.h"

#include "fade.h"

#include "titleLogo.h"
#include "createButton.h"
#include "titleCamera.h"
#include "titlePaper.h"
#include "cherryBlossomParticle.h"
#include "titleSkydome.h"
#include "imGuiManager.h"

void Title::Init()
{	
	AddGameObject<TitleCamera>(CAMERA);
	AddGameObject<TitleSkydome>(OBJECT_3D);
	AddGameObject<TitlePaper>(OBJECT_3D);
	AddGameObject<CherryBlossomParticle>(OBJECT_3D);
	AddGameObject<TitleLogo>(OBJECT_2D);
	AddGameObject<CreateButton>(OBJECT_2D);

	Scene::Init();
}

void Title::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN) && Input::GetKeyTrigger(VK_SPACE))
	{
		Fade::SetFadeScene(Fade::GAME);
	}
}

void Title::Draw()
{
	Scene::Draw();

	ImGuiManager::BegineFrame();
	ImGuiManager::DrawProjectSelect();
	ImGuiManager::EndFrame();
}
