//=============================================================================
//
// ���@���� [player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

XMFLOAT3& GetPlayerPos();	// �v���C���[�ʒu���擾
float GetPlayerSize();		// �v���C���[�T�C�Y���擾
bool GetPlayerJump();		// ���ł��邩��bool���擾����	true:���ł��Ȃ�
void SetPlayerJump(bool jump);		// ���ł��邩��bool���Z�b�g����	


bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage);
