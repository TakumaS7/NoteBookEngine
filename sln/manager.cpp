/*===============================================================*/
//
//	manager.cpp(マネージャー処理)
// 
// 
// ---------------------------------- 2025/05/14 製作者：佐藤琢磨
//
/*===============================================================*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"

#include "title.h"

std::unique_ptr<Scene> Manager::m_scene = nullptr;
std::unique_ptr<Scene> Manager::m_nextScene = nullptr;
Fade* Manager::m_fade = nullptr;
Debug* Manager::m_debugScene = nullptr;
bool Manager::m_isDebug = false;

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	InitMediaFoundation();
	InitSound(GetDebugWindow());

	m_fade = new Fade();

	m_scene = std::make_unique<Title>();	//開始のシーンをセット

	if (m_scene) {
		m_scene->Init();
	}
}


void Manager::Uninit()
{
	if (m_debugScene)
	{
		StopSoundAll();
		m_debugScene->Uninit();
		delete m_debugScene;
		m_debugScene = nullptr;
	}

	if (m_scene) {
		m_scene->Uninit();
		m_scene.reset();
	}

	delete m_fade;

	UninitSound();
	UninitMediaFoundation();

	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	/* デバッグウィンドウのON・OFF管理 */
	m_isDebug = GetDebugWindow();

	/* DebugウィンドウONのタイミングで生成 */
	/* DebugウィンドウOFFのタイミングで破棄 */
	if (m_isDebug)
	{
		if (!m_debugScene) {
			m_debugScene = new Debug();
			m_debugScene->Init();
		}
	}
	else
	{
		if (m_debugScene){
			StopSoundAll();
			m_debugScene->Uninit();
			delete m_debugScene;
			m_debugScene = nullptr;
		}
	}

	Input::Update();

	/* デバッグ中はエディタは動かない */
	if (m_scene && !m_debugScene){
		m_scene->Update();
	}

	if (m_debugScene)
	{
		m_debugScene->Update();
	}

	m_fade->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	if (m_scene) {
		m_scene->Draw();
	}

	m_fade->Draw();

	Renderer::End();

	/* デバッグウィンドウに表示したいものを描画 */
	if (m_debugScene)
	{
		Renderer::BeginDebug();

		m_debugScene->Draw();

		Renderer::EndDebug();
	}

	if (m_nextScene != nullptr)
	{
		m_scene = std::move(m_nextScene);
		m_scene->Init();

		m_nextScene = nullptr;
	}
}
