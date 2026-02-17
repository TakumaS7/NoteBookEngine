/*===============================================================*/
//
//	createButton.cpp(CreateÉ{É^Éìä«óù)
// 
// 
// ---------------------------------- 2025/09/25 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "createButton.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"
#include "mouseCollision.h"
#include "createButtonComponent.h"
#include "buttonZoomInOut.h"

void CreateButton::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, 600.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 140.0f, 50.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\TitleButtonCreate.png", true);
	AddComponent<SquareVertex>();
	AddComponent<MouseCollision>()->Set(this);
	AddComponent<CreateButtonComponent>()->Set(this);
	AddComponent<ButtonZoomInOut>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}

}
