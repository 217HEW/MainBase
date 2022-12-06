//**************************************************************
//
//	Block.cpp
//	ブロック(壁箱)の処理
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/21	　初期Wall.cppから改造して制作	
//				　通常ブロック、ひび割れブロックの当たり判定と切り替えを実装
//				　半透明処理が不要なのでFPS向上の為、コメントアウト(Draw)
//																		||変更者：上月大地
//--------------------------------------------------------------
//	2021/12/21	　通常ブロック、ひび割れブロックの当たり判定と切り替えを実装		
//	2021/12/21	　ブロックのサイズを構造体の要素から、グローバル変数へ　||変更者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/22	　モデルデータのテクスチャを読み込めるようにしました。
//				　無敵ブロックを呼び出せるようにしました。				
//				　ジャンプ状態のプレイヤーが当ったら耐久値が減り
//				　プレイヤーを接地させるようにしました 123～147			||変更者：上月大地
//--------------------------------------------------------------
//	2021/12/21	ブロックのサイズを構造体の要素から、グローバル変数へ
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2121/12/22	コメントの編集&追加、不要なソースの削除
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2121/12/28	モデルの読込処理の見直し開始
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2122/1/4	効果音を追加
//	編集者：上月大地
//--------------------------------------------------------------
//	2122/01/07	ブロックのサイズの変数名の一部をsize→scaleに変更
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2122/1/8	サイズとスケールを分けました
//				スケールだけY軸に2倍しています
//				座標がズレている疑惑がある為補正
//	編集者：上月大地
//--------------------------------------------------------------
//	2122/1/22	モデルカラーの切り替えやクリエイトフィールドで呼び指す際の
//				引数を追加してました
//	編集者：上月大地
//--------------------------------------------------------------
//	2122/1/30	モデルのデータを配列で管理できるようにしました。
//				モデルの種類指定用の列挙体を作成しました。
//				それに伴い一部定義していた定数を削除しました。
//				ブロックの管理を2次元配列にしました。
//				それに伴いfor文を処理ごとに1つ追加しました
//				モデルの読込みを初期化の部分に置き換えました。
//				SetBlock関数内にfor文でブン回せなかったので
//				ブロックの種類ごとに処理をベタ書きしました。
//	編集者：柴山凜太郎
//**************************************************************

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Block.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIFE		 (1)						// ブロック耐久値
#define BLOCK_SCALE		 (20.0f)					// ブロックのスケールサイズ

//*****************************************************************************
// グローバル変数
//*****************************************************************************

// ブロックモデル格納用配列
const char* c_aFileNameStageModel[MAX_BLOCK_TYPE] =
{
	"data/model/Block/Normal/Hew_3_3.fbx",	// 通常ブロック
	"data/model/Block/Cracks/Hew_2.fbx",	// ひび割れたブロック
	"data/model/Block/Block.fbx",		// 無敵ブロック
};

