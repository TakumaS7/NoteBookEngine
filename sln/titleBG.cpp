/*===============================================================*/
//
//	titleBG.cpp(タイトル背景管理)
// 
// 
// ---------------------------------- 2025/07/20 製作者：佐藤琢磨
//
/*===============================================================*/

#include "titleBG.h"

#include "sortingLayer.h"
#include "transform.h"
#include "transformDraw.h"
#include "shader2D.h"
#include "textureComponent.h"
#include "squareVertex.h"

void TitleBG::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\titleBG.png", true);
	AddComponent<SquareVertex>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
