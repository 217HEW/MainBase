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
#include "SceneGameover.h"
#include "SceneGameclear.h"
#include "StageSelect.h"
#include "CreateStage.h"
#include "Fade.h"
#include "bg.h"


CSceneManager::CSceneManager()
{
	m_fade = nullptr;
	m_nScene = SCENE_TITLE;		// 最初の画面をタイトルに設定
}
CSceneManager::~CSceneManager()
{
	delete m_fade;
}
//**************************************************************
// 初期化処理
//**************************************************************
HRESULT CSceneManager::Init()
{
	HRESULT hr = S_OK;
	// フェードインスタンス
	m_fade = new CFade();
	// フェード初期化
	m_fade->Init();
	
	hr = Set(SCENE_TITLE);	//最初はタイトル画面へ

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void CSceneManager::Uninit()
{
	Set(SCENE_NONE);	// 現在の画面を終了
	m_fade->Uninit();			// フェード終了処理
}

//**************************************************************
// 更新処理
//**************************************************************
void CSceneManager::Update()
{
	switch (m_nScene)
	{
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAME:
		m_SGame->Update();
		break;
	case SCENE_STAGE2:	// ゲーム画面
		m_SGame->Update();
		break;
	case SCENE_STAGE3:	// ゲーム画面
		m_SGame->Update();
		break;
	case SCENE_STAGE4: // ゲーム画面エリア4
		m_SGame->Update();
		break;
	case SCENE_STAGE5: // ゲーム画面エリア5
		m_SGame->Update();
		break;
	case SCENE_STAGE6: // ゲーム画面エリア6
		m_SGame->Update();
		break;
	case SCENE_STAGE7: // ゲーム画面エリア7
		m_SGame->Update();
		break;
	case SCENE_STAGE8: // ゲーム画面エリア8
		m_SGame->Update();
		break;
	case SCENE_STAGE9: // ゲーム画面エリア9
		m_SGame->Update();
		break;
	case SCENE_STAGE10: // ゲーム画面エリア10
		m_SGame->Update();
		break;
	// case SCENE_STAGE_DEBUG:		// ゲーム画面
	// 	m_SGame->Update();
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
	m_fade->Update();
}

//**************************************************************
// 描画処理
//**************************************************************
void CSceneManager::Draw()
{
	switch (m_nScene)
	{
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAME:
		m_SGame->Draw();
		break;
	case SCENE_STAGE2:		// ゲーム画面
		m_SGame->Update();
		break;
	case SCENE_STAGE3:		// ゲーム画面
		m_SGame->Update();
		break;
	case SCENE_STAGE4: // ゲーム画面エリア4
		m_SGame->Update();
		break;
	case SCENE_STAGE5: // ゲーム画面エリア5
		m_SGame->Update();
		break;
	case SCENE_STAGE6: // ゲーム画面エリア6
		m_SGame->Update();
		break;
	case SCENE_STAGE7: // ゲーム画面エリア7
		m_SGame->Update();
		break;
	case SCENE_STAGE8: // ゲーム画面エリア8
		m_SGame->Update();
		break;
	case SCENE_STAGE9: // ゲーム画面エリア9
		m_SGame->Update();
		break;
	case SCENE_STAGE10: // ゲーム画面エリア10
		m_SGame->Update();
		break;
	// case SCENE_STAGE_DEBUG:		// ゲーム画面
	// 	m_SGame->Update();
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
	m_fade->Draw();

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
HRESULT CSceneManager::Set(int eScene)
{
	HRESULT hr = S_OK;
	// 現在の画面を終了
	switch (m_nScene)
	{
	case SCENE_TITLE:
		UninitTitle();
		break;
	case SCENE_GAME:
		m_SGame->Uninit();
		break;
	case SCENE_STAGE2: // ゲーム画面
		m_SGame->Uninit();
		break;
	case SCENE_STAGE3: // ゲーム画面
		m_SGame->Uninit();
		break;
	case SCENE_STAGE4: // ゲーム画面エリア4
		m_SGame->Uninit();
		break;
	case SCENE_STAGE5: // ゲーム画面エリア5
		m_SGame->Uninit();
		break;
	case SCENE_STAGE6: // ゲーム画面エリア6
		m_SGame->Uninit();
		break;
	case SCENE_STAGE7: // ゲーム画面エリア7
		m_SGame->Uninit();
		break;
	case SCENE_STAGE8: // ゲーム画面エリア8
		m_SGame->Uninit();
		break;
	case SCENE_STAGE9: // ゲーム画面エリア9
		m_SGame->Uninit();
		break;
	case SCENE_STAGE10: // ゲーム画面エリア10
		m_SGame->Uninit();
		break;
	//case SCENE_STAGE_DEBUG: // ゲーム画面
		//m_SGame->Uninit();
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
	m_nScene = eScene;

	// 次の画面を初期化
	switch (m_nScene)
	{
	case SCENE_TITLE:
		InitTitle();
		break;
	case SCENE_GAME:
		m_SGame->Init(STAGE_1);
		break;
	case SCENE_STAGE2: // ゲーム画面エリア2
		m_SGame->Init(STAGE_2);
		break;
	case SCENE_STAGE3: // ゲーム画面エリア3
		m_SGame->Init(STAGE_3);
		break;
	case SCENE_STAGE4: // ゲーム画面エリア4
		m_SGame->Init(STAGE_4);
		break;
	case SCENE_STAGE5: // ゲーム画面エリア5
		m_SGame->Init(STAGE_5);
		break;
	case SCENE_STAGE6: // ゲーム画面エリア6
		m_SGame->Init(STAGE_6);
		break;
	case SCENE_STAGE7: // ゲーム画面エリア7
		m_SGame->Init(STAGE_7);
		break;
	case SCENE_STAGE8: // ゲーム画面エリア8
		m_SGame->Init(STAGE_8);
		break;
	case SCENE_STAGE9: // ゲーム画面エリア9
		m_SGame->Init(STAGE_9);
		break;
	case SCENE_STAGE10: // ゲーム画面エリア10
		m_SGame->Init(STAGE_10);
		break;
	// case SCENE_STAGE_DEBUG: // ゲーム画面エリア3
		//m_SGame->Init(STAGE_DEBUG);
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
