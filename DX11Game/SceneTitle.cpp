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
//				追加しました。97～106行
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/22	タイトルからゲームに行く正規ボタンの実装
//				「Enter」「Space」
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/27	タイトルBGMを追加しました。音量がとても大きいので
//				手動で下げています。
//				セレクト音も入っています
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/28	デバッグ操作で遷移できるシーンの追加(AREA2,3,BOSS)
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/1/5	コントローラーでもシーン遷移出来るようにしました
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/1/22	「OOを押して下さい！」を追加できるようにしました
//				画像入れれば描画出来ます
//	編集者：上月大地
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
// 背景
#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT

// 決定アイコン
#define ENTER_WIDTH	 550
#define ENTER_HEIGHT 80
#define ENTER_POS_X	 0.0f
#define ENTER_POS_Y	 (SCREEN_HEIGHT-ENTER_HEIGHT) * -0.5f

#define BLINK_TIMER 60
#define BLINK_START_TIMER 5

//**************************************************************
// グローバル変数
//**************************************************************

// 画像ファイルネーム
static LPCWSTR g_pszTexFName[TITLE_TEX_MAX] = {
 L"data/texture/Title.png",			// タイトル背景テクスチャ
 L"data/texture/Title.png",			// 決定アイコンテクスチャ
};

static ID3D11ShaderResourceView* g_pTexture[TITLE_TEX_MAX];
static DWORD	Joycon;		// コントローラー情報

// 点滅タイマー用
static int g_nBlink;
static bool g_bStart;


//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitTitle()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// 複数のテクスチャ読込
	for (int i = 0; i < TITLE_TEX_MAX; ++i)
	{
		hr = CreateTextureFromFile(pDevice, g_pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	// 変数初期化
	g_nBlink = BLINK_TIMER;
	g_bStart = false;

	// タイトルBGM再生
	CSound::Play(BGM_TITLE);

	// 音量が大きいので少し下げる
	CSound::SetVolume(BGM_TITLE, 0.01f);	// 下げたい音源と下げる量(多分1.0fが最大です)

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitTitle()
{
	// タイトルBGM終了
	CSound::Stop(BGM_TITLE);

	// 複数のテクスチャ解放
	for (int i = 0; i < TITLE_TEX_MAX; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateTitle()
{
	// コントローラー情報
	GetJoyState(Joycon);

	// 点滅タイマー更新
	--g_nBlink;
	if (g_nBlink <= 0)
	{
		g_nBlink = (g_bStart) ? BLINK_START_TIMER : BLINK_TIMER;
	}

	// キー入力でシーン遷移
	if (GetFadeState() == FADE_NONE)
	{
		if (GetKeyRelease(VK_T))
		{
			StartFadeOut(SCENE_TITLE);
		}
		else if (GetKeyRelease(VK_1) || GetKeyRelease(VK_SPACE) || GetKeyRelease(VK_RETURN)|| GetJoyTrigger(Joycon, JOYSTICKID1))
		{
			g_bStart = true;
			g_nBlink = BLINK_START_TIMER;

			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);	// まだ音量の全体ボリュームを下げるやり方がわかって無いので手打ち

			StartFadeOut(SCENE_GAME);
		}
		else if (GetKeyRelease(VK_2))
		{
			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA2);
		}
		else if (GetKeyRelease(VK_3))
		{
			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA3);
		}
		else if (GetKeyRelease(VK_4))
		{
			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);
			
			StartFadeOut(SCENE_AREA4);
		}
		else if (GetKeyRelease(VK_5))
		{
			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA5);
		}
		else if (GetKeyRelease(VK_6))
		{
			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA6);
		}
		else if (GetKeyRelease(VK_7))
		{
			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA7);
		}
		else if (GetKeyRelease(VK_8))
		{
			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA8);
		}
		else if (GetKeyRelease(VK_9))
		{
			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA9);
		}
		else if (GetKeyRelease(VK_0))
		{
			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA10);
		}
		else if (GetKeyRelease(VK_D))
		{
			// 決定音
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA_DEBUG);
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
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);

	// タイトル背景描画
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture[TITLE_TEX_BG]);
	DrawPolygon(pDC);

	// 点滅タイマー用
	if (g_bStart)
	{
		if (g_nBlink < BLINK_START_TIMER / 2)
			return;
	}
	else
	{
		if (g_nBlink < BLINK_TIMER / 2)
			return;
	}

	// 決定アイコン描画
	SetPolygonSize(ENTER_WIDTH, ENTER_HEIGHT);
	SetPolygonPos(ENTER_POS_X, ENTER_POS_Y);
	SetPolygonTexture(g_pTexture[TITLE_TEX_ENTER]);
	DrawPolygon(pDC);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
}