//**************************************************************
//
//	ClearC_Pause.cpp
//	ポーズ機能
//
//--------------------------------------------------------------
//	製作者：澤村瑠人
//--------------------------------------------------------------
//**************************************************************

#include "ClearPause.h"
#include "input.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"
#include "Fade.h"
#include "SceneManager.h"
#include "StageSelect.h"
#include <xinput.h>		// コントローラー情報取得に必要

#pragma comment (lib, "xinput.lib")	// コントローラー情報取得に必要

// コントローラ十字キー用
typedef enum {
	NOT = 0,	// 何も押してない
	UP,			// 上
	DOWN,		// 下
	RIGHT,		// 右
	LEFT,		// 左

	CROSS_MAX
} C_PCROSS;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_C_PAUSE_MENU		(3)			// ポーズメニュー数
#define	C_PAUSE_MENU_WIDTH	(320.0f)	// ポーズメニュー幅
#define	C_PAUSE_MENU_HEIGHT	(60.0f)		// ポーズメニュー高さ
#define	C_PAUSE_MENU_POS_X	(0.0f)		// ポーズメニュー位置(X座標)
#define	C_PAUSE_MENU_INTERVAL	(100.0f)	// ポーズメニュー間隔
#define	C_PAUSE_MENU_POS_Y	(C_PAUSE_MENU_INTERVAL)	// ポーズメニュー位置(Y座標)
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(320.0f)	// プレートの幅
#define	PLATE_POS_X			(0.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

#define WAIT_CPTIME			(15)			// 選択待機時間

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// テクスチャへのポインタ

static C_PAUSE_MENU g_nSelectMenu = C_PAUSE_MENU_NEXTSTAGE;		// 選択中のメニューNo.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static DWORD	Joystate;	// 接続確認用
static DWORD	Joycon;		// コントローラー情報

bool g_bcPTime;	// 待機用
int g_bcPJoySelect;	// コントローラ選択用

// 待機タイマーカウントダウン
int g_ncPTime;

bool g_SetClearPause;


static LPCWSTR c_aFileNameC_PauseMenu[NUM_C_PAUSE_MENU] =
{
	L"data/texture/ClearNextStage.png",	// ネクステージ
	L"data/texture/ClearSelect.png",	// セレクト
	L"data/texture/ClearQuit.png",	// クイット
};

static ID3D11ShaderResourceView* g_pSTexture;//説明用
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitC_Pause(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntC_PauseMenu = 0; nCntC_PauseMenu < NUM_C_PAUSE_MENU; ++nCntC_PauseMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,			// デバイスへのポインタ
			c_aFileNameC_PauseMenu[nCntC_PauseMenu],	// ファイルの名前
			&g_pTextures[nCntC_PauseMenu]);			// 読み込むメモリー
	}
	g_bcPTime = false;
	g_ncPTime = WAIT_CPTIME;
	g_nSelectMenu = C_PAUSE_MENU_NEXTSTAGE;
	g_fCurve = 0.0f;

	g_SetClearPause = false;
	return hr;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitC_Pause(void)
{
	// テクスチャの開放
	for (int nCntC_PauseMenu = 0; nCntC_PauseMenu < NUM_C_PAUSE_MENU; ++nCntC_PauseMenu)
	{
		SAFE_RELEASE(g_pTextures[nCntC_PauseMenu]);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateC_Pause(void)
{
	//ゲームパッドの状態を取得
	XINPUT_STATE state;

	// コントローラー情報
	GetJoyState(Joycon);

	// 選択待機時間
	if (g_ncPTime < 0)
	{
		g_bcPTime = true;
	}
	g_bcPJoySelect = NOT;	// コントローラ用選択初期化
	// -------コントローラー操作------------------------------------------
	GetJoyState(Joycon);
	// コントローラーの接続状況確認
	Joystate = XInputGetState(0, &state);

	if (Joystate == ERROR_SUCCESS)
	{	// 接続有り↓
		if (GetJoyDpadUp(Joycon)) {		g_bcPJoySelect = UP; }
		if (GetJoyDpadDown(Joycon)) {	g_bcPJoySelect = DOWN; }
		if (GetJoyDpadRight(Joycon)) {	g_bcPJoySelect = RIGHT; }
		if (GetJoyDpadLeft(Joycon)) {	g_bcPJoySelect = LEFT; }
	}
	else
	{	// 接続無し↓
		g_bcPJoySelect = NOT;
	}

	if (g_bcPTime == true) {
		if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP) || (g_bcPJoySelect == UP))
		{
			CSound::SetPlayVol(SE_SELECT, 0.1f); // セレクト音

			g_nSelectMenu = (C_PAUSE_MENU)((g_nSelectMenu + NUM_C_PAUSE_MENU - 1) % NUM_C_PAUSE_MENU);
			SetC_PauseMenu();
			g_bcPTime = false;
			g_ncPTime = WAIT_CPTIME;
		}
		else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN) || (g_bcPJoySelect == DOWN))
		{
			CSound::SetPlayVol(SE_SELECT, 0.1f); // セレクト音

			g_nSelectMenu = (C_PAUSE_MENU)((g_nSelectMenu + 1) % NUM_C_PAUSE_MENU);
			SetC_PauseMenu();
			g_bcPTime = false;
			g_ncPTime = WAIT_CPTIME;
		}
	}
	--g_ncPTime;
	// 反射光の設定
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawC_Pause(void)
{
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetPolygonTexture(nullptr);
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(g_fCol, g_fCol, g_fCol);
	SetPolygonAlpha(1.0f);
	DrawPolygon(pDeviceContext);

	SetPolygonSize(C_PAUSE_MENU_WIDTH, C_PAUSE_MENU_HEIGHT);
	for (int nCntC_PauseMenu = 0; nCntC_PauseMenu < NUM_C_PAUSE_MENU; ++nCntC_PauseMenu) 
	{

		SetPolygonPos(C_PAUSE_MENU_POS_X, C_PAUSE_MENU_POS_Y - nCntC_PauseMenu * C_PAUSE_MENU_INTERVAL);

		if (nCntC_PauseMenu == g_nSelectMenu) {
			SetPolygonColor(1.0f, 1.0f, 0.1f);
		}
		else {
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntC_PauseMenu]);
		// ポリゴンの描画
		DrawPolygon(pDeviceContext);

	}

	

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}

//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetC_PauseMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
C_PAUSE_MENU GetC_PauseMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetC_PauseMenu(void)
{
	g_nSelectMenu = C_PAUSE_MENU_NEXTSTAGE;
	// CSound::Play(SE_SELECT);
	// CSound::SetVolume(SE_SELECT, 0.02f);
	SetC_PauseMenu();
}

bool GetClearPause()
{
	return g_SetClearPause;
}

void SetClearPause(bool Setpause)
{
	g_SetClearPause = Setpause;
}
