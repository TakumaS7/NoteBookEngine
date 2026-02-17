/*===============================================================*/
//
//	plane.cpp(è∞ä«óù)
// 
// 
// ---------------------------------- 2025/07/17 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "plane.h"

#include "sortingLayer.h"
#include "transform.h"
#include "transformDraw.h"
#include "shader3D.h"
#include "textureComponent.h"
#include "planeVertex.h"
#include "rigidbody.h"
#include "aabb.h"

void Plane::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 50.0f, 0.0f, 50.0f });
	AddComponent<Shader3D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\plane.png", true);
	AddComponent<PlaneVertex>();
	AddComponent<Rigidbody>()->Set(this, false, Rigidbody::RigidbodyConstraints::FREEZE_ALL);
	AddComponent<AABB>()->Set(this, this->GetComponent<Transform>()->scale);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
