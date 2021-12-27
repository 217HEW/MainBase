//**************************************************************
//
//	SceneArea3.cpp
//	タイトルシーン中身
//
//--------------------------------------------------------------
//	製作者：小嶋悟
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/22	ゲームエリア3の実装
//	編集者：小嶋悟
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "SceneArea2.h"
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
#include "EnemyMelee.h"
#include "Pause.h"
#include "Texture.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define MAX_POLYLINE	(20)					// ポリライン数
#define THICKNESS		(10.0f)					// 線の太さ

#define PATH_BGTEXTURE "data/texture/Area2.png"
#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT

//**************************************************************
// グローバル変数
//**************************************************************
//TPolyline					g_polyline[MAX_POLYLINE];	// ポリライン情報

static bool g_bPause;								//一時停止中
static ID3D11ShaderResourceView* g_pTexture;

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitArea2()
{
	HRESULT hr = S_OK;

	//// メッシュ初期化
	//hr = InitMesh();
	//if (FAILED(hr))
	//	return hr;

	//// 丸影初期化
	//hr = InitShadow();
	//if (FAILED(hr))
	//	return hr;

	//// №初期化
	//hr = InitNumber();
	//if (FAILED(hr))
	//	return hr;


	////*12/17澤村瑠人追加
	//// タイマー表示初期化
	//hr = InitTimer();
	//if (FAILED(hr))
	//{
	//	MessageBox(GetMainWnd(), _T("タイマー表示初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
	//	return hr;
	//}

	//// ライフ表示初期化
	//hr = InitLife();
	//if (FAILED(hr))
	//{
	//	MessageBox(GetMainWnd(), _T("ライフ表示初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
	//	return hr;
	//}

	//// 自機初期化
	//hr = InitPlayer();
	//if (FAILED(hr))
	//	return hr;

	//// フィールド初期化
	//hr = InitMeshField(16, 16, 80.0f, 80.0f);
	//if (FAILED(hr))
	//	return hr;

	////二次元配列マップ
	//hr = InitCField();
	//if (FAILED(hr))
	//	return hr;

	//// 背景初期化
	//hr = InitBG();
	//if (FAILED(hr))
	//	return hr;

	//// ビルボード弾初期化
	//hr = InitBullet();
	//if (FAILED(hr))
	//	return hr;

	//// 爆発初期化
	//hr = InitExplosion();
	//if (FAILED(hr))
	//	return hr;

	//// エフェクト初期化
	//hr = InitEffect();
	//if (FAILED(hr))
	//	return hr;

	//// 煙初期化
	//hr = InitSmoke();
	//if (FAILED(hr))
	//	return hr;

	//// ブロック初期化
	////hr = InitBlock();
	////if (FAILED(hr))
	////return hr;

	//// エネミーメレー初期化
	//hr = InitEnemyMelee();
	//if (FAILED(hr))
	//	return hr;

	//// メレー呼び出し
	//SetEnemyMelee(XMFLOAT3(30.0f, 30.0f, 0.0f));

	//// メッシュ壁初期化
	//hr = InitMeshWall();
	//if (FAILED(hr))
	//	return hr;

	//// ポーズ初期化
	//hr = InitPause();
	//g_bPause = false;
	//if (FAILED(hr))
	//	return hr;
	//// SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(40.0f, 40.0f));
	//// SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//// SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//// SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));

	//// ブロックセット
	//// for (int y = 0; y < 10; ++y)
	//// {
	////	 for (int x = 0; x < 12; ++x)
	////	 {
	////		 SetBlock(XMFLOAT3(21.0f * x, 41.0f * y, 100.0f));
	////	 }
	//// }
	//// ボリライン初期化
	//// hr = InitPolyline();
	//// if (FAILED(hr))
	//// 	return hr;
	//// XMFLOAT4 vColor[8] = {
	//// 	XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
	//// 	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	//// 	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	//// 	XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
	//// 	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//// 	XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
	//// 	XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
	//// 	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//// };
	//// for (int i = 0; i < MAX_POLYLINE; ++i) {
	//// 	hr = CreatePolyline(&g_polyline[i], THICKNESS, true, vColor[i % 7 + 1], BS_ADDITIVE);
	//// 	if (FAILED(hr)) {
	//// 		return hr;
	//// 	}
	//// 	XMFLOAT3 pos;
	//// 	pos.x = rand() % 1240 - 620.0f;
	//// 	pos.y = rand() % 140 + 10.0f;
	//// 	pos.z = rand() % 1240 - 620.0f;
	//// 	AddPolylinePoint(&g_polyline[i], pos);
	//// 	pos.x = rand() % 1240 - 620.0f;
	//// 	pos.y = rand() % 140 + 10.0f;
	//// 	pos.z = rand() % 1240 - 620.0f;
	//// 	AddPolylinePoint(&g_polyline[i], pos);
	//// }
	//return hr;
	// // BGM再生開始
	//CSound::Play(BGM_000);
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if (FAILED(hr))
		return hr;

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitArea2()
{
	//ポーズ終了処理
	UninitPause();
	//// BGM再生停止
	//CSound::Stop(BGM_000);

	//// ポリライン終了処理
	////UninitPolyline();

	//// 壁終了
	//UninitMeshWall();

	//// エネミーメレー終了
	//UninitEnemyMelee();

	//// ブロック終了
	////UninitBlock();

	//// 煙終了
	//UninitSmoke();

	//// エフェクト終了
	//UninitEffect();

	//// 爆発終了
	//UninitExplosion();

	//// ビルボード弾終了
	//UninitBullet();

	//// 背景終了
	//UninitBG();

	//// フィールド終了
	//UninitMeshField();

	//// 二次元配列マップ終了
	//UninitCField();

	//// 自機終了
	//UninitPlayer();

	//// 丸影終了
	//UninitShadow();

	////ナンバー終了
	//UninitNumber();

	////ライフ終了
	//UninitLife();

	////タイマー終了
	//UninitTimer();

	//// メッシュ終了
	//UninitMesh();
	// 中身無し
	//// BGM再生停止
	//CSound::Stop(BGM_001);
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateArea2()
{
	// 入力処理更新
	//UpdateInput();	// 必ずUpdate関数の先頭で実行.

	//一時停止中?
	if (g_bPause) {
		//一時停止更新
		UpdatePause();
	}
	else
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
				StartFadeOut(SCENE_AREA2);
			}
			else if (GetKeyRelease(VK_4))
			{
				StartFadeOut(SCENE_AREA3);
			}
			else if (GetKeyRelease(VK_5))
			{
				StartFadeOut(SCENE_GAMEOVER);
			}
			else if (GetKeyRelease(VK_6))
			{
				StartFadeOut(SCENE_GAMECLEAR);
			}
		}




		// デバッグ文字列表示更新
		UpdateDebugProc();

		// デバッグ文字列設定
		StartDebugProc();

		//// ポリゴン表示更新
		////UpdatePolygon();

		//// 自機更新
		//UpdatePlayer();

		//// エネミーメレー
		//UpdateEnemyMelee();

		//// 背景更新
		//UpdateBG();

		//// 壁更新
		//UpdateMeshWall();

		//// フィールド更新
		//UpdateMeshField();

		//// 二次元配列マップ更新
		//UpdateCField();

		//*12/17澤村瑠人追加
		// タイマー更新
		UpdateTimer();

		//// 丸影更新
		//UpdateShadow();

		//// カメラ更新
		//CCamera::Get()->Update();

		//// ビルボード弾更新
		//UpdateBullet();

		//// 爆発更新
		//UpdateExplosion();

		//// エフェクト更新
		//UpdateEffect();

		//// ブロック更新
		//// UpdateBlock();

		//// 煙更新
		//UpdateSmoke();

		//// ブロック更新
		//// ポリライン更新
		//// for (int i = 0; i < MAX_POLYLINE; ++i) {
		//// 	UpdatePolyline(&g_polyline[i]);
		//// }


	}
	//一時停止ON/OFF
	if (GetKeyTrigger(VK_P) || GetKeyTrigger(VK_PAUSE))
	{
		if (GetFadeState() == FADE_NONE)
		{
			g_bPause = !g_bPause;
			if (g_bPause) {
				//CSound::Pause();
				//CSound::Play(SE_DECIDE);
				ResetPauseMenu();
			}
			else
			{
				//CSound::Play(SE_CANCEL);
				//CSound::Resume();
			}
		}
	}

	//一時停止メニューの選択
	if (g_bPause && GetFadeState() == FADE_NONE)
	{
		//[ENTER]が押された?
		if (GetKeyTrigger(VK_RETURN))
		{
			//選択中のメニュー項目により分岐
			switch (GetPauseMenu())
			{
			case PAUSE_MENU_CONTINUE:
				g_bPause = false;
				//CSound::Play(SE_CANCEL);
				//CSound::Resume();
				break;
			case PAUSE_MENU_RETRY:
				StartFadeOut(SCENE_GAME);
				break;
			case PAUSE_MENU_QUIT:
				StartFadeOut(SCENE_TITLE);
				break;
			}
		}
	}
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawArea2()
{
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	//// 背景描画
	//DrawBG();

	//// Zバッファ有効(Zチェック有&Z更新有)
	//SetZBuffer(true);

	//// フィールド描画
	////DrawMeshField();

	//// 二次元配列マップ描画
	//DrawCField();

	//// 自機描画
	//DrawPlayer();

	//// エネミーメレー
	//DrawEnemyMelee();

	//// 丸影描画
	//DrawShadow();

	//// ビルボード弾描画
	//DrawBullet();

	//// 煙描画
	//DrawSmoke();

	//// エフェクト描画
	//DrawEffect();

	//// 壁描画 (不透明部分)
	//DrawMeshWall(DRAWPART_OPAQUE);

	//// ブロック描画
	////DrawBlock();

	//// 爆発描画
	//DrawExplosion();

	//// ポリライン描画
	//// for (int i = 0; i < MAX_POLYLINE; ++i)
	//// {
	//// 	DrawPolyline(&g_polyline[i]);
	//// }

	//// 壁描画 (半透明部分)
	//DrawMeshWall(DRAWPART_TRANSLUCENT);

	SetZBuffer(false);
	//一時停止描画
	if (g_bPause) {
		DrawPause();
	}
	SetZBuffer(true);

	//// Zバッファ無効(Zチェック無&Z更新無)
	//SetZBuffer(false);

	//// デバッグ文字列表示(透過)
	//SetBlendState(BS_ALPHABLEND);
	//// タイマー表示
	//DrawTimer();
	//// ライフ表示(完了)
	//DrawLife();
	//DrawDebugProc();

		// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);
	SetBlendState(BS_NONE);
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);
}


