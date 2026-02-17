/*===============================================================*/
//
//	transformDrawParticle.h(パーティクル用トランスフォーム描画管理)
// 
// 
// ---------------------------------- 2025/12/07 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TRANSFORM_DRAW_PARTICLE_H
#define _TRANSFORM_DRAW_PARTICLE_H

#include "main.h"
#include "component.h"
#include "gameObject.h"
#include "transform.h"
#include "particleVertex.h"
#include "titleCamera.h"
#include "manager.h"
#include "scene.h"
#include "cameraView.h"

class TransformDrawParticle : public Component
{
private:
	GameObject* m_gameObject;
	Transform* m_transform;
	ParticleVertex* m_particleVertex;
	TitleCamera* m_titleCamera;

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override
	{
		m_transform = m_gameObject->GetComponent<Transform>();

		/* 自分自身のParticleVertexコンポーネントを取得 */
		m_particleVertex = m_gameObject->GetComponent<ParticleVertex>();
		/* シーン上のカメラを取得 */
		m_titleCamera = Manager::GetScene()->GetGameObject<TitleCamera>();

		if (!m_transform) { return; }
		if (!m_particleVertex) { return; }
		if (!m_titleCamera) { return; }

		XMMATRIX view = m_titleCamera->GetComponent<CameraView>()->GetViewMatrix();

		//ビューの逆行列
		XMMATRIX invView;
		invView = XMMatrixInverse(nullptr, view); //逆行列
		invView.r[3].m128_f32[0] = 0.0f;
		invView.r[3].m128_f32[1] = 0.0f;
		invView.r[3].m128_f32[2] = 0.0f;
	
		Renderer::SetRasterizeBackCull(false);
		for (int i = 0; i < PARTICLE_MAX; ++i)
		{
			/* enableのみを表示 */
			if (m_particleVertex->GetParticleStatus()[i].enable)
			{
				//マトリクス設定
				XMMATRIX world, matrixScale, matrixRot, matrixTrans;
				matrixScale = XMMatrixScaling(m_transform->scale.x, m_transform->scale.y, m_transform->scale.z);
				matrixRot = XMMatrixRotationRollPitchYaw(m_particleVertex->GetParticleStatus()[i].rotation.x, m_particleVertex->GetParticleStatus()[i].rotation.y, m_particleVertex->GetParticleStatus()[i].rotation.z);
				matrixTrans = XMMatrixTranslation(m_particleVertex->GetParticleStatus()[i].position.x, m_particleVertex->GetParticleStatus()[i].position.y, m_particleVertex->GetParticleStatus()[i].position.z);
				world = matrixScale * invView * matrixRot * matrixTrans;

				Renderer::SetWorldMatrix(world);

				/* ポリゴン描画 */
				m_particleVertex->ParticleDeviceContext();
			}
		}
		Renderer::SetRasterizeBackCull(true);
	}
};

#endif //_TRANSFORM_DRAW_PARTICLE_H