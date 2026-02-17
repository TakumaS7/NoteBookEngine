/*===============================================================*/
//
//	createCharaAppearance.h(作成キャラ登場管理)
// 
// 
// ---------------------------------- 2026/02/02 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _CREATE_CHARA_APPEARANCE_H
#define _CREATE_CHARA_APPEARANCE_H
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "gameObject.h"
#include <string>
#include <experimental/filesystem>
#include "transform.h"

class CreateCharaAppearance : public GameObject
{
private:
	std::wstring m_textureName = L"キャラ画像名";	// パス全部
	std::wstring m_frameName = L"フレーム表示名";	// ファイル名と座標、回転、大きさと、キャラの名前
	std::wstring m_charaName = L"キャラの名前";		// 自分でつけたキャラの名前
	Transform m_charaTransform{};

	XMFLOAT2 m_setPosition = { 0.0f, 0.0f };	// エディタに登録された座標
	float m_setRotation = 0.0f;					// エディタに登録された回転（度）
	XMFLOAT2 m_setScale = { 0.0f, 0.0f };		// エディタに登録された大きさ

public:
	void Set(const std::wstring& texture, XMFLOAT2& pos, float& rotation, XMFLOAT2& scale, const std::wstring& charaName);

	/* 各ゲッター */
	std::wstring GetTexture() const;		// キャラテクスチャのパス
	Transform GetCharaTransform() const;	// キャラのトランスフォーム
	std::wstring GetCharaName() const;		// キャラの名前

	XMFLOAT2 GetPosition() const { return m_setPosition; }
	float GetRotation() const { return m_setRotation; }
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

#endif //_CREATE_CHARA_APPEARANCE_H