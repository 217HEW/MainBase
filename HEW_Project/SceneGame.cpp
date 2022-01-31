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
//	編集者：??
//--------------------------------------------------------------
//	2021/12/17	TimerとLifeを前回のDX作品をもとに制作
//				描画にまだ問題あり
//	編集者：??
//--------------------------------------------------------------
//	2021/12/19	描画完了
//	編集者：??
//--------------------------------------------------------------
//	2021/12/21	GetDevice関数格納用ポインタ変数を作成し、適所の変更
//				フェード中に別のフェード処理をしないよう補正
//				ポリゴン、デバッグ表示、入力処理の4大処理削除(ポリゴンは描画処理以外)
//				ポリライン関連削除
//				描画関数の削除
//														編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/22	必要が無いモノの描画、更新を中断しています。
//														変更者：上月大地
//--------------------------------------------------------------
//	2021/12/22	ポーズに移るキーを一つ削除(VK_PAUSE)
//														編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/27	音データを追加したので実装(BGM)
//														変更者：上月大地
//--------------------------------------------------------------
//	2021/12/28	デバッグ用シーン切り替えににボスエリアの追加
//				InitGameの引数にエリア用の引数追加
//				BGM再生処理の手前にエリア毎にBGMを変えたい場合の手順コメントを追加
//				デバッグ操作で遷移できるシーンの追加(AREA2,3,BOSS)
//				CreateField.hをSceneGame.hに移動
//														編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/30	フィールド生成をコメントアウトしてしまっていたので
//				元に戻しました。
//														変更者：上月大地
//--------------------------------------------------------------
//	2022/01/22	Effekseerで作成したエフェクトを再生する処理の追加
//														変更者：柴山凜太郎
//--------------------------------------------------------------
//	2022/01/25	エフェクトクラス用の変数の名前を変更
//				いらない終了処理(MeshuField,MeshWall,Bullet,Smoke)をコメントアウト＆削除しました
//				UpdateBGの呼び出す順番を変更しました
//														変更者：柴山凜太郎
//--------------------------------------------------------------
//	2022/01/30	ポーズ中にリトライを押すと変なエリアに飛ぶ
//				不具合をなくしました
//														変更者：上月大地
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
// #include "polyline.h"
#include "Sound.h"
#include "timer.h"
#include "life.h"
#include "number.h"
// #include "CreateField.h"
#include "Block.h"
#include "EnemyMelee.h"
// #include "EnemyExplode.h"
#include "EnemyRange.h"
#include "Pause.h"
#include "PlayEffect.h"
#include "ClearPause.h"	//テストインクルード
#include "StageSelect.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define MAX_POLYLINE	(20)					// ポリライン数
#define THICKNESS		(10.0f)					// 線の太さ

//**************************************************************
// グローバル変数
//**************************************************************
//TPolyline			g_polyline[MAX_POLYLINE];	// ポリライン情報

