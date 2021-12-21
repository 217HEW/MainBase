//**************************************************************
//
//	CreateField.cpp
//	マップ生成
//
//==============================================================
//	製作者：澤村瑠人
//==============================================================
//	開発履歴
//	2021/12/20	二次元配列を用いた、マップ構築
//				生成範囲は未実装
//	変更者：澤村瑠人
//--------------------------------------------------------------
//	2021/12/21	
//**************************************************************

#include "CreateField.h"
#include "input.h"
#include "AssimpModel.h"
#include "Block.h"

//#define STAGEWIDTH		500
//#define STAGEHEIGHT		500
//#define FIELD_TEST		"data/model/airplane000.fbx"

//static CAssimpModel	g_model;		// モデル
//static XMFLOAT4X4	g_mtxWorld;		// ワールドマトリックス
//MAP g_Map;


//関数化してスケールサイズを指定できるようにする
	//g_Map[10][10] =
bool Map[10][10] =
{
	//		100
	1,1,1,1,1,1,1,1,1,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//1	
	1,0,0,0,0,0,0,0,0,1,//0
	1,0,0,0,0,0,0,0,0,1,//0
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,1,1,1,1,1,1,1,1,1,//

};

XMFLOAT3 g_MapPosOrizin = XMFLOAT3(0.0f, 0.0f, 100.0f);	// マップ生成開始座標

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCField(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// ブロック初期化
	hr = InitBlock();
	if (FAILED(hr))
		return hr;
	
	XMFLOAT3 BlockSize = GetBlockSize();

	// 位置・回転・スケールの初期設定
	//g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// モデルデータの読み込み
	//if (!g_model.Load(pDevice, pDeviceContext, FIELD_TEST)) {
	//	MessageBoxA(GetMainWnd(), "自機モデルデータ読み込みエラー", "InitModel", MB_OK);
	//	return E_FAIL;
	//}

	//配列の中身の処理
	for (int Height = 0; Height < 10; Height++)
	{
		for (int Width = 0; Width < 10; Width++)
		{
			//if (g_Map[Height][Width] == 1)
			if (Map[Height][Width])
			{
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x), 
								  g_MapPosOrizin.y + (Height * BlockSize.y),
								  g_MapPosOrizin.z));
				//二次元配列Map内で、"1"の場所に描画する
				//描画するオブジェクトの関数を呼び出す
				// 不透明部分を描画
				//g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);

				// 半透明部分を描画
				//SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
				//SetZWrite(false);				// Zバッファ更新しない
				//g_model.Draw(pDC, g_mtxWorld, eTransparentOnly);
				//SetZWrite(true);				// Zバッファ更新する
				//SetBlendState(BS_NONE);			// アルファブレンド無効

			}
		}
	}

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
	//g_model.Release();

	// ブロック終了
	UninitBlock();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCField(void)
{
	//XMMATRIX mtxWorld;
	XMFLOAT3 model;

	

	// ワールドマトリックスの初期化
	//mtxWorld = XMMatrixIdentity();
	// ワールドマトリックス設定
	//XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// 丸影の移動
	//MoveShadow(g_nShadow, g_posModel);

	// ブロック更新
	UpdateBlock();

}

//=============================================================================
// 描画処理
// やりたいこと：マップチップを三次元空間に500 * 500で描画したい
// 出来ていないこと：座標の割り当て？
//=============================================================================
void DrawCField(void)
{
	//ID3D11DeviceContext* pDC = GetDeviceContext();
	
	// ブロック描画
	DrawBlock();
}
