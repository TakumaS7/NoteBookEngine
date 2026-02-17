/*===============================================================*/
//
//	boxVertex.h(立方体頂点管理)
// 
// 
// ---------------------------------- 2025/07/17 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _BOX_VERTEX_H
#define _BOX_VERTEX_H

#include "component.h"
#include "main.h"
#include "renderer.h"

class BoxVertex : public Component
{
private:
	ID3D11Buffer* m_vertexBuffer;

public:
	void Init() override
	{
		VERTEX_3D vertex[24];

		//正面
		vertex[0].Position = XMFLOAT3(1.0f, 1.0f, -1.0f);
		vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[1].Position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[2].Position = XMFLOAT3(1.0f, -1.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[3].Position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


		//上面
		vertex[4].Position = XMFLOAT3(1.0f, 1.0f, -1.0f);
		vertex[4].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[4].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[4].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[5].Position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		vertex[5].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[5].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[5].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[6].Position = XMFLOAT3(1.0f, 1.0f, 1.0f);
		vertex[6].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[6].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[6].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[7].Position = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		vertex[7].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[7].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[7].TexCoord = XMFLOAT2(1.0f, 1.0f);


		//下面
		vertex[8].Position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		vertex[8].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[8].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[8].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[9].Position = XMFLOAT3(1.0f, -1.0f, -1.0f);
		vertex[9].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[9].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[9].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[10].Position = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		vertex[10].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[10].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[11].Position = XMFLOAT3(1.0f, -1.0f, 1.0f);
		vertex[11].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[11].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[11].TexCoord = XMFLOAT2(1.0f, 1.0f);


		//左面
		vertex[12].Position = XMFLOAT3(1.0f, 1.0f, -1.0f);
		vertex[12].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[12].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[12].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[13].Position = XMFLOAT3(1.0f, 1.0f, 1.0f);
		vertex[13].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[13].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[13].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[14].Position = XMFLOAT3(1.0f, -1.0f, -1.0f);
		vertex[14].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[14].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[14].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[15].Position = XMFLOAT3(1.0f, -1.0f, 1.0f);
		vertex[15].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[15].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[15].TexCoord = XMFLOAT2(1.0f, 1.0f);


		//右面
		vertex[16].Position = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		vertex[16].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[16].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[16].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[17].Position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		vertex[17].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[17].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[17].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[18].Position = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		vertex[18].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[18].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[18].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[19].Position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		vertex[19].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[19].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[19].TexCoord = XMFLOAT2(1.0f, 1.0f);


		//背面
		vertex[20].Position = XMFLOAT3(1.0f, 1.0f, 1.0f);
		vertex[20].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[20].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[20].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[21].Position = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		vertex[21].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[21].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[21].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[22].Position = XMFLOAT3(1.0f, -1.0f, 1.0f);
		vertex[22].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[22].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[22].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[23].Position = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		vertex[23].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[23].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[23].TexCoord = XMFLOAT2(1.0f, 1.0f);

		//頂点バッファ生成
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 24;
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
		Renderer::SetDepthEnable(true);

		//頂点バッファー設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		//プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//ポリゴン描画
		Renderer::GetDeviceContext()->Draw(24, 0);
	}

};

#endif //_BOX_VERTEX_H