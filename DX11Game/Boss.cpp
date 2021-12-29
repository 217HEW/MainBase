//**************************************************************
//
//	Boss.cpp
//	ボスを動かす処理
//
//--------------------------------------------------------------
//	製作者：柴山凜太郎
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//**************************************************************
//	2021/12/27　Boss大方のガワ処理作成
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "Boss.h"
#include "AssimpModel.h"
#include "collision.h"
#include "player.h"
#include "Block.h"
#include "life.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define MODEL_BOSS			"data/model/helicopter000.fbx"	// ボスモデル

#define	VALUE_MOVE_BOSS		(0.40f)		// 移動速度
#define	BOSS_SIZE_X			(100)		// モデルの横幅
#define	BOSS_SIZE_Y			(100)		// モデルの縦幅
#define	MAX_BOSS_LIFE		(100)		// 最大体力

//**************************************************************
// 構造体定義
//**************************************************************
struct TBoss {
	XMFLOAT3	pos;		// 現在の位置
	XMFLOAT3	size;		// 大きさ
	XMFLOAT3	rot;		// 現在の向き
	XMFLOAT3	rotDest;	// 目的の向き
	XMFLOAT3	move;		// 移動量
	int			life;		// 体力
	bool		use;		// 使用してるか否か	ON:使用中

	XMFLOAT4X4	mtxWorld;	// ワールドマトリックス
};

//**************************************************************
// グローバル変数
//**************************************************************
static CAssimpModel	g_model;			// モデル情報
static TBoss		g_boss;			// 敵機情報

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitBoss(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// モデルデータの読み込み-------------------------------
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_BOSS))
	{
		MessageBoxA(GetMainWnd(), "Bossモデルデータ読み込みエラー", "InitBoss", MB_OK);
		return E_FAIL;
	}

	// 変数の初期化-----------------------------------------
	g_boss.pos  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_boss.size = XMFLOAT3(BOSS_SIZE_X, BOSS_SIZE_Y, 0.0f);
	g_boss.rot  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_boss.rotDest = g_boss.rot;
	g_boss.move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_boss.life = MAX_BOSS_LIFE;
	g_boss.use  = false;

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitBoss(void)
{
	// モデルの解放
	g_model.Release();
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateBoss(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ここにbossの挙動を入れる
	//	┗

// -------ワールドマトリクス制御------------------------------------------

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_boss.rot.x),
				XMConvertToRadians(g_boss.rot.y),
				XMConvertToRadians(g_boss.rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(
					g_boss.pos.x,
					g_boss.pos.y,
					g_boss.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&g_boss.mtxWorld, mtxWorld);
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawBoss(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	g_model.Draw(pDC, g_boss.mtxWorld, eOpacityOnly);
	

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	g_model.Draw(pDC, g_boss.mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効
	
}

//*******************************
//
//	敵の配置処理
//	
//	引数:配置したい座標 x y z
//
//	戻り値
//		:使用している敵の最大数
//
//*******************************
int SetBoss(XMFLOAT3 pos)
{
	// 戻り値の初期化
	int Enemy = 1;

	g_boss.use = true;	// 使用中ON
	g_boss.pos = pos;	// 指定した座標を代入

	return Enemy;
}