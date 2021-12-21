//=============================================================================
//
// 弾 [CreateField.cpp]
// Author : SAWAMURA RYUTO
// 開発履歴：12/20	二次元配列を用いた、マップ構築
//					生成範囲は未実装
//
//=============================================================================
#include "CreateField.h"
#include "input.h"
#include "AssimpModel.h"
#include "meshwall.h"

#define MAX_MODEL		110
#define STAGEHEIGHT		500	//ステージの縦幅
#define STAGEWIDTH		500 //ステージの横幅
#define	SETMAPCHIP_HEIGHT	10	//マップチップの縦幅
#define	SETMAPCHIP_WIDTH	10	//マップチップの横幅
//#define MODEL_SIZE_X	50
//#define MODEL_SIZE_Y	50

#define FIELD_TEST		"data/model/airplane000.fbx"
//#define MAP_SIZE_HEIGHT	(STAGEHEIGHT / SETMAPCHIP_HEIGHT)	//描画するモデル１つの縦幅
//#define MAP_SIZE_WIDTH	(STAGEWIDTH / SETMAPCHIP_WIDTH)		//描画するモデル１つの横幅

static CAssimpModel	g_model;		// モデル
static XMFLOAT4X4	g_mtxWorld;		// ワールドマトリックス


struct MODEL
{
	XMFLOAT3	g_posModel;
	XMFLOAT3	g_sizeModel;
};
	MODEL g_block[MAX_MODEL];


bool g_Map[SETMAPCHIP_HEIGHT][SETMAPCHIP_WIDTH] =
{
	//*******500*******
	1,1,1,1,1,1,1,1,1,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//5
	1,0,0,0,0,0,0,0,0,1,//0
	1,0,0,0,0,0,0,0,0,1,//0
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,1,1,1,1,1,1,1,1,1,//

};
//MAP g_Map;

//typedef struct
//{
//	XMFLOAT2 g_MapSize;	//配列の中身
//	XMFLOAT2 g_MapScale;//マップ自体の大きさ
//
//}MAP;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCField(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	TBLOCK* Mesh = GetMesh();
	
	//for (int i = 0; i < MAX_MODEL; i++)
	//{
	//	//g_block[i].g_posModel  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	//g_block[i].g_sizeModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//}
	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, FIELD_TEST)) 
	{
		MessageBoxA(GetMainWnd(), "自機モデルデータ読み込みエラー", "InitModel", MB_OK);
		return E_FAIL;
	}
	//{
	//	for (int Width = 0; Width < SETMAPCHIP_WIDTH; Width++)
	//	{
	//		
	//		//マップチップ番号所得
	//		bool chip_number = g_Map[Height][Width];
	//
	//		//if (g_Map[Height][Width] == 1)
	//		//二次元配列Map内で、"1"の場所に描画する
	//		if (chip_number)
	//		{
	//			// ここにブロック情報を入れる
	//			SetMeshBlock(XMFLOAT3(Mesh->m_size.x * Width, Mesh->m_size.y * Height, 0.0f));
	//		}//0であるならば、処理をしない
	//		else if (!chip_number)
	//		{
	//			continue;
	//		}
	//	}
	//}
	// 位置・回転・スケールの初期設定
	//g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);


	

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCField(void)
{
	// 丸影の解放
	//ReleaseShadow(g_nShadow);

	// モデルの解放
	g_model.Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCField(void)
{

	

	
	
	// 壁更新
	UpdateMeshWall();
	
	XMMATRIX mtxWorld;
	XMFLOAT3 model;

	

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();
	// ワールドマトリックス設定
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// 丸影の移動
	//MoveShadow(g_nShadow, g_posModel);

}

//=============================================================================
// 描画処理
// やりたいこと：マップチップを三次元空間に500 * 500で描画したい
// 出来ていないこと：座標の割り当て？
//=============================================================================
void DrawCField(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	//g_Map[10][10] =
	//現状[縦10][横10]
	
	//配列の中身の処理
	for (int Height = 0; Height < SETMAPCHIP_HEIGHT; Height++)
	{
		for (int Width = 0; Width < SETMAPCHIP_WIDTH; Width++)
		{
			//マップチップ番号所得
			int chip_number = g_Map[Height][Width];
			//if (g_Map[Height][Width] == 1)
			//二次元配列Map内で、"1"の場所に描画する
			if(chip_number == 1)
			{
				//0であるならば、処理をしない
				if (chip_number == 0)
				{
					continue;
				}
	
	
				//描画するオブジェクトの関数を呼び出す
				// 不透明部分を描画
				g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);
	
				// 半透明部分を描画
				SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
				SetZWrite(false);				// Zバッファ更新しない
				g_model.Draw(pDC, g_mtxWorld, eTransparentOnly);
				SetZWrite(true);				// Zバッファ更新する
				SetBlendState(BS_NONE);			// アルファブレンド無効
	
			}
		}
	}

	// 壁描画 (不透明部分)
	//DrawMeshWall(DRAWPART_OPAQUE);


}