static CAssimpModel	g_model[MAX_BLOCK_TYPE];	// モデル情報
static TBLOCK		g_block[MAX_BLOCK][MAX_BLOCK_TYPE];	// ブロック情報
static XMFLOAT3		g_BlockScale;		// 現在のサイズ
static XMFLOAT3		g_BlockHalfScale;	// ブロックの半分のサイズ
static XMMATRIX		mtxWorldinv;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBlock(void)
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	HRESULT hr = S_OK;

	for (int nCntBlockType = 0; nCntBlockType < MAX_BLOCK_TYPE; ++nCntBlockType)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; ++nCntBlock)
		{
			//Xが二倍になる為Yの二分の一にしておく
			g_block[nCntBlock][nCntBlockType].m_size = g_BlockScale = XMFLOAT3(10.0f, 10.0f, 10.0f);
			g_block[nCntBlock][nCntBlockType].m_Halfsize = g_BlockHalfScale = XMFLOAT3(5.0f, 10.0f, 10.0f);
			g_block[nCntBlock][nCntBlockType].m_nLife = MAX_LIFE;
			g_block[nCntBlock][nCntBlockType].m_use = false;
			g_block[nCntBlock][nCntBlockType].m_invincible = false;
		}
		switch (nCntBlockType)
		{// モデルロード前にカラーを指定
		case BLOCK_NORMAL:
			g_model[nCntBlockType].SetDif(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		case BLOCK_CRACKS:
			g_model[nCntBlockType].SetDif(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		case BLOCK_INVINCIBLE:
			g_model[nCntBlockType].SetDif(XMFLOAT4(1.0f, 0.8f, 0.1f, 1.0f));
			break;
		default:
			break;
		}
		// モデルデータの読み込み
		if (!g_model[nCntBlockType].Load(pDevice, pDeviceContext, c_aFileNameStageModel[nCntBlockType]))
		{
			MessageBoxA(GetMainWnd(), "ブロックモデル読み込みエラー", "InitBlock", MB_OK);
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
	for (int i = 0; i < MAX_BLOCK_TYPE; ++i)
	{
		g_model[i].Release();
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlock(void)
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	for (int nCntBlockType = 0; nCntBlockType < MAX_BLOCK_TYPE; ++nCntBlockType)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; ++nCntBlock)
		{
			XMMATRIX mtxWorld, mtxRot, mtxTranslate;

			if (!g_block[nCntBlock][nCntBlockType].m_use)
			{// 未使用なら次へ
				continue;
			}
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			mtxWorld = XMMatrixScaling(g_block[nCntBlock][nCntBlockType].m_size.x,
									   g_block[nCntBlock][nCntBlockType].m_size.y * 2,
									   g_block[nCntBlock][nCntBlockType].m_size.z);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(
									   g_block[nCntBlock][nCntBlockType].m_pos.x,
									   g_block[nCntBlock][nCntBlockType].m_pos.y,
									   g_block[nCntBlock][nCntBlockType].m_pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックス設定
			XMStoreFloat4x4(&g_block[nCntBlock][nCntBlockType].m_mtxWorld, mtxWorld);

		}
	}

	//------ブロックとプレイヤーの当たり判定処理-------------------------------

	for (int nCntBlockType = 0; nCntBlockType < MAX_BLOCK_TYPE; nCntBlockType++)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; ++nCntBlock)
		{
			if (!g_block[nCntBlock][nCntBlockType].m_use)
			{// 未使用なら次へ
				continue;
			}

			// 壁とプレイヤーが衝突していたら
			if (CollisionAABB(g_block[nCntBlock][nCntBlockType].m_pos, g_block[nCntBlock][nCntBlockType].m_Halfsize,
								GetPlayerPos(), XMFLOAT3(7.0f, 10.0f, 0.5f)))//プレイヤーのサイズ 
			{
				// プレイヤーがとんでいたら
				if (GetPlayerJump() == false)
				{

					// 体力が無くなったら使わない
					if (g_block[nCntBlock][nCntBlockType].m_nLife <= 0)
					{
						SetExplosionColor(StartExplosion(g_block[nCntBlock][nCntBlockType].m_pos, XMFLOAT2(80.0f, 80.0f)),XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
						CSound::SetPlayVol(SE_WALLBREAK, 0.05f); // 壁破壊音
						g_block[nCntBlock][nCntBlockType].m_use = false;
						//g_model[i].Release();
						break;
					}
					CSound::SetPlayVol(SE_LANDING, 0.1f); // 着地音

					// 無敵ブロックだったら出る
					if (g_block[nCntBlock][nCntBlockType].m_invincible)
					{
						// 爆発エフェクト再生
						StartExplosion(GetPlayerPos(), XMFLOAT2(60.0f, 30.0f));
						// 接地状態ON
						SetPlayerJump(true);
						break;
					}
					// ブロックにひびが入る
					g_block[nCntBlock][nCntBlockType].m_nLife--;
					
					// 接地状態ON
					SetPlayerJump(true);

					// 爆発エフェクト再生
					StartExplosion(GetPlayerPos(), XMFLOAT2(50.0f, 30.0f));
				}
				// 壁の右に触れていたら
				if (g_block[nCntBlock][nCntBlockType].m_pos.x + g_block[nCntBlock][nCntBlockType].m_Halfsize.x <= GetPlayerPos().x - (3.0f / 2))
				{
					SetPlayerDir(DIR_RIGHT);
				}
				// 壁の左に触れていたら
				else if (g_block[nCntBlock][nCntBlockType].m_pos.x - g_block[nCntBlock][nCntBlockType].m_Halfsize.x >= GetPlayerPos().x + (3.0f / 2))
				{
					SetPlayerDir(DIR_LEFT);
				}
				// 壁の上に触れていたら
				else if (g_block[nCntBlock][nCntBlockType].m_pos.y - g_block[nCntBlock][nCntBlockType].m_Halfsize.y >= GetPlayerPos().y + (5.0f / 2))
				{
					SetPlayerDir(DIR_UP);
				}
				// 壁の下に触れていたら
				else if (g_block[nCntBlock][nCntBlockType].m_pos.y + g_block[nCntBlock][nCntBlockType].m_Halfsize.y <= GetPlayerPos().y - (5.0f / 2))
				{
					SetPlayerDir(DIR_DOWN);
				}
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
	for (int nCntBlockType = 0; nCntBlockType < MAX_BLOCK_TYPE; ++nCntBlockType)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; ++nCntBlock)
		{
			// 使っているブロックの描画
			if (!g_block[nCntBlock][nCntBlockType].m_use){
				continue;
			}
			// ブロックモデル描画
			g_model[nCntBlockType].Draw(pDC, g_block[nCntBlock][nCntBlockType].m_mtxWorld, eOpacityOnly);
		}
	}

	// 半透明部分を描画
	for (int i = 0; i < MAX_BLOCK_TYPE; ++i) {
		for (int j = 0; j < MAX_BLOCK; j++)
		{
			SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
			SetZWrite(false);				// Zバッファ更新しない
			g_model[i].Draw(pDC, g_block[i][j].m_mtxWorld, eTransparentOnly);
			SetZWrite(true);				// Zバッファ更新する
			SetBlendState(BS_NONE);			// アルファブレンド無効
		}
		
	}
}

//*******************************
//
//		ブロックの配置処理
//	
//	引数:
//		置きたい座標,
//		無敵か？,true:無敵
//		ブロックのサイズ
//		座標補正値
//	戻り値:
//		使用したブロックの総数
//
//*******************************
int SetBlock(XMFLOAT3 pos, bool inv,XMFLOAT2 size, XMFLOAT2 cpos)
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	int Block = -1;
	for (int cntBlock = 0; cntBlock < MAX_BLOCK; ++cntBlock)
	{
		if (inv)
		{// -----------------無敵壁の設置------------------------------------------------------------
			// 使用中ならスキップ
			if (g_block[cntBlock][BLOCK_INVINCIBLE].m_use)
				continue;
			g_block[cntBlock][BLOCK_INVINCIBLE].m_invincible = true;
			// 使用中ＯＮ
			g_block[cntBlock][BLOCK_INVINCIBLE].m_use = true;
			g_block[cntBlock][BLOCK_INVINCIBLE].m_pos = pos;
			g_block[cntBlock][BLOCK_INVINCIBLE].m_pos.x += cpos.x;
			g_block[cntBlock][BLOCK_INVINCIBLE].m_pos.y += cpos.y;

			// 指定されたサイズに変更
			g_block[cntBlock][BLOCK_INVINCIBLE].m_size.x *= size.x;
			g_block[cntBlock][BLOCK_INVINCIBLE].m_size.y *= size.y;


			if (((g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.x * size.x) > g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.x = 
			   (g_block[cntBlock][BLOCK_INVINCIBLE].m_size.x / 2);
			}
			else if (((g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.x * size.x) < g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.x = 
			   (g_block[cntBlock][BLOCK_INVINCIBLE].m_size.x / 2);
			}
			else { g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.x = g_BlockHalfScale.x; }

			if (((g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.y * size.y) > g_BlockHalfScale.y))
			{
				g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.y = 
			   (g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.y * size.y);// -(g_BlockHalfScale.y * 2.0f) + g_BlockHalfScale.y;
			}
			else
			{
				g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.y = g_BlockHalfScale.y;
			}
		}
		else 
		{// --------------通常壁の設置------------------------------------------------------------
			if (g_block[cntBlock][BLOCK_NORMAL].m_use)
				continue;	// 使用中ならスキップ

			// 使用中ＯＮ
			g_block[cntBlock][BLOCK_NORMAL].m_use = true;
			
			// 触れた瞬間に消えるようにする
			g_block[cntBlock][BLOCK_NORMAL].m_nLife = 0;
			
			// 座標設定
			g_block[cntBlock][BLOCK_NORMAL].m_pos = pos;
			g_block[cntBlock][BLOCK_NORMAL].m_pos.x += cpos.x;
			g_block[cntBlock][BLOCK_NORMAL].m_pos.y += cpos.y;

			// 指定されたサイズに変更
			g_block[cntBlock][BLOCK_NORMAL].m_size.x *= size.x;
			g_block[cntBlock][BLOCK_NORMAL].m_size.y *= size.y;


			if (((g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.x * size.x) > g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.x =
			   (g_block[cntBlock][BLOCK_NORMAL].m_size.x / 2);
			}
			else if (((g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.x * size.x) < g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.x =
			   (g_block[cntBlock][BLOCK_NORMAL].m_size.x / 2);
			}
			else { g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.x = g_BlockHalfScale.x; }

			if (((g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.y * size.y) > g_BlockHalfScale.y))
			{
				g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.y =
			   (g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.y * size.y);// -(g_BlockHalfScale.y * 2.0f) + g_BlockHalfScale.y;
			}
			else
			{
				g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.y = g_BlockHalfScale.y;
			}

			// --------------ヒビ壁の設置------------------------------------------------------------
			if (g_block[cntBlock][BLOCK_CRACKS].m_use)
				continue;	// 使用中ならスキップ

			// 使用中ＯＮ
			g_block[cntBlock][BLOCK_CRACKS].m_use = true;
			
			// 座標設定
			g_block[cntBlock][BLOCK_CRACKS].m_pos = pos;
			g_block[cntBlock][BLOCK_CRACKS].m_pos.x += cpos.x;
			g_block[cntBlock][BLOCK_CRACKS].m_pos.y += cpos.y;

			// 指定されたサイズに変更
			g_block[cntBlock][BLOCK_CRACKS].m_size.x *= size.x;
			g_block[cntBlock][BLOCK_CRACKS].m_size.y *= size.y;


			if (((g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.x * size.x) > g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.x =
			   (g_block[cntBlock][BLOCK_CRACKS].m_size.x / 2);
			}
			else if (((g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.x * size.x) < g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.x =
			   (g_block[cntBlock][BLOCK_CRACKS].m_size.x / 2);
			}
			else { g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.x = g_BlockHalfScale.x; }

			if (((g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.y * size.y) > g_BlockHalfScale.y))
			{
				g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.y =
			   (g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.y * size.y);// -(g_BlockHalfScale.y * 2.0f) + g_BlockHalfScale.y;
			}
			else
			{
				g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.y = g_BlockHalfScale.y;
			}
		}
		
		// 使用しているブロック数のカウント
		Block = cntBlock + 1;
		break;
	}
	return Block;
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
XMFLOAT3 GetBlockSize()
{
	return g_BlockScale;
}

//***********************************
//
//		ブロック配列取得
//		
//		戻り値：ブロックの情報
//
//***********************************
TBLOCK* GetBlockArray()
{
	return *g_block;
}
