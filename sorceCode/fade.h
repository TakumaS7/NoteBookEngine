/*===============================================================*/
//
//	fade.h(フェードシーン管理)
// 
// 
// ---------------------------------- 2025/09/25 製作者：佐藤琢磨
//
/*===============================================================*/

#ifndef _FADE_H
#define _FADE_H

#include "main.h"
#include "renderer.h"
#include "vector3.h"
#include "manager.h"
#include "title.h"
#include "game.h"
#include "result.h"

class Fade
{
private:
	/* フェード状態遷移 */
	enum FadeState
	{
		NONE,
		FADE_OUT,
		FADE_LOAD_START,
		FADE_LOADING,
		FADE_IN,
	};

	static ID3D11Buffer* m_VertexBuffer;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11ShaderResourceView* m_Texture;

	static float m_Alpha;			// ポリゴンα値
	static float m_FadeSpeed;		// フェードスピード
	static float m_AddSpeed;		// スピード倍率
	static FadeState m_FadeState;	// フェード状態格納変数

public:
	Fade();		// デフォルトコンストラクタ
	~Fade();	// デストラクタ

	/* シーン遷移管理enum */
	enum FadeSceneState
	{
		TITLE,
		GAME,
		RESULT,
		CREATE_SCENE,
		SCENE_MAX,
	};

	static FadeSceneState m_FadeSceneState;	// シーン遷移管理enum格納変数

	static void Update();
	static void Draw();
	static void FadeOut();		// フェードアウト関数
	static void LoadStart();	// ロードし始める
	static void Loading();		// ロード中関数
	static void FadeIn();		// フェードイン関数

	/* フェード実行関数 */
	static void SetFadeScene(FadeSceneState state) {
		m_FadeState = FADE_OUT;
		m_FadeSceneState = state;
	}
};

#endif //_FADE_H