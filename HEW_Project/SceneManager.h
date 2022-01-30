//**************************************************************
//
//	SceneManager.h
//	シーンマネージャーヘッダ
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/06	柴山凜太郎君のプログラムを元に作成
//	編集者：??
//--------------------------------------------------------------
//	開発履歴
//	2021/12/22	コメントの編集&追加
//	編集者：柴山凜太郎
//**************************************************************
#pragma once

//**************************************************************
// インクルード部
//**************************************************************
#include "main.h"

//**************************************************************
// 列挙体宣言
//**************************************************************

// シーン状態
enum EScene
{
	SCENE_NONE = 0,		// 未設定
	SCENE_TITLE,		// タイトル画面
	SCENE_GAME,			// ゲーム画面
	SCENE_AREA2,		// ゲーム画面エリア2
	SCENE_AREA3,		// ゲーム画面エリア3
	SCENE_AREA4,		// ゲーム画面エリア4
	SCENE_AREA5,		// ゲーム画面エリア5
	SCENE_AREA6,		// ゲーム画面エリア6
	SCENE_AREA7,		// ゲーム画面エリア7
	SCENE_AREA8,		// ゲーム画面エリア8
	SCENE_AREA9,		// ゲーム画面エリア9
	SCENE_AREA10,		// ゲーム画面エリア10

	//SCENE_AREA_DEBUG,	// ゲーム画面デバッグエリア
	
// ↓シーン追加↓
	SCENE_SELECT,		// セレクト画面
	SCENE_GAMEOVER,		// ゲームオーバー画面
	SCENE_GAMECLEAR,	//ゲームクリア画面
	MAX_SCENE			// シーン総数
};

//**************************************************************
// プロトタイプ宣言
//**************************************************************
HRESULT InitScene();	// 初期化
void UninitScene();		// 終了処理
void UpdateScene();		// 更新
void DrawScene();		// 描画

HRESULT SetScene(int eScene); // 切替処理
int GetScene(); // シーン情報取得


