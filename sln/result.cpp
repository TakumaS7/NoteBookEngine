/*===============================================================*/
//
//	result.cpp(ƒŠƒUƒ‹ƒgˆ—)
// 
// 
// ---------------------------------- 2025/07/19 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#include "result.h"
#include "renderer.h"
#include "input.h"

#include "manager.h"
#include "title.h"

void Result::Init()
{
	Scene::Init();
}

void Result::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Title>();
	}

}
