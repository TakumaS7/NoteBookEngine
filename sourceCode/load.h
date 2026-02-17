/*===============================================================*/
//
//	load.h(マルチスレッドシーンロード処理)
// 
// 
// ---------------------------------- 2025/09/25 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _LOAD_H
#define _LOAD_H

#include <thread>
#include <mutex>
#include <chrono>

class Load
{
private:
	static bool m_loaded;		// ロード
	static std::mutex m_mtx;	// 排他的制御のためのミューテックスオブジェクト

public:
	/* ロードシーン管理enum */
	enum LoadSceneType
	{
		NONE,
		LOAD_TITLE,
		LOAD_GAME,
		LOAD_RESULT,
		LOAD_CREATE_SCENE,
	};

	static LoadSceneType m_sceneType;

	static bool GetLoadedFrag() { 
		return m_loaded;
	}
	static bool SetLoadedFrag(bool frag) { 
		m_loaded = frag;
	}

	static void StartLoading(LoadSceneType type);	// ロードを開始し始める関数
	static void SceneLoad(LoadSceneType type);		// どのシーンに遷移するか分岐する関数
};

#endif //_LOAD_H