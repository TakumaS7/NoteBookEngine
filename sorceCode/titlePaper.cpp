/*===============================================================*/
//
//	titlePaper.cpp(タイトルの紙管理)
// 
// 
// ---------------------------------- 2025/12/05 製作者：佐藤琢磨
//
/*===============================================================*/

#include "titlePaper.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader3D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "meshVertex.h"
#include "paperWave.h"

void TitlePaper::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.015f, 0.15f, 0.02f });
	AddComponent<Shader3D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\titlePaper.png", true);
	AddComponent<MeshVertex>();
	AddComponent<PaperWave>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
