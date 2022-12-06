//**************************************************************
//
//	CountEnemy.cpp
//	数値表示処理
//
//--------------------------------------------------------------
//	製作者：小嶋悟
//--------------------------------------------------------------
//**************************************************************
//	開発履歴
//	2021/12/22	コメントの編集&追加
//	編集者：小嶋悟
//**************************************************************
//**************************************************************
// インクルード部
//**************************************************************
#include "EnemyMelee.h"
#include "EnemyRange.h"
#include "Texture.h"
#include "polygon.h"
#include "number.h"
#include "CountEnemy.h"
#include "Sound.h"
#include "SceneGameclear.h"
#include "SceneManager.h"
#include "Fade.h"
//**************************************************************
// マクロ定義
//**************************************************************
// タイマー枠
#define PATH_CountEnemyFRAMETEXTURE L"data/texture/GameScene/frame_CountEnemy.png"		//カウンター枠テクスチャ
#define CountEnemyFRAME_WIDTH (399.0f)		//カウンター枠横幅
#define CountEnemyFRAME_HEIGHT (80.0f)		//カウンター枠縦幅
#define CountEnemyFRAME_POS_X (400.0f)			//カウンター枠横軸座標
#define CountEnemyFRAME_POS_Y ((SCREEN_HEIGHT-CountEnemyFRAME_HEIGHT)*0.5f)	//カウンター枠縦軸座標
//カウンター
#define CountEnemy_CHRCNT (2)		// カウンターの表示桁数
#define CountEnemy_POS_X (CountEnemyFRAME_POS_X-CountEnemy_CHRCNT*NUMBER_SIZE_X+90.0f)	// カウンター横軸座標
#define CountEnemy_POS_Y (CountEnemyFRAME_POS_Y+50.0f)	//カウンター縦軸座標
#define CountEnemy_NumSize_X 50.0f
#define CountEnemy_NumSize_Y 100.0f

//
#define MaxCountEnemy_POS_X (CountEnemyFRAME_POS_X-CountEnemy_CHRCNT*NUMBER_SIZE_X*-2.0f)	// 総数カウンター横軸座標
#define MaxCountEnemy_POS_Y (CountEnemyFRAME_POS_Y+16.0f)									//総数カウンター縦軸座標
//
#define MAX_ENEMY_NUM  99//エネミーの上限
//
//**************************************************************
// グローバル変数
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;	// テクスチャ用変数
int g_nCountEnemy;		//エネミーをカウントする
int g_nMaxCountEnemy;	//そのエリアのエネミーの最大数を保管する

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT CCountEnemy::InitCountEnemy()
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
void CCountEnemy::UninitCountEnemy()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}
//**************************************************************
// 更新処理
//**************************************************************
void CCountEnemy::UpdateCountEnemy()
{
}
//**************************************************************
// 描画処理
//**************************************************************
void CCountEnemy::DrawCountEnemy()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	// タイマー枠表示
	SetPolygonSize(CountEnemyFRAME_WIDTH, CountEnemyFRAME_HEIGHT);
	SetPolygonPos(CountEnemyFRAME_POS_X, CountEnemyFRAME_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// カウンター表示
	DrawNumber(XMFLOAT2(CountEnemy_POS_X, CountEnemy_POS_Y), g_nCountEnemy, CountEnemy_CHRCNT, CountEnemy_NumSize_X, CountEnemy_NumSize_Y);
	
	// 元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}
//**************************************************************
// カウントエネミー加算
//**************************************************************
void CCountEnemy::AddCountEnemy(int nCount)
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
void CCountEnemy::DelCountEnemy(int nCount)
{
	g_nCountEnemy -= nCount;
	if (nCount < 0)
	{
		nCount = 0;
	}
	if (g_nCountEnemy == 0)
	{
		GameclearFlag();
	}
}
//**************************************************************
//カウントエネミー取得
//**************************************************************
int CCountEnemy::GetCountEnemy()
{
	return g_nCountEnemy;
}