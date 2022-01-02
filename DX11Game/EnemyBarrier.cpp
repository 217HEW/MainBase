//**************************************************************
//
//	EnemyBarrier
//	プレイヤーが範囲内に入ってきたらバリアを貼り敵を倒すまで出れなくするやつ
//
//--------------------------------------------------------------
//	製作者：小嶋悟
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/27
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "EnemyBarrier.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "Block.h"
#include "explosion.h"
#include "life.h"
#include "SceneManager.h"

#include "player.h"
//**************************************************************
// 構造体定義
//**************************************************************
struct TEnemyBarrier {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_size;		// 現在のサイズ
	XMFLOAT3	m_rotDest;	// 目的の向き
	bool		m_use;		// 使用してるか否か	ON:使用中

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
	int			m_speed;	// スピード
};

//**************************************************************
// マクロ定義
//**************************************************************
#define MODEL_ENEMY			"data/model/Hew4.fbx"//バリアエネミー
#define MAX_EnemyBarrier			(10)// 敵機最大数

//**************************************************************
// グローバル変数
//**************************************************************
static CAssimpModel		g_model;			// モデル情報
static TEnemyBarrier		g_EMelee[MAX_EnemyBarrier];	// 敵機情報
static XMFLOAT3			Blocksize;

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitEnemyBarrier(void)
{
	HRESULT hr = S_OK;//
	ID3D11Device* pDevice = GetDevice();//
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ブロックのサイズ取得
	Blocksize = GetBlockSize();

	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY))
	{
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < MAX_EnemyBarrier; ++i)
	{// 初期化したいモノがあればここに↓
		g_EMelee[i].m_pos = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EMelee[i].m_size = (XMFLOAT3(10.0f, 10.0f, 10.0f));
		g_EMelee[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f));
		g_EMelee[i].m_rotDest = g_EMelee[i].m_rot;
		g_EMelee[i].m_use = false;
	}

	return hr;//
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitEnemyBarrier(void)
{
	// モデルの解放
	g_model.Release();
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateEnemyBarrier(void)
{
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	//プレイヤーの座標・サイズ取得
	XMFLOAT3 posPlayer = GetPlayerPos();
	float sizePlayer = GetPlayerSize();

	for (int i = 0; i < MAX_EnemyBarrier; ++i)
	{
		//ブロック配列取得
		TBLOCK *Block = GetBlockArray();

		//敵とプレイヤーの距離が近づいたら
		if (CollisionSphere(posPlayer, sizePlayer, g_EMelee[i].m_pos, SEARCH_ENEMY))
		{
			if (!g_EMelee[i].m_use)
			{//未使用なら次へ
				continue;
			}

			//常にプレイヤーの方向を向く
			/*g_EMelee[i].m_rotDest = posPlayer;
			g_EMelee[i].m_rot = g_EMelee[i].m_rotDest;
			g_EMelee[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/

			//敵のx座標がプレイヤーよりも大きかったら
			if (g_EMelee[i].m_pos.x >= posPlayer.x)
			{
				g_EMelee[i].m_pos.x += -VALUE_MOVE_ENEMY;
				g_EMelee[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f));

				g_EMelee[i].m_rotDest.y = rotCamera.y - 90.0f;
			}
			//敵のx座標がプレイヤーよりも小さかったら
			if (g_EMelee[i].m_pos.x <= posPlayer.x)
			{
				g_EMelee[i].m_pos.x += VALUE_MOVE_ENEMY;
				g_EMelee[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 90.0f));
				g_EMelee[i].m_rotDest.y = rotCamera.y + 90.0f;

			}
			//敵のy座標がプレイヤーよりも大きかったら
			if (g_EMelee[i].m_pos.y >= posPlayer.y)
			{
				g_EMelee[i].m_pos.y += -VALUE_MOVE_ENEMY;

			}
			//敵のy座標がプレイヤーよりも小さかったら
			if (g_EMelee[i].m_pos.y <= posPlayer.y)
			{
				g_EMelee[i].m_pos.y += VALUE_MOVE_ENEMY;

			}

			//**************************************************************************************
			//		当たり判定
			//**************************************************************************************

			// 敵同士の当たり判定
			for (int j = 0; j < MAX_EnemyBarrier; ++j)
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

			// 敵とプレイヤーの当たり判定
			if (!g_EMelee[i].m_use)
			{// 未使用なら次へ
				continue;
			}
			if (CollisionSphere(g_EMelee[i].m_pos, g_EMelee[i].m_size.x, posPlayer, sizePlayer))
			{
				DelLife();
				if (GetLife() == 0)
				{
					SetScene(SCENE_GAMEOVER);
				}
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
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックス設定
			XMStoreFloat4x4(&g_EMelee[i].m_mtxWorld, mtxWorld);

		}
	}
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawEnemyBarrier(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	for (int i = 0; i < MAX_EnemyBarrier; ++i) {
		if (!g_EMelee[i].m_use)
		{
			continue;
		}
		g_model.Draw(pDC, g_EMelee[i].m_mtxWorld, eOpacityOnly);
	}

	// 半透明部分を描画
	for (int i = 0; i < MAX_EnemyBarrier; ++i) {
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		g_model.Draw(pDC, g_EMelee[i].m_mtxWorld, eTransparentOnly);
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
int SetEnemyBarrier(XMFLOAT3 pos)
{
	// 戻り値の初期化
	int EnemyBarrier = -1;

	for (int cntEnemyBarrier = 0; cntEnemyBarrier < MAX_EnemyBarrier; ++cntEnemyBarrier)
	{
		// 使用中ならスキップ
		if (g_EMelee[cntEnemyBarrier].m_use)
		{
			continue;
		}
		g_EMelee[cntEnemyBarrier].m_use = true;	// 使用中ON
		g_EMelee[cntEnemyBarrier].m_pos = pos;	// 指定した座標を代入

		EnemyBarrier = cntEnemyBarrier + 1;	// 使用中の敵数を代入
		break;
	}

	return EnemyBarrier;
}
