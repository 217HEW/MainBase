//**************************************************************
//
//	SceneGameover.cpp
//	タイトルシーン中身
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//	開発履歴
//	2021/12/06	小嶋悟君のプログラムを元に作成	
//
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "SceneGameover.h"
#include "input.h"
#include "SceneManager.h"
#include "polygon.h"
#include "Fade.h"
#include "Texture.h"
// マクロ定義
#define PATH_BGTEXTURE "data/texture/Gameover.png"
#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT
// グローバル変数
static ID3D11ShaderResourceView* g_pTexture;
//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitGameover()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// ポリゴン表示初期化
	hr = InitPolygon(GetDevice());
	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if (FAILED(hr))
		return hr;

	// 中身はまだない

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitGameover()
{
	// 中身無し
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
		// ポリゴン表示終了処理
	UninitPolygon();
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateGameover()
{
	if (GetKeyRelease(VK_2))
	{
		StartFadeOut(SCENE_TITLE);
	}
	// ポリゴン表示更新
	UpdatePolygon();

}

//**************************************************************
// 描画処理
//**************************************************************
void DrawGameover()
{
	// 特になし
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);
	
}