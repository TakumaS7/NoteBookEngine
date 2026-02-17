/*===============================================================*/
//
//	paperWave.h(波挙動管理)
// 
// 
// ---------------------------------- 2025/12/05 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _PAPER_WAVE_H
#define _PAPER_WAVE_H

#include "main.h"
#include "component.h"
#include "gameObject.h"
#include "meshVertex.h"
#include "time.h"

class PaperWave : public Component
{
private:
	GameObject* m_gameObject;		            // 自分自身
	VERTEX_3D(*m_vertex)[MESH_VERTEX_MAX] {};	// メッシュ頂点
	ID3D11Buffer* m_vertexBuffer{};	            // メッシュバーテックスバッファ
	float m_time = 0.0f;			            // 波の揺れ速度
	float m_offsetY = 0.0f;			            // 波の高さ

public:
	void Set(GameObject* gameObject) { m_gameObject = gameObject; }

	void Init() override 
	{
	}
	void Uninit() override {}
    void Update() override
    {
        m_vertex = m_gameObject->GetComponent<MeshVertex>()->GetVertex();
        m_vertexBuffer = m_gameObject->GetComponent<MeshVertex>()->GetVertexBuffer();

        if (!m_vertex) { return; }
        if (!m_vertexBuffer) { return; }

        /* 波の挙動制御 */
        for (int x = 0; x < MESH_VERTEX_MAX; ++x)
        {
            for (int z = 0; z < MESH_VERTEX_MAX; ++z)
            {
                float wave =
                    sinf(z * 0.05f + m_time * 3.0f) * 2.0f;
                m_vertex[x][z].Position.y = wave + m_offsetY;
            }
        }

        m_time += Time::m_deltaTime;

        /* 頂点書き込み */
        D3D11_MAPPED_SUBRESOURCE msr;
        Renderer::GetDeviceContext()->Map(m_vertexBuffer, 0,
            D3D11_MAP_WRITE_DISCARD, 0, &msr);

        VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

        memcpy(vertex, m_vertex,
            sizeof(VERTEX_3D) * MESH_VERTEX_MAX * MESH_VERTEX_MAX);

        Renderer::GetDeviceContext()->Unmap(m_vertexBuffer, 0);
    }
    void Draw() override {}
};

#endif //_PAPER_WAVE_H