//**************************************************************
//
//	EnemyRange.cpp
//	遠距離攻撃エネミー
//
//--------------------------------------------------------------
//	製作者：石原聖斗
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/28	敵の範囲に入ったらタイマーでダメージを喰らう処理
//				の実装
//	2021/01/03	ジャンプ中は攻撃を喰らわない処理の実装
//	編集者：石原聖斗
//--------------------------------------------------------------
//	2022/01/14	正式遠距離エネミーモデルを導入しました。
//				射撃までのクールタイムを多少ランダムにする処理を
//				作成中
//	編集者：上月大地
//--------------------------------------------------------------
//	2022/01/22	遠隔敵の情報を取得する関数の作成
//				プレイヤーのライフ処理はここで屋あるべきでないため削除
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2022/01/31	敵がプレイヤーの方角へ向く簡易的な処理を追加しました
//	編集者：上月大地
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "EnemyRange.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"
#include "life.h"
#include "timer.h"
#include "Texture.h"
#include "polygon.h"
#include "Reticle.h"
#include "Sound.h"
#include "CountEnemy.h"
//#include "SceneManager.h"



//**************************************************************
// マクロ定義
//**************************************************************
#define MODEL_ENEMY			"data/model/Range/Range.fbx"	// "data/model/helicopter000.fbx"
#define RETICLE_TEXTURE		L"data/texture/reticle000.png"

#define MAX_ENEMYRANGE			(10)		// 敵機最大数
#define SEARCH_RANGE			(200)		// 探索範囲
#define ENEMY_TIMER				(5)			// 制限時間
#define SCALE_E_RANGE		(XMFLOAT3(0.03f, 0.06f, 0.06f))
#define COLLAR_ENEMY		(XMFLOAT4(0.0f, 20.0f, 0.0f,1.0f))	// プレイヤーカラー(仮)ここをいじるとカラーが変わります
//////////////////////////////////////////////////////////////////
#define MODEL_ENEMY1			"data/model/Range/Range.fbx"	// "data/model/helicopter000.fbx"
#define MAX_ENEMYRANGE1			(10)		// 敵機最大数
#define SEARCH_RANGE1			(200)		// 探索範囲
#define ENEMY_TIMER1			(5)			// 制限時間
#define SCALE_E_RANGE1		(XMFLOAT3(0.05f, 0.1f, 0.1f))
#define COLLAR_ENEMY1		(XMFLOAT4(0.0f, 0.0f, 20.0f,1.0f))	// プレイヤーカラー(仮)ここをいじるとカラーが変わります
//////////////////////////////////////////////////////////////////
#define MODEL_ENEMY2			"data/model/Range/Range.fbx"	// "data/model/helicopter000.fbx"
#define MAX_ENEMYRANGE2			(10)		// 敵機最大数
#define SEARCH_RANGE2			(200)		// 探索範囲
#define ENEMY_TIMER2				(5)			// 制限時間
#define SCALE_E_RANGE2		(XMFLOAT3(0.05f, 0.1f, 0.1f))
#define COLLAR_ENEMY2		(XMFLOAT4(20.0f, 0.0f, 20.0f,1.0f))	// プレイヤーカラー(仮)ここをいじるとカラーが変わります


