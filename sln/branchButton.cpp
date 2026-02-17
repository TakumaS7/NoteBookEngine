/*===============================================================*/
//
//	branchButton.cpp(ï™äÚÉ{É^Éìä«óù)
// 
// 
// ---------------------------------- 2026/02/12 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "branchButton.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "squareVertex.h"
#include "textureComponent.h"
#include "textComponent.h"
#include "mouseCollision.h"
#include "branchButtonComponent.h"

void BranchButton::Init()
{
	AddComponent<SortingLayer>()->Set(2);
	AddComponent<Transform>()->Set(m_transform);
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(m_texturePath, true);
	AddComponent<SquareVertex>();
	AddComponent<TextComponent>()->Set(m_text, &m_transform, true, m_textColor, { 0.0f, 0.0f, 0.0f }, m_transform.scale.y, TEXT_ALIGNMENT_CENTER, PARAGRAPH_ALIGNMENT_CENTER);
	AddComponent<MouseCollision>()->Set(this);
	AddComponent<BranchButtonComponent>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
