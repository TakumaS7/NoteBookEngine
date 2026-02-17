/*===============================================================*/
//
//	texture.cpp(テクスチャ処理)
// 
// 
// ---------------------------------- 2025/06/11 製作者：佐藤琢磨
//
/*===============================================================*/

#include "texture.h"

std::unordered_map<std::string, ID3D11ShaderResourceView*> Texture::m_TexturePool;
wchar_t Texture::wFileName[512];
size_t Texture::convertedChars = 0;
std::unordered_map<std::wstring, ID3D11ShaderResourceView*> Texture::m_textureMap;

ID3D11ShaderResourceView* Texture::Load(const char* FileName)
{
	if (m_TexturePool.count(FileName) > 0)
	{
		return m_TexturePool[FileName];
	}

	wchar_t wFileName[512];
	mbstowcs_s(&convertedChars, wFileName, _countof(wFileName), FileName, _TRUNCATE);

	TexMetadata metadata;
	ScratchImage image;
	ID3D11ShaderResourceView* texture;
	LoadFromWICFile(wFileName, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(),
		image.GetImageCount(), metadata, &texture);
	assert(texture);

	m_TexturePool[FileName] = texture;

	return texture;
}

void Texture::EditorLoad(const std::wstring& path)
{
	HRESULT hr;

	/* DirectXTexで画像データを読み込む */
	TexMetadata metadata;
	ScratchImage image;
	ID3D11ShaderResourceView* srv;

	LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &metadata, image);
	hr = CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(),
		image.GetImageCount(), metadata, &srv);

	if (FAILED(hr))
	{
		OutputDebugStringW((L"SRVの作成に失敗しました：" + path + L"\n").c_str());
		return;
	}

	/* mapに登録 */
	m_textureMap[path] = srv;
}

ID3D11ShaderResourceView* Texture::EditorGet(const std::wstring& path)
{
	auto it = m_textureMap.find(path);

	if (it != m_textureMap.end())
	{
		/* 登録済み→SRVを返す */
		return it->second;
	}
	return nullptr;
}
