//**************************************************************
//
//	EnemyMelee.h
//	プレイヤーを追従する近接敵
//
//--------------------------------------------------------------
//	製作者：石原聖斗
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/15 プレイヤーを追従する動きの実装
//	2021/12/18 敵同士、壁、プレイヤーとの当たり判定の実装
//	2021/12/21 壁と当たったら止まる(全方位)処理の実装
//	2021/12/13	エネミーベースを元に追加
//	編集者：??
//--------------------------------------------------------------
//　2021/12/30	プレイヤーの境界球半径を取得する関数名の変更
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//　2022/01/22	近接敵の情報を取得する関数の作成
//				プレイヤーの体力が０になった時、ゲームオーバーに
//				シーン遷移する処理はここでやるべきでないため削除
//	編集者：柴山凜太郎
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "EnemyMelee.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "Block.h"
#include "explosion.h"
#include "life.h"
#include "SceneManager.h"
#include "PlayEffect.h"

//**************************************************************
// マクロ定義
//**************************************************************
//#define MODEL_ENEMY			"data/model/helicopter000.fbx"
#define MODEL_ENEMY			"data/model/Melee/Melee.fbx"

#define	VALUE_MOVE_ENEMY		(1.0f)		// 移動速度
#define MAX_ENEMYMELEE			(10)		// 近接敵最大数

#define	VALUE_ROTATE_ENEMY		(7.0f)		// 回転速度
#define	RATE_ROTATE_ENEMY		(0.20f)		// 回転慣性係数

#define SEARCH_ENEMY			(250)		// 探索範囲


