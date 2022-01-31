//**************************************************************
//
//	SceneGameclear.cpp
//	ゲームクリアシーン
//
//--------------------------------------------------------------
//	製作者：小嶋悟
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/22	ゲームクリアシーンの実装
//	編集者：小嶋悟
//				ゲームオーバーからタイトルに行く正規ボタンの実装
//				「Enter」「Space」
//				コメントの編集&追加
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/28	デバッグ操作で遷移できるシーンの追加(AREA2,3,BOSS)
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//　2022/1/26	コンテニューか終るかの処理制作開始
//	編集者：澤村瑠人
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

//**************************************************************
// マクロ定義
//**************************************************************
#define PATH_BGTEXTURE "data/texture/Gameclear.png"		// ゲームクリアテクスチャ
#define BG_POS_X (0.0f)				// 背景座標X
#define BG_POS_Y (0.0f)				// 背景座標Y
#define BG_WIDTH (SCREEN_WIDTH)		// 背景横幅
#define BG_HEIGHT (SCREEN_HEIGHT)	// 背景縦幅
//**************************************************************
// グローバル変数
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;	// テクスチャ用変数

int SceneClearFlag=0;//シーンクリアフラグ
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

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitGameclear()
{
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
		if (GetKeyRelease(VK_1))// || GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE))
		{
			StartFadeOut(SCENE_TITLE);
		}
		else if (GetKeyRelease(VK_2))
		{
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
	//	else if (GetKeyRelease(VK_5))
	//	{
	//		StartFadeOut(SCENE_AREA_DEBUG);
	//	}
		else if (GetKeyRelease(VK_6))
		{
			StartFadeOut(SCENE_GAMEOVER);
		}
		else if (GetKeyRelease(VK_7))
		{
			StartFadeOut(SCENE_GAMECLEAR);
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
void GameclearFlag()
{
	switch (GetScene())
	{
	case SCENE_GAME:  // ゲーム画面エリア1
		SceneClearFlag = 1;
		break;
	case SCENE_AREA2: // ゲーム画面エリア2
		SceneClearFlag = 2;
		break;
	case SCENE_AREA3: // ゲーム画面エリア3
		SceneClearFlag = 3;
		break;
	case SCENE_AREA4: // ゲーム画面エリア4
		SceneClearFlag = 4;
		break;
	case SCENE_AREA5: // ゲーム画面エリア5
		SceneClearFlag = 5;
		break;
	case SCENE_AREA6: // ゲーム画面エリア6
		SceneClearFlag = 6;
		break;
	case SCENE_AREA7: // ゲーム画面エリア7
		SceneClearFlag = 7;
		break;
	case SCENE_AREA8: // ゲーム画面エリア8
		SceneClearFlag = 8;
		break;
	case SCENE_AREA9: // ゲーム画面エリア9
		SceneClearFlag = 9;
		break;
	case SCENE_AREA10: // ゲーム画面エリア10
		SceneClearFlag = 10;
		break;
	}
	StartFadeOut(SCENE_GAMECLEAR);
}
