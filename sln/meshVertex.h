/*===============================================================*/
//
//	meshVertex.h(メッシュ頂点管理)
// 
// 
// ---------------------------------- 2025/07/05 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _MESH_VERTEX_H
#define _MESH_VERTEX_H

#include "main.h"
#include "component.h"
#include "renderer.h"

#define MESH_VERTEX_MAX 50

class MeshVertex : public Component
{
private:
	ID3D11Buffer* m_vertexBuffer{};
	ID3D11Buffer* m_indexBuffer{};
	VERTEX_3D m_vertex[MESH_VERTEX_MAX][MESH_VERTEX_MAX];
	float m_time = 0.0f;			// 波の揺れ速度
	float m_offsetY = 0.0f;			// 波の高さ

public:
	VERTEX_3D(*GetVertex())[MESH_VERTEX_MAX] { return m_vertex; }
	ID3D11Buffer* GetVertexBuffer() { return m_vertexBuffer; }

	void Init() override
	{
		/* 頂点バッファ生成 */
		{
			for (int x = 0; x < MESH_VERTEX_MAX; x++)
			{
				for (int z = 0; z < MESH_VERTEX_MAX; z++)
				{
					m_vertex[x][z].Position =
						XMFLOAT3((x - MESH_VERTEX_MAX / 2) * 5.0f, 0.0f, (z - MESH_VERTEX_MAX / 2) * -5.0f);
					m_vertex[x][z].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
					m_vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
					m_vertex[x][z].TexCoord = XMFLOAT2(
						(float)x / (MESH_VERTEX_MAX - 1),
						(float)z / (MESH_VERTEX_MAX - 1)
					);
				}
			}


			/* 法線ベクトル算出 */
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * MESH_VERTEX_MAX * MESH_VERTEX_MAX;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = m_vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);
		}




		/* インデックスバッファ生成 */
		{
			unsigned int index[((MESH_VERTEX_MAX + 1) * 2) * (MESH_VERTEX_MAX - 1) - 2];

			int i = 0;
			for (int x = 0; x < (MESH_VERTEX_MAX - 1); x++)
			{
				for (int z = 0; z < MESH_VERTEX_MAX; z++)
				{
					index[i] = x * MESH_VERTEX_MAX + z;
					i++;

					index[i] = (x + 1) * MESH_VERTEX_MAX + z;
					i++;
				}

				if (x == (MESH_VERTEX_MAX - 2))
					break;

				index[i] = (x + 1) * MESH_VERTEX_MAX + (MESH_VERTEX_MAX - 1);
				i++;

				index[i] = (x + 1) * MESH_VERTEX_MAX;
				i++;
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * (((MESH_VERTEX_MAX + 1) * 2) * (MESH_VERTEX_MAX - 1) - 2);
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_indexBuffer);
		}
	}

	void Uninit() override
	{
		m_vertexBuffer->Release();
		m_indexBuffer->Release();
	}

	void Update() override {}

	void Draw() override
	{
		Renderer::SetDepthEnable(true);

		/* 頂点バッファ設定 */
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		/* インデックスバッファ設定 */
		Renderer::GetDeviceContext()->IASetIndexBuffer(
			m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		/* プリミティブトポロジ設定 */
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		/* ポリゴン描画 */
		Renderer::GetDeviceContext()->DrawIndexed((101 * 2) * (MESH_VERTEX_MAX - 1) - 2, 0, 0);
	}
};

#endif //_MESH_VERTEX_H