//**************************************************************
//
//	EnemyExplode.cpp
//	プレイヤーが接近すると爆発する敵
//
//--------------------------------------------------------------
//	製作者：澤村瑠人
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//**************************************************************
//	2021/12/24　EnemyExplode作成
//	編集者：澤村瑠人
//--------------------------------------------------------------
//　2021/12/25	EnemyExplodeの範囲判定完了	
//	編集者：澤村瑠人
//--------------------------------------------------------------
//　2021/12/29	爆発までの猶予(5秒)の完了	
//	編集者：澤村瑠人
//--------------------------------------------------------------
//　2021/12/31	モデルのスケールを組み込みました。347行	
//	編集者：上月大地
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	コメント（意見）
//	2021/12/29	壁を這うのはむずいかもです... _澤村瑠人
//	2021/12/29	プランナーに相談してみます	  _上月大地
//**************************************************************




//**************************************************************
// インクルード部
//**************************************************************
#include "EnemyExplode.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "Block.h"
#include "explosion.h"
#include "life.h"
//#include "timer.h"
#include "SceneManager.h"
#include <stdlib.h>

//**************************************************************
// 構造体定義
//**************************************************************
struct TEnemyExplode {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_size;		// 現在のサイズ
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_move;		// 移動量
	bool		m_use;		// 使用してるか否か	ON:使用中

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
	int			m_speed;	// スピード
};

//**************************************************************
// マクロ定義
//**************************************************************
#define MODEL_ENEMY			"data/model/Explode/Explode.fbx"//"data/model/sentouki.fbx"
//#define MODEL_ENEMY			"data/model/enemy3.fbx"

#define	VALUE_MOVE_ENEMY		(1.0f)		// 移動速度
#define MAX_ENEMYEXPLODE		(10)		// 敵機最大数

#define	VALUE_ROTATE_ENEMY		(7.0f)		// 回転速度
#define	RATE_ROTATE_ENEMY		(0.20f)		// 回転慣性係数

#define SEARCH_ENEMY			(250)		// 探索範囲

#define ENEMY_EXPLODE_TIMER_START (5)// 制限時間



