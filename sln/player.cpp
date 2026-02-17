/*===============================================================*/
//
//	player.cpp(プレイヤー管理)
// 
// 
// ---------------------------------- 2025/07/15 製作者：佐藤琢磨
//
/*===============================================================*/

#include "player.h"

#include "camera.h"
#include "manager.h"

#include "sortingLayer.h"
#include "transform.h"
#include "transformDraw.h"
#include "animationModelComponent.h"
#include "shader3D.h"
#include "keyMove.h"
#include "keyJump.h"
#include "rigidbody.h"
#include "aabb.h"

void Player::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ 0.0f, 5.0f, -7.0f }, { 0.0f, 0.0f, 0.0f }, { 0.01f, 0.01f, 0.01f });
	AddComponent<Shader3D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<Rigidbody>()->Set(this, true, Rigidbody::RigidbodyConstraints::NONE);

	AddComponent<AnimationModelComponent>()->Set(this->GetComponent<Rigidbody>(), 
		"asset\\model\\AkaiOBJ\\Akai.fbx", 
		"asset\\model\\AkaiOBJ\\Akai_Idle.fbx", 
		"asset\\model\\AkaiOBJ\\Akai_Run.fbx");
	
	AddComponent<KeyMove>()->Set(this, Manager::GetScene()->GetGameObject<Camera>());
	AddComponent<KeyJump>()->Set(this);
	AddComponent<AABB>()->Set(this, {1.0f, 1.0f, 1.0f});

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
