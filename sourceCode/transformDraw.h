/*===============================================================*/
//
//	transformDraw.h(トランスフォーム描画管理)
// 
// 
// ---------------------------------- 2025/07/15 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TRANSFORM_DRAW_H
#define _TRANSFORM_DRAW_H

#include "main.h"
#include "component.h"
#include "gameObject.h"
#include "transform.h"
#include "textureCharaComponent.h"

class TransformDraw : public Component
{
private:
	GameObject* m_gameObject{};
	Transform* m_transform{};

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override 
	{
		TextureCharaComponent* textureChara = m_gameObject->GetComponent<TextureCharaComponent>();
		if (textureChara && !textureChara->IsDrawable())
		{
			return;
		}

		m_transform = m_gameObject->GetComponent<Transform>();

		if (!m_transform) { return; }

		//マトリクス設定
		XMMATRIX world, matrixScale, matrixRot, matrixTrans;
		matrixScale = XMMatrixScaling(m_transform->scale.x, m_transform->scale.y, m_transform->scale.z);
		matrixRot = XMMatrixRotationRollPitchYaw(m_transform->rotation.x, m_transform->rotation.y, m_transform->rotation.z);
		matrixTrans = XMMatrixTranslation(m_transform->position.x, m_transform->position.y, m_transform->position.z);
		world = matrixScale * matrixRot * matrixTrans;

		Renderer::SetWorldMatrix(world);
	}
};

#endif //_TRANSFORM_DRAW_H