/*===============================================================*/
//
//	titleSkydome.cpp(タイトルスカイドーム管理)
// 
// 
// ---------------------------------- 2025/12/10 製作者：佐藤琢磨
//
/*===============================================================*/

#include "titleSkydome.h"

#include "sortingLayer.h"
#include "transform.h"
#include "transformDraw.h"
#include "modelRendererComponent.h"
#include "shader3D.h"

void TitleSkydome::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f, 100.0f });
	AddComponent<Shader3D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<ModelRendererComponent>()->Set("asset\\model\\skydomeOBJ\\sky.obj");

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
