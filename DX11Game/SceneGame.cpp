//**************************************************************
//
//	SceneGame.cpp
//	タイトルシーン中身
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/06	小嶋悟君のプログラムを元に作成	
//	2021/12/17	TimerとLifeを前回のDX作品をもとに制作
//				描画にまだ問題あり
//--------------------------------------------------------------
//	2021/12/19	描画完了
//--------------------------------------------------------------
//	2021/12/21	GetDevice関数格納用ポインタ変数を作成し、適所の変更
//				フェード中に別のフェード処理をしないよう補正
//				ポリゴン、デバッグ表示、入力処理の4大処理削除(ポリゴンは描画処理以外)
//				ポリライン関連削除
//				描画関数の削除
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "SceneGame.h"
#include "SceneManager.h"
#include "Fade.h"
#include "AssimpModel.h"
#include "input.h"
#include "polygon.h"
#include "debugproc.h"
#include "mesh.h"
#include "meshfield.h"
#include "player.h"
#include "shadow.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "smoke.h"
#include "meshwall.h"
//#include "polyline.h"
#include "Sound.h"
#include "timer.h"
#include "life.h"
#include "number.h"
#include "CreateField.h"
#include "Block.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define MAX_POLYLINE	(20)					// ポリライン数
#define THICKNESS		(10.0f)					// 線の太さ

