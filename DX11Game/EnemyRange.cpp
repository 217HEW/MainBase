//**************************************************************
//
//	EnemyRange.cpp
//	遠距離攻撃エネミー
//
//--------------------------------------------------------------
//	製作者：石原聖斗
//--------------------------------------------------------------
//	開発履歴
//	2021/12/28	敵の範囲に入ったらタイマーでダメージを喰らう処理
//				の実装
//	2021/01/03	ジャンプ中は攻撃を喰らわない処理の実装
//
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "EnemyBase.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"
#include "life.h"
#include "timer.h"
#include "SceneManager.h"

//**************************************************************
// 構造体定義
//**************************************************************
struct TEnemyRange {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_size;		// 現在のサイズ
	bool		m_use;		// 使用してるか否か	ON:使用中
	int			m_Time;		// 敵のタイマー

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
};

//**************************************************************
// マクロ定義
//**************************************************************
#define MODEL_ENEMY			"data/model/helicopter000.fbx"

#define MAX_ENEMYRANGE			(10)		// 敵機最大数

#define SEARCH_RANGE			(200)		// 探索範囲

#define ENEMY_TIMER				(3)			// 制限時間

//**************************************************************
// グローバル変数
//**************************************************************
static CAssimpModel	g_model;			// モデル
static TEnemyRange		g_ERange[MAX_ENEMYRANGE];	// 敵機情報


//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitEnemyRange(void)
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

	for (int i = 0; i < MAX_ENEMYRANGE; ++i)
	{// 初期化したいモノがあればここに↓
		g_ERange[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_ERange[i].m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_ERange[i].m_rotDest = g_ERange[i].m_rot;
		g_ERange[i].m_size = XMFLOAT3(5.0f, 5.0f, 5.0f);
		g_ERange[i].m_use = false;
		g_ERange[i].m_Time = ENEMY_TIMER * 60 + 59;
	}

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitEnemyRange(void)
{
	// モデルの解放
	g_model.Release();
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateEnemyRange(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	//プレイヤーの座標・サイズ取得
	XMFLOAT3 posPlayer = GetPlayerPos();
	float sizePlayer = GetPlayerRadSize();

	for (int i = 0; i < MAX_ENEMYRANGE; ++i)
	{
		//GetPlayerJump();
		//敵とプレイヤーの距離が近づいたら
		if (CollisionSphere(posPlayer, sizePlayer, g_ERange[i].m_pos, SEARCH_RANGE))
		{
			// 使用中ならスキップ
			if (!g_ERange[i].m_use)
			{
				continue;
			}
			// タイマーカウントダウン
			if (g_ERange[i].m_Time > 0)
			{
				--g_ERange[i].m_Time;
				if (g_ERange[i].m_Time == 0)
				{
					if (GetPlayerJump())
					{
						DelLife();
						StartExplosion(posPlayer, XMFLOAT2(40.0f, 40.0f));
						if (GetLife() == 0)
						{
							SetScene(SCENE_GAMEOVER);
						}
					}
					else
					{
						continue;
					}
					g_ERange[i].m_Time += ENEMY_TIMER * 60 + 59;
					
				}
			}
			/*else
			{
				g_ERange[i].m_Time;
			}*/


			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_ERange[i].m_rot.x),
				XMConvertToRadians(g_ERange[i].m_rot.y),
				XMConvertToRadians(g_ERange[i].m_rot.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(
				g_ERange[i].m_pos.x,
				g_ERange[i].m_pos.y,
				g_ERange[i].m_pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックス設定
			XMStoreFloat4x4(&g_ERange[i].m_mtxWorld, mtxWorld);
		}
	}
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawEnemyRange(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	for (int i = 0; i < MAX_ENEMYRANGE; ++i) {
		g_model.Draw(pDC, g_ERange[i].m_mtxWorld, eOpacityOnly);
	}

	// 半透明部分を描画
	for (int i = 0; i < MAX_ENEMYRANGE; ++i) {
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		g_model.Draw(pDC, g_ERange[i].m_mtxWorld, eTransparentOnly);
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
int SetEnemyRange(XMFLOAT3 pos)
{
	// 戻り値の初期化
	int Enemy = -1;

	for (int cntEnemy = 0; cntEnemy < MAX_ENEMYRANGE; ++cntEnemy)
	{
		// 使用中ならスキップ
		if (g_ERange[cntEnemy].m_use)
		{
			continue;
		}
		g_ERange[cntEnemy].m_use = true;	// 使用中ON
		g_ERange[cntEnemy].m_pos = pos;	// 指定した座標を代入

		Enemy = cntEnemy + 1;	// 使用中の敵数を代入
		break;
	}

	return Enemy;
}
