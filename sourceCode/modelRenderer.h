/*===============================================================*/
//
//	modelRenderer.h(モデル描画処理)
// 
// 
// ---------------------------------- 2025/09/25 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _MODEL_RENDERER_H
#define _MODEL_RENDERER_H

// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// モデル構造体
struct MODEL_OBJ
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};

struct MODEL
{
	ID3D11Buffer*	VertexBuffer;
	ID3D11Buffer*	IndexBuffer;

	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};


#include "modelComponent.h"
#include <string>
#include <unordered_map>


class ModelRenderer : public ModelComponent
{
private:

	static std::unordered_map<std::string, MODEL*> m_ModelPool;

	static void LoadModel(const char *FileName, MODEL *Model);
	static void LoadObj( const char *FileName, MODEL_OBJ *ModelObj );
	static void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum );

	MODEL* m_Model{};

public:

	static void Preload( const char *FileName );
	static void UnloadAll();


	using ModelComponent::ModelComponent;

	void Load( const char *FileName );
	void Draw() override;

};

#endif //_MODEL_RENDERER_H