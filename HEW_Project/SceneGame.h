//**************************************************************
//
//	SceneGame.h
//	ゲームシーンのヘッダ
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/06	小嶋悟君のプログラムを元に作成	
//	編集者：??
//	2021/12/28	CreateField.hのインクルード
//				初期化の引数にエリア切り替え用の引数追加
//	編集者：柴山凜太郎
//**************************************************************
#pragma once

//**************************************************************
// インクルード部
//**************************************************************
#include "main.h"
#include "CreateField.h"
#include "player.h"
#include "EnemyMelee.h"
#include "EnemyRange.h"


//**************************************************************
// プロトタイプ宣言
//**************************************************************
class CSceneGame
{
public:
	CSceneGame();
	~CSceneGame();

	virtual HRESULT InitGame(AREA Area); // 初期化
	virtual void UninitGame();	// 終了処理
	virtual void UpdateGame();	// 更新
	virtual void DrawGame();	// 描画


private:
	//本来はPlayerやEnemyを除いたものは別cppなどでまとめて一括で呼びたい
	CPlayer* m_Player;
	CEnemyMelee* m_EnemyMelee;
	CEnemyRange* m_EnemyRange;

};





// HRESULT SetMeshBlock(XMFLOAT3 pos);	// 箱生成
