//**************************************************************
//
//	EnemyRange.h
//	���u�G�̃w�b�_
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//**************************************************************
#pragma once

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "main.h"

//**************************************************************
// �\���̒�`
//**************************************************************
struct TEnemyRange {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_size;		// ���݂̃T�C�Y
	bool		m_use;		// �g�p���Ă邩�ۂ�	ON:�g�p��
	float ReticleSize;		// ���e�B�N���̃T�C�Y
	int			m_Time;		// �G�̃^�C�}�[

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
};
struct TEnemyRange1 {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_size;		// ���݂̃T�C�Y
	bool		m_use;		// �g�p���Ă邩�ۂ�	ON:�g�p��
	int			m_Time;		// �G�̃^�C�}�[

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
};
struct TEnemyRange2 {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_size;		// ���݂̃T�C�Y
	bool		m_use;		// �g�p���Ă邩�ۂ�	ON:�g�p��
	int			m_Time;		// �G�̃^�C�}�[

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
};
//**************************************************************
// �v���g�^�C�v�錾
//**************************************************************
HRESULT InitEnemyRange(void);	// ������
void UninitEnemyRange(void);	// �I��
void UpdateEnemyRange(void);	// �X�V
void DrawEnemyRange(void);		// �`��

int SetEnemyRange(XMFLOAT3 pos,int id);	// ���u�G�ݒu
TEnemyRange* GetEnemyRange();		// ���u�G���擾