//**************************************************************
// グローバル変数
//**************************************************************
static CAssimpModel			g_model;	// モデル情報
static TEnemyExplode		g_EExplode[MAX_ENEMYEXPLODE];	// 敵機情報
static XMFLOAT3				Blocksize;
static int g_nEETimer;		// 時間をカウントする

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitEnemyExplode(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ブロックのサイズ取得
	Blocksize = GetBlockSize();

	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY))
	{
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < MAX_ENEMYEXPLODE; ++i)
	{// 初期化したいモノがあればここに↓
		g_EExplode[i].m_pos = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EExplode[i].m_size = (XMFLOAT3(20.0f, 10.0f, 10.0f));
		g_EExplode[i].m_move = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EExplode[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f));
		g_EExplode[i].m_rotDest = g_EExplode[i].m_rot;
		g_EExplode[i].m_use = false;
	}

	//変数初期化
	g_nEETimer = ENEMY_EXPLODE_TIMER_START * 60 + 59;

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitEnemyExplode(void)
{
	// モデルの解放
	g_model.Release();
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateEnemyExplode(void)
{
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	//プレイヤーの座標・サイズ取得
	XMFLOAT3 posPlayer = GetPlayerPos();
	float sizePlayer = GetPlayerSize();

	// タイマーカウントダウン
	if (g_nEETimer > 0)
	{
		--g_nEETimer;
	}
	else
	{
		g_nEETimer;
	}

	for (int i = 0; i < MAX_ENEMYEXPLODE; ++i)
	{
		//ブロック配列取得
		TBLOCK *Block = GetBlockArray();

		//敵とプレイヤーの距離が近づいたら
		if (CollisionSphere(posPlayer, sizePlayer, g_EExplode[i].m_pos, SEARCH_ENEMY))
		{
			if (!g_EExplode[i].m_use)
			{//未使用なら次へ
				continue;
			}

			//常にプレイヤーの方向を向く
			/*g_EExplode[i].m_rotDest = posPlayer;
			g_EExplode[i].m_rot = g_EExplode[i].m_rotDest;
			g_EExplode[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/

			//*********************************
			//追従処理
			//*********************************

			//敵のx座標がプレイヤーよりも大きかったら
			if (g_EExplode[i].m_pos.x >= posPlayer.x)
			{
				g_EExplode[i].m_pos.x += -VALUE_MOVE_ENEMY;
				g_EExplode[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f) );

				g_EExplode[i].m_rotDest.y = rotCamera.y - 90.0f;
			}
			//敵のx座標がプレイヤーよりも小さかったら
			if (g_EExplode[i].m_pos.x <= posPlayer.x)
			{
				g_EExplode[i].m_pos.x += VALUE_MOVE_ENEMY;
				g_EExplode[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 90.0f));
				g_EExplode[i].m_rotDest.y = rotCamera.y + 90.0f;

			}
			//敵のy座標がプレイヤーよりも大きかったら
			if (g_EExplode[i].m_pos.y >= posPlayer.y)
			{
				g_EExplode[i].m_pos.y += -VALUE_MOVE_ENEMY;

			}
			//敵のy座標がプレイヤーよりも小さかったら
			if (g_EExplode[i].m_pos.y <= posPlayer.y)
			{
				g_EExplode[i].m_pos.y += VALUE_MOVE_ENEMY;

			}
			
			
			
			



			//**************************************************************************************
			//		当たり判定
			//**************************************************************************************

			// 敵同士の当たり判定
			for (int j = 0; j < MAX_ENEMYEXPLODE; ++j)
			{
				if (CollisionSphere(g_EExplode[i].m_pos, g_EExplode[i].m_size.x, g_EExplode[j].m_pos, g_EExplode[j].m_size.x))
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
				if (CollisionAABB(g_EExplode[i].m_pos, g_EExplode[i].m_size, Block->m_pos,Blocksize))
				{
					//壁に当たると止まる
					//右
					if (Block->m_pos.x + Blocksize.x / 2 <= g_EExplode[i].m_pos.x - g_EExplode[i].m_size.x / 2)
					{
					 	g_EExplode[i].m_pos.x = (Block->m_pos.x + Blocksize.x + g_EExplode[i].m_size.x);
					}
					//左
					else if (Block->m_pos.x - Blocksize.x / 2 >= g_EExplode[i].m_pos.x + g_EExplode[i].m_size.x / 2)
					{
					 	g_EExplode[i].m_pos.x = (Block->m_pos.x - Blocksize.x - g_EExplode[i].m_size.x);
					}
					//上
					else if (Block->m_pos.y - Blocksize.y / 2 >= g_EExplode[i].m_pos.y + g_EExplode[i].m_size.y / 2)
					{
						g_EExplode[i].m_pos.y = (Block->m_pos.y - Blocksize.y - g_EExplode[i].m_size.y);
					}
					//下
					else if (Block->m_pos.y + Blocksize.y / 2 <= g_EExplode[i].m_pos.y - g_EExplode[i].m_size.y / 2)
					{
						g_EExplode[i].m_pos.y = (Block->m_pos.y + Blocksize.y + g_EExplode[i].m_size.y);
					}
				}
			}

			// 敵とプレイヤーの当たり判定
			if (!g_EExplode[i].m_use)
			{// 未使用なら次へ
				continue;
			}

			//*********************************
			//敵とプレイヤーの距離測定処理
			//*********************************
			//※)毎回SetSceneを読み込む必要は無いが現状放置
			//※)完成してもエリアに入った瞬間爆発が当たる為、制限は必要
			//※)XMStoreMatrixがあいまいといわれるがおそらく、いきなり絶対値を用いているからと推測される
			//敵とプレイヤーのX座標が"10.0f"以内であるならば爆発(消滅)する
			//if (g_EExplode[i].m_pos.x - posPlayer.x <= 10.0f)
			
				if (abs(g_EExplode[i].m_pos.x - posPlayer.x <= 10.0f))
				{
					// 敵とプレイヤーのY座標が"10.0f"以内であるならば爆発(消滅)する
						//if (g_EExplode[i].m_pos.y - posPlayer.y <= 10.0f)
					if (abs(g_EExplode[i].m_pos.y - posPlayer.y <= 10.0f))
					{
						if (g_nEETimer <= 0)
						{
							DelLife();
							if (GetLife() == 0)
							{
								SetScene(SCENE_GAMEOVER);
							}
							g_EExplode[i].m_use = false;
						}
					}
				}

			

			//if (CollisionSphere(g_EExplode[i].m_pos, g_EExplode[i].m_size.x, posPlayer, sizePlayer))
			//{
			//	DelLife();
			//	if (GetLife() == 0)
			//	{
			//		SetScene(SCENE_GAMEOVER);
			//	}
			//	g_EExplode[i].m_use = false;
			//}

			 //目的の角度までの差分
			float fDiffRotY = g_EExplode[i].m_rotDest.y - g_EExplode[i].m_rot.y;
			if (fDiffRotY >= 180.0f) {
				fDiffRotY -= 360.0f;
			}
			if (fDiffRotY < -180.0f) {
				fDiffRotY += 360.0f;
			}

			// 目的の角度まで慣性をかける
			g_EExplode[i].m_rot.y += fDiffRotY * RATE_ROTATE_ENEMY;
			if (g_EExplode[i].m_rot.y >= 180.0f) {
				g_EExplode[i].m_rot.y -= 360.0f;
			}
			if (g_EExplode[i].m_rot.y < -180.0f) {
				g_EExplode[i].m_rot.y += 360.0f;
			}
			g_EExplode[i].m_move = XMFLOAT3(
				-SinDeg(g_EExplode[i].m_rot.y) * VALUE_MOVE_ENEMY,
				0.0f,
				-CosDeg(g_EExplode[i].m_rot.y) * VALUE_MOVE_ENEMY);

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_EExplode[i].m_rot.x),
				XMConvertToRadians(g_EExplode[i].m_rot.y),
				XMConvertToRadians(g_EExplode[i].m_rot.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// モデルのサイズ
			mtxWorld = XMMatrixScaling(0.15f, 0.15f, 0.15f);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(
				g_EExplode[i].m_pos.x,
				g_EExplode[i].m_pos.y,
				g_EExplode[i].m_pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックス設定
			XMStoreFloat4x4(&g_EExplode[i].m_mtxWorld, mtxWorld);

		}
	}
	PrintDebugProc("[ﾋｺｳｷ ｲﾁ : (%f : %f : %f)]\n", g_EExplode->m_pos.x, g_EExplode->m_pos.y, g_EExplode->m_pos.z);
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawEnemyExplode(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	for (int i = 0; i < MAX_ENEMYEXPLODE; ++i) {
		if (!g_EExplode[i].m_use)
		{
			continue;
		}
		g_model.Draw(pDC, g_EExplode[i].m_mtxWorld, eOpacityOnly);
	}

	// 半透明部分を描画
	for (int i = 0; i < MAX_ENEMYEXPLODE; ++i) {
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		g_model.Draw(pDC, g_EExplode[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}
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
int SetEnemyExplode(XMFLOAT3 pos)
{
	// 戻り値の初期化
	int EnemyExplode = -1;

	for (int cntEnemyExplode = 0; cntEnemyExplode < MAX_ENEMYEXPLODE; ++cntEnemyExplode)
	{
		// 使用中ならスキップ
		if (g_EExplode[cntEnemyExplode].m_use)
		{
			continue;
		}
		g_EExplode[cntEnemyExplode].m_use = true;	// 使用中ON
		g_EExplode[cntEnemyExplode].m_pos = pos;	// 指定した座標を代入

		EnemyExplode = cntEnemyExplode + 1;	// 使用中の敵数を代入
		break;
	}

	return EnemyExplode;
}