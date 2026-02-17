/*===============================================================*/
//
//	deleteSpeakText.cpp(セリフテキスト削除管理)
// 
// 
// ---------------------------------- 2025/10/04 製作者：佐藤琢磨
//
/*===============================================================*/

#include "deleteSpeakText.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"
#include "mouseCollision.h"
#include "deleteObjectButtonComponent.h"

void DeleteSpeakText::Init()
{
	AddComponent<SortingLayer>()->Set(1);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\deleteButton.png", true);
	AddComponent<SquareVertex>();
	AddComponent<MouseCollision>()->Set(this);
	AddComponent<DeleteObjectButtonComponent>()->Set(this, m_deleteObject);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
