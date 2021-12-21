//=============================================================================
//
// ���b�V���ǂ̏��� [meshwall.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//**************************************
// �`�敔��
//**************************************
enum EDrawPart {
	DRAWPART_ALL = 0,		// ��ʂȂ��\��
	DRAWPART_OPAQUE,		// �s�����̂ݕ`��
	DRAWPART_TRANSLUCENT,	// �������̂ݕ`��

	DRAWPART_MAX
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TBLOCK {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3    m_size;		// ���݂̃T�C�Y
	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X

	int			m_nLife;	// �ǂ̑ϋv�l
	bool		use;		// �g�p���Ă��邩

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshWall(void);
HRESULT SetMeshWall(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col,
	int nNumBlockX, int nNumBlockY, XMFLOAT2 sizeBlock);
TBLOCK *GetMesh();
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(EDrawPart dp = DRAWPART_ALL);
HRESULT SetMeshBlock(XMFLOAT3 pos);		// ������
