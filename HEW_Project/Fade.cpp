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

// 画像ファイルネーム
static LPCWSTR g_pszTexFName[FADE_TEX_MAX] = {
	L"data/texture/Loading/NowLoading.png",
	L"data/texture/Loading/Logo.png",
};


CFade::CFade()
{

}
CFade::~CFade()
{

}

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT CFade::Init()
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
	m_color.r = m_color.g = m_color.b = m_color.a = 1.0f;
	m_eState = FADE_IN;
	m_nNext = SCENE_TITLE;
	m_SManager = GetSManager();

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void CFade::Uninit()
{
	// 複数のテクスチャ解放
	for (int i = 0; i < FADE_TEX_MAX; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}

	// 終了時に初期化
	m_color.a = 0.0f;
	m_eState = FADE_NONE;
}

//**************************************************************
// 更新処理
//**************************************************************
void CFade::Update()
{
	// フェードイン・アウト
	switch (m_eState)
	{
	case FADE_NONE:
		break;

	case FADE_OUT:
		// フェードアウト---------------------------------------
		m_color.a += FADE_RATE; // 不透明度を増す
		if (m_color.a >= 1.0f)
		{
			// フェードイン処理に切り替え
			m_color.a = 1.0f;
			m_eState = FADE_IN;

			// シーン切替
			m_SManager->Set(m_nNext);
		}
		// ボリュームもフェードアウト
		CSound::SetVolume(1.0f - m_color.a);
		break;
		// -----------------------------------------------------

	case FADE_IN:
		// フェードイン-----------------------------------------
		m_color.a -= FADE_RATE; // 透明度を増す
		if (m_color.a <= 0.0f)
		{
			// フェードイン終了
			m_color.a = 0.0f;
			m_eState = FADE_NONE;
		}
		// ボリュームもフェードイン
		CSound::SetVolume(1.0f - m_color.a);
		break;
		// -----------------------------------------------------
	}
	if (m_color.r <= 0.0f)
	{
		m_color.r = m_color.g = m_color.b += FADE_RATE;
	}
	else
	{
		m_color.r = m_color.g = m_color.b -= FADE_RATE;
	}

	if (m_nNext == SCENE_GAMECLEAR || m_nNext == SCENE_GAMEOVER)
	{
		m_color.r = m_color.g = m_color.b = 0.0f;
	}
	else
	{
		m_color.r = m_color.g = m_color.b = 1.0f;
	}
}

//**************************************************************
// 終了処理
//**************************************************************
void CFade::Draw()
{
	// 画面全体に半透明の矩形を描画
	SetBlendState(BS_ALPHABLEND);
	 ID3D11DeviceContext* pDC = GetDeviceContext();
	 SetPolygonPos(0.0f, 0.0f);		// 座標設定
	 SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);	// 額縁サイズ設定
	 SetPolygonUV(0.0f, 0.0f);		// テクスチャ座標設定
	 SetPolygonFrameSize(1.0f, 1.0f);	// テクスチャサイズ設定
	 if(m_nNext == SCENE_TITLE)		// テクスチャ情報設定
	 { SetPolygonTexture(g_pTexture[LOGO]);} 
	 else {SetPolygonTexture(g_pTexture[LOAD]);}
	 SetPolygonColor(m_color.r, m_color.g, m_color.b);// 色情報設定
	 SetPolygonAlpha(m_color.a);		// 透明度設定
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
void CFade::StartFadeOut(int eNext)
{
	// フェードアウトしていなければ処理
	if (m_eState != FADE_OUT)
	{
		m_eState = FADE_OUT;
		m_color.a = 0.0f;
		m_nNext = eNext;
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
E_FADE_STATE CFade::GetFadeState()
{
	return m_eState;
}
