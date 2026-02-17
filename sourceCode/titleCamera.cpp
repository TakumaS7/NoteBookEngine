/*===============================================================*/
//
//	titleCamera.cpp(タイトルカメラ管理)
// 
// 
// ---------------------------------- 2025/12/05 製作者：佐藤琢磨
//
/*===============================================================*/

#include "titleCamera.h"

#include "sortingLayer.h"
#include "transform.h"
#include "cameraChase.h"
#include "manager.h"
#include "titlePaper.h"
#include "cameraView.h"
#include "cameraAutoRotate.h"

void TitleCamera::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
	AddComponent<CameraAutoRotate>()->Set(this->GetComponent<Transform>());
	AddComponent<CameraChase>()->Set(this->GetComponent<Transform>(), Manager::GetScene()->GetGameObject<TitlePaper>());
	AddComponent<CameraView>()->Set(this->GetComponent<Transform>(), Manager::GetScene()->GetGameObject<TitlePaper>());

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
