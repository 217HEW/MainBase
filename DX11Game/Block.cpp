//**************************************************************
//
//	Block.cpp
//	ブロック(壁箱)の処理
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/21	　初期Wall.cppから改造して制作	
//				　通常ブロック、ひび割れブロックの当たり判定と切り替えを実装
//				　半透明処理が不要なのでFPS向上の為、コメントアウト(Draw)
//																		||変更者：上月大地
//--------------------------------------------------------------
//	2021/12/21	　通常ブロック、ひび割れブロックの当たり判定と切り替えを実装		
//	2021/12/21	　ブロックのサイズを構造体の要素から、グローバル変数へ　||変更者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/22	　モデルデータのテクスチャを読み込めるようにしました。
//				　無敵ブロックを呼び出せるようにしました。				
//				　ジャンプ状態のプレイヤーが当ったら耐久値が減り
//				　プレイヤーを接地させるようにしました 123～147			||変更者：上月大地
//	2021/12/21	ブロックのサイズを構造体の要素から、グローバル変数へ
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2121/12/22	コメントの編集&追加、不要なソースの削除
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2121/12/28	モデルの読込処理の見直し開始
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2122/01/07	ブロックのサイズの変数名の一部をsize→scaleに変更
//	編集者：柴山凜太郎
//**************************************************************

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Block.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_BLOCK		 "data/model/Block.fbx"		// "data/model/Hew_3_3.fbx"		// 通常ブロック			テクスチャ名 Block1.jpg
#define MODEL_CRACKS	 "data/model/Block2.fbx"	// "data/model2/Hew_2.fbx"		// ひび割れたブロック	テクスチャ名 Block1.jpg※今はフォルダを変えて反映しています
#define MODEL_INVINCIBLE "data/model/Block2.fbx"	// 無敵ブロック			テクスチャ無し
#define MAX_LIFE			(1)						// ブロック耐久値

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel	g_model[MAX_BLOCK];	// モデル
static TBLOCK		g_block[MAX_BLOCK];	// ブロック情報
static XMFLOAT3		g_BlockScale;		// 現在のサイズ
static XMFLOAT3		g_BlockHalfScale;	// ブロックの半分のサイズ
static XMMATRIX		mtxWorldinv;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBlock(void)
{
	HRESULT hr = S_OK;
	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		//Xが二倍になる為Yの二分の一にしておく
		g_BlockScale = XMFLOAT3(20.0f, 40.0f, 20.0f);
		g_BlockHalfScale = XMFLOAT3(15.0f, 28.0f, 10.0f);
		// g_wall->m_pos = XMFLOAT3(0.0f, 50.0f, 150.0f);
		g_block[i].m_3Dmodel = MODEL_BLOCK;
		g_block[i].m_nLife = MAX_LIFE;
		g_block[i].m_use = false;
		g_block[i].m_invincible = false;
		// モデルデータの読み込み
		//if (!g_model[i].Load(pDevice, pDeviceContext, g_block[i].m_3Dmodel))
		//{
		//	MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitBlock", MB_OK);
		//	return E_FAIL;
		//}
	}

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlock(void)
{
	// モデルの解放
	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		g_model[i].Release();
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlock(void)
{

	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	//------ブロックとプレイヤーの当たり判定処理-------------------------------
	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		if (!g_block[i].m_use)
		{// 未使用なら次へ
			continue;
		}

		// 壁とプレイヤーが衝突していたら
		if (CollisionAABB(g_block[i].m_pos, g_BlockHalfScale, GetPlayerPos(), GetPlayerSize()))//プレイヤーのサイズ XMFLOAT3(3.0f, 7.0f, 0.5f)
		{
			// プレイヤーがとんでいたら
			if (GetPlayerJump() == false)
			{
				// 体力が無くなったら使わない
				if (g_block[i].m_nLife <= 0)
				{
					g_block[i].m_use = false;
					//g_model[i].Release();
					break;
				}

				// 無敵ブロックだったら出る
				if (g_block[i].m_invincible)
				{
					// 接地状態ON
					SetPlayerJump(true);
					break;
				}
				// ブロックにひびが入る
				g_block[i].m_3Dmodel = MODEL_CRACKS;
				g_model[i].Load(pDevice, pDeviceContext, g_block[i].m_3Dmodel);
				g_block[i].m_nLife--;

				// 接地状態ON
				SetPlayerJump(true);

				StartExplosion(GetPlayerPos(), XMFLOAT2(80.0f, 80.0f));
			}
		}
	}

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		if (!g_block[i].m_use)
		{// 未使用なら次へ
			continue;
		}
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 箱のサイズ
		mtxWorld = XMMatrixScaling(g_BlockScale.x,
			g_BlockScale.y,
			g_BlockScale.z);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(
			g_block[i].m_pos.x,
			g_block[i].m_pos.y,
			g_block[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_block[i].m_mtxWorld, mtxWorld);

	}

	


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBlock(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		// 使っているブロックの描画
		if (!g_block[i].m_use)
		{
			continue;
		}
		// ブロックモデル描画
		g_model[i].Draw(pDC, g_block[i].m_mtxWorld, eOpacityOnly);
	}

	// 半透明部分を描画
	//for (int i = 0; i < MAX_BLOCK; ++i) {
	//	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	//	SetZWrite(false);				// Zバッファ更新しない
	//	g_model[i].Draw(pDC, g_block[i].m_mtxWorld, eTransparentOnly);
	//	SetZWrite(true);				// Zバッファ更新する
	//	SetBlendState(BS_NONE);			// アルファブレンド無効
	//}
}

//*******************************
//
//		ブロックの配置処理
//	
//	引数:
//		置きたい座標,無敵か？
//						true:無敵
//
//	戻り値:
//		使用したブロックの総数
//
//*******************************
int SetBlock(XMFLOAT3 pos, bool inv)
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	int Block = -1;

	for (int cntBlock = 0; cntBlock < MAX_BLOCK; ++cntBlock)
	{
		// 使用中ならスキップ
		if (g_block[cntBlock].m_use) {
			continue;
		}

		// モデルデータの読み込みif文
		if (inv == true)
		{
			// 無敵ブロックのモデルデータ
			g_block[cntBlock].m_3Dmodel = MODEL_INVINCIBLE;

			// 無敵オン
			g_block[cntBlock].m_invincible = true;
		}
		else
		{	// 通常ブロックのモデルデータ
			g_block[cntBlock].m_3Dmodel = MODEL_BLOCK;
		}
		g_model[cntBlock].Load(pDevice, pDeviceContext, g_block[cntBlock].m_3Dmodel);

		// 使用中ＯＮ
		g_block[cntBlock].m_use = true;
		g_block[cntBlock].m_pos = pos;

		Block = cntBlock + 1;
		break;
	}

	return Block;
}

//*******************************
//
//		ブロックのサイズ取得
//	
//	引数:なし
//
//	戻り値:
//		ブロックのサイズ
//
//*******************************
XMFLOAT3 GetBlockSize()
{
	return g_BlockScale;
}

//***********************************
//
//		ブロック配列取得
//		
//		戻り値：ブロックのサイズ
//
//***********************************
TBLOCK* GetBlockArray()
{
	return g_block;
}
