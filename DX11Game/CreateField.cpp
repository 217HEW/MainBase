#include "CreateField.h"
#include "input.h"
#include "AssimpModel.h"

#define STAGEWIDTH		500
#define STAGEHEIGHT		500
#define FIELD_TEST		"data/model/airplane000.fbx"

static CAssimpModel	g_model;		// モデル
static XMFLOAT4X4	g_mtxWorld;		// ワールドマトリックス
MAP g_Map;

typedef struct
{
	XMFLOAT2 g_MapSize;	//配列の中身
	XMFLOAT2 g_MapScale;//マップ自体の大きさ

}MAP;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCField(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	//g_posModel = XMFLOAT3(0.0f, 40.0f, 0.0f);
	//g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, FIELD_TEST)) {
		MessageBoxA(GetMainWnd(), "自機モデルデータ読み込みエラー", "InitModel", MB_OK);
		return E_FAIL;
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
	g_model.Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCField(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();
	// ワールドマトリックス設定
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// 丸影の移動
	//MoveShadow(g_nShadow, g_posModel);

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCField(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();


	//関数化してスケールサイズを指定できるようにする
	g_Map[10][10] =
	{
		//		500
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

	//配列の中身の処理
	for (int Height = 0; Height < 10; Height++)
	{
		for (int Width = 0; Width < 10; Width++)
		{
			if (g_Map[Height][Width] == 1)
			{
				//二次元配列Map内で、"1"の場所に描画する
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


}
