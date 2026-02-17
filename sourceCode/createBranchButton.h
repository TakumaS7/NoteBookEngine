/*===============================================================*/
//
//	createBranchButton.h(作成分岐ボタン管理)
// 
// 
// ---------------------------------- 2026/02/12 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_BRANCH_BUTTON_H
#define _CREATE_BRANCH_BUTTON_H
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "gameObject.h"
#include <string>
#include <experimental/filesystem>
#include "transform.h"
#include "vector3.h"

class CreateBranchButton : public GameObject
{
private:
	std::wstring m_targetRavelName = L"ラベル名";		// 指定したラベルまでとぶ
	Vector3 m_textColor = { 0.0f, 0.0f, 0.0f };			// テキストの色
	std::wstring m_text = L"ボタンテキスト";			// ボタンに表示されるテキスト
	Transform m_buttonTransform{};

	std::wstring m_frameName = L"フレーム表示名";	// ターゲットラベル名、ボタンテキスト、座標、回転、大きさ

	std::wstring m_texturePath = L"画像パス";

	XMFLOAT2 m_setPosition = { 0.0f, 0.0f };	// エディタに登録された座標
	XMFLOAT2 m_setScale = { 0.0f, 0.0f };		// エディタに登録された大きさ

public:
	void Set(const std::wstring& targetRavelName, const std::wstring& buttonText, const std::wstring& texturePath, XMFLOAT2& pos, XMFLOAT2& scale, const Vector3& textColor);

	/* 各ゲッター */
	std::wstring GetTargetRavelName() const;	// タ－ゲットラベル名
	std::wstring GetButtonText() const;			// ボタンに表示されるテキスト
	std::wstring GetTexturePath() const;		// 画像パス
	Transform GetTransform() const;				// ボタンのトランスフォーム
	Vector3 GetTextColor() const;				// テキストの色

	XMFLOAT2 GetPosition() const { return m_setPosition; }
	XMFLOAT2 GetScale() const { return m_setScale; }

	void Init() override;
	void Uninit() override
	{
		for (auto component : GetComponentVector())
		{
			component->Uninit();
		}
	}

	void Update() override
	{
		for (auto component : GetComponentVector())
		{
			component->Update();
		}
	}

	void Draw() override
	{
		for (auto component : GetComponentVector())
		{
			component->Draw();
		}
	}
};

#endif //_CREATE_BRANCH_BUTTON_H