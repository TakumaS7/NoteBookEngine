/*===============================================================*/
//
//	createCharaAppearance.cpp(作成キャラ登場管理)
// 
// 
// ---------------------------------- 2026/02/02 製作者：佐藤琢磨
//
/*===============================================================*/

#include "createCharaAppearance.h"

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



void CreateCharaAppearance::Set(const std::wstring& texture, XMFLOAT2& pos, float& rotation, XMFLOAT2& scale, const std::wstring& charaName)
{
	/* テクスチャパスとキャラの名前を取得 */
	m_textureName = texture;
	m_charaName = charaName;

	wchar_t buffer[256];
	swprintf(
		buffer, 256,
		L"座標:{%.2f, %.2f} 回転:%.2f サイズ:{%.2f, %.2f}",
		pos.x,
		pos.y,
		rotation,
		scale.x,
		scale.y
	);

	std::wstring charaTransformString = buffer;

	/* フレームネームとしてキャラクター名、ファイル名、トランスフォーム情報を表示させる */
	m_frameName = m_charaName + L"　　ファイル名：" + std::experimental::filesystem::path(texture).filename().wstring() + L"\n" + charaTransformString;

	/* 各トランスフォーム情報の代入 */
	m_charaTransform.position = { pos.x, pos.y, 0.0f };
	m_charaTransform.rotation = { 0.0f, 0.0f, XMConvertToRadians(rotation) };
	m_charaTransform.scale = { scale.x, scale.y, 0.0f };

	/* セーブ・ロード用に保存 */
	m_setPosition = pos;
	m_setRotation = rotation;
	m_setScale = scale;
}



std::wstring CreateCharaAppearance::GetTexture() const
{
	return m_textureName;
}



Transform CreateCharaAppearance::GetCharaTransform() const
{
	return m_charaTransform;
}



std::wstring CreateCharaAppearance::GetCharaName() const
{
	return m_charaName;
}



void CreateCharaAppearance::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 450.0f, 50.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\CreateCharaTextureFrame.png", true);
	AddComponent<SquareVertex>();
	AddComponent<TextComponent>()->Set(L"キャラクター登場", this->GetComponent<Transform>(), false, D2D1::ColorF::White, {-10.0f, -36.0f, 0.0f});
	AddComponent<TextComponent>()->Set(m_frameName.c_str(), this->GetComponent<Transform>(), false, D2D1::ColorF::Black, { 0.0f, 0.0f, 0.0f });
	AddComponent<MouseCollision>()->Set(this);
	AddComponent<ObjectDragDrop>()->Set(this);
	AddComponent<SetDeleteButton>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
