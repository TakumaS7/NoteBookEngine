/*===============================================================*/
//
//	createBranchButton.h(作成分岐ボタン管理)
// 
// 
// ---------------------------------- 2026/02/12 製作者：佐藤琢磨
//
/*===============================================================*/

#include "createBranchButton.h"

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

void CreateBranchButton::Set(const std::wstring& targetRavelName, const std::wstring& buttonText, const std::wstring& texturePath, XMFLOAT2& pos, XMFLOAT2& scale, const Vector3& textColor)
{
	/* ターゲットラベル名とテキスト、画像パスを取得 */
	m_targetRavelName = targetRavelName;
	m_text = buttonText;
	m_texturePath = texturePath;

	wchar_t buffer[256];
	swprintf(
		buffer, 256,
		L"座標:{%.1f, %.1f} サイズ:{%.1f, %.1f}",
		pos.x,
		pos.y,
		scale.x,
		scale.y
	);

	std::wstring charaTransformString = buffer;

	/* フレームネームとしてキャラクター名、ファイル名、ファイル名、トランスフォーム情報を表示させる */
	m_frameName = 
		L"ターゲット：" + m_targetRavelName +
		L"　　テキスト：" + m_text + 
		L"　　ファイル名：" + std::experimental::filesystem::path(m_texturePath).filename().wstring() +
		L"\n" +
		charaTransformString;

	/* 各トランスフォーム情報の代入 */
	m_buttonTransform.position = { pos.x, pos.y, 0.0f };
	m_buttonTransform.rotation = { 0.0f, 0.0f, 0.0f };
	m_buttonTransform.scale = { scale.x, scale.y, 0.0f };

	/* 文字色を取得 */
	m_textColor = textColor;

	/* セーブ・ロード用に保存 */
	m_setPosition = pos;
	m_setScale = scale;
}

std::wstring CreateBranchButton::GetTargetRavelName() const
{
	return m_targetRavelName;
}

std::wstring CreateBranchButton::GetButtonText() const
{
	return m_text;
}

std::wstring CreateBranchButton::GetTexturePath() const
{
	return m_texturePath;
}

Transform CreateBranchButton::GetTransform() const
{
	return m_buttonTransform;
}

Vector3 CreateBranchButton::GetTextColor() const
{
	return m_textColor;
}

void CreateBranchButton::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 450.0f, 50.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\CreateSystemFrame.png", true);
	AddComponent<SquareVertex>();
	AddComponent<TextComponent>()->Set(L"分岐ボタン", this->GetComponent<Transform>(), false, D2D1::ColorF::White, { -10.0f, -36.0f, 0.0f });
	AddComponent<TextComponent>()->Set(m_frameName.c_str(), this->GetComponent<Transform>(), false, D2D1::ColorF::Black, { 0.0f, 0.0f, 0.0f });
	AddComponent<MouseCollision>()->Set(this);
	AddComponent<ObjectDragDrop>()->Set(this);
	AddComponent<SetDeleteButton>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}
