/*===============================================================*/
//
//	projectSaveButton.cpp(プロジェクトセーブボタン管理)
// 
// 
// ---------------------------------- 2026/02/15 製作者：佐藤琢磨
//
/*===============================================================*/

#include "projectSaveButton.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"
#include "mouseCollision.h"
#include "saveButtonComponent.h"

void ProjectSaveButton::Init()
{
	AddComponent<SortingLayer>()->Set(3);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.9f, 25.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 25.0f, 25.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\projectSaveIcon.png", true);
	AddComponent<SquareVertex>();
	AddComponent<MouseCollision>()->Set(this);
	AddComponent<SaveButtonComponent>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
