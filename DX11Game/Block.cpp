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
//
//**************************************************************
#include "Block.h"
#include "main.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_WALL			"data/model/Block.fbx"
// #define BLOCK_TEXTURE		"data/model/Block.jpg"
#define MAX_LIFE			(2)			// 壁耐久置
#define MAX_BLOCK			(256)		// 壁最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TBLOCK {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3    m_size;		// 現在のサイズ
	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス

	int			m_nLife;	// 壁の耐久置
	bool		use;		// 使用しているか

};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAssimpModel	g_model;			// モデル
static TBLOCK		g_wall[MAX_BLOCK];	// 壁情報

//=============================================================================
// 初期化処理
//=============================================================================
// HRESULT InitBlock(void)
// {
// 	HRESULT hr = S_OK;
// 	ID3D11Device* pDevice = GetDevice();
// 	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
// 
// 	for (int i = 0; i < MAX_WALL; ++i)
// 	{
// 		g_wall[i].m_size = XMFLOAT3(38.0f, 38.0f, 38.0f);
// 		//g_wall->m_pos = XMFLOAT3(0.0f, 50.0f, 150.0f);
// 		g_wall[i].m_nLife = MAX_LIFE;
// 		g_wall[i].use = false;
// 	}
// 
// 	// モデルデータの読み込み
// 	if (!g_model.Load(pDevice, pDeviceContext, MODEL_WALL)) {
// 		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitWall", MB_OK);
// 		return E_FAIL;
// 	}
// 
// 	return hr;
// }

//=============================================================================
// 終了処理
//=============================================================================
// void UninitBlock(void)
// {
// 	// モデルの解放
// 	g_model.Release();
// }

//=============================================================================
// 更新処理
//=============================================================================
// void UpdateBlock(void)
// {
// 
// 	XMMATRIX mtxWorld, mtxRot, mtxTranslate;
// 
// 	for (int i = 0; i < MAX_WALL; ++i)
// 	{
// 		// ワールドマトリックスの初期化
// 		mtxWorld = XMMatrixIdentity();
// 
// 		// 回転を反映
// 		mtxRot = XMMatrixRotationRollPitchYaw(
// 			XMConvertToRadians(g_wall[i].m_rot.x),
// 			XMConvertToRadians(g_wall[i].m_rot.y),
// 			XMConvertToRadians(g_wall[i].m_rot.z));
// 		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);
// 		mtxWorld = XMMatrixScaling(10.0f, 10.0f, 10.0f);
// 		// 移動を反映
// 		mtxTranslate = XMMatrixTranslation(
// 			g_wall[i].m_pos.x,
// 			g_wall[i].m_pos.y,
// 			g_wall[i].m_pos.z);
// 		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);
// 
// 		// ワールドマトリックス設定
// 		XMStoreFloat4x4(&g_wall[i].m_mtxWorld, mtxWorld);
// 
// 	}
// 
// 	for (int i = 0; i < MAX_WALL; ++i)
// 	{
// 		if (!g_wall[i].use)
// 		{// 未使用なら次へ
// 			continue;
// 		}
// 
// 		// 壁とプレイヤーが衝突していたら
// 		if (CollisionWall(g_wall[i].m_pos, g_wall[i].m_size, GetPlayerPos(), GetPlayerSize()))
// 		{
// 
// 			StartExplosion(g_wall[i].m_pos, XMFLOAT2(80.0f, 80.0f));
// 			g_wall[i].m_nLife--;
// 
// 			if (g_wall[i].m_nLife <= 0)
// 			{
// 				g_wall[i].use = false;
// 			}
// 		}
// 	}
// 
// 
// }

//=============================================================================
// 描画処理
//=============================================================================
// void DrawBlock(void)
// {
// 	ID3D11DeviceContext* pDC = GetDeviceContext();
// 
// 
// 	// 不透明部分を描画
// 	for (int i = 0; i < MAX_WALL; ++i)
// 	{
// 		if (!g_wall[i].use)
// 		{
// 			continue;
// 		}
// 		g_model.Draw(pDC, g_wall[i].m_mtxWorld, eOpacityOnly);
// 	}
// 
// 	// 半透明部分を描画
// 	for (int i = 0; i < MAX_WALL; ++i) {
// 		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
// 		SetZWrite(false);				// Zバッファ更新しない
// 		g_model.Draw(pDC, g_wall[i].m_mtxWorld, eTransparentOnly);
// 		SetZWrite(true);				// Zバッファ更新する
// 		SetBlendState(BS_NONE);			// アルファブレンド無効
// 
// 
// 	}
// 
// 
// }

//===============================================================================
//
//  　　壁配置
//
//===============================================================================
// int SetBlock(XMFLOAT3 pos)
// {
// 	int Wall = -1;
// 
// 	for (int cntWall = 0; cntWall < MAX_WALL; ++cntWall) {
// 		// 使用中ならスキップ
// 		if (g_wall[cntWall].use) {
// 			continue;
// 		}
// 		g_wall[cntWall].use = true;
// 		g_wall[cntWall].m_pos = pos;
// 
// 		Wall = cntWall;
// 		break;
// 	}
// 
// 	return Wall;
// }
