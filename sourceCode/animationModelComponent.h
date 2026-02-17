/*===============================================================*/
//
//	animationModelComponent.h(モデルアニメーションコンポーネント)
// 
// 
// ---------------------------------- 2025/09/21 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _ANIMATION_MODEL_COMPONENT_H
#define _ANIMATION_MODEL_COMPONENT_H

#include "component.h"
#include "rigidbody.h"
#include "animationModel.h"
#include <string>
#include "time.h"

class AnimationModelComponent : public Component
{
private:
	Rigidbody* m_rigidbody;
	std::string m_modelFileName;
	std::string m_idleFileName;
	std::string m_runFileName;
	AnimationModel* m_animationModel;
	float m_frame = 0.0f;
	float m_blendRate = 0.0f;

public:
	void Set(Rigidbody* rigidbody, const std::string& modelFileName, const std::string& idleFileName, const std::string& runFileName)
	{
		m_rigidbody = rigidbody;
		m_modelFileName = modelFileName;
		m_idleFileName = idleFileName;
		m_runFileName = runFileName;
	}

	void Init() override 
	{
		/* モデル読込 */
		m_animationModel = new AnimationModel();
		m_animationModel->Load(m_modelFileName.c_str());					// 基盤となるモデルを読み込む
		m_animationModel->LoadAnimation(m_idleFileName.c_str(), "Idle");	// IDLE状態のアニメーション読込
		m_animationModel->LoadAnimation(m_runFileName.c_str(), "Run");		// Run状態のアニメーション読込
	}

	void Uninit() override 
	{
		m_animationModel->Uninit();
		delete m_animationModel;
	}

	void Update() override 
	{
		/* アニメーションのフレームレート計測 */
		m_frame += Time::m_deltaTime * 60.0f;

		/* アニメーションモデルの更新処理 */
		m_animationModel->Update("Idle", (int)m_frame, "Run", (int)m_frame, m_blendRate);

		/* ブレンドレートの操作 */
		if (abs(m_rigidbody->GetVelocity().x) <= 0.1f && abs(m_rigidbody->GetVelocity().z) <= 0.1f) {
			m_blendRate += -Time::m_deltaTime * 3.0f;	//少しずつ変化（RunからIdleへ）
		}
		else
		{
			m_blendRate += Time::m_deltaTime * 3.0f;	// 少しずつ変化（IdleからRunへ）
		}

		/* ブレンドステート制御処理 */
		if (m_blendRate >= 1.0f) {
			m_blendRate = 1.0f;
		}
		if (m_blendRate <= 0.0f) {
			m_blendRate = 0.0f;
		}
	}

	void Draw() override 
	{
		m_animationModel->Draw();
	}
};

#endif //_ANIMATION_MODEL_COMPONENT_H