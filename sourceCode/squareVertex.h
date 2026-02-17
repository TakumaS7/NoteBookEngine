/*===============================================================*/
//
//	squareVertex.h(正方形頂点管理)
// 
// 
// ---------------------------------- 2025/07/20 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SQUARE_VERTEX_H
#define _SQUARE_VERTEX_H

#include "main.h"
#include "component.h"
#include "renderer.h"
#include "vector3.h"
#include "gameObject.h"
#include "textureCharaComponent.h"

class SquareVertex : public Component
{
private:
	ID3D11Buffer* m_vertexBuffer{};
	Vector3 m_color = {1.0f, 1.0f, 1.0f };
	float m_alpha = 1.0f;
	GameObject* m_gameObject{};

public:
	void Set(Vector3 color, float alpha)
	{
		m_color = color;
		m_alpha = alpha;
	}

	void SetCharaObject(GameObject* gameObject)
	{
		m_gameObject = gameObject;
	}

	void Init() override 
	{
		VERTEX_3D vertex[4];

		vertex[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = XMFLOAT4(m_color.x, m_color.y, m_color.z, m_alpha);
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[1].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = XMFLOAT4(m_color.x, m_color.y, m_color.z, m_alpha);
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[2].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = XMFLOAT4(m_color.x, m_color.y, m_color.z, m_alpha);
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[3].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = XMFLOAT4(m_color.x, m_color.y, m_color.z, m_alpha);
		vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

		//頂点バッファ生成
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);
	}

	void Uninit() override 
	{
		m_vertexBuffer->Release();
	}

	void Update() override {}
	void Draw() override
	{
		if (m_gameObject)
		{
			TextureCharaComponent* tex = m_gameObject->GetComponent<TextureCharaComponent>();
			if (!tex || !tex->IsDrawable())
			{
				return;
			}
		}
		Renderer::SetDepthEnable(false);

		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(
			0, 1, &m_vertexBuffer, &stride, &offset);

		Renderer::GetDeviceContext()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		Renderer::GetDeviceContext()->Draw(4, 0);

		Renderer::SetDepthEnable(true);
	}
};

#endif //_SQUARE_VERTEX_H