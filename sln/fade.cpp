/*===============================================================*/
//
//	fade.cpp(フェードシーン管理)
// 
// 
// ---------------------------------- 2025/09/25 製作者：佐藤琢磨
//
/*===============================================================*/

#include "fade.h"
#include "time.h"
#include "texture.h"
#include "load.h"
#include "loadingRowling.h"

/* static変数の実体化 */
ID3D11Buffer* Fade::m_VertexBuffer;
ID3D11VertexShader* Fade::m_VertexShader;
ID3D11PixelShader* Fade::m_PixelShader;
ID3D11InputLayout* Fade::m_VertexLayout;
ID3D11ShaderResourceView* Fade::m_Texture;
float Fade::m_Alpha;
float Fade::m_FadeSpeed;
float Fade::m_AddSpeed;
Fade::FadeState Fade::m_FadeState;
Fade::FadeSceneState Fade::m_FadeSceneState;

Fade::Fade()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	m_Texture = Texture::Load("asset\\texture\\LoadBG.png");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	/* メンバー変数の初期化 */
	m_Alpha = 0.0f;
	m_FadeSpeed = 0.1f;
	m_AddSpeed = 10.0f;

	/* ロード中回転する画像 */
	LoadingRowling::Init();
}

Fade::~Fade()
{
	/* ロード中回転する画像 */
	LoadingRowling::Uninit();

	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Fade::Update()
{
	switch (m_FadeState)
	{
	case NONE:
		break;
	case FADE_OUT:
		FadeOut();
		break;
	case FADE_LOAD_START:
		LoadStart();
		break;
	case FADE_LOADING:
		Loading();
		break;
	case FADE_IN:
		///* シーン分岐 */
		//if (m_FadeSceneState == TITLE) {
		//	Manager::SetScene<Title>();
		//}
		//else if (m_FadeSceneState == GAME) {
		//	Manager::SetScene<Game>();
		//}
		//else {
		//	Manager::SetScene<Result>();
		//}
		FadeIn();
		break;
	}

	/* ロード中回転する画像 */
	LoadingRowling::Update();
}

void Fade::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	trans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	//マテリアル設定
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, m_Alpha };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//頂点バッファー設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	/* ロード中回転する画像 */
	LoadingRowling::Draw(m_Alpha);
}

void Fade::FadeOut()
{
	/* 透明化解除 */
	m_Alpha += m_FadeSpeed * m_AddSpeed * Time::m_deltaTime;

	/* α値が1.0になったら、シーン遷移してフェードアウトへ */
	if (m_Alpha >= 1.0f)
	{
		m_Alpha = 1.0f;
		m_FadeState = FADE_LOAD_START;
	}
}

void Fade::LoadStart()
{
	/* シーン分岐 */
	if (m_FadeSceneState == TITLE) {
		Load::StartLoading(Load::LOAD_TITLE);
	}
	else if (m_FadeSceneState == CREATE_SCENE) {
		Load::StartLoading(Load::LOAD_CREATE_SCENE);
	}

	m_FadeState = FADE_LOADING;
}

void Fade::Loading()
{
	if (Load::GetLoadedFrag())
	{
		/* ロードが完了したらフェードイン */
		m_FadeState = FADE_IN;
	}
}

void Fade::FadeIn()
{
	/* 透明化開始 */
	m_Alpha += -m_FadeSpeed * m_AddSpeed * Time::m_deltaTime;

	/* α値が0,0になったら、何もしない状態へ移行 */
	if (m_Alpha <= 0.0f)
	{
		m_Alpha = 0.0f;
		m_FadeState = NONE;
	}
}
