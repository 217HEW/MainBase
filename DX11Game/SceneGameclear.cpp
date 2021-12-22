//**************************************************************
//
//	SceneGameclear.cpp
//	ゲームオーバーシーン
//
//--------------------------------------------------------------
//	製作者：小嶋悟
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/06	小嶋悟君のプログラムを元に作成
//--------------------------------------------------------------
//	2021/12/21	GetDevice関数格納用ポインタ変数を作成し、適所の変更
//				フェード中に別のフェード処理をしないよう補正
//				ポリゴン4大処理消去(描画処理以外)
//	変更者：柴山凜太郎
//				
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "SceneGameclear.h"
#include "input.h"
#include "SceneManager.h"
#include "polygon.h"
#include "Fade.h"
#include "Texture.h"
// マクロ定義
#define PATH_BGTEXTURE "data/texture/Gameclear.png"
#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT
// グローバル変数
static ID3D11ShaderResourceView* g_pTexture;
//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitGameclear()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

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
void UninitGameclear()
{
	// 中身無し
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateGameclear()
{
	if (GetFadeState() == FADE_NONE)
	{
		if (GetKeyRelease(VK_1))
		{
			StartFadeOut(SCENE_TITLE);
		}
		else if (GetKeyRelease(VK_2))
		{
			StartFadeOut(SCENE_GAME);
		}
		else if (GetKeyRelease(VK_3))
		{
			StartFadeOut(SCENE_GAMEOVER);
		}
	}

	// ポリゴン表示更新
	//UpdatePolygon();

}

//**************************************************************
// 描画処理
//**************************************************************
void DrawGameclear()
{
	// 特になし
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

}