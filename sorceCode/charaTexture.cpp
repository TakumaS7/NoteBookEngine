/*===============================================================*/
//
//	charaTexture.cpp(ƒLƒƒƒ‰‰æ‘œŠÇ—)
// 
// 
// ---------------------------------- 2026/01/06 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#include "charaTexture.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureCharaComponent.h"
#include "squareVertex.h"

void CharaTexture::Init()
{
	AddComponent<SortingLayer>()->Set(1);
	AddComponent<Transform>();
	AddComponent<Shader2D>()->Set("shader\\charaTextureVS.cso", "shader\\charaTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<SquareVertex>()->SetCharaObject(this);
	AddComponent<TextureCharaComponent>()->SetMaterialFlagDrawable(true, false);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
