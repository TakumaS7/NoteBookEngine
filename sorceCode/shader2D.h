/*===============================================================*/
//
//	shader2D.h(2Dシェーダー)
// 
// 
// ---------------------------------- 2025/07/20 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _SHADER_2D_H
#define _SHADER_2D_H

#include "main.h"
#include "component.h"
#include "renderer.h"
#include <string>

class Shader2D : public Component
{
private:
	std::string m_vertexFileName;
	std::string m_pixelFileName;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

public:
	void Set(const std::string& vertexFileName, const std::string& pixelFileName)
	{
		m_vertexFileName = vertexFileName;
		m_pixelFileName = pixelFileName;
	}

	void Init() override 
	{
		//シェーダー読み込み
		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_vertexFileName.c_str());

		Renderer::CreatePixelShader(&m_PixelShader, m_pixelFileName.c_str());
	}

	void Uninit() override 
	{
		m_VertexLayout->Release();
		m_VertexShader->Release();
		m_PixelShader->Release();
	}

	void Update() override {}
	void Draw() override 
	{
		//入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//シェーダー設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//マトリクス設定
		Renderer::SetWorldViewProjection2D();
	}
};

#endif //_SHADER_2D_H