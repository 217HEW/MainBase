//**************************************************************
//
//	SceneGameover.cpp
//	ゲームオーバーシーン
//
//--------------------------------------------------------------
//	製作者：小嶋悟
//--------------------------------------------------------------
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
#include "Sound.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define PATH_BGTEXTURE "data/texture/Gameover.png"	//ゲームオーバーテクスチャ
#define BG_POS_X (0.0f)	// シーン背景の横軸座標
#define BG_POS_Y (0.0f)	// シーン背景の横軸座標
#define BG_WIDTH SCREEN_WIDTH	// テクスチャの横幅
#define BG_HEIGHT SCREEN_HEIGHT	// テクスチャの縦幅
//**************************************************************
// グローバル変数
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;	// テクスチャ用変数
static DWORD	Joycon;		// コントローラー情報
//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitGameover()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if (FAILED(hr))
		return hr;

	CSound::SetPlayVol(SE_GAMEOVER, 0.1f);

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitGameover()
{
	CSound::Stop(SE_GAMEOVER);
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateGameover()
{
	// コントローラー情報
	GetJoyState(Joycon);

	// フェード処理していなかったら
	if (GetFadeState() == FADE_NONE)
	{
		if (GetKeyRelease(VK_1) || GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE)||GetJoyTrigger(Joycon, JOYSTICKID1))
		{
			// タイトルへ
			StartFadeOut(SCENE_TITLE);
		}
		else if (GetKeyRelease(VK_2))
		{
			// ゲームシーンへ
			StartFadeOut(SCENE_GAME);
		}
		else if (GetKeyRelease(VK_3))
		{
			StartFadeOut(SCENE_AREA2);
		}
		else if (GetKeyRelease(VK_4))
		{
			StartFadeOut(SCENE_AREA3);
		}
		//else if (GetKeyRelease(VK_5))
		//{
		//	StartFadeOut(SCENE_AREA_DEBUG);
		//}
		else if (GetKeyRelease(VK_6))
		{
			StartFadeOut(SCENE_GAMEOVER);
		}
		else if (GetKeyRelease(VK_7))
		{
			StartFadeOut(SCENE_GAMECLEAR);
		}
	}
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawGameover()
{
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);
	
}