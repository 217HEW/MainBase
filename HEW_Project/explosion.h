//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : 上月大地
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);

int StartExplosion(XMFLOAT3 pos, XMFLOAT2 size);
void SetExplosionColor(int nExplosion, XMFLOAT4 color);
