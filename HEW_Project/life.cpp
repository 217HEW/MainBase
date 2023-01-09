//**************************************************************
//
//	Life.cpp
//	ライフ処理
//
//--------------------------------------------------------------
//	製作者：澤村瑠人
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "Texture.h"
#include "life.h"
#include "polygon.h"

//**************************************************************
// マクロ定義
//**************************************************************

// ライフ枠
#define PATH_LIFEFRAMETEXTURE L"data/texture/frame_life.png"	//ライフ枠テクスチャ
#define LIFEFRAME_WIDTH (200.0f)		// ライフ枠の横幅
#define LIFEFRAME_HEIGHT (80.0f)		// ライフ枠の縦幅
#define LIFEFRAME_POS_X ((SCREEN_WIDTH-LIFEFRAME_WIDTH)*-0.5f)	// ライフ枠横軸座標
#define LIFEFRAME_POS_Y ((SCREEN_HEIGHT-LIFEFRAME_HEIGHT)*0.5f)	// ライフ枠縦軸座標

// ライフアイコン
#define PATH_LIFETEXTURE L"data/texture/hart.tga"	//ライフアイコンテクスチャ
#define MAX_LIFE (1)		// ライフ最大値
#define LIFE_SIZE_X (40.0f)	// 横テクスチャサイズ
#define LIFE_SIZE_Y (40.0f)	// 縦テクスチャサイズ
#define LIFE_POS_X (LIFEFRAME_POS_X-LIFE_SIZE_X*(MAX_LIFE*0.5f-0.5f))	// ライフ横軸座標
#define LIFE_POS_Y (LIFEFRAME_POS_Y-6.0f)		// ライフ縦軸座標

//**************************************************************
// 列挙体宣言
//**************************************************************
enum TEXTURE
{
	TEX_FRAME = 0,	// ライフ枠
	TEX_LIFE,		// ライフアイコン

	MAX_TEXTURE		// ライフで使う総テクスチャ数
};


//**************************************************************
//構造体定義
//**************************************************************




//**************************************************************
// グローバル変数
//**************************************************************



static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];	// テクスチャ情報
static int g_nLife;	// 体力

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitLife()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	LPCWSTR pszTexFName[] = { PATH_LIFEFRAMETEXTURE, PATH_LIFETEXTURE };
	for (int i = 0; i < MAX_TEXTURE; ++i) 
	{
		hr = CreateTextureFromFile(pDevice, pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) 
		{
			return hr;
		}
	}
	// 変数初期化
	g_nLife = MAX_LIFE;
	
	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitLife()
{
	// テクスチャ解放
	for (int i = 0; i < MAX_TEXTURE; ++i) 
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
}
//**************************************************************
// 更新処理
//**************************************************************
void UpdateLife()
{

}
//**************************************************************
// 描画処理
//**************************************************************
void DrawLife()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	// テクスチャと同色に設定
	SetPolygonColor(0.929412f, 0.152941f, 0.564706f);

	// ライフ枠表示
	SetPolygonSize(LIFEFRAME_WIDTH, LIFEFRAME_HEIGHT);
	SetPolygonPos(LIFEFRAME_POS_X, LIFEFRAME_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_FRAME]);
	DrawPolygon(pDC);

	// 元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);

	// ライフ表示
	SetPolygonSize(LIFE_SIZE_X, LIFE_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_LIFE]);

	//Lifeをアニメーションさせるには、UV,FramSizeが必要
	for(int i = 0; i < g_nLife; ++i) 
	{
		SetPolygonPos(LIFE_POS_X + i * LIFE_SIZE_X, LIFE_POS_Y);
		DrawPolygon(pDC);
	}
}
//**************************************************************
// ライフ加算
//**************************************************************
void AddLife(int nLife)
{
	g_nLife += nLife;
	if (g_nLife > MAX_LIFE) 
	{
		g_nLife = MAX_LIFE;
	}
}
//**************************************************************
// ライフ減算
//**************************************************************
void DelLife(int nLife)
{
	g_nLife -= nLife;
	if (g_nLife < 0) 
	{
		g_nLife = 0;
	}
}
//**************************************************************
// ライフ取得
//**************************************************************
int GetLife()
{
	return g_nLife;
}