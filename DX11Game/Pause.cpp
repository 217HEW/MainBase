//**************************************************************
//
//	Pause.cpp
//	ポーズ機能
//
//--------------------------------------------------------------
//	製作者：小嶋悟
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/30	ポーズ画面デザイン変更及び画像追加
//	編集者：??
//--------------------------------------------------------------
//	2021/12/27	音データを追加したので実装(セレクト＆キャンセル音)
//														変更者：上月大地
//**************************************************************

#include "Pause.h"
#include "input.h"
//#include "fade.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_PAUSE_MENU		(3)			// ポーズメニュー数
#define	PAUSE_MENU_WIDTH	(320.0f)	// ポーズメニュー幅
#define	PAUSE_MENU_HEIGHT	(60.0f)		// ポーズメニュー高さ
#define	PAUSE_MENU_POS_X	(-450.0f)		// ポーズメニュー位置(X座標)
#define	PAUSE_MENU_POS_Y	(PAUSE_MENU_INTERVAL)	// ポーズメニュー位置(Y座標)
#define	PAUSE_MENU_INTERVAL	(100.0f)	// ポーズメニュー間隔
#define	PLATE_WIDTH			(360.0f)	// プレートの幅
#define	PLATE_HEIGHT		(320.0f)	// プレートの幅
#define	PLATE_POS_X			(-450.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(0.0f)		// プレートの位置(Y座標)

//説明用
#define PATH_STEXTURE "data/texture/pause003.png"//説明画像
#define S_POS_X 200.0f
#define S_POS_Y 0.0f
#define S_WIDTH 809
#define S_HEIGHT 500
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// テクスチャへのポインタ

static PAUSE_MENU g_nSelectMenu = PAUSE_MENU_CONTINUE;		// 選択中のメニューNo.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;

static LPCWSTR c_aFileNamePauseMenu[NUM_PAUSE_MENU] =
{
	L"data/texture/pause000.png",	// コンティニュー
	L"data/texture/pause001.png",	// リトライ
	L"data/texture/pause002.png",	// クイット
};

static ID3D11ShaderResourceView* g_pSTexture;//説明用
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,			// デバイスへのポインタ
			c_aFileNamePauseMenu[nCntPauseMenu],	// ファイルの名前
			&g_pTextures[nCntPauseMenu]);			// 読み込むメモリー
	}

	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	g_fCurve = 0.0f;

	// 効果音初期化
	//g_pSE_Select = CreateSound(SE_SELECT_PATH);

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_STEXTURE, &g_pSTexture);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	// テクスチャの開放
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu)
	{
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
	// テクスチャ解放
	SAFE_RELEASE(g_pSTexture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
	if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP)) 
	{
		CSound::Play(SE_SELECT);
		CSound::SetVolume(SE_SELECT, 0.02f);
		g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}
	else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN))
	{
		CSound::Play(SE_SELECT);
		CSound::SetVolume(SE_SELECT, 0.02f);
		g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}

	//g_fCurve += XM_PI * 0.01f;//ピカピカの原因
	//if (g_fCurve > XM_PI) {
	//	g_fCurve -= XM_2PI;
	//}

	// 反射光の設定
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
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

	SetPolygonSize(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) 
	{
		SetPolygonPos(PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y - nCntPauseMenu * PAUSE_MENU_INTERVAL);

		if (nCntPauseMenu == g_nSelectMenu) {
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else {
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntPauseMenu]);
		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}

	//説明用
	SetPolygonColor(3.0f, 3.0f, 3.0f);//1以上にすることで明るさ変更可能
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(S_WIDTH, S_HEIGHT);
	SetPolygonPos(S_POS_X, S_POS_Y);
	SetPolygonTexture(g_pSTexture);
	DrawPolygon(pDC);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
	//SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetPauseMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
PAUSE_MENU GetPauseMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetPauseMenu(void)
{
	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	// CSound::Play(SE_SELECT);
	// CSound::SetVolume(SE_SELECT, 0.02f);
	SetPauseMenu();
}
