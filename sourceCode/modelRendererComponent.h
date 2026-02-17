/*===============================================================*/
//
//	modelRendererComponent.h(モデル描画コンポーネント)
// 
// 
// ---------------------------------- 2025/07/15 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _MODEL_RENDERER_COMPONENT_H
#define _MODEL_RENDERER_COMPONENT_H

#include "component.h"
#include "modelRenderer.h"
#include <string>

class ModelRendererComponent : public Component
{
private:
	std::string m_fileName;
	ModelRenderer* m_ModelRenderer;

public:
	void Set(const std::string& fileName) { m_fileName = fileName; }

	void Init() override
	{
		m_ModelRenderer = new ModelRenderer();
		m_ModelRenderer->Load(m_fileName.c_str());
	}

	void Uninit() override 
	{
		delete m_ModelRenderer;
	}

	void Update() override {}
	void Draw() override 
	{
		m_ModelRenderer->Draw();
	}
};

#endif //_MODEL_RENDERER_COMPONENT_H