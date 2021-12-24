//**************************************************************
//
//	SceneTitle.cpp
//	タイトルシーン中身
//
//--------------------------------------------------------------
//	製作者：上月大地
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/06	小嶋悟君のプログラムを元に作成	
//--------------------------------------------------------------
//	2021/12/21	GetDevice関数格納用ポインタ変数を作成し、適所の変更
//				フェード中に別のフェード処理をしないよう補正
//				ポリゴン4大処理消去
//	変更者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/21	フェード処理が出来るようにDraw処理にZバッファを
//				追加しました。97〜106行
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/22	タイトルからゲームに行く正規ボタンの実装
//				「Enter」「Space」
//	編集者：柴山凜太郎
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "SceneTitle.h"
#include "input.h"
#include "SceneManager.h"
#include "polygon.h"
#include "Fade.h"
#include "Texture.h"
#include "Sound.h"
//**************************************************************
// マクロ定義
//**************************************************************
#define PATH_BGTEXTURE "data/texture/Title.png"
#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT

//**************************************************************
// グローバル変数
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitTitle()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if (FAILED(hr))
		return hr;

	// 中身はまだない
	// BGM再生開始
	//CSound::Play(BGM_001);
	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitTitle()
{
	// 中身無し
	// BGM再生停止
	CSound::Stop(BGM_001);
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateTitle()
{
	// キー入力でシーン遷移
	if (GetFadeState() == FADE_NONE)
	{
		if (GetKeyRelease(VK_1))
		{
			StartFadeOut(SCENE_TITLE);
		}
		else if (GetKeyRelease(VK_2) || GetKeyRelease(VK_SPACE) || GetKeyRelease(VK_RETURN))
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
		else if (GetKeyRelease(VK_5))
		{
			StartFadeOut(SCENE_GAMEOVER);
		}
		else if (GetKeyRelease(VK_6))
		{
			StartFadeOut(SCENE_GAMECLEAR);
		}
		
	}
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawTitle()
{
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);
	
	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
}