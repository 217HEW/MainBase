//**************************************************************
//
//	Player.cpp
//	プレイヤー処理
//
//--------------------------------------------------------------
//	製作者：柴山凜太郎
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/22	コメントの編集&追加
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//**************************************************************
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);	// 初期化
void UninitPlayer(void);	// 終了
void UpdatePlayer(void);	// 更新
void DrawPlayer(void);		// 描画

XMFLOAT3& GetPlayerPos();	// プレイヤー位置情報取得
float GetPlayerRadSize();		// プライヤーサイズ情報取得
bool GetPlayerJump();		// 飛んでいるかのboolを取得する	true:飛んでいない
void SetPlayerJump(bool jump);		// 飛んでいるかのboolをセットする	


bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage);
