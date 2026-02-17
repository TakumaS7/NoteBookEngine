/*===============================================================*/
//
//	textureComponent.h(âÊëú)
// 
// 
// ---------------------------------- 2025/07/15 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#ifndef _TEXTURE_COMPONRNENT_H
#define _TEXTURE_COMPONRNENT_H

#include "component.h"
#include <string>
#include "main.h"
#include "renderer.h"

class TextureComponent : public Component
{
private:
	std::wstring m_name;
	bool m_material = false;
	ID3D11ShaderResourceView* m_Texture = nullptr;
	float m_alpha = 1.0f;	// Éøílí≤êÆ

public:
	void Set(const std::wstring& name, const bool& material = true)
	{ 
		m_name = name;
		m_material = material;
	}

	void SetMaterialFlag(const bool& material = true)
	{
		m_material = material;
	}

	void SetAlpha(float alpha) { m_alpha = alpha; }

	void SetTexture(const std::wstring& name)
	{
		if (m_Texture)
		{
			m_Texture->Release();
			m_Texture = nullptr;
		}

		m_name = name;

		TexMetadata metadata;
		ScratchImage image;

		HRESULT hr = LoadFromWICFile(
			m_name.c_str(),
			WIC_FLAGS_NONE,
			&metadata,
			image);

		if (FAILED(hr))
		{
			m_Texture = nullptr;
			return;
		}

		hr = CreateShaderResourceView(
			Renderer::GetDevice(),
			image.GetImages(),
			image.GetImageCount(),
			metadata,
			&m_Texture);

		if (FAILED(hr))
		{
			assert(false && "Texture load failed");
			m_Texture = nullptr;
			return;
		}
	}

	void Init() override
	{
		if (m_Texture) { return; }  // ìÒèdInitñhé~
		
		if (!m_name.empty()) 
		{
			TexMetadata metadata;
			ScratchImage image;
			HRESULT hr = LoadFromWICFile(
				m_name.c_str(),
				WIC_FLAGS_NONE,
				&metadata,
				image);

			if (FAILED(hr))
			{
				m_Texture = nullptr;
				return;
			}

			hr = CreateShaderResourceView(
				Renderer::GetDevice(),
				image.GetImages(),
				image.GetImageCount(),
				metadata,
				&m_Texture);

			if (FAILED(hr))
			{
				assert(false && "Texture load failed");
				m_Texture = nullptr;
				return;
			}
		}
	}

	void Uninit() override 
	{
		if (m_Texture)
		{
			m_Texture->Release();
			m_Texture = nullptr;
		}
	}

	void Update() override {}
	void Draw() override
	{
		if (!m_Texture)
		{
			ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, nullSRV);
			return;
		}

		MATERIAL material{};
		material.Diffuse = { 1.0f, 1.0f, 1.0f, m_alpha };
		material.TextureEnable = m_material;
		Renderer::SetMaterial(material);

		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	}
};

#endif //_TEXTURE_COMPONRNENT_H