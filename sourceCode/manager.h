/*===============================================================*/
//
//	manager.h(マネージャー処理)
// 
// 
// ---------------------------------- 2025/05/14 製作者：佐藤琢磨
//
/*===============================================================*/
#ifndef _MANAGER_H
#define _MANAGER_H

#include <memory>
#include "scene.h"
#include "fade.h"
#include "debug.h"

class Manager
{
private:
	static std::unique_ptr<Scene> m_scene;
	static std::unique_ptr<Scene> m_nextScene;
	static class Fade* m_fade;
	static class Debug* m_debugScene;
	static bool m_isDebug;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	template<typename T>
	static void SetScene()
	{
		m_nextScene = std::make_unique<T>();
	}

	static Scene* GetScene()
	{
		return m_scene.get();
	}

	static Debug* GetDebugScene()
	{
		return m_debugScene;
	}
};

#endif //_MANAGER_H