bool g_bPause;		// 一時停止中
bool g_bC_Pause;				//一時停止中
int	 g_nNowScene;	// 現在のシーン		
Effect g_GameEffect;			// エフェクト変数
static int g_EffectTimer = 0;	// エフェクト制御用タイマー
//**************************************************************
// 初期化処理
//	引数：遷移先のエリア
//**************************************************************
HRESULT InitGame(AREA Area)
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

	// // フィールド初期化
	// hr = InitMeshField(16, 16, 80.0f, 80.0f);
	// if (FAILED(hr))
	// 	return hr;

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

	// エネミーメレー初期化
	hr = InitEnemyMelee();
	if (FAILED(hr))
		return hr;

	//二次元配列マップ
	hr = InitCField(Area);
	if (FAILED(hr))
		return hr;

	// エネミーエクスプロード初期化
	// hr = InitEnemyExplode();
	// if (FAILED(hr))
	// 	return hr;

	// エクスプロード呼び出し
	// SetEnemyExplode(XMFLOAT3(60.0f, -790.0f, 0.0f));

	// レンジ初期化
	hr = InitEnemyRange();
	if (FAILED(hr))
		return hr;

	// レンジ呼び出し
	//SetEnemyRange(XMFLOAT3(40.0f, -500.0f, 0.0f));

	// メッシュ壁初期化
	hr = InitMeshWall();
	if (FAILED(hr))
		return hr;

	// エフェクト(for Effekseer)初期化
	hr = g_GameEffect.Load();
	if (FAILED(hr))
		return hr;

	// ポーズ初期化
	hr = InitPause();
	g_bPause = false;
	if (FAILED(hr))
		return hr;

	// テスト
	hr = InitC_Pause();
	g_bC_Pause = false;
	if (FAILED(hr))
		return hr;

	//g_nNowScene = GetScene();
	
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

	// シーン番号取得
	g_nNowScene = GetScene();
	g_nNowScene++;

	// BGM再生開始
	// エリア毎にBGMを変えたい時はここをswitch文で切り替えるようにする
	CSound::SetPlayVol(BGM_GAME000, 0.1f); // ゲーム本編BGM

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitGame()
{
	// BGM再生停止
	CSound::Stop(BGM_GAME000);

	//ポーズ終了処理
	UninitPause();

	//テスト
	UninitC_Pause();

	// ポリライン終了処理
	//UninitPolyline();

	// 壁終了
	//UninitMeshWall();

	// レンジ終了
	UninitEnemyRange();

	// エネミーメレー終了
	// UninitEnemyMelee();

	// エネミーエクスプロード終了
	// UninitEnemyExplode();

	// ブロック終了
	//UninitBlock();

	// エフェクト終了
	UninitEffect();

	// 爆発終了
	UninitExplosion();

	// 背景終了
	UninitBG();

	// フィールド終了
	//UninitMeshField();

	// 二次元配列マップ終了
	UninitCField();

	// 自機終了
	UninitPlayer();

	// 丸影終了
	//UninitShadow();

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
	TEnemyMelee* pEMelee = GetEnemyMelee();
	// 入力処理更新
	//UpdateInput();	// 必ずUpdate関数の先頭で実行.

	if (g_bC_Pause) {
		//一時停止更新
		UpdateC_Pause();
	}
	else if (GetFadeState() == FADE_NONE)
	{
		int Timer = GetTimer();
		if (Timer <= 0)
		{
			StartFadeOut(SCENE_GAMEOVER);
		}
	}

	//一時停止中?
	if (g_bPause) {
		//一時停止更新
		UpdatePause();
	}
	else
	{
		if (GetFadeState() == FADE_NONE)
		{
#ifdef _DEBUG
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
		// 	else if (GetKeyRelease(VK_5))
		// 	{
		// 		StartFadeOut(SCENE_AREA_DEBUG);
		// 	}
			else if (GetKeyRelease(VK_6))
			{
				StartFadeOut(SCENE_GAMEOVER);
			}
			else if (GetKeyRelease(VK_7))
			{
				StartFadeOut(SCENE_GAMECLEAR);
			}
			else if (GetKeyRelease(VK_8))
			{
				StartFadeOut(SCENE_SELECT);
			}
#endif
			int Timer = GetTimer();
			if (Timer <= 0)
			{
				StartFadeOut(SCENE_GAMEOVER);
			}
		}


		// デバッグ文字列表示更新
		//UpdateDebugProc();

		// デバッグ文字列設定
		//StartDebugProc();

		// ポリゴン表示更新
		//UpdatePolygon();

		// 背景更新
		UpdateBG();

		// 自機更新
		UpdatePlayer();

		// エネミーメレー更新
		// UpdateEnemyMelee();

		// エネミーエクスプロード更新
		// UpdateEnemyExplode();

		// エネミーレンジ更新
		UpdateEnemyRange();

		// 壁更新
		//UpdateMeshWall();

		// フィールド更新
		//UpdateMeshField();

		// 二次元配列マップ更新
		UpdateCField();

		//*12/17澤村瑠人追加
		// タイマー更新
		UpdateTimer();

		// 丸影更新
		//UpdateShadow();

		// カメラ更新
		CCamera::Get()->Update();

		// ビルボード弾更新
		//UpdateBullet();

		// 爆発更新
		UpdateExplosion();

		// エフェクト更新
		UpdateEffect();

		// エフェクト(for Effekseer)更新
		if (g_EffectTimer == 0)
		{
			g_GameEffect.Set(EFFECT_FIRE, XMFLOAT3(-50, -50, 0), XMFLOAT3(10.0f, 10.0f, 10.0f), 0.1f, XMFLOAT3(1.0f, 1.0f, 1.0f));
			g_EffectTimer = 30;
		}
		--g_EffectTimer;
		g_GameEffect.Update();


		// ブロック更新
		// UpdateBlock();

		// 煙更新
		// UpdateSmoke();

		// ブロック更新
		// ポリライン更新
		// for (int i = 0; i < MAX_POLYLINE; ++i) {
		// 	UpdatePolyline(&g_polyline[i]);
		// }


	}
	//一時停止ON/OFF
	if (GetKeyTrigger(VK_P))
	{
		if (GetFadeState() == FADE_NONE)
		{
			g_bPause = !g_bPause;
			if (g_bPause) {
				//CSound::Pause();
				CSound::SetVolume(BGM_GAME000, 0.06f);
				CSound::SetPlayVol(SE_SELECT, 0.1f); // セレクト

				ResetPauseMenu();
			}
			else
			{
				CSound::SetVolume(BGM_GAME000, 0.1f);
				CSound::SetPlayVol(SE_CANCEL, 0.1f); // キャンセル
				//CSound::Resume();
			}
		}
	}

	//一時停止ON/OFF
	if (GetKeyTrigger(VK_M))
	{
		if (GetFadeState() == FADE_NONE)
		{
			g_bC_Pause = !g_bC_Pause;
			if (g_bC_Pause) {
				//CSound::Pause();
				CSound::SetVolume(BGM_GAME000, 0.06f);
				CSound::SetPlayVol(SE_SELECT, 0.1f); // セレクト
	
				ResetPauseMenu();
			}
			else
			{
				CSound::SetVolume(BGM_GAME000, 0.1f);
				CSound::SetPlayVol(SE_CANCEL, 0.1f); // キャンセル
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
			case PAUSE_MENU_CONTINUE:	// コンテニュー
				g_bPause = false;
				CSound::SetVolume(BGM_GAME000, 0.1f);
				CSound::SetPlayVol(SE_CANCEL, 0.1f); // キャンセル
				//CSound::Resume();
				break;
			case PAUSE_MENU_RETRY:		// リトライ
				StartFadeOut(g_nNowScene);
				CSound::SetPlayVol(SE_SELECT, 0.1f); // セレクト
				break;
			case PAUSE_MENU_QUIT:		// ゲームを辞める
				StartFadeOut(SCENE_TITLE);
				CSound::SetPlayVol(SE_SELECT, 0.1f); // セレクト
				break;
			}
		}
	}

	//テスト
	if (g_bC_Pause && GetFadeState() == FADE_NONE)
	{
		//[ENTER]が押された?
		if (GetKeyTrigger(VK_RETURN))
		{
			//選択中のメニュー項目により分岐
			switch (GetC_PauseMenu())
			{
			case C_PAUSE_MENU_NEXTSTAGE:	// ネクステージ
				StartFadeOut(g_nNowScene);
				//g_bC_Pause = false;
				CSound::SetVolume(BGM_GAME000, 0.1f);
				CSound::SetPlayVol(SE_CANCEL, 0.1f); // キャンセル
				//CSound::Resume();
				break;
			case C_PAUSE_MENU_SELECT:		// セレクト画面
				StartFadeOut(SCENE_SELECT);
				CSound::SetPlayVol(SE_SELECT, 0.1f); // セレクト
				break;
			case C_PAUSE_MENU_QUIT:		// ゲームを辞める
				StartFadeOut(SCENE_TITLE);
				CSound::SetPlayVol(SE_SELECT, 0.1f); // セレクト
				break;
			}
		}
	}
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

	// エネミーメレー
	DrawEnemyMelee();

	// エネミーエクスプロード
	// DrawEnemyExplode();

	// エネミーレンジ
	DrawEnemyRange();

	// 丸影描画
	//DrawShadow();

	// ビルボード弾描画
	DrawBullet();

	// 煙描画
	//DrawSmoke();

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

	SetZBuffer(false);

	// エフェクト描画
	DrawEffect();
	SetZBuffer(true);
	//一時停止描画
	if (g_bPause) {
		DrawPause();
	}

	//一時停止描画
	if (g_bC_Pause) {
		DrawC_Pause();
	}

	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	// デバッグ文字列表示(透過)
	SetBlendState(BS_ALPHABLEND);
	SetZBuffer(false);

	// タイマー表示
	DrawTimer();
	// ライフ表示(完了)
	DrawLife();
	//DrawDebugProc();
	SetBlendState(BS_NONE);
}


