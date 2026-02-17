/*===============================================================*/
//
//	playButton.cpp(実行ボタン管理)
// 
// 
// ---------------------------------- 2025/10/12 製作者：佐藤琢磨
//
/*===============================================================*/

#include "playButton.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"
#include "mouseCollision.h"
#include "playButtonComponent.h"

void PlayButton::Init()
{
	AddComponent<SortingLayer>()->Set(3);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.97f, 25.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 25.0f, 25.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\playIcon.png", true);
	AddComponent<SquareVertex>();
	AddComponent<MouseCollision>()->Set(this);
	AddComponent<PlayButtonComponent>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
