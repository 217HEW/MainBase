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
//--------------------------------------------------------------
//	2022/12/06	シーン列挙ESceneの定数名をAREA→STAGEに変更しました
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
	SCENE_STAGE2,		// ゲーム画面エリア2
	SCENE_STAGE3,		// ゲーム画面エリア3
	SCENE_STAGE4,		// ゲーム画面エリア4
	SCENE_STAGE5,		// ゲーム画面エリア5
	SCENE_STAGE6,		// ゲーム画面エリア6
	SCENE_STAGE7,		// ゲーム画面エリア7
	SCENE_STAGE8,		// ゲーム画面エリア8
	SCENE_STAGE9,		// ゲーム画面エリア9
	SCENE_STAGE10,		// ゲーム画面エリア10

	//SCENE_AREA_DEBUG,	// ゲーム画面デバッグエリア
	
// ↓シーン追加↓
	SCENE_GAMECLEAR,	//ゲームクリア画面
	SCENE_SELECT,		// セレクト画面
	SCENE_GAMEOVER,		// ゲームオーバー画面
	MAX_SCENE			// シーン総数
};

//**************************************************************
// プロトタイプ宣言
//**************************************************************
class CSceneGame;
class CSceneManager {
public:

private:
	HRESULT Init();	// 初期化
	void Uninit();		// 終了処理
	void Update();		// 更新
	void Draw();		// 描画

	static int m_nScene;	// シーン設定用変数

	CSceneGame* m_SGame;
public:
	 // シーン切替処理
	HRESULT Set(int eScene);
	// シーン情報取得
	int Get() { return m_nScene; };
};





