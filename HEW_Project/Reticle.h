//=============================================================================
//
// ���e�B�N������ [Reticle.h]
// Author : �㌎��n
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitReticle(void);
void UninitReticle(void);
void UpdateReticle(void);
void DrawReticle(void);

// �Ə��ݒu
int SetReticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT2 size, int timer);