//**************************************************************
// グローバル変数
//**************************************************************
//TPolyline					g_polyline[MAX_POLYLINE];	// ポリライン情報

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitGame()
{
	HRESULT hr = S_OK;

	// メッシュ初期化
	hr = InitMesh();
	if (FAILED(hr))
		return hr;

	// 丸影初期化
	hr = InitShadow();
	if (FAILED(hr))
		return hr;

	// №初期化
	hr = InitNumber();
	if (FAILED(hr))
		return hr;


	//*12/17澤村瑠人追加
	// タイマー表示初期化
	hr = InitTimer();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("タイマー表示初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// ライフ表示初期化
	hr = InitLife();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("ライフ表示初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// 自機初期化
	hr = InitPlayer();
	if (FAILED(hr))
		return hr;

	// フィールド初期化
	hr = InitMeshField(16, 16, 80.0f, 80.0f);
	if (FAILED(hr))
		return hr;

	//二次元配列マップ
	hr = InitCField();
	if (FAILED(hr))
		return hr;

	// 背景初期化
	hr = InitBG();
	if (FAILED(hr))
		return hr;

	// ビルボード弾初期化
	hr = InitBullet();
	if (FAILED(hr))
		return hr;

	// 爆発初期化
	hr = InitExplosion();
	if (FAILED(hr))
		return hr;

	// エフェクト初期化
	hr = InitEffect();
	if (FAILED(hr))
		return hr;

	// 煙初期化
	hr = InitSmoke();
	if (FAILED(hr))
		return hr;

	// ブロック初期化
	//hr = InitBlock();
	//if (FAILED(hr))
	//return hr;

	// メッシュ壁初期化
	 hr = InitMeshWall();
	 if (FAILED(hr))
	 	return hr;
	// SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(40.0f, 40.0f));
	// SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	// SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	// SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));

	// ブロックセット
	// for (int y = 0; y < 10; ++y)
	// {
	//	 for (int x = 0; x < 12; ++x)
	//	 {
	//		 SetBlock(XMFLOAT3(21.0f * x, 41.0f * y, 100.0f));
	//	 }
	// }
	// ボリライン初期化
	// hr = InitPolyline();
	// if (FAILED(hr))
	// 	return hr;
	// XMFLOAT4 vColor[8] = {
	// 	XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
	// 	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	// 	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	// 	XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
	// 	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	// 	XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
	// 	XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
	// 	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	// };
	// for (int i = 0; i < MAX_POLYLINE; ++i) {
	// 	hr = CreatePolyline(&g_polyline[i], THICKNESS, true, vColor[i % 7 + 1], BS_ADDITIVE);
	// 	if (FAILED(hr)) {
	// 		return hr;
	// 	}
	// 	XMFLOAT3 pos;
	// 	pos.x = rand() % 1240 - 620.0f;
	// 	pos.y = rand() % 140 + 10.0f;
	// 	pos.z = rand() % 1240 - 620.0f;
	// 	AddPolylinePoint(&g_polyline[i], pos);
	// 	pos.x = rand() % 1240 - 620.0f;
	// 	pos.y = rand() % 140 + 10.0f;
	// 	pos.z = rand() % 1240 - 620.0f;
	// 	AddPolylinePoint(&g_polyline[i], pos);
	// }

	 

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitGame()
{
	

	// ポリライン終了処理
	//UninitPolyline();

	// 壁終了
	UninitMeshWall();

	// ブロック終了
	//UninitBlock();

	// 煙終了
	UninitSmoke();

	// エフェクト終了
	UninitEffect();

	// 爆発終了
	UninitExplosion();

	// ビルボード弾終了
	UninitBullet();

	// 背景終了
	UninitBG();

	// フィールド終了
	UninitMeshField();

	// 二次元配列マップ終了
	UninitCField();

	// 自機終了
	UninitPlayer();

	// 丸影終了
	UninitShadow();

	//ナンバー終了
	UninitNumber();

	//ライフ終了
	UninitLife();

	//タイマー終了
	UninitTimer();

	// メッシュ終了
	UninitMesh();
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateGame()
{
	if (GetFadeState() == FADE_NONE)
	{
		if (GetKeyRelease(VK_1))
		{
			StartFadeOut(SCENE_TITLE);
		}
		else if (GetKeyRelease(VK_2))
		{
			StartFadeOut(SCENE_GAME);
		}
		else if (GetKeyRelease(VK_3))
		{
			StartFadeOut(SCENE_GAMEOVER);
		}
		else if (GetKeyRelease(VK_4))
		{
			StartFadeOut(SCENE_GAMECLEAR);
		}
	}
	

	// 入力処理更新
	//UpdateInput();	// 必ずUpdate関数の先頭で実行.

	// デバッグ文字列表示更新
	//UpdateDebugProc();

	// デバッグ文字列設定
	//StartDebugProc();
	
	// ポリゴン表示更新
	//UpdatePolygon();

	// 自機更新
	UpdatePlayer();

	// 背景更新
	UpdateBG();

	// 壁更新
	UpdateMeshWall();

	// フィールド更新
	UpdateMeshField();

	// 二次元配列マップ更新
	UpdateCField();

	//*12/17澤村瑠人追加
	// タイマー更新
	UpdateTimer();

	// 丸影更新
	UpdateShadow();

	// カメラ更新
	CCamera::Get()->Update();

	// ビルボード弾更新
	UpdateBullet();

	// 爆発更新
	UpdateExplosion();

	// エフェクト更新
	UpdateEffect();

	// ブロック更新
	// UpdateBlock();

	// 煙更新
	UpdateSmoke();

	// ブロック更新
	// ポリライン更新
	// for (int i = 0; i < MAX_POLYLINE; ++i) {
	// 	UpdatePolyline(&g_polyline[i]);
	// }
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawGame()
{
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	// 背景描画
	DrawBG();

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);

	// フィールド描画
	//DrawMeshField();

	// 二次元配列マップ描画
	DrawCField();

	// 自機描画
	DrawPlayer();
	
	// 丸影描画
	DrawShadow();

	// ビルボード弾描画
	DrawBullet();

	// 煙描画
	DrawSmoke();

	// エフェクト描画
	DrawEffect();

	// 壁描画 (不透明部分)
	DrawMeshWall(DRAWPART_OPAQUE);

	// ブロック描画
	//DrawBlock();

	// 爆発描画
	DrawExplosion();

	// ポリライン描画
	// for (int i = 0; i < MAX_POLYLINE; ++i)
	// {
	// 	DrawPolyline(&g_polyline[i]);
	// }

	// 壁描画 (半透明部分)
	 DrawMeshWall(DRAWPART_TRANSLUCENT);
	 
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	// デバッグ文字列表示(透過)
	SetBlendState(BS_ALPHABLEND);
	// タイマー表示
	DrawTimer();
	// ライフ表示(完了)
	DrawLife();
	//DrawDebugProc();
	SetBlendState(BS_NONE);
}


