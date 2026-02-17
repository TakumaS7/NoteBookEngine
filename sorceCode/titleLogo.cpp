/*===============================================================*/
//
//	titleLogo.cpp(タイトルロゴ管理)
// 
// 
// ---------------------------------- 2025/07/20 製作者：佐藤琢磨
//
/*===============================================================*/

#include "titleLogo.h"

#include "sortingLayer.h"
#include "transform.h"
#include "transformDraw.h"
#include "shader2D.h"
#include "textureComponent.h"
#include "squareVertex.h"

void TitleLogo::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ 640.0f, 210.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 250.0f, 250.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\NoteBookEngine_Title.png", true);
	AddComponent<SquareVertex>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
