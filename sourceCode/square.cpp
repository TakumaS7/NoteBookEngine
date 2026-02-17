/*===============================================================*/
//
//	square.cpp(ê≥ï˚å`ä«óù)
// 
// 
// ---------------------------------- 2025/07/20 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "square.h"

#include "sortingLayer.h"
#include "transform.h"
#include "transformDraw.h"
#include "shader2D.h"
#include "textureComponent.h"
#include "squareVertex.h"

void Square::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ 100.0f, 100.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 200.0f, 200.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\square.png", true);
	AddComponent<SquareVertex>();

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
