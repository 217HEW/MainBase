//**************************************************************
//
//	SceneManager.cpp
//	シーンの制御を担うプログラム
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/11/30	シーン制御処理の実装開始
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/03	シーン制御処理ベース実装完了
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/06	クラスで扱おうと試みたが断念、柴山凜太郎君が組んでくれたプログラムを置き換え
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/19	Draw関数にSetBlendState関数の追加
//				フェード処理完成、暗転→明転が可能
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/21	SetScene関数の型を「int→HRESULT」に変更
//				シーンの初期化処理部分にあったタイトル初期化処理をSetScene関数へ変更
//				SetBlendState関数を「SceneManager.cppからFade.cppへ移動」
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/28	遷移するシーンに新しくボスエリアの追加
//				エリア系cppの4大処理からGame.cppの4大処理に変更
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//--------------------------------------------------------------
//	2021/01/22	10エリア分のスイッチを用意しました。
//				もしかしたらもっと短くなるかもです
//	編集者：上月大地
//--------------------------------------------------------------
//	2022/01/30	シーン用の引数をintに変更しました
//	編集者：上月大地
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneArea2.h"
#include "SceneArea3.h"
#include "SceneGameover.h"
#include "SceneGameclear.h"
#include "StageSelect.h"
#include "Fade.h"
#include "bg.h"
//**************************************************************
// グローバル宣言
//**************************************************************
static int g_eScene;	// シーン設定用変数

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitScene()
{
	HRESULT hr = S_OK;
	// フェード初期化
	InitFade();
	g_eScene = SCENE_TITLE;		// 最初の画面をタイトルに設定
	hr = SetScene(SCENE_TITLE);	//最初はタイトル画面へ

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitScene()
{
	SetScene(SCENE_NONE);	// 現在の画面を終了
	UninitFade();			// フェード終了処理
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdateScene()
{
	switch (g_eScene)
	{
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAME:
		UpdateGame();
		break;
	case SCENE_AREA2:		// ゲーム画面
		UpdateGame();
		break;
	case SCENE_AREA3:		// ゲーム画面
		UpdateGame();
		break;
	case SCENE_AREA4: // ゲーム画面エリア4
		UpdateGame();
		break;
	case SCENE_AREA5: // ゲーム画面エリア5
		UpdateGame();
		break;
	case SCENE_AREA6: // ゲーム画面エリア6
		UpdateGame();
		break;
	case SCENE_AREA7: // ゲーム画面エリア7
		UpdateGame();
		break;
	case SCENE_AREA8: // ゲーム画面エリア8
		UpdateGame();
		break;
	case SCENE_AREA9: // ゲーム画面エリア9
		UpdateGame();
		break;
	case SCENE_AREA10: // ゲーム画面エリア10
		UpdateGame();
		break;
	// case SCENE_AREA_DEBUG:		// ゲーム画面
	// 	UpdateGame();
	// 	break;
	// 何かシーンを追加する場合はこちらに
	case SCENE_GAMEOVER:
		UpdateGameover();
		break;
	case SCENE_GAMECLEAR:	// ゲームクリア画面
		UpdateGameclear();
		break;
	case SCENE_SELECT:		// セレクト画面
		UpdateSelect();
		break;
	}

	//フェード更新
	UpdateFade();
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawScene()
{
	switch (g_eScene)
	{
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAME:
		DrawGame();
		break;
	case SCENE_AREA2:		// ゲーム画面
		DrawGame();
		break;
	case SCENE_AREA3:		// ゲーム画面
		DrawGame();
		break;
	case SCENE_AREA4: // ゲーム画面エリア4
		DrawGame();
		break;
	case SCENE_AREA5: // ゲーム画面エリア5
		DrawGame();
		break;
	case SCENE_AREA6: // ゲーム画面エリア6
		DrawGame();
		break;
	case SCENE_AREA7: // ゲーム画面エリア7
		DrawGame();
		break;
	case SCENE_AREA8: // ゲーム画面エリア8
		DrawGame();
		break;
	case SCENE_AREA9: // ゲーム画面エリア9
		DrawGame();
		break;
	case SCENE_AREA10: // ゲーム画面エリア10
		DrawGame();
		break;
	// case SCENE_AREA_DEBUG:		// ゲーム画面
	// 	DrawGame();
	// 	break;
	// 追加シーンの描画
	case SCENE_GAMEOVER:
		DrawGameover();
		break;
	case SCENE_GAMECLEAR:		// ゲームクリア画面
		DrawGameclear();
		break;
	case SCENE_SELECT:		// セレクト画面
		DrawSelect();
		break;
	}

	// フェード描画
	DrawFade();

}

//*******************************
//
//	シーンの切り替え処理
//	
//	引数:入れ替えたいシーン番号
//
//	戻り値
//			:エラーかどうか
//
//*******************************
HRESULT SetScene(int eScene)
{
	HRESULT hr = S_OK;
	// 現在の画面を終了
	switch (g_eScene)
	{
	case SCENE_TITLE:
		UninitTitle();
		break;
	case SCENE_GAME:
		UninitGame();
		break;
	case SCENE_AREA2: // ゲーム画面
		UninitGame();
		break;
	case SCENE_AREA3: // ゲーム画面
		UninitGame();
		break;
	case SCENE_AREA4: // ゲーム画面エリア4
		UninitGame();
		break;
	case SCENE_AREA5: // ゲーム画面エリア5
		UninitGame();
		break;
	case SCENE_AREA6: // ゲーム画面エリア6
		UninitGame();
		break;
	case SCENE_AREA7: // ゲーム画面エリア7
		UninitGame();
		break;
	case SCENE_AREA8: // ゲーム画面エリア8
		UninitGame();
		break;
	case SCENE_AREA9: // ゲーム画面エリア9
		UninitGame();
		break;
	case SCENE_AREA10: // ゲーム画面エリア10
		UninitGame();
		break;
	//case SCENE_AREA_DEBUG: // ゲーム画面
		//UninitGame();
		//break;
	case SCENE_GAMEOVER: // ゲームオーバー画面
		UninitGameover();
		break;
	case SCENE_GAMECLEAR: // ゲームクリア画面
		UninitGameclear();
		break;
	case SCENE_SELECT:	// セレクト画面
		UninitSelect();
		break;
	}

	// 画面を入れ替え
	g_eScene = eScene;

	// 次の画面を初期化
	switch (g_eScene)
	{
	case SCENE_TITLE:
		InitTitle();
		break;
	case SCENE_GAME:
		InitGame(AREA_1);
		break;
	case SCENE_AREA2: // ゲーム画面エリア2
		InitGame(AREA_2);
		break;
	case SCENE_AREA3: // ゲーム画面エリア3
		InitGame(AREA_3);
		break;
	case SCENE_AREA4: // ゲーム画面エリア4
		InitGame(AREA_4);
		break;
	case SCENE_AREA5: // ゲーム画面エリア5
		InitGame(AREA_5);
		break;
	case SCENE_AREA6: // ゲーム画面エリア6
		InitGame(AREA_6);
		break;
	case SCENE_AREA7: // ゲーム画面エリア7
		InitGame(AREA_7);
		break;
	case SCENE_AREA8: // ゲーム画面エリア8
		InitGame(AREA_8);
		break;
	case SCENE_AREA9: // ゲーム画面エリア9
		InitGame(AREA_9);
		break;
	case SCENE_AREA10: // ゲーム画面エリア10
		InitGame(AREA_10);
		break;
	// case SCENE_AREA_DEBUG: // ゲーム画面エリア3
		//InitGame(AREA_DEBUG);
		break;
	case SCENE_GAMEOVER: // ゲームオーバー画面
		InitGameover();
		break;
	case SCENE_GAMECLEAR: // ゲームクリア画面
		InitGameclear();
		break;
	case SCENE_SELECT:	// セレクト画面
		InitSelect();
		break;
	}
	return hr;
}

// 現在のシーン取得
int GetScene()
{
	return g_eScene;
}