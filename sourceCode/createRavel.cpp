/*===============================================================*/
//
//	createRavel.cpp(作成ラベル管理)
// 
// 
// ---------------------------------- 2026/02/12 製作者：佐藤琢磨
//
/*===============================================================*/
//	分岐で使用
//
/*===============================================================*/

#include "createRavel.h"

#include "sortingLayer.h"
#include "transform.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"
#include "textComponent.h"
#include "mouseCollision.h"
#include "objectDragDrop.h"
#include "setDeleteButton.h"

void CreateRavel::Set(const std::wstring& ravelName)
{
	m_ravelName = ravelName;

	/* フレームに表示するテキストを指定 */
	m_frameName = L"ラベル名：" + m_ravelName;
}

std::wstring CreateRavel::GetRavelName() const
{
	return m_ravelName;
}

void CreateRavel::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 450.0f, 50.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\CreateSystemFrame.png", true);
	AddComponent<SquareVertex>();
	AddComponent<TextComponent>()->Set(L"ラベル", this->GetComponent<Transform>(), false, D2D1::ColorF::White, { -10.0f, -36.0f, 0.0f });
	AddComponent<TextComponent>()->Set(m_frameName.c_str(), this->GetComponent<Transform>(), false, D2D1::ColorF::Black, { 0.0f, 0.0f, 0.0f });
	AddComponent<MouseCollision>()->Set(this);
	AddComponent<ObjectDragDrop>()->Set(this);
	AddComponent<SetDeleteButton>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
