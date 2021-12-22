//**************************************************************
//
//	SceneManager.h
//	シーンマネージャーヘッダ
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//	開発履歴
//	2021/12/06	柴山凜太郎君のプログラムを元に作成	
//
//**************************************************************
#pragma once

//---インクルード部---
#include "main.h"

// 定数定義
enum EScene
{
	SCENE_NONE = 0,	 // 未設定
	SCENE_TITLE,	 // タイトル画面
	SCENE_GAME,		 // ゲーム画面
	SCENE_AREA2,	// ゲーム画面エリア2
	SCENE_AREA3,	// ゲーム画面エリア3
	
// ↓シーン追加↓
	SCENE_GAMEOVER,		 // ゲームオーバー画面
	SCENE_GAMECLEAR,	//ゲームクリア画面
	MAX_SCENE // シーン総数
};

// 関数プロトタイプ
HRESULT InitScene();	// 初期化
void UninitScene();		// 終了処理
void UpdateScene();		// 更新
void DrawScene();		// 描画

HRESULT SetScene(EScene eScene); // 切替処理
HRESULT GetScene(); // シーン情報取得


