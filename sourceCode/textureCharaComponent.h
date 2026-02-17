/*===============================================================*/
//
//	textureChara.h(キャラ画像)
// 
// 
// ---------------------------------- 2025/01/07 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _TEXTURE_CHARA_H
#define _TEXTURE_CHARA_H

#include "component.h"
#include <string>
#include "main.h"
#include "renderer.h"

class TextureCharaComponent : public Component
{
private:
	std::wstring m_name;
	bool m_material = false;
	ID3D11ShaderResourceView* m_Texture = nullptr;
	ID3D11ShaderResourceView* m_srv = nullptr;
	float m_alpha = 1.0f;		// α値調整
	bool m_drawable = false;	// 描画許可フラグ

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

	bool IsDrawable() const { return m_drawable; }
	void SetMaterialFlagDrawable(bool material, bool drawable) { 
		m_material = material;
		m_drawable = drawable;
	}

	void SetDrawable(bool frag){
		m_drawable = frag;
	}

	void Init() override
	{
		if (m_Texture) { return; }  // 二重Init防止

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
		ID3D11ShaderResourceView* srv = nullptr;

		if (m_Texture && m_material && m_drawable)
		{
			MATERIAL material{};
			material.Diffuse = { 1,1,1,m_alpha };
			material.TextureEnable = true;
			Renderer::SetMaterial(material);

			srv = m_Texture;
		}

		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
	}
};

#endif //_TEXTURE_CHARA_H