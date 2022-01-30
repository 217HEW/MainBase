#include "EnemyMelee.h"
#include "EnemyRange.h"
// 画面遷移 [CountEnemy.cpp]
//**************************************************************
//
//	number.cpp
//	数値表示処理
//
//--------------------------------------------------------------
//	製作者：澤村瑠人
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/22	コメントの編集&追加
//	編集者：柴山凜太郎
//**************************************************************

//時間の計算は行われているが描画がされていないもしくは見えない
//**************************************************************
// インクルード部
//**************************************************************
#include "Texture.h"
#include "polygon.h"
#include "number.h"
#include "CountEnemy.h"
#include "Sound.h"
#include "SceneGameclear.h"
#include "SceneManager.h"
//**************************************************************
// マクロ定義
//**************************************************************

// タイマー枠
#define PATH_CountEnemyFRAMETEXTURE L"data/texture/frame_CountEnemy.png"		// タイマー枠テクスチャ
#define CountEnemyFRAME_WIDTH (399.0f)		// タイマー枠横幅
#define CountEnemyFRAME_HEIGHT (80.0f)		// タイマー枠縦幅
#define CountEnemyFRAME_POS_X (400.0f)			// タイマー枠横軸座標
#define CountEnemyFRAME_POS_Y ((SCREEN_HEIGHT-CountEnemyFRAME_HEIGHT)*0.5f)	// タイマー枠縦軸座標

//カウンター
#define CountEnemy_CHRCNT (2)		// カウンターの表示桁数
#define CountEnemy_POS_X (CountEnemyFRAME_POS_X-CountEnemy_CHRCNT*NUMBER_SIZE_X*0.1f)	// カウンター横軸座標
#define CountEnemy_POS_Y (CountEnemyFRAME_POS_Y+16.0f)	//カウンター縦軸座標

#define MaxCountEnemy_POS_X (CountEnemyFRAME_POS_X-CountEnemy_CHRCNT*NUMBER_SIZE_X*-2.0f)	// 総数カウンター横軸座標
#define MaxCountEnemy_POS_Y (CountEnemyFRAME_POS_Y+16.0f)									//総数カウンター縦軸座標

#define MAX_ENEMY_NUM  99//エネミーの上限

//**************************************************************
// グローバル変数
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;	// テクスチャ用変数
int g_nCountEnemy;		//エネミーをカウントする
int g_nMaxCountEnemy;	//そのエリアのエネミーの最大数を保管する

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitCountEnemy()
{
	HRESULT hr = S_OK;

	ID3D11Device* pDevice = GetDevice();
	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_CountEnemyFRAMETEXTURE, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}
	// 変数初期化
	g_nCountEnemy=0;	
	g_nMaxCountEnemy=0;
	return hr;

}
//**************************************************************
// 終了処理
//**************************************************************
void UninitCountEnemy()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}
//**************************************************************
// 更新処理
//**************************************************************
void UpdateCountEnemy()
{
}
//**************************************************************
// 描画処理
//**************************************************************
void DrawCountEnemy()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	//// 白か赤に設定
	//if (g_nCountEnemy < 10 * 60)
	//{
	//	if (g_nCountEnemy == 9.7 * 60)
	//	{
	//		CSound::SetPlayVol(SE_BEEP, 0.07f); // 警告音
	//	}
	//	SetPolygonColor(1.0f, 0.0f, 0.0f);//残り10秒で赤くなる
	//}
	//else {
	//	SetPolygonColor(1.0f, 1.0f, 1.0f);
	//}
	// タイマー枠表示
	SetPolygonSize(CountEnemyFRAME_WIDTH, CountEnemyFRAME_HEIGHT);
	SetPolygonPos(CountEnemyFRAME_POS_X, CountEnemyFRAME_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// カウンター表示
	DrawNumber(XMFLOAT2(CountEnemy_POS_X, CountEnemy_POS_Y), g_nCountEnemy, CountEnemy_CHRCNT);
	// カウンター最大表示
	DrawNumber(XMFLOAT2(MaxCountEnemy_POS_X, MaxCountEnemy_POS_Y), g_nMaxCountEnemy, CountEnemy_CHRCNT);

	// 元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}
//**************************************************************
// カウントエネミー加算
//**************************************************************
void AddCountEnemy(int nCount)
{
	g_nCountEnemy += nCount;
	g_nMaxCountEnemy += nCount;
	if (nCount > MAX_ENEMY_NUM)
	{
		nCount = MAX_ENEMY_NUM;
		g_nMaxCountEnemy = MAX_ENEMY_NUM;
	}
}
//**************************************************************
// カウントエネミー減算
//**************************************************************
void DelCountEnemy(int nCount)
{
	g_nCountEnemy -= nCount;
	if (nCount < 0)
	{
		nCount = 0;
	}
	if (g_nCountEnemy == 0)
	{
		GameclearFlag();
		SetScene(SCENE_GAMECLEAR);
	}
}
//**************************************************************
//カウントエネミー取得
//**************************************************************
int GetCountEnemy()
{
	return g_nCountEnemy;
}