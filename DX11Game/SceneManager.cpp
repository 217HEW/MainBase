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
#include "Fade.h"
#include "bg.h"
//**************************************************************
// グローバル宣言
//**************************************************************
static EScene g_eScene;	// シーン設定用変数

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
	case SCENE_AREA_BOSS:		// ゲーム画面
		UpdateGame();
		break;
	// 何かシーンを追加する場合はこちらに
	case SCENE_GAMEOVER:
		UpdateGameover();
		break;
	case SCENE_GAMECLEAR:	// ゲームクリア画面
		UpdateGameclear();
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
	case SCENE_AREA_BOSS:		// ゲーム画面
		DrawGame();
		break;
	// 追加シーンの描画
	case SCENE_GAMEOVER:
		DrawGameover();
		break;
	case SCENE_GAMECLEAR:		// ゲームクリア画面
		DrawGameclear();
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
HRESULT SetScene(EScene eScene)
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
	case SCENE_AREA_BOSS: // ゲーム画面
		UninitGame();
		break;
	case SCENE_GAMEOVER: // ゲームオーバー画面
		UninitGameover();
		break;
	case SCENE_GAMECLEAR: // ゲームクリア画面
		UninitGameclear();
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
	case SCENE_AREA_BOSS: // ゲーム画面エリア3
		InitGame(AREA_BOSS);
		break;
	case SCENE_GAMEOVER: // ゲームオーバー画面
		InitGameover();
		break;
	case SCENE_GAMECLEAR: // ゲームクリア画面
		InitGameclear();
		break;
	}
	return hr;
}
