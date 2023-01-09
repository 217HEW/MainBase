//=============================================================================
//
// レティクル処理 [Reticle.h]
// Author : 上月大地
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitReticle(void);
void UninitReticle(void);
void UpdateReticle(void);
void DrawReticle(void);

// 照準設置
int SetReticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT2 size, int timer);