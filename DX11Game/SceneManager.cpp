//**************************************************************
//
//	SceneManager.cpp
//	シーンの制御を担うプログラム
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//	開発履歴
//	2021/11/30	シーン制御処理の実装開始	/柴山凜太郎
//	2021/12/03	シーン制御処理ベース実装完了/柴山凜太郎
//	2021/12/06	クラスで扱おうと試みたが断念、柴山凜太郎君が組んでくれたプログラムを置き換え/上月大地
//	2021/12/19	フェード処理を完成、暗転→明転が可能/上月大地
//
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneGameover.h"
#include "Fade.h"

//**************************************************************
// グローバル宣言
//**************************************************************
static EScene g_eScene = SCENE_TITLE;	// 最初の画面をタイトルに設定

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitScene()
{
	HRESULT hr = S_OK;
	InitFade();		// フェード初期化
	InitTitle();

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
	case SCENE_TITLE:		// タイトル画面
		UpdateTitle();
		break;
	case SCENE_GAME:		// ゲーム画面
		UpdateGame();
		break;
	// 何かシーンを追加する場合はこちらに
	case SCENE_GAMEOVER:	// ゲームオーバー画面
		UpdateGameover();
		break;
	}
	UpdateFade();			//フェード更新
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawScene()
{
	switch (g_eScene)
	{
	case SCENE_TITLE:		// タイトル画面
		DrawTitle();
		break;
	case SCENE_GAME:		// ゲーム画面
		DrawGame();
		break;
	// 追加シーンの描画
	case SCENE_GAMEOVER:		// ゲームオーバー画面
		DrawGameover();
		break;
	}

	// 透過(アルファ値)をいじるときはセットブレンドではさむ
	SetBlendState(BS_ALPHABLEND);
	DrawFade();				// フェード描画
	SetBlendState(BS_NONE);

}


//*******************************
//
//	シーンの切り替え処理
//	
//	引数:入れ替えたいシーン番号
//
//	戻り値
//			:無し
//
//*******************************
void SetScene(EScene eScene)
{
	// 現在の画面を終了
	switch (g_eScene)
	{
	case SCENE_TITLE: // タイトル画面
		UninitTitle();
		break;
	case SCENE_GAME: // ゲーム画面
		UninitGame();
		break;
	case SCENE_GAMEOVER: // ゲームオーバー画面
		UninitGameover();
		break;
	}

	// 画面を入れ替え
	g_eScene = eScene;

	// 次の画面を初期化
	switch (g_eScene)
	{
	case SCENE_TITLE: // タイトル画面
		InitTitle();
		break;
	case SCENE_GAME: // ゲーム画面
		InitGame();
		break;
	case SCENE_GAMEOVER: // ゲームオーバー画面
		InitGameover();
		break;
	}
}