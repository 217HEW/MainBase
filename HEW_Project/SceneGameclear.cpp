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
//　2022/1/31	クリア時ポーズ制作中
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
#include "ClearPause.h"
#include "Sound.h"
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

int g_nNowScene;
bool ClearFlag;//trueだとCLEAR
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
	
	//g_nNowScene = GetScene();
	//g_nNowScene++;

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
		if (GetKeyRelease(VK_1) || GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE))
		{

			//[ENTER]が押された?
			//if (GetKeyTrigger(VK_RETURN))
			//{
				//選択中のメニュー項目により分岐
				switch (GetC_PauseMenu())
				{
				case C_PAUSE_MENU_NEXTSTAGE:	// ネクステージ
					StartFadeOut(g_nNowScene);
					//g_bC_Pause = false;
					CSound::SetVolume(BGM_GAME000, 0.1f);
					CSound::SetPlayVol(SE_CANCEL, 0.1f); // キャンセル
					//CSound::Resume();
					break;
				case C_PAUSE_MENU_SELECT:		// セレクト画面
					StartFadeOut(SCENE_SELECT);
					CSound::SetPlayVol(SE_SELECT, 0.1f); // セレクト
					break;
				case C_PAUSE_MENU_QUIT:		// ゲームを辞める
					StartFadeOut(SCENE_TITLE);
					CSound::SetPlayVol(SE_SELECT, 0.1f); // セレクト
					break;
				}
			//}
		

			StartFadeOut(SCENE_TITLE);
		}
		/*else if (GetKeyRelease(VK_2))
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
		}*/
	//	else if (GetKeyRelease(VK_5))
	//	{
	//		StartFadeOut(SCENE_AREA_DEBUG);
	//	}
		/*else if (GetKeyRelease(VK_6))
		{
			StartFadeOut(SCENE_GAMEOVER);
		}
		else if (GetKeyRelease(VK_7))
		{
			StartFadeOut(SCENE_GAMECLEAR);
		}*/
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

	//if (ClearFlag == true)
	//{
		DrawC_Pause();
	//}

}
void GameclearFlag()
{
	ClearFlag = true;
}
