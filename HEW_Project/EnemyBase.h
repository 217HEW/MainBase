//**************************************************************
//
//	EnemyBase.h
//	�G�̌��^�w�b�_
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//	�J������
//	2021/12/13	�G�l�~�[�̌��^�Ƃ��Ēǉ�
//
//**************************************************************
#pragma once

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "main.h"

//**************************************************************
// �v���g�^�C�v�錾
//**************************************************************
HRESULT InitEnemy(void);	// ������
void UninitEnemy(void);		// �I��
void UpdateEnemy(void);		// �X�V
void DrawEnemy(void);		// �`��

int SetEnemy(XMFLOAT3 pos);	// �G�z�u