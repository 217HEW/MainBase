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
//	開発履歴
//	2021/12/06	小嶋悟君のプログラムを元に作成
//--------------------------------------------------------------
//	2021/12/21	GetDevice関数格納用ポインタ変数を作成し、適所の変更
//				フェード中に別のフェード処理をしないよう補正
//				ポリゴン4大処理消去(描画処理以外)
//	変更者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/22	コメントの追加&編集
//				フェードインの挙動がおかしかったためDraw関数に
//				Zバッファの処理を追加
//				ゲームオーバーからタイトルに行く正規ボタンの実装
//				「Enter」「Space」
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/28	デバッグ操作で遷移できるシーンの追加(AREA2,3,BOSS)
//	編集者：柴山凜太郎
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

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitGameover()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateGameover()
{
	// フェード処理していなかったら
	if (GetFadeState() == FADE_NONE)
	{
		if (GetKeyRelease(VK_1) || GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE))
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
		else if (GetKeyRelease(VK_5))
		{
			StartFadeOut(SCENE_AREA_BOSS);
		}
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