//**************************************************************
// グローバル変数
//**************************************************************
static CAssimpModel		g_model;					// モデル情報
static TEnemyMelee		g_EMelee[MAX_ENEMYMELEE];	// 敵機情報
static XMFLOAT3			Blocksize;
static int g_EffectTimer = 0;		// エフェクト制御用タイマー
Effect g_MeleeEffect[MAX_ENEMYMELEE];
//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitEnemyMelee(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ブロックのサイズ取得
	Blocksize = GetBlockSize();

	// モデルデータの読み込み
	g_model.SetDif(XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f));
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY))
	{
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		return E_FAIL;
	}
	g_model.SetDif(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	for (int i = 0; i < MAX_ENEMYMELEE; ++i)
	{// 初期化したいモノがあればここに↓
		g_EMelee[i].m_pos = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EMelee[i].m_size = (XMFLOAT3(10.0f, 10.0f, 10.0f));
		g_EMelee[i].m_move = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EMelee[i].m_rot = (XMFLOAT3(90.0f, 90.0f, 0.0f));
		g_EMelee[i].m_rotDest = g_EMelee[i].m_rot;
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
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	TBLOCK *Block = GetBlockArray();
	//プレイヤーの座標・サイズ取得
	XMFLOAT3 posPlayer = GetPlayerPos();
	float sizePlayer = GetPlayerRadSize();

	for (int i = 0; i < MAX_ENEMYMELEE; ++i)
	{
		//ブロック配列取得

		//敵とプレイヤーの距離が近づいたら
		//if (CollisionSphere(posPlayer, sizePlayer, g_EMelee[i].m_pos, SEARCH_ENEMY))
		//{
		//	if (!g_EMelee[i].m_use)
		//	{//未使用なら次へ
		//		continue;
		//	}

		//	//常にプレイヤーの方向を向く
		//	/*g_EMelee[i].m_rotDest = posPlayer;
		//	g_EMelee[i].m_rot = g_EMelee[i].m_rotDest;
		//	g_EMelee[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/

		//	//敵のx座標がプレイヤーよりも大きかったら
		//	if (g_EMelee[i].m_pos.x >= posPlayer.x)
		//	{
		//		g_EMelee[i].m_pos.x += -VALUE_MOVE_ENEMY;
		//		g_EMelee[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f) );

		//		g_EMelee[i].m_rotDest.y = rotCamera.y - 90.0f;
		//	}
		//	//敵のx座標がプレイヤーよりも小さかったら
		//	if (g_EMelee[i].m_pos.x <= posPlayer.x)
		//	{
		//		g_EMelee[i].m_pos.x += VALUE_MOVE_ENEMY;
		//		g_EMelee[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 90.0f));
		//		g_EMelee[i].m_rotDest.y = rotCamera.y + 90.0f;

		//	}
		//	//敵のy座標がプレイヤーよりも大きかったら
		//	if (g_EMelee[i].m_pos.y >= posPlayer.y)
		//	{
		//		g_EMelee[i].m_pos.y += -VALUE_MOVE_ENEMY;

		//	}
		//	//敵のy座標がプレイヤーよりも小さかったら
		//	if (g_EMelee[i].m_pos.y <= posPlayer.y)
		//	{
		//		g_EMelee[i].m_pos.y += VALUE_MOVE_ENEMY;

		//	}
		//}
			//**************************************************************************************
			//		当たり判定
			//**************************************************************************************

			// 敵同士の当たり判定
		for (int j = 0; j < MAX_ENEMYMELEE; ++j)
		{
			if (CollisionSphere(g_EMelee[i].m_pos, g_EMelee[i].m_size.x, g_EMelee[j].m_pos, g_EMelee[j].m_size.x))
			{
				if (i == j)
				{//同じ敵ならとばす
					continue;
				}
				//敵同士が重ならない処理

			}
		}

		// 敵と壁の当たり判定
		for (int j = 0; j < MAX_BLOCK; ++j, ++Block)
		{
			if (!Block->m_use)
			{// 未使用なら次へ
				continue;
			}
			if (CollisionAABB(g_EMelee[i].m_pos, g_EMelee[i].m_size, Block->m_pos, Blocksize))
			{
				//壁に当たると止まる
				//右
				if (Block->m_pos.x + Blocksize.x / 2 <= g_EMelee[i].m_pos.x - g_EMelee[i].m_size.x / 2)
				{
					g_EMelee[i].m_pos.x = (Block->m_pos.x + Blocksize.x + g_EMelee[i].m_size.x);
				}
				//左
				else if (Block->m_pos.x - Blocksize.x / 2 >= g_EMelee[i].m_pos.x + g_EMelee[i].m_size.x / 2)
				{
					g_EMelee[i].m_pos.x = (Block->m_pos.x - Blocksize.x - g_EMelee[i].m_size.x);
				}
				//上
				else if (Block->m_pos.y - Blocksize.y / 2 >= g_EMelee[i].m_pos.y + g_EMelee[i].m_size.y / 2)
				{
					g_EMelee[i].m_pos.y = (Block->m_pos.y - Blocksize.y - g_EMelee[i].m_size.y);
				}
				//下
				else if (Block->m_pos.y + Blocksize.y / 2 <= g_EMelee[i].m_pos.y - g_EMelee[i].m_size.y / 2)
				{
					g_EMelee[i].m_pos.y = (Block->m_pos.y + Blocksize.y + g_EMelee[i].m_size.y);
				}
			}
		}

		// エフェクトセット＆制御
		//if (g_EffectTimer == 0)
		//{
		//	g_MeleeEffect[i].Set(0, g_EMelee[i].m_pos, XMFLOAT3(1.0f, 1.0f, 1.0f), 0.5f, XMFLOAT3(1.0f,1.0f,1.0f));
		//	g_EffectTimer = 30;
		//}
		//--g_EffectTimer;

		// 敵とプレイヤーの当たり判定
		if (!g_EMelee[i].m_use)
		{// 未使用なら次へ
			continue;
		}
		if (CollisionSphere(g_EMelee[i].m_pos, g_EMelee[i].m_size.x, posPlayer, sizePlayer))
		{
			DelLife();
			g_EMelee[i].m_use = false;
		}

		//目的の角度までの差分
		float fDiffRotY = g_EMelee[i].m_rotDest.y - g_EMelee[i].m_rot.y;
		if (fDiffRotY >= 180.0f) {
			fDiffRotY -= 360.0f;
		}
		if (fDiffRotY < -180.0f) {
			fDiffRotY += 360.0f;
		}

		// 目的の角度まで慣性をかける
		g_EMelee[i].m_rot.y += fDiffRotY * RATE_ROTATE_ENEMY;
		if (g_EMelee[i].m_rot.y >= 180.0f) {
			g_EMelee[i].m_rot.y -= 360.0f;
		}
		if (g_EMelee[i].m_rot.y < -180.0f) {
			g_EMelee[i].m_rot.y += 360.0f;
		}
		g_EMelee[i].m_move = XMFLOAT3(
			-SinDeg(g_EMelee[i].m_rot.y) * VALUE_MOVE_ENEMY,
			0.0f,
			-CosDeg(g_EMelee[i].m_rot.y) * VALUE_MOVE_ENEMY);

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
		mtxScale = XMMatrixScaling(4.0f, 8.0f, 4.0f);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);
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
		if (!g_EMelee[i].m_use)
		{
			continue;
		}
		g_model.Draw(pDC, g_EMelee[i].m_mtxWorld, eOpacityOnly);
	}

	// // 半透明部分を描画
	// for (int i = 0; i < MAX_ENEMYMELEE; ++i) {
	// 	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	// 	SetZWrite(false);				// Zバッファ更新しない
	// 	g_model.Draw(pDC, g_EMelee[i].m_mtxWorld, eTransparentOnly);
	// 	SetZWrite(true);				// Zバッファ更新する
	// 	SetBlendState(BS_NONE);			// アルファブレンド無効
	// }
}

//*****************************************************************
//
//	敵の配置処理
//	
//	引数:配置したい座標 x y z
//
//	戻り値
//		:使用している敵の最大数
//
//****************************************************************
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

		EnemyMelee = cntEnemyMelee + 1;	// 使用中の敵数を代入
		break;
	}

	return EnemyMelee;
}

//**************************************************************
// 近接敵情報取得
//
// 戻り値：近接敵構造体変数
//**************************************************************
TEnemyMelee* GetEnemyMelee()
{
	return g_EMelee;
}

//**************************************************************
// 近接敵との当たり判定を行う関数
//
//引数：pos 座標
//		radius	半径
//
// 戻り値：近接敵構造体変数
//**************************************************************
bool CollisionEnemyMelee(XMFLOAT3 pos, float radius)
{
	bool hit = 0;
	for (int i = 0; i < MAX_ENEMYMELEE; i++)
	{
		if (!g_EMelee[i].m_use)
			continue;
		hit = CollisionSphere(g_EMelee[i].m_pos, g_EMelee[i].m_size.x, pos, radius);
		if (hit) {
			g_EMelee[i].m_use = false;
			return true;
		}
	}
	return false;
}