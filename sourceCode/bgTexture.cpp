/*===============================================================*/
//
//	bgTexture.cpp(îwåiâÊëúä«óù)
// 
// 
// ---------------------------------- 2025/12/04 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "bgTexture.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"

void BgTexture::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<SquareVertex>();
	AddComponent<TextureComponent>()->SetMaterialFlag(true);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