#define DEFAULT_COLLAR		(XMFLOAT4(1.0f, 1.0f, 0.0f,1.0f))
#define SCALE_E_RANGE		(XMFLOAT3(0.03f, 0.06f, 0.06f))
//**************************************************************
// グローバル変数
//**************************************************************
static CAssimpModel	g_model;			// モデル
static TEnemyRange	g_ERange[MAX_ENEMYRANGE];	// 敵機情報
static ID3D11ShaderResourceView* g_pTexture;
//////////////////////////////////////////////////////////////////
static CAssimpModel	g_model1;			// モデル
static TEnemyRange1	g_ERange1[MAX_ENEMYRANGE1];	// 敵機情報
//////////////////////////////////////////////////////////////////
static CAssimpModel	g_model2;			// モデル
static TEnemyRange2	g_ERange2[MAX_ENEMYRANGE2];	// 敵機情報

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitEnemyRange(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//// テクスチャ読込
	//hr = CreateTextureFromFile(pDevice, RETICLE_TEXTURE, &g_pTexture);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}

	// モデルデータの読み込み
	g_model.SetDif(XMFLOAT4(0.2f,20.0f,10.0f,1.0f));
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
		// g_ERange[i].m_Time = ENEMY_TIMER * 60 + 59;
		g_ERange[i].m_Time = (ENEMY_TIMER + rand() % 3) * 60 + 59;	// 3～6秒のランダムで
		g_ERange[i].ReticleSize = g_ERange[i].m_Time / 3;
	}
	//////////////////////////////////////////////////////////////////////////////////
	//// モデルデータの読み込み
	//g_model1.SetDif(COLLAR_ENEMY1); // モデルロード前にカラーを指定
	//if (!g_model1.Load(pDevice, pDeviceContext, MODEL_ENEMY1))
	//{
	//	MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
	//	return E_FAIL;
	//}
	//g_model1.SetDif(DEFAULT_COLLAR);
	//for (int i = 0; i < MAX_ENEMYRANGE1; ++i)
	//{// 初期化したいモノがあればここに↓
	//	g_ERange1[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_ERange1[i].m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_ERange1[i].m_rotDest = g_ERange1[i].m_rot;
	//	g_ERange1[i].m_size = XMFLOAT3(5.0f, 5.0f, 5.0f);
	//	g_ERange1[i].m_use = false;
	//	// g_ERange1[i].m_Time = ENEMY_TIMER1 * 60 + 59;
	//	g_ERange1[i].m_Time = (ENEMY_TIMER + rand() % 3) * 60 + 59;	// 3～6秒のランダムで
	//}
	//////////////////////////////////////////////////////////////////////////////////
	//// モデルデータの読み込み
	//g_model2.SetDif(COLLAR_ENEMY2); // モデルロード前にカラーを指定
	//if (!g_model2.Load(pDevice, pDeviceContext, MODEL_ENEMY2))
	//{
	//	MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
	//	return E_FAIL;
	//}
	//g_model2.SetDif(DEFAULT_COLLAR);
	//for (int i = 0; i < MAX_ENEMYRANGE2; ++i)
	//{// 初期化したいモノがあればここに↓
	//	g_ERange2[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_ERange2[i].m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_ERange2[i].m_rotDest = g_ERange2[i].m_rot;
	//	g_ERange2[i].m_size = XMFLOAT3(5.0f, 5.0f, 5.0f);
	//	g_ERange2[i].m_use = false;
	//	// g_ERange2[i].m_Time = ENEMY_TIMER2 * 60 + 59;
	//	g_ERange2[i].m_Time = (ENEMY_TIMER + rand() % 3) * 60 + 59;	// 3～6秒のランダムで
	//}


	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitEnemyRange(void)
{
	// モデルの解放
	g_model.Release();

	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
	// モデルの解放
	g_model1.Release();
	// モデルの解放
	g_model2.Release();

}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateEnemyRange(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxSize, mtxScale;
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//プレイヤーの座標・サイズ取得
	XMFLOAT3 posPlayer = GetPlayerPos();
	float sizePlayer = GetPlayerRadSize();

	for (int i = 0; i < MAX_ENEMYRANGE; ++i)
	{
		// 未使用なら
		if (!g_ERange[i].m_use)
		{
			continue;
		}
		if (g_ERange[i].m_pos.x >= posPlayer.x)
		{
			g_ERange[i].m_rot = (XMFLOAT3(0.0f, 180.0f, 0.0f));
		}	
		//敵座標がプレイヤーよりも小さかったら
		if (g_ERange[i].m_pos.x <= posPlayer.x)
		{	
			g_ERange[i].m_rot = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		}
			// 使用中ならスキップ
			if (!g_ERange[i].m_use)
			{
				continue;
			}
			if (CollisionSphere(g_ERange[i].m_pos, g_ERange[i].m_size.x, posPlayer, sizePlayer))
			{
				//DelLife();
				//if (GetLife() == 0)
				//{
				//	SetScene(SCENE_GAMEOVER);
				//}
				DelCountEnemy();
				g_ERange[i].m_use = false;
			}
			//常にプレイヤーの方向を向く
			/*g_ERange[i].m_rotDest = posPlayer;
			g_ERange[i].m_rot = g_ERange[i].m_rotDest;
			g_ERange[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/
			// タイマーカウントダウン
			if ((g_ERange[i].m_Time > 0) && (!GetPlayerInv()))
			{
				--g_ERange[i].m_Time;
				g_ERange[i].ReticleSize -= 0.25f;
				SetReticle(posPlayer, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					XMFLOAT2(g_ERange[i].ReticleSize, g_ERange[i].ReticleSize), 2);
				if (g_ERange[i].m_Time == 0)
				{
					CSound::SetPlayVol(SE_LASER, 0.1f);
					StartExplosion(posPlayer, XMFLOAT2(40.0f, 40.0f));
					if (GetPlayerJump())
					{
						DelLife();
					}
					g_ERange[i].m_Time += (ENEMY_TIMER + rand() % 3) * 60 + 59;	// もう一度3～6秒数える
					g_ERange[i].ReticleSize = g_ERange[i].m_Time / 3;
				}
			}

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_ERange[i].m_rot.x),
				XMConvertToRadians(g_ERange[i].m_rot.y),
				XMConvertToRadians(g_ERange[i].m_rot.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			mtxSize = XMMatrixScaling(SCALE_E_RANGE.x, SCALE_E_RANGE.y, SCALE_E_RANGE.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxSize);
			// 移動を反映
			mtxTranslate = XMMatrixTranslation(
				g_ERange[i].m_pos.x,
				g_ERange[i].m_pos.y,
				g_ERange[i].m_pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックス設定
			XMStoreFloat4x4(&g_ERange[i].m_mtxWorld, mtxWorld);
		//}
	}
	//for (int i = 0; i < MAX_ENEMYRANGE1; ++i)
	//{
	//	//GetPlayerJump();
	//	//敵とプレイヤーの距離が近づいたら
	//	//if (CollisionSphere(posPlayer, sizePlayer, g_ERange1[i].m_pos, SEARCH_RANGE))
	//	//{
	//		// 使用中ならスキップ
	//	if (!g_ERange1[i].m_use)
	//	{
	//		continue;
	//	}
	//	if (CollisionSphere(g_ERange1[i].m_pos, g_ERange1[i].m_size.x, posPlayer, sizePlayer))
	//	{
	//		//DelLife();
	//		//if (GetLife() == 0)
	//		//{
	//		//	SetScene(SCENE_GAMEOVER);
	//		//}
	//		DelCountEnemy();
	//		g_ERange1[i].m_use = false;
	//	}
	//	//常にプレイヤーの方向を向く
	//	/*g_ERange1[i].m_rotDest = posPlayer;
	//	g_ERange1[i].m_rot = g_ERange1[i].m_rotDest;
	//	g_ERange1[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/
	//	// タイマーカウントダウン
	//	if (g_ERange1[i].m_Time > 0)
	//	{
	//		--g_ERange1[i].m_Time;
	//		if (g_ERange1[i].m_Time == 0)
	//		{
	//			if (GetPlayerJump())
	//			{
	//				//DelLife();
	//				StartExplosion(posPlayer, XMFLOAT2(40.0f, 40.0f));
	//			}
	//			else
	//			{
	//				continue;
	//			}
	//			g_ERange1[i].m_Time += (ENEMY_TIMER2 + rand() % 3) * 60 + 59;	// もう一度3～6秒数える

	//		}
	//	}
	//	/*else
	//	{
	//		g_ERange1[i].m_Time;
	//	}*/

	//	// ワールドマトリックスの初期化
	//	mtxWorld = XMMatrixIdentity();

	//	// 回転を反映
	//	mtxRot = XMMatrixRotationRollPitchYaw(
	//		XMConvertToRadians(g_ERange1[i].m_rot.x),
	//		XMConvertToRadians(g_ERange1[i].m_rot.y),
	//		XMConvertToRadians(g_ERange1[i].m_rot.z));
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	mtxWorld = XMMatrixScaling(SCALE_E_RANGE1.x, SCALE_E_RANGE1.y, SCALE_E_RANGE1.z);
	//	// 移動を反映
	//	mtxTranslate = XMMatrixTranslation(
	//		g_ERange1[i].m_pos.x,
	//		g_ERange1[i].m_pos.y,
	//		g_ERange1[i].m_pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ワールドマトリックス設定
	//	XMStoreFloat4x4(&g_ERange1[i].m_mtxWorld, mtxWorld);
	//	//}
	//}
	//for (int i = 0; i < MAX_ENEMYRANGE2; ++i)
	//{
	//	//GetPlayerJump();
	//	//敵とプレイヤーの距離が近づいたら
	//	//if (CollisionSphere(posPlayer, sizePlayer, g_ERange2[i].m_pos, SEARCH_RANGE2))
	//	//{
	//		// 使用中ならスキップ
	//	if (!g_ERange2[i].m_use)
	//	{
	//		continue;
	//	}
	//	if (CollisionSphere(g_ERange2[i].m_pos, g_ERange2[i].m_size.x, posPlayer, sizePlayer))
	//	{
	//		//DelLife();
	//		//if (GetLife() == 0)
	//		//{
	//		//	SetScene(SCENE_GAMEOVER);
	//		//}
	//		DelCountEnemy();
	//		g_ERange2[i].m_use = false;
	//	}
	//	//常にプレイヤーの方向を向く
	//	/*g_ERange2[i].m_rotDest = posPlayer;
	//	g_ERange2[i].m_rot = g_ERange2[i].m_rotDest;
	//	g_ERange2[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/
	//	// タイマーカウントダウン
	//	if (g_ERange2[i].m_Time > 0)
	//	{
	//		--g_ERange2[i].m_Time;
	//		if (g_ERange2[i].m_Time == 0)
	//		{
	//			if (GetPlayerJump())
	//			{
	//				//DelLife();
	//				StartExplosion(posPlayer, XMFLOAT2(40.0f, 40.0f));
	//			}
	//			else
	//			{
	//				continue;
	//			}
	//			g_ERange2[i].m_Time += (ENEMY_TIMER2 + rand() % 3) * 60 + 59;	// もう一度3～6秒数える

	//		}
	//	}
	//	/*else
	//	{
	//		g_ERange2[i].m_Time;
	//	}*/

	//	// ワールドマトリックスの初期化
	//	mtxWorld = XMMatrixIdentity();

	//	// 回転を反映
	//	mtxRot = XMMatrixRotationRollPitchYaw(
	//		XMConvertToRadians(g_ERange2[i].m_rot.x),
	//		XMConvertToRadians(g_ERange2[i].m_rot.y),
	//		XMConvertToRadians(g_ERange2[i].m_rot.z));
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	mtxWorld = XMMatrixScaling(SCALE_E_RANGE2.x, SCALE_E_RANGE2.y, SCALE_E_RANGE2.z);
	//	// 移動を反映
	//	mtxTranslate = XMMatrixTranslation(
	//		g_ERange2[i].m_pos.x,
	//		g_ERange2[i].m_pos.y,
	//		g_ERange2[i].m_pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ワールドマトリックス設定
	//	XMStoreFloat4x4(&g_ERange2[i].m_mtxWorld, mtxWorld);
	//	//}
	//}
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawEnemyRange(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	for (int i = 0; i < MAX_ENEMYRANGE; ++i) {

		// 使ってないならスキップ
		if (!g_ERange[i].m_use)
		{
			continue;
		}
		g_model.Draw(pDC, g_ERange[i].m_mtxWorld, eOpacityOnly);
	

		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		g_model.Draw(pDC, g_ERange[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}
	/////////////////////////////////////////////////////////////////////
	//// 不透明部分を描画
	//for (int i = 0; i < MAX_ENEMYRANGE1; ++i) {
	//	// 使ってないならスキップ
	//	if (!g_ERange1[i].m_use)
	//	{
	//		continue;
	//	}
	//	g_model1.Draw(pDC, g_ERange1[i].m_mtxWorld, eOpacityOnly);
	//}
	////// 半透明部分を描画
	//for (int i = 0; i < MAX_ENEMYRANGE1; ++i) {
	//	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	//	SetZWrite(false);				// Zバッファ更新しない
	//	g_model1.Draw(pDC, g_ERange1[i].m_mtxWorld, eTransparentOnly);
	//	SetZWrite(true);				// Zバッファ更新する
	//	SetBlendState(BS_NONE);			// アルファブレンド無効
	//}
	/////////////////////////////////////////////////////////////////////
	// //不透明部分を描画
	//for (int i = 0; i < MAX_ENEMYRANGE2; ++i) {
	//	// 使ってないならスキップ
	//	if (!g_ERange2[i].m_use)
	//	{
	//		continue;
	//	}
	//	g_model2.Draw(pDC, g_ERange2[i].m_mtxWorld, eOpacityOnly);
	//}
	//// 半透明部分を描画
	//for (int i = 0; i < MAX_ENEMYRANGE2; ++i) {
	//	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	//	SetZWrite(false);				// Zバッファ更新しない
	//	g_model2.Draw(pDC, g_ERange2[i].m_mtxWorld, eTransparentOnly);
	//	SetZWrite(true);				// Zバッファ更新する
	//	SetBlendState(BS_NONE);			// アルファブレンド無効
	//}
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
int SetEnemyRange(XMFLOAT3 pos,int id)
{
	if (id == 0)
	{
		// 戻り値の初期化
		int EnemyRange = -1;

		for (int cntEnemy = 0; cntEnemy < MAX_ENEMYRANGE; ++cntEnemy)
		{
			// 使用中ならスキップ
			if (g_ERange[cntEnemy].m_use)
			{
				continue;
			}
			g_ERange[cntEnemy].m_use = true;	// 使用中ON
			g_ERange[cntEnemy].m_pos = pos;	// 指定した座標を代入

			EnemyRange = cntEnemy + 1;	// 使用中の敵数を代入
			break;
		}

		return EnemyRange;
	}
	//else if (id == 1)
	//{
	//	// 戻り値の初期化
	//	int EnemyRange1 = -1;

	//	for (int cntEnemy1 = 0; cntEnemy1 < MAX_ENEMYRANGE1; ++cntEnemy1)
	//	{
	//		// 使用中ならスキップ
	//		if (g_ERange1[cntEnemy1].m_use)
	//		{
	//			continue;
	//		}
	//		g_ERange1[cntEnemy1].m_use = true;	// 使用中ON
	//		g_ERange1[cntEnemy1].m_pos = pos;	// 指定した座標を代入

	//		EnemyRange1 = cntEnemy1 + 1;	// 使用中の敵数を代入
	//		break;
	//	}

	//	return EnemyRange1;
	//}
	//else if (id == 2)
	//{
	//	// 戻り値の初期化
	//	int EnemyRange2 = -1;

	//	for (int cntEnemy2 = 0; cntEnemy2 < MAX_ENEMYRANGE2; ++cntEnemy2)
	//	{
	//		// 使用中ならスキップ
	//		if (g_ERange2[cntEnemy2].m_use)
	//		{
	//			continue;
	//		}
	//		g_ERange2[cntEnemy2].m_use = true;	// 使用中ON
	//		g_ERange2[cntEnemy2].m_pos = pos;	// 指定した座標を代入

	//		EnemyRange2 = cntEnemy2 + 1;	// 使用中の敵数を代入
	//		break;
	//	}

	//	return EnemyRange2;
	//}
	return 0;
}

TEnemyRange* GetEnemyRange()
{
	return g_ERange;	
}
