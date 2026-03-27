/*===============================================================*/
//
//	load.hcpp(マルチスレッドシーンロード処理)
// 
// 
// ---------------------------------- 2025/09/25 製作者：佐藤琢磨
//
/*===============================================================*/

#include "load.h"
#include "manager.h"
#include "scene.h"

#include "title.h"
#include "createScene.h"

bool Load::m_loaded = false;
std::mutex Load::m_mtx;
Load::LoadSceneType Load::m_sceneType = Load::NONE;

void Load::StartLoading(LoadSceneType type)
{
	m_sceneType = type;	// 遷移したいシーンを取得

	std::thread([]() {
		/* シーン読込開始 */
		SceneLoad(m_sceneType);

		std::this_thread::sleep_for(std::chrono::seconds(3));	//ロード
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			m_loaded = true;	//ロードが完了したらtrue
		}

		/* シーン読込完了 */
		}).detach();
}

void Load::SceneLoad(LoadSceneType type)
{
	switch (type)
	{
	case NONE:
		break;
	case LOAD_TITLE:
		Manager::SetScene<Title>();
		break;
	case LOAD_CREATE_SCENE:
		Manager::SetScene<CreateScene>();
		break;
	default:
		break;
	}
}
