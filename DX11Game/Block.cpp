//**************************************************************
//
//	Block.cpp
//	ブロック(壁箱)のcpp
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//	開発履歴
//	2021/12/21	　初期Wall.cppから改造して制作	
//				　通常ブロック、ひび割れブロックの当たり判定と切り替えを実装
//				　半透明処理が不要なのでFPS向上の為、コメントアウト(Draw)
//																		変更者：上月大地
//	2021/12/21	　通常ブロック、ひび割れブロックの当たり判定と切り替えを実装	
//	2021/12/21	　通常ブロック、ひび割れブロックの当たり判定と切り替えを実装	
//	2021/12/21	　ブロックのサイズを構造体の要素から、グローバル変数へ　||変更者：柴山凜太郎
//	
//**************************************************************
#include "Block.h"
#include "main.h"
#include "AssimpModel.h"
#include "Texture.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_BLOCK			"data/model/Hew_3_1.fbx"		// 通常ブロック
#define MODEL_CRACKS		"data/model/Block2.fbx"			// ひび割れたブロック

#define MAX_LIFE		(2)		// ブロック耐久値
#define BLOCK_X			(23)	// ブロック最大数X
#define BLOCK_Y			(25)	// ブロック最大数Y
#define MAX_BLOCK		(BLOCK_X * BLOCK_Y)					// ブロック最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TBLOCK {
	XMFLOAT3	m_pos;		// 現在の位置
	//XMFLOAT3    m_size;	// 現在のサイズ
	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
 std::string	m_3Dmodel;	// モデル情報
	int			m_nLife;	// 壁の耐久置
	bool		m_use;		// 使用しているか
	bool		m_invincible;// 無敵かどうか　ON：無敵
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel	g_model[MAX_BLOCK];	// モデル
static TBLOCK		g_block[MAX_BLOCK];	// 壁情報
XMFLOAT3			g_BlockSize;		// 現在のサイズ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBlock(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
 	for (int i = 0; i < MAX_BLOCK; ++i)
 	{
 		g_BlockSize = XMFLOAT3(20.0f, 40.0f, 10.0f);
 		// g_wall->m_pos = XMFLOAT3(0.0f, 50.0f, 150.0f);
		g_block[i].m_3Dmodel = MODEL_BLOCK;
 		g_block[i].m_nLife = MAX_LIFE;
 		g_block[i].m_use = false;
		g_block[i].m_invincible = false;
		// モデルデータの読み込み
		if (!g_model[i].Load(pDevice, pDeviceContext, g_block[i].m_3Dmodel))
		{
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitBlock", MB_OK);
			return E_FAIL;
		}
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
//	戻り値
//		使用したブロックの総数
//
//*******************************
 int SetBlock(XMFLOAT3 pos)
 {
 	int Block = -1;
 
 	for (int cntBlock = 0; cntBlock < MAX_BLOCK; ++cntBlock) {
 		// 使用中ならスキップ
 		if (g_block[cntBlock].m_use) {
 			continue;
 		}
 		g_block[cntBlock].m_use = true;
 		g_block[cntBlock].m_pos = pos;
 
		Block = cntBlock +1;
 		break;
 	}
 
 	return Block;
 }

XMFLOAT3 GetBlockSize()
{
	return g_BlockSize;
}
