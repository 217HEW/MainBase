//**************************************************************
//
//	EnemyMelee.h
//	敵の原型ヘッダ
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/13	エネミーベースを元に追加
//	編集者：??
//
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "EnemyMelee.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"

//**************************************************************
// 構造体定義
//**************************************************************
struct TEnemy {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_move;		// 移動量
	bool		m_use;		// 使用してるか否か	ON:使用中

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
};

//**************************************************************
// マクロ定義
//**************************************************************
#define MODEL_ENEMY			"data/model/helicopter000.fbx"	// 敵モデル

#define	VALUE_MOVE_ENEMY	(0.40f)		// 移動速度
#define MAX_ENEMYMELEE			(10)		// 敵機最大数

//**************************************************************
// グローバル変数
//**************************************************************
static CAssimpModel	g_model;			// モデル情報
static TEnemy		g_EMelee[MAX_ENEMYMELEE];	// 敵機情報

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitEnemyMelee(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY))
	{
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < MAX_ENEMYMELEE; ++i)
	{// 初期化したいモノがあればここに↓

		g_EMelee[i].m_use = false;
	}

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitEnemyMelee(void)
{
	// モデルの解放
	g_model.Release();
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateEnemyMelee(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	for (int i = 0; i < MAX_ENEMYMELEE; ++i)
	{
		// ここに敵の挙動を入れる
		//	┗

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_EMelee[i].m_rot.x),
			XMConvertToRadians(g_EMelee[i].m_rot.y),
			XMConvertToRadians(g_EMelee[i].m_rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(
			g_EMelee[i].m_pos.x,
			g_EMelee[i].m_pos.y,
			g_EMelee[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&g_EMelee[i].m_mtxWorld, mtxWorld);
	}
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawEnemyMelee(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	for (int i = 0; i < MAX_ENEMYMELEE; ++i) {
		g_model.Draw(pDC, g_EMelee[i].m_mtxWorld, eOpacityOnly);
	}

	// 半透明部分を描画
	for (int i = 0; i < MAX_ENEMYMELEE; ++i) {
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		g_model.Draw(pDC, g_EMelee[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}
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
int SetEnemyMelee(XMFLOAT3 pos)
{
	// 戻り値の初期化
	int EnemyMelee = -1;

	for (int cntEnemyMelee = 0; cntEnemyMelee < MAX_ENEMYMELEE; ++cntEnemyMelee)
	{
		// 使用中ならスキップ
		if (g_EMelee[cntEnemyMelee].m_use)
		{
			continue;
		}
		g_EMelee[cntEnemyMelee].m_use = true;	// 使用中ON
		g_EMelee[cntEnemyMelee].m_pos = pos;	// 指定した座標を代入

		EnemyMelee = cntEnemyMelee +1;	// 使用中の敵数を代入
		break;
	}

	return EnemyMelee;
}
