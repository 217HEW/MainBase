//**************************************************************
//
//	Fade.cpp
//	フェード処理
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/11/30	フェード制御実装開始
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/03	フェード制御ベース実装完了
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/06	柴山凜太郎君のプログラムを元に変更
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/19	柴山凜太郎君のプログラムを元に変更
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/21	g_fFadeRate変数の削除
//				グローバル変数初期化位置を「グローバル領域→初期化関数内」に変更
//				Draw関数内にSetBlendState関数の追加
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/22	コメントの編集&追加
//--------------------------------------------------------------
//	2022/1/27	正式な画像に変更しました
//	編集者：上月大地
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "Fade.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define FADE_RATE 0.02f // フェードイン/アウトの刻み
#define FADE_TEX	
#define FADE_TEX_2	

//**************************************************************
// グローバル変数
//**************************************************************
static float g_fRed;		// 赤:0.0f~255.0fト色
static float g_fGreen;		// 緑:0.0f~255.0f
static float g_fBlue;		// 青:0.0f~255.0f
static float g_fAlpha;		// α値:0.0f~1.0f

// 画像ファイルネーム
static LPCWSTR g_pszTexFName[FADE_TEX_MAX] = {
	L"data/texture/Loading/NowLoading.png",
	L"data/texture/Loading/Logo.png",
};

static ID3D11ShaderResourceView* g_pTexture[FADE_TEX_MAX];
static E_FADE_STATE g_eState;	// フェード状態
static int g_eNext;			// 次のシーン

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitFade()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	for (int i = 0; i < FADE_TEX_MAX; ++i)
	{
		hr = CreateTextureFromFile(pDevice, g_pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	// 変数の初期化
	g_fRed = 1.0f;
	g_fGreen = 1.0f;
	g_fBlue = 1.0f;
	g_fAlpha = 1.0f;
	g_eState = FADE_IN;
	g_eNext = SCENE_TITLE;

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitFade()
{
	// 複数のテクスチャ解放
	for (int i = 0; i < FADE_TEX_MAX; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}

	// 終了時に初期化
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
		g_fAlpha += FADE_RATE; // 不透明度を増す
		if (g_fAlpha >= 1.0f)
		{
			// フェードイン処理に切り替え
			g_fAlpha = 1.0f;
			g_eState = FADE_IN;

			// シーン切替
			SetScene(g_eNext);
		}
		// ボリュームもフェードアウト
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
		// -----------------------------------------------------

	case FADE_IN:
		// フェードイン-----------------------------------------
		g_fAlpha -= FADE_RATE; // 透明度を増す
		if (g_fAlpha <= 0.0f)
		{
			// フェードイン終了
			g_fAlpha = 0.0f;
			g_eState = FADE_NONE;
		}
		// ボリュームもフェードイン
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
		// -----------------------------------------------------
	}
	if (g_fRed <= 0.0f)
	{
		g_fRed = g_fGreen = g_fBlue += FADE_RATE;
	}
	else
	{
		g_fRed = g_fGreen = g_fBlue -= FADE_RATE;
	}

	if (g_eNext == SCENE_GAMECLEAR || g_eNext == SCENE_GAMEOVER)
	{
		g_fRed = g_fGreen = g_fBlue = 0.0f;
	}
	else
	{
		g_fRed = g_fGreen = g_fBlue = 1.0f;
	}
}

//**************************************************************
// 終了処理
//**************************************************************
void DrawFade()
{
	// 画面全体に半透明の矩形を描画
	SetBlendState(BS_ALPHABLEND);
	 ID3D11DeviceContext* pDC = GetDeviceContext();
	 SetPolygonPos(0.0f, 0.0f);		// 座標設定
	 SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);	// 額縁サイズ設定
	 SetPolygonUV(0.0f, 0.0f);		// テクスチャ座標設定
	 SetPolygonFrameSize(1.0f, 1.0f);	// テクスチャサイズ設定
	 if(g_eNext == SCENE_TITLE)		// テクスチャ情報設定
	 { SetPolygonTexture(g_pTexture[LOGO]);} 
	 else {SetPolygonTexture(g_pTexture[LOAD]);}
	 SetPolygonColor(g_fRed, g_fGreen, g_fBlue);// 色情報設定
	 SetPolygonAlpha(g_fAlpha);		// 透明度設定
	 DrawPolygon(pDC);
	 
	  // 元に戻す
	 SetBlendState(BS_NONE);// 透過(アルファ値)をいじるときはセットブレンドではさむ
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
void StartFadeOut(int eNext)
{
	// フェードアウトしていなければ処理
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
