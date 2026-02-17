/*===============================================================*/
//
//	particleVertex.h(パーティクル頂点管理)
// 
// 
// ---------------------------------- 2025/12/05 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _PARTICLE_VERTEX_H
#define _PARTICLE_VERTEX_H

#include "main.h"
#include "component.h"
#include "renderer.h"
#include "vector3.h"

#include "transform.h"
#include "gameObject.h"
#include "titleCamera.h"

#include "manager.h"
#include "scene.h"
#include "titleCamera.h"
#include "cameraView.h"

#include "texture.h"

#define PARTICLE_MAX 10000
#define COLOR_MIN 0.7f

class ParticleVertex : public Component
{
private:
	ID3D11Buffer* m_vertexBuffer;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11ShaderResourceView* m_Texture;

	struct PARTICLE
	{
		bool enable;		// 使用フラグ
		int life;			// 寿命
		Vector3 position;	// パーティクルごとの座標
		Vector3 velocity;	// 移動速度
		Vector3 rotation;	// パーティクルごとの回転
		Vector3 spinSpeed;	// 回転速度
	};

	PARTICLE m_particle[PARTICLE_MAX];
	Vector3 m_color = { 0.0f, 0.0f, 0.0f };
	Vector3 m_addColor = { 0.0005f, 0.001f, 0.004f };

	GameObject* m_gameObject;
	Transform* m_transform;
	TitleCamera* m_titleCamera;
public:
	PARTICLE (&GetParticleStatus())[PARTICLE_MAX] { return m_particle; }

	void Set(GameObject* gameObject) { m_gameObject = gameObject; }
	
	/* ポリゴン描画 */
	void ParticleDeviceContext(){ Renderer::GetDeviceContext()->Draw(4, 0); }

	void Init() override
	{
		VERTEX_3D vertex[4];

		vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
		vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
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
	
		for (int i = 0; i < PARTICLE_MAX; ++i)
		{
			m_particle[i].enable = false;
		}
	}

	void Uninit() override
	{
		m_vertexBuffer->Release();
	}

	void Update() override {}
	void Draw() override
	{
		Renderer::SetDepthEnable(true);

		//頂点バッファー設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		//プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}


};

#endif //_PARTICLE_VERTEX_H