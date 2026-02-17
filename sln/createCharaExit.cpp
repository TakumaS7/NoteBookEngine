/*===============================================================*/
//
//	createCharaExit.cpp(作成キャラ退場管理)
// 
// 
// ---------------------------------- 2026/02/011 製作者：佐藤琢磨
//
/*===============================================================*/

#include "createCharaExit.h"

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

void CreateCharaExit::Set(const std::wstring& charaName)
{
	m_charaName = charaName;

	m_frameName = m_charaName;
}

std::wstring CreateCharaExit::GetCharaName() const
{
	return m_charaName;
}

void CreateCharaExit::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 450.0f, 50.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\CreateCharaTextureFrame.png", true);
	AddComponent<SquareVertex>();
	AddComponent<TextComponent>()->Set(L"キャラクター退場", this->GetComponent<Transform>(), false, D2D1::ColorF::White, { -10.0f, -36.0f, 0.0f });
	AddComponent<TextComponent>()->Set(m_frameName.c_str(), this->GetComponent<Transform>(), false, D2D1::ColorF::Black, { 0.0f, 0.0f, 0.0f });
	AddComponent<MouseCollision>()->Set(this);
	AddComponent<ObjectDragDrop>()->Set(this);
	AddComponent<SetDeleteButton>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
