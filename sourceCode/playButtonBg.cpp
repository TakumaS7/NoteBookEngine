/*===============================================================*/
//
//	playButtonBg.cpp(実行ボタン背景管理)
// 
// 
// ---------------------------------- 2025/10/12 製作者：佐藤琢磨
//
/*===============================================================*/

#include "playButtonBg.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"
#include "mouseCollision.h"

void PlayButtonBg::Init()
{
	AddComponent<SortingLayer>()->Set(2);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, 25.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { SCREEN_WIDTH * 0.5f, 25.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\playButtonBG.png", true);
	AddComponent<SquareVertex>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}

}
