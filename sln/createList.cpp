/*===============================================================*/
//
//	createList.cpp(エディタリスト管理)
// 
// 
// ---------------------------------- 2025/09/28 製作者：佐藤琢磨
//
/*===============================================================*/

#include <set>
#include "createList.h"
#include "manager.h"
#include "scene.h"

#include "createSpeakText.h"
#include "createBgTexture.h"
#include "createCharaAppearance.h"
#include "createCharaChange.h"
#include "createCharaExit.h"
#include "createBgm.h"
#include "createSe.h"
#include "createGameStop.h"
#include "createRavel.h"
#include "createBranchButton.h"
#include "createJump.h"
#include "createSpeakTextHidden.h"
#include "createSpeakTextDisplay.h"

#include "transform.h"
#include "sortingLayer.h"
#include "shader2D.h"
#include "transformDraw.h"
#include "textureComponent.h"
#include "squareVertex.h"
#include "mouseWheelScroll.h"
#include "mouseDragScroll.h"

void CreateList::Init()
{
	AddComponent<SortingLayer>()->Set(0);
	AddComponent<Transform>()->Set({ SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 450.0f, 300.0f, 0.0f });
	AddComponent<Shader2D>()->Set("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<TransformDraw>()->Set(this);
	AddComponent<TextureComponent>()->Set(L"asset\\texture\\white.png", true);
	AddComponent<SquareVertex>()->Set({ 1.0f, 1.0f, 1.0f }, 0.5f);
	AddComponent<MouseWheelScroll>()->Set(this->GetComponent<Transform>());
	AddComponent<MouseDragScroll>()->Set(this);

	for (auto component : GetComponentVector())
	{
		component->Init();
	}
}

/* テキストの生成 */
CreateSpeakText* CreateList::AddSpeakText(const std::wstring& name, const std::wstring& text)
{
	CreateSpeakText* speakText = Manager::GetScene()->AddGameObject<CreateSpeakText>(OBJECT_2D);
	speakText->SetName(name);
	speakText->SetText(text);
	speakText->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(speakText), ObjectType::SpeakText });

	SetScrollMinMax();

	return speakText;
}

/* テクスチャの生成 */
CreateBgTexture* CreateList::AddBgTexture(const std::wstring& textureName)
{
	CreateBgTexture* bgTexture = Manager::GetScene()->AddGameObject<CreateBgTexture>(OBJECT_2D);
	bgTexture->Set(textureName);
	bgTexture->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(bgTexture), ObjectType::BgTexture });

	SetScrollMinMax();

	return bgTexture;
}

/* キャラ登場を生成 */
CreateCharaAppearance* CreateList::AddCharaAppearance(const std::wstring& textureName, XMFLOAT2& pos, float& rotation, XMFLOAT2& scale, const std::wstring charaName)
{
	CreateCharaAppearance* charaAppearance = Manager::GetScene()->AddGameObject<CreateCharaAppearance>(OBJECT_2D);
	charaAppearance->Set(textureName, pos, rotation, scale, charaName);
	charaAppearance->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(charaAppearance), ObjectType::CharaAppearance });

	SetScrollMinMax();

	return charaAppearance;
}

CreateCharaChange* CreateList::AddCharaChange(const std::wstring& textureName, XMFLOAT2& pos, float& rotation, XMFLOAT2& scale, const std::wstring charaName)
{
	CreateCharaChange* charaChange = Manager::GetScene()->AddGameObject<CreateCharaChange>(OBJECT_2D);
	charaChange->Set(textureName, pos, rotation, scale, charaName);
	charaChange->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(charaChange), ObjectType::CharaChange });

	SetScrollMinMax();

	return charaChange;
}

CreateCharaExit* CreateList::AddCharaExit(const std::wstring& charaName)
{
	CreateCharaExit* charaExit = Manager::GetScene()->AddGameObject<CreateCharaExit>(OBJECT_2D);
	charaExit->Set(charaName);
	charaExit->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(charaExit), ObjectType::CharaExit });

	SetScrollMinMax();

	return charaExit;
}

