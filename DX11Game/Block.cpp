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
//	2021/12/21	初期Wall.cppから改造して制作	
//				通常ブロック、ひび割れブロックの当たり判定と切り替えを実装
//				半透明処理が不要なのでFPS向上の為、コメントアウト(Draw)
//--------------------------------------------------------------
//	2021/12/21	通常ブロック、ひび割れブロックの当たり判定と切り替えを実装	
//	2021/12/21	通常ブロック、ひび割れブロックの当たり判定と切り替えを実装	
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/21	ブロックのサイズを構造体の要素から、グローバル変数へ
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2121/12/22	コメントの編集&追加、不要なソースの削除
//**************************************************************
#include "Block.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_BLOCK			"data/model/Block.fbx"	// 通常ブロック
//#define TEXTURE_BLOCK		"data/model/Block.jpg"	// 通常ブロック
#define MODEL_CRACKS		"data/model/Block2.fbx"	// ひび割れたブロック
#define MAX_LIFE			(2)			// 壁の耐久値

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel	g_model[MAX_BLOCK];	// モデル
static TBLOCK		g_block[MAX_BLOCK];	// 壁情報
static XMFLOAT3		g_BlockSize;		// 現在のサイズ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBlock(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
 // ------ブロックの初期化-----------------------------------------------------
	for (int i = 0; i < MAX_BLOCK; ++i)
 	{
		//Xが二倍になる為Yの二分の一にしておく
 		g_BlockSize = XMFLOAT3(20.0f, 40.0f, 10.0f);
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
 //------ワールドマトリクスにブロックのデータを反映----------------------------

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 箱のサイズ
		mtxWorld = XMMatrixScaling(g_BlockSize.x,
								   g_BlockSize.y, 
								   g_BlockSize.z);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(
			g_block[i].m_pos.x,
			g_block[i].m_pos.y,
			g_block[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_block[i].m_mtxWorld, mtxWorld);

	}

//------ブロックとプレイヤーの当たり判定処理-------------------------------
	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		if (!g_block[i].m_use)
		{// 未使用なら次へ
			continue;
		}
	
		// 壁とプレイヤーが衝突していたら
		 if (CollisionAABB(g_block[i].m_pos, g_BlockSize, GetPlayerPos(), XMFLOAT3(5.0f,5.0f,10.0f)))
		 {
			 // プレイヤーがとんでいたら

			 ID3D11Device* pDevice = GetDevice();
			 ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

			// ブロックにひびが入る
			g_block[i].m_3Dmodel = MODEL_CRACKS;
			g_model[i].Load(pDevice, pDeviceContext, g_block[i].m_3Dmodel);
			g_block[i].m_nLife--;
		 	
		 	// 体力が無くなったら使わない
		 	if (g_block[i].m_nLife <= 0)
		 	{
		 		g_block[i].m_use = false;
				g_model[i].Release();
		 	}
		 }
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
//		置きたい座標
//
//	戻り値:
//		使用したブロックの総数
//
//*******************************
 int SetBlock(XMFLOAT3 pos)
 {
	 ID3D11Device* pDevice = GetDevice();
	 ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
 	int Block = -1;
 
 	for (int cntBlock = 0; cntBlock < MAX_BLOCK; ++cntBlock) {
 		// 使用中ならスキップ
 		if (g_block[cntBlock].m_use) {
 			continue;
 		}
 		g_block[cntBlock].m_use = true;	// 使用中
 		g_block[cntBlock].m_pos = pos;	// 座標設定
 
		Block = cntBlock +1;
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
	return g_BlockSize;
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
