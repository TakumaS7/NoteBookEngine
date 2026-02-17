/*===============================================================*/
//
//	loadingRowling.h(ƒ[ƒh’†‰ñ‚é‰æ‘œˆ—)
// 
// 
// ---------------------------------- 2025/09/25 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _LOADING_ROWLING_H
#define _LOADING_ROWLING_H

#include "main.h"
#include "renderer.h"
#include "manager.h"

class LoadingRowling
{
private:
	static ID3D11Buffer* m_VertexBuffer;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11ShaderResourceView* m_Texture;

	static float m_rotation;	//‰ñ“]•Ï”

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw(float alpha);
};

#endif //_LOADING_ROWLING_H