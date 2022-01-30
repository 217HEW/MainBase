//**************************************************************
//
//	EnemyMelee.h
//	�ߐړG�̃w�b�_
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//	�J������
//	2021/12/14	EnemyBase����R�s�[���č쐬
//	�ҏW�ҁF??
//--------------------------------------------------------------
//	2022/01/22	�\���̂̐錾�ꏊ��.cpp����.h�Ɉړ�
//	�ҏW�ҁF�ĎR꣑��Y
//**************************************************************
#pragma once

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "main.h"

//**************************************************************
// �\���̒�`
//**************************************************************
struct TEnemyMelee {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_size;		// ���݂̃T�C�Y
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_move;		// �ړ���
	bool		m_use;		// �g�p���Ă邩�ۂ�	ON:�g�p��

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
	int			m_speed;	// �X�s�[�h
};
struct TEnemyMelee1 {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_size;		// ���݂̃T�C�Y
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_move;		// �ړ���
	bool		m_use;		// �g�p���Ă邩�ۂ�	ON:�g�p��

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
	int			m_speed;	// �X�s�[�h
};
struct TEnemyMelee2 {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_size;		// ���݂̃T�C�Y
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_move;		// �ړ���
	bool		m_use;		// �g�p���Ă邩�ۂ�	ON:�g�p��

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
	int			m_speed;	// �X�s�[�h
};
//**************************************************************
// �v���g�^�C�v�錾
//**************************************************************
HRESULT InitEnemyMelee(void);	// ������
void UninitEnemyMelee(void);	// �I��
void UpdateEnemyMelee(void);	// �X�V
void DrawEnemyMelee(void);		// �`��

int SetEnemyMelee(XMFLOAT3 pos, int id);	 // �ߐړG�ݒu
TEnemyMelee* GetEnemyMelee();		// �ߐړG���擾
