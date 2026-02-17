/*===============================================================*/
//
//	nameText.cpp(名前テキスト管理)
// 
// 
// ---------------------------------- 2026/01/01 製作者：佐藤琢磨
//
/*===============================================================*/

#include "nameText.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"
#include "textComponent.h"
#include "mouseCollision.h"
#include "objectDragDrop.h"
#include "setDeleteButton.h"

void NameText::Init()
{
	AddComponent<SortingLayer>()->Set(4);
	AddComponent<Transform>()->Set(m_position, { 0.0f, 0.0f, 0.0f }, { 450.0f, 50.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextComponent>()->Set(m_text.c_str(), this->GetComponent<Transform>(), true, D2D1::ColorF::White, { 0.0f, 0.0f, 0.0f });

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
