//**************************************************************
//
//	Gate.cpp
//	ゲームシーン開始時のゲートの処理
//
//--------------------------------------------------------------
//	製作者：柴山凜太郎
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/21	ゲートの作成開始
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/30	for文を回すための変数の初期値を0→1に変更
//				そのためSetGateの戻り値の計算「Gate = cntGate + 1」
//				から「+1」を削除
//	編集者：柴山凜太郎
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "Gate.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define MODEL_GATE		"data/model/Block.fbx"	// ゲートモデル
#define GATE_SIZE		(XMFLOAT3(20.0f,40.0f,10.0f))	// ゲートの大きさ
#define MAX_GATE		(2)		// 


//**************************************************************
// グローバル変数
//**************************************************************
static CAssimpModel	g_model;	// モデル
static TGATE		g_gate[MAX_GATE];	// 壁情報
static XMFLOAT3		g_GateSize;		// 現在のサイズ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGate(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// ------ブロックの初期化-----------------------------------------------------
	for (int i = 1; i < MAX_GATE; ++i)
	{
		//Xが二倍になる為、XをYの二分の一にしておく
		//g_gate[i].model3D = MODEL_GATE;
		g_GateSize = GATE_SIZE;
		g_gate[i].use = false;
		g_gate[i].invincible = false;
		
	}
	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_GATE))
	{
		MessageBoxA(GetMainWnd(), "ゲートモデルデータ読み込みエラー", "InitGate", MB_OK);
		return E_FAIL;
	}
	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGate(void)
{
	g_model.Release();	
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGate(void)
{
	//------ワールドマトリクスにブロックのデータを反映----------------------------

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	for (int i = 0; i < MAX_GATE; ++i)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 箱のサイズ
		mtxWorld = XMMatrixScaling(g_GateSize.x,
								   g_GateSize.y,
								   g_GateSize.z);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(
					   g_gate[i].pos.x,
					   g_gate[i].pos.y,
					   g_gate[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_gate[i].mtxWorld, mtxWorld);

	}

	XMFLOAT3 PlayerSize = GetPlayerSize();
	XMFLOAT3 PlayerPos = GetPlayerPos();

	//------ブロックとプレイヤーの当たり判定処理-------------------------------
	for (int i = 0; i < MAX_GATE; ++i)
	{
		if (!g_gate[i].use)
		// 未使用なら次へ
			continue;
		
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGate(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	for (int i = 0; i < MAX_GATE; ++i)
	{
		// 使っているブロックの描画
		if (!g_gate[i].use)
		{
			continue;
		}
		// ブロックモデル描画
		g_model.Draw(pDC, g_gate[i].mtxWorld, eOpacityOnly);
	}

	// 半透明部分を描画
	//for (int i = 0; i < MAX_GATE; ++i) {
	//	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	//	SetZWrite(false);				// Zバッファ更新しない
	//	g_model[i].Draw(pDC, g_gate[i].m_mtxWorld, eTransparentOnly);
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
int SetGate(XMFLOAT3 pos)
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	int Gate = -1;

	for (int cntGate = 1; cntGate < MAX_GATE; ++cntGate) {
		// 使用中ならスキップ
		if (g_gate[cntGate].use) {
			continue;
		}
		g_gate[cntGate].use = true;	// 使用中
		g_gate[cntGate].pos = pos;	// 座標設定

		Gate = cntGate;
		break;
	}

	return Gate;
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
XMFLOAT3 GetGateSize()
{
	return g_GateSize;
}

//***********************************
//
//		ブロック配列取得
//		
//		戻り値：ブロックのサイズ
//
//***********************************
TGATE* GetGateArray()
{
	return g_gate;
}