CreateBgm* CreateList::AddBgm(const std::wstring& bgmName)
{
	CreateBgm* bgm = Manager::GetScene()->AddGameObject<CreateBgm>(OBJECT_2D);
	bgm->Set(bgmName);
	bgm->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(bgm), ObjectType::BGM });

	SetScrollMinMax();

	return bgm;
}

CreateSe* CreateList::AddSe(const std::wstring& seName)
{
	CreateSe* se = Manager::GetScene()->AddGameObject<CreateSe>(OBJECT_2D);
	se->Set(seName);
	se->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(se), ObjectType::SE });

	SetScrollMinMax();

	return se;
}

CreateGameStop* CreateList::AddGameStop()
{
	CreateGameStop* gameStop = Manager::GetScene()->AddGameObject<CreateGameStop>(OBJECT_2D);
	gameStop->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(gameStop), ObjectType::GameStop });

	SetScrollMinMax();

	return gameStop;
}

CreateRavel* CreateList::AddRavel(const std::wstring& ravelName)
{
	CreateRavel* ravel = Manager::GetScene()->AddGameObject<CreateRavel>(OBJECT_2D);
	ravel->Set(ravelName);
	ravel->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(ravel), ObjectType::Ravel });

	SetScrollMinMax();

	return ravel;
}

CreateBranchButton* CreateList::AddBranchButton(const std::wstring& targetRavelName, const std::wstring& buttonText, const std::wstring texturePath, XMFLOAT2& pos, XMFLOAT2& scale, const Vector3& textColor)
{
	CreateBranchButton* branchButton = Manager::GetScene()->AddGameObject<CreateBranchButton>(OBJECT_2D);
	branchButton->Set(targetRavelName, buttonText, texturePath, pos, scale, textColor);
	branchButton->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(branchButton), ObjectType::BranchButton });

	SetScrollMinMax();

	return branchButton;
}

CreateJump* CreateList::AddJump(const std::wstring& targetRavelName)
{
	CreateJump* jump = Manager::GetScene()->AddGameObject<CreateJump>(OBJECT_2D);
	jump->Set(targetRavelName);
	jump->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(jump), ObjectType::Jump });

	SetScrollMinMax();

	return jump;
}

CreateSpeakTextHidden* CreateList::AddSpeakTextHidden()
{
	CreateSpeakTextHidden* speakTextHidden = Manager::GetScene()->AddGameObject<CreateSpeakTextHidden>(OBJECT_2D);
	speakTextHidden->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(speakTextHidden), ObjectType::SpeakTextHidden });

	SetScrollMinMax();

	return speakTextHidden;
}

CreateSpeakTextDisplay* CreateList::AddSpeakTextDisplay()
{
	CreateSpeakTextDisplay* speakTextDisplay = Manager::GetScene()->AddGameObject<CreateSpeakTextDisplay>(OBJECT_2D);
	speakTextDisplay->Init();

	/* リストに登録 */
	m_listObjects.push_back({ std::move(speakTextDisplay), ObjectType::SpeakTextDisplay });

	SetScrollMinMax();

	return speakTextDisplay;
}

void CreateList::SetScrollMinMax()
{
	Transform* listTransform = this->GetComponent<Transform>();

	if (m_listObjects.size() <= 1) { return; }

	this->GetComponent<MouseWheelScroll>()->SetScrollMin(-listTransform->scale.y * m_listObjects.size());
	this->GetComponent<MouseWheelScroll>()->SetScrollMax(listTransform->scale.y * m_listObjects.size());
}

void CreateList::ChildMove()
{
	/* リストに合わせてリストに入っているものも動く */
	Transform* listTransform = GetComponent<Transform>();
	if (!listTransform) { return; }

	/* リストのTransformが動いた分だけ、listObjectも動かす */
	for (auto& obj : m_listObjects)
	{
		GameObject* listObject = dynamic_cast<GameObject*>(obj.listObject);
		if (!listObject) { continue; }

		Transform* transform = listObject->GetComponent<Transform>();
		if (!transform) { continue; }

		/* listTransformのYに合わせてオフセット */
		transform->position.y += (listTransform->position.y - m_prevListPosY);
	}

	m_prevListPosY = listTransform->position.y;
}

