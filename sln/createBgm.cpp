/*===============================================================*/
//
//	createBgm.cpp(çÏê¨BGMä«óù)
// 
// 
// ---------------------------------- 2026/01/29 êªçÏé“ÅFç≤ì°ëÙñÅ
//
/*===============================================================*/

#include "createBgm.h"

#include "sortingLayer.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"
#include "textComponent.h"
#include "mouseCollision.h"
#include "objectDragDrop.h"
#include "setDeleteButton.h"

void CreateBgm::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 450.0f, 50.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\CreateBGMFrame.png", true);
	AddComponent<SquareVertex>();
	AddComponent<TextComponent>()->Set(L"BGMÇó¨Ç∑", this->GetComponent<Transform>(), false, D2D1::ColorF::White, { -10.0f, -36.0f, 0.0f });
	AddComponent<TextComponent>()->Set(m_frameName.c_str(), this->GetComponent<Transform>(), false, D2D1::ColorF::Black, { 0.0f, 0.0f, 0.0f });
	AddComponent<MouseCollision>()->Set(this);
	AddComponent<ObjectDragDrop>()->Set(this);
	AddComponent<SetDeleteButton>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}

std::wstring CreateBgm::GetBgm() const
{
	return m_bgmName;
}
