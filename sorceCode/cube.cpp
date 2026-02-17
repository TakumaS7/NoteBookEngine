/*===============================================================*/
//
//	cube.cpp(óßï˚ëÃä«óù)
// 
// 
// ---------------------------------- 2025/07/17 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "cube.h"

#include "sortingLayer.h"
#include "transform.h"
#include "transformDraw.h"
#include "shader3D.h"
#include "textureComponent.h"
#include "boxVertex.h"
#include "rigidbody.h"
#include "aabb.h"
#include "keyJump.h"

void Cube::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ 0.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
	AddComponent<Shader3D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\plane.png", true);
	AddComponent<BoxVertex>();
	AddComponent<Rigidbody>()->Set(this, true, Rigidbody::RigidbodyConstraints::NONE);
	AddComponent<AABB>()->Set(this, this->GetComponent<Transform>()->scale);


	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