void CreateList::SetSequence()
{
	/* リストから削除 */
	ListObjectDestroy();

	/* リスト内に残っているものを再配置する */
	Transform* listTransform = this->GetComponent<Transform>();
	if (!listTransform) { return; }

	int index = 0;
	for (auto& obj : m_listObjects)
	{
		GameObject* listObject = dynamic_cast<GameObject*>(obj.listObject);
		if (!listObject) { continue; }

		Transform* transform = listObject->GetComponent<Transform>();
		if (!transform) { continue; }

		m_objWidth = listObject->GetComponent<Transform>()->scale.x;
		m_objHeight = listObject->GetComponent<Transform>()->scale.y;

		float x = listTransform->position.x;
		float y = listTransform->position.y -	// まず、リストのY座標から
			(listTransform->scale.y) +			// リストの高さ分、上に上げて
			(m_objHeight) +						// そこから、リストオブジェクトの高さ分、下に下げて
			(index * (m_objHeight * 2.0f));		// リストオブジェクトの個数分下へ下げる

		transform->Set(
			{ x, y, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ m_objWidth, m_objHeight, 0.0f }
		);

		index++; // 表示されるオブジェクトだけカウント
	}
}

void CreateList::ResizeList()
{
	Transform* listTransform = this->GetComponent<Transform>();
	if (!listTransform) return;

	const int defaultVisibleCount = 6;   // デフォルトで表示されるオブジェクト数
	const float defaultHeight = 50.0f * defaultVisibleCount;  // デフォルトの枠の高さ

	int count = static_cast<int>(m_listObjects.size());
	float newHeight = defaultHeight;	// 更新される高さ

	if (count > defaultVisibleCount)
	{
		int extra = count - defaultVisibleCount;
		newHeight += extra * (m_objHeight); // オブジェクト分追加
	}

	/* 枠の高さをセット */
	listTransform->scale.y = newHeight;

	/* オブジェクトの再配置 */
	SetSequence();
}

std::vector<CreateSpeakText*> CreateList::GetAllTexts() const
{
	std::vector<CreateSpeakText*> texts;
	texts.reserve(m_listObjects.size());

	for (auto& obj : m_listObjects)
	{
		/* ここでオブジェクトタイプを調べる */
		if (!obj.listObject) { continue; }
		if (obj.objectType != ObjectType::SpeakText) { continue; }

		CreateSpeakText* speakText = dynamic_cast<CreateSpeakText*>(obj.listObject);
		if (!speakText) { continue; }

		texts.push_back(speakText);
	}

	return texts;
}

std::vector<std::wstring> CreateList::GetAllBgTextures() const
{
	std::vector<std::wstring> textures;
	textures.reserve(m_listObjects.size());

	for (auto& obj : m_listObjects)
	{
		/* ここでオブジェクトタイプを調べる */
		if (!obj.listObject) { continue; }
		if (obj.objectType != ObjectType::BgTexture) { continue; }

		CreateBgTexture* bgTexture = dynamic_cast<CreateBgTexture*>(obj.listObject);
		if (!bgTexture) { continue; }

		textures.push_back(bgTexture->GetTexture());
	}

	return textures;
}

std::vector<std::wstring> CreateList::GetAllBgm() const
{
	std::vector<std::wstring> bgms;
	bgms.reserve(m_listObjects.size());

	for (auto& obj : m_listObjects)
	{
		/* ここでオブジェクトタイプを調べる */
		if (!obj.listObject) { continue; }
		if (obj.objectType != ObjectType::BGM) { continue; }

		CreateBgm* bgm = dynamic_cast<CreateBgm*>(obj.listObject);
		if (!bgm) { continue; }

		bgms.push_back(bgm->GetBgm());
	}

	return bgms;
}

