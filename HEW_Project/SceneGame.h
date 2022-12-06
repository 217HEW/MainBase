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
#include "CreateStage.h"
#include "player.h"
#include "timer.h"
#include "CountEnemy.h"
#include "life.h"
#include "bg.h"
#include "explosion.h"
#include "Pause.h"
#include "ClearPause.h"	//テストインクルード
#include "Reticle.h"
#include "effect.h"
#include "SceneManager.h"
#include "mesh.h"
#include "number.h"

//**************************************************************
// プロトタイプ宣言
//**************************************************************
class CSceneManager;
class CFade;
class CSceneGame
{
public:
	CSceneGame();
	~CSceneGame();

	virtual HRESULT Init(STAGE Stage); // 初期化
	virtual void Uninit();	// 終了処理
	virtual void Update();	// 更新
	virtual void Draw();	// 描画


private:
	//本来はPlayerやEnemyを除いたものは別cppなどでまとめて一括で呼びたい
	//ポインタ変数だからm_p～と後で変える。コードの見やすさ意識する
	CPlayer* m_pPlayer;
	CStage* m_pStage;
	CTimer* m_pTimer;
	CCountEnemy* m_pCountEnemy;
	CLife* m_pLife;
	CBG* m_pBG;
	CExplosion* m_pExplosion;
	CPause* m_pPause;
	CClearPause* m_pCPause;
	CReticle* m_pReticle;
	CEffect* m_pEffect;
	CMesh* m_pMesh;
	CNumber* m_pNumber;
	CPlayer* m_Player;
	CStage* m_Stage;
	CSceneManager* m_SManager;
	CFade* m_fade;
};
