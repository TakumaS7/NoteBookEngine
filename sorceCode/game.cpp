/*===============================================================*/
//
//	game.cpp(ÉQÅ[ÉÄèàóù)
// 
// 
// ---------------------------------- 2025/07/19 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "game.h"
#include "renderer.h"
#include "input.h"

#include "camera.h"
#include "player.h"
#include "plane.h"
#include "square.h"
#include "cube.h"

#include "manager.h"
#include "result.h"

void Game::Init()
{
	AddGameObject<Camera>(CAMERA);
	AddGameObject<Player>(OBJECT_3D);
	AddGameObject<Cube>(OBJECT_3D);
	AddGameObject<Plane>(OBJECT_3D);
	AddGameObject<Square>(OBJECT_2D);

	Scene::Init();
}

void Game::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Result>();
	}
}
