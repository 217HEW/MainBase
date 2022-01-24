//**************************************************************
//
//	StageSelect.cpp
//	ステージセレクト機能
//
//--------------------------------------------------------------
//	製作者：石原聖斗
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2022/01/21	計10ステージのセレクトボタンの配置とシーン遷移の実装
//	2022/01/23	タイトルに戻るボタンの設置
//	2022/01/24	選択中の画像の差し替えと大きさの調整
//**************************************************************

#include "StageSelect.h"
#include "input.h"
#include "fade.h"
#include "SceneManager.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PATH_BGTEXTURE	L"data/texture/selectBG.PNG"	// 背景テクスチャ
#define BG_POS_X		(0.0f)	// 背景X座標
#define BG_POS_Y		(0.0f)	// 背景Y座標
#define BG_WIDTH		SCREEN_WIDTH	// テクスチャの横幅
#define BG_HEIGHT		SCREEN_HEIGHT	// テクスチャの縦幅




#define	NUM_SELECT_MENU		(12)			// セレクトメニュー数
#define	SELECT_MENU_WIDTH	(150.0f)		// セレクトメニュー幅
#define	SELECT_MENU_HEIGHT	(150.0f)		// セレクトメニュー高さ
#define	SELECT_MENU_POS_X	(-360.0f)		// セレクトメニュー位置(X座標)
#define	SELECT_MENU_POS_Y	(130.0f)		// セレクトメニュー位置(Y座標)
#define	SELECT_MENU_INTERVAL (180.0f)		// セレクトメニュー間隔



#define	PLATE_WIDTH			(150.0f)		// プレートの幅
#define	PLATE_HEIGHT		(100.0f)		// プレートの幅
#define	PLATE_POS_X			(-500.0f)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(300.0f)		// プレートの位置(Y座標)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[12] = { nullptr };	// テクスチャへのポインタ
static ID3D11ShaderResourceView*	g_pBGTexture; // 背景


static SELECT_MENU g_nSelectMenu = STAGE_1;		// 選択中のメニューNo.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;

static LPCWSTR c_aFileNameStageMenu[NUM_SELECT_MENU] =
{
	L"data/texture/hero002.jpg",// hero001 → 選択中
	L"data/texture/hero002.jpg",// hero002 → 選択できない
	L"data/texture/hero002.jpg",// hero003 → 選択できるが選択していない
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/STitle.png",// タイトルに戻る
	L"data/texture/hero001.jpg",
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSelect(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntStageMenu = 0; nCntStageMenu < NUM_SELECT_MENU; ++nCntStageMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,			// デバイスへのポインタ
			c_aFileNameStageMenu[nCntStageMenu],	// ファイルの名前
			&g_pTextures[nCntStageMenu]);			// 読み込むメモリー
	}

	g_nSelectMenu = STAGE_1;
	g_fCurve = 0.0f;

	// 効果音初期化
	//g_pSE_Select = CreateSound(SE_SELECT_PATH);

	// 背景テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pBGTexture);
	return hr;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSelect(void)
{
	// テクスチャの開放
	for (int nCntStageMenu = 0; nCntStageMenu < NUM_SELECT_MENU; ++nCntStageMenu)
	{
		SAFE_RELEASE(g_pTextures[nCntStageMenu]);
	}
	// 背景テクスチャ解放
	SAFE_RELEASE(g_pBGTexture);
	
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSelect(void)
{
	if (GetKeyRepeat(VK_UP))
	{
		CSound::Play(SE_SELECT);
		CSound::SetVolume(SE_SELECT, 0.02f);
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + (NUM_SELECT_MENU - 2) - 5) % (NUM_SELECT_MENU - 2));
		SetSelectMenu();
	}
	else if (GetKeyRepeat(VK_DOWN))
	{
		CSound::Play(SE_SELECT);
		CSound::SetVolume(SE_SELECT, 0.02f);
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + 5) % (NUM_SELECT_MENU - 2));
		SetSelectMenu();
	}
	else if (GetKeyRepeat(VK_RIGHT))
	{
		CSound::Play(SE_SELECT);
		CSound::SetVolume(SE_SELECT, 0.02f);
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + 1) % (NUM_SELECT_MENU - 1));
		SetSelectMenu();
	}
	else if (GetKeyRepeat(VK_LEFT))
	{
		CSound::Play(SE_SELECT);
		CSound::SetVolume(SE_SELECT, 0.02f);
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + NUM_SELECT_MENU - 1) % (NUM_SELECT_MENU));
		
		SetSelectMenu();
	}
	

	//g_fCurve += XM_PI * 0.01f;//ピカピカの原因
	//if (g_fCurve > XM_PI) {
	//	g_fCurve -= XM_2PI;
	//}

	// 反射光の設定
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;


	//[ENTER]が押された
	if (GetKeyTrigger(VK_RETURN))
	{
		//選択中のメニュー項目により分岐
		switch (GetSelectMenu())
		{
		case STAGE_1:	// スレージ1
			StartFadeOut(SCENE_GAME);
			break;
		case STAGE_2:	// ステージ2
			StartFadeOut(SCENE_AREA2);
			break;
		case STAGE_3:	// ステージ3
			StartFadeOut(SCENE_AREA3);
			break;
		case STAGE_4:	// ステージ4
			StartFadeOut(SCENE_AREA4);
			break;
		case STAGE_5:	// ステージ5
			StartFadeOut(SCENE_AREA5);
			break;
		case STAGE_6:	// ステージ6
			StartFadeOut(SCENE_AREA6);
			break;
		case STAGE_7:	// ステージ7
			StartFadeOut(SCENE_AREA7);
			break;
		case STAGE_8:	// ステージ8
			StartFadeOut(SCENE_AREA8);
			break;
		case STAGE_9:	// ステージ9
			StartFadeOut(SCENE_AREA9);
			break;
		case STAGE_10:	// ステージ10
			StartFadeOut(SCENE_AREA10);
			break;
		case SELECT_TITLE:
			StartFadeOut(SCENE_TITLE); // タイトルへ
			break;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSelect(void)
{
	int nCntStageMenu;

	// 背景
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pBGTexture);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// プレート
	/*SetPolygonTexture(nullptr);
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(g_fCol, g_fCol, g_fCol);
	SetPolygonAlpha(1.0f);
	DrawPolygon(pDeviceContext);*/
	
	// ｽﾃｰｼﾞ1～10
	for (nCntStageMenu = 0; nCntStageMenu < NUM_SELECT_MENU - 2; ++nCntStageMenu)
	{
		//ｽﾃｰｼﾞ5までいったら改行
		if(nCntStageMenu <= 4)
		{ 
			SetPolygonPos(SELECT_MENU_POS_X+ nCntStageMenu * SELECT_MENU_INTERVAL,
				SELECT_MENU_POS_Y);
		}
		else if(!nCntStageMenu <= 4)
		{
			
			SetPolygonPos(SELECT_MENU_POS_X + (nCntStageMenu-5) * SELECT_MENU_INTERVAL,
				SELECT_MENU_POS_Y - SELECT_MENU_INTERVAL * 1.5);
		}
		
		if (nCntStageMenu == g_nSelectMenu) {
			//SetPolygonColor(1.0f, 1.0f, 0.1f);
		}
		else {
			//SetPolygonColor(0.3f, 0.3f, 0.3f);
		}

		// テクスチャの設定
		// 選ばれているステージの画像の入れ替え
		if (nCntStageMenu == g_nSelectMenu) {
			SetPolygonTexture(g_pTextures[SELECT]);
			SetPolygonSize(SELECT_MENU_WIDTH + 25, SELECT_MENU_HEIGHT + 25);
		}
		else
		{
			SetPolygonTexture(g_pTextures[nCntStageMenu]);
			SetPolygonSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
		}
		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}

	// タイトル
	SetPolygonSize(SELECT_MENU_WIDTH / 2, SELECT_MENU_HEIGHT);
	SetPolygonPos(-500.0f, 0.0f);
	if (g_nSelectMenu == SELECT_TITLE) {
		SetPolygonColor(1.0f, 1.0f, 0.1f);
	}
	else {
		SetPolygonColor(0.3f, 0.3f, 0.3f);
	}
	SetPolygonTexture(g_pTextures[SELECT_TITLE]);
	DrawPolygon(pDeviceContext);


	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetSelectMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
SELECT_MENU GetSelectMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetSelectMenu(void)
{
	g_nSelectMenu = STAGE_1;
	// CSound::Play(SE_SELECT);
	// CSound::SetVolume(SE_SELECT, 0.02f);
	SetSelectMenu();
}
