//**************************************************************
//
//	Fade.cpp
//	フェード処理
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//	開発履歴
//	2021/11/30	フェード制御実装開始		：柴山凜太郎
//	2021/12/03	フェード制御ベース実装完了	：柴山凜太郎
//	2021/12/06	柴山凜太郎君のプログラムを元に変更	：上月大地	
//
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "Fade.h"
#include "polygon.h"
#include "Sound.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define FADE_RATE 0.02f // フェードイン/アウトの刻み

//**************************************************************
// グローバル変数
//**************************************************************
static float g_fRed = 0.0f;			// 赤:0.0f~255.0fト色
static float g_fGreen = 0.0f;		// 緑:0.0f~255.0f
static float g_fBlue = 0.0f;		// 青:0.0f~255.0f
static float g_fAlpha = 1.0f;		// α値:0.0f~1.0f
static float g_fFadeRate = FADE_RATE;	// フェードイン/アウトの刻み

static E_FADE_STATE g_eState = FADE_IN;	// フェード状態
static EScene g_eNext = SCENE_TITLE;	// 次のシーン

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitFade()
{
	HRESULT hr = S_OK;

	g_fFadeRate = FADE_RATE;	//1.0fだと一瞬でおわる
	g_eState = FADE_IN;
	g_eNext = SCENE_TITLE;

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitFade()
{
	g_fAlpha = 0.0f;
	g_eState = FADE_NONE;
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateFade()
{
	// フェードイン・アウト
	switch (g_eState)
	{
	case FADE_NONE:
		break;

	case FADE_OUT:
		// フェードアウト---------------------------------------
		g_fAlpha += g_fFadeRate; // 不透明度を増す
		if (g_fAlpha >= 1.0f)
		{
			// フェードイン処理に切り替え
			g_fAlpha = 1.0f;
			g_eState = FADE_IN;

			// シーン切替
			SetScene(g_eNext);
			// SetBlendState(BS_NONE);
			// SetZWrite(false);
		
		}
		// ボリュームもフェードアウト
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
		// -----------------------------------------------------

	case FADE_IN:
		// フェードイン-----------------------------------------
		g_fAlpha -= g_fFadeRate; // 透明度を増す
		if (g_fAlpha <= 0.0f)
		{
			// フェードイン終了
			g_fAlpha = 0.0f;
			g_eState = FADE_NONE;
			// SetBlendState(BS_ALPHABLEND);
			// SetZBuffer(true);
		}
		// ボリュームもフェードイン
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
		// -----------------------------------------------------
	}
}

//**************************************************************
// 終了処理
//**************************************************************
void DrawFade()
{
	// 画面全体に半透明の矩形を描画
	 ID3D11DeviceContext* pDC = GetDeviceContext();
	 SetPolygonPos(0.0f, 0.0f);				// 座標
	 SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);	// 額縁サイズ
	 SetPolygonUV(0.0f, 0.0f);				// テクスチャ座標
	 SetPolygonFrameSize(1.0f, 1.0f);		// テクスチャサイズ
	 SetPolygonTexture(nullptr);				// テクスチャ情報
	 SetPolygonColor(g_fRed, g_fGreen, g_fBlue);		// 色情報
	 SetPolygonAlpha(g_fAlpha);				// 透明度
	 DrawPolygon(pDC);
	 
	 // 元に戻す
	 SetPolygonColor(1.0f, 1.0f, 1.0f);
	 SetPolygonAlpha(1.0f);

}

//*******************************
//
//	フェードアウト処理
//	
//	引数:
//		次のシーン番号
//
//	戻り値
//		無し
//
//*******************************
void StartFadeOut(EScene eNext)
{
	if (g_eState != FADE_OUT)
	{
		g_eState = FADE_OUT;
		g_fAlpha = 0.0f;
		g_eNext = eNext;
	}
}

//*******************************
//
//	フェード状態取得
//	
//	引数:
//		無し
//
//	戻り値
//		フェードの状態
//		1:イン
//		2:アウト
//
//*******************************
E_FADE_STATE GetFadeState()
{
	return g_eState;
}
