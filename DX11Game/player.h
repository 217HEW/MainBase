//=============================================================================
//
// 自機処理 [player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

XMFLOAT3& GetPlayerPos();	// プレイヤー位置情報取得
float GetPlayerSize();		// プライヤーサイズ情報取得
bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage);
