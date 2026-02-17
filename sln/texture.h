/*===============================================================*/
//
//	texture.h(テクスチャ処理)
// 
// 
// ---------------------------------- 2025/06/11 製作者：佐藤琢磨
//
/*===============================================================*/
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <unordered_map>
#include <string> 
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include <cstdlib>

class Texture 
{
private:
	static std::unordered_map<std::string, ID3D11ShaderResourceView*> m_TexturePool;
	static wchar_t wFileName[512];
	static size_t convertedChars;

	static std::unordered_map<std::wstring,
		ID3D11ShaderResourceView*> m_textureMap;

public:
	static ID3D11ShaderResourceView* Load(const char* FileName);

	static void EditorLoad(const std::wstring& path);
	static ID3D11ShaderResourceView* EditorGet(const std::wstring& path);
};

#endif //_TEXTURE_H