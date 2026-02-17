/*===============================================================*/
//
//	textBox.cpp(テキスト枠管理)
// 
// 
// ---------------------------------- 2025/10/13 製作者：佐藤琢磨
//
/*===============================================================*/

#include "textBox.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"
#include "textBoxComponent.h"

void TextBox::Init()
{
	AddComponent<SortingLayer>()->Set(3);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, 600.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 500.0f, 110.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\TextFrame.png", true);
	AddComponent<SquareVertex>();
	AddComponent<TextBoxComponent>()->Set(this->GetComponent<Transform>());


	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
