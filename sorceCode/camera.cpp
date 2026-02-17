/*===============================================================*/
//
//	camera.cpp(ƒJƒƒ‰ŠÇ—)
// 
// 
// ---------------------------------- 2025/07/16 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#include "camera.h"

#include "sortingLayer.h"
#include "transform.h"
#include "keyRotate.h"
#include "cameraChase.h"
#include "manager.h"
#include "player.h"
#include "cameraView.h"

void Camera::Init()
{
	{
		
		AddComponent<SortingLayer>()->Set(0);
		AddComponent<Transform>()->Set({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
		AddComponent<KeyRotate>()->Set(this->GetComponent<Transform>());
		AddComponent<CameraChase>()->Set(this->GetComponent<Transform>(), Manager::GetScene()->GetGameObject<Player>());
		AddComponent<CameraView>()->Set(this->GetComponent<Transform>(), Manager::GetScene()->GetGameObject<Player>());

		for (auto component : GetComponentVector())
		{
			component->Init();
		}
	}
}
