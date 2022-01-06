//**************************************************************
//
//	CreateField.cpp
//	マップ生成
//
//--------------------------------------------------------------
//	製作者：澤村瑠人
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/20	二次元配列を用いた、マップ構築
//				生成範囲は未実装
//	編集者：澤村瑠人
//--------------------------------------------------------------
//	2021/12/21 マップチップ完成
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//　2021/12/22	マップチップのswith管理に変更
//　編集者：澤村瑠人
//　		コメントの編集&追加、switch文に「case 0」の追加
//　		何も処理せずにとばす
//			マップチップの縦横を定数で宣言
//　編集者：柴山凜太郎
//--------------------------------------------------------------
//　2021/12/24	ブロックサイズが統一化されたの為、61行にあった
//				サイズに関するコメントを削除しました。
//				マップサイズを25*25に変更しました
//　編集者：上月大地
//--------------------------------------------------------------
//　2021/12/28	エリアの量産のための改良開始
//				g_Mapの要素にエリアを切り替える要素の追加
//				エリアごとのマップチップ(仮)の作成
//				列挙型のグローバル変数を作成
//				InitCFieldに新しくエリアの切り替え用引数を追加
//				エリア状態を取得する関数の作成
//				エリア量産のための改良完了
//　編集者：柴山凜太郎
//**************************************************************

//**************************************************************
//	コメント（意見）
//	2021/12/22	10 * 10 だと物足りないかも？	__澤村瑠人
//	2021/12/22	25 * 25 がレベデザの要望です	＿上月大地
//
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "CreateField.h"
#include "input.h"
#include "AssimpModel.h"
#include "Block.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define MAP_WIDTH (25)
#define MAP_HEIGHT (25)

//**************************************************************
// グローバル変数
//**************************************************************

//関数化してスケールサイズを指定できるようにする
int g_Map[MAX_AREA][MAP_WIDTH][MAP_HEIGHT] =
{
	//壊れるブロック"1"
	//壊れないブロック"2"

	//--------------------------------------------------
	//					エリア１
	//--------------------------------------------------
	//						X
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,//
	 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,2,//
	 1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,2,//
	 1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,2,//
	 1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,2,//
	 1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,2,//
	 1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,2,//
	 1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,2,//
	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//
	 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,//
	 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,//
	 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,//
	 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,//
	 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,//
	 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,//
	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,//
	 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,//
	 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,//
	 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,//
	 1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,//
	 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,//
	},

	//--------------------------------------------------
	//					エリア２
	//--------------------------------------------------
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,//
	 0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	},

	//--------------------------------------------------
	//					エリア３
	//--------------------------------------------------
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,//
	 0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	},

	//--------------------------------------------------
	//					BOSSエリア
	//--------------------------------------------------
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	},
};

static XMFLOAT3 g_MapPosOrizin = XMFLOAT3(-40.0f, 130.0f, 0.0f);	// マップ生成開始座標
AREA eArea;		// エリア切り替え用変数
//=============================================================================
// 初期化処理
// 引数：エリアの切り替え
//=============================================================================
HRESULT InitCField(AREA Area)
{
	HRESULT hr = S_OK;
	//ID3D11Device* pDevice = GetDevice();
	//ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// ブロック初期化
	hr = InitBlock();
	if (FAILED(hr))
		return hr;
	
	// 変数の初期化
	eArea = Area;

	// ブロックのサイズ取得
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
	//二次元配列Map内で、"1"の場所に描画する
	//描画するオブジェクトの関数を呼び出す
	for (int Height = 0; Height < MAP_HEIGHT; Height++)
	{
		for (int Width = 0; Width < MAP_WIDTH; Width++)
		{
			//if (g_Map[Height][Width] == 0)
			//{
			//	continue;
			//}

			switch (g_Map[eArea][Height][Width])
			{
			case 0:
				// 何もしない
				break;
			case 1:
				//マップチップ"1"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y),
					g_MapPosOrizin.z), false);
				break;
			case 2:
				//マップチップ"2"の場所に描画するもの

				// 無敵ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y),
					g_MapPosOrizin.z), true);
				break;
			default:
				break;
			}

			//if (Map[Height][Width] == 1)
			//{
			//	
			//	/*SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x), 
			//					  g_MapPosOrizin.y + (Height * BlockSize.y),
			//					  g_MapPosOrizin.z));*/
			//	
			//}
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
	//XMFLOAT3 model;



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

//=============================================================================
// エリア状態の取得
//=============================================================================
AREA GetAreaState()
{
	return eArea;
}
