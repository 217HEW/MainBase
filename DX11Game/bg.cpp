//**************************************************************
//
//	背景 [bg.cpp]
//	シーン毎に背景制御を行うプログラム
//
//--------------------------------------------------------------
//	製作者：小嶋悟
//--------------------------------------------------------------
//	開発履歴
//	2021/12/22	bg.hにシーンごとに背景画像を切り替える処理追加
//--------------------------------------------------------------
//	2021/12/2
//
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "bg.h"
#include "Texture.h"
#include "polygon.h"
#include "SceneManager.h"

//**************************************************************
// マクロ定義
//**************************************************************

#define PATH_BGTEXTURE	L"data/texture/sky001.jpg"
#define BG_POS_X		0.0f
#define BG_POS_Y		0.0f
#define BG_WIDTH		SCREEN_WIDTH
#define BG_HEIGHT		SCREEN_HEIGHT

//**************************************************************
// グローバル宣言
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static EScene g_eScene;	// シーン設定用変数

						
//**************************************************************
// 初期化
//**************************************************************
HRESULT InitBG()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);

	return hr;
}

// 終了処理
void UninitBG()
{
	// 背景テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

// 更新
void UpdateBG()
{
	// 何もしない
}

// 描画
void DrawBG()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);
}
