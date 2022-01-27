//**************************************************************
//
//	Boss.cpp
//	�{�X�𓮂�������
//
//--------------------------------------------------------------
//	����ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//**************************************************************
//	2021/12/27�@Boss����̃K�������쐬
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "Boss.h"
#include "AssimpModel.h"
#include "collision.h"
#include "player.h"
#include "Block.h"
#include "life.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define MODEL_BOSS			"data/model/helicopter000.fbx"	// �{�X���f��

#define	VALUE_MOVE_BOSS		(0.40f)		// �ړ����x
#define	BOSS_SIZE_X			(100)		// ���f���̉���
#define	BOSS_SIZE_Y			(100)		// ���f���̏c��
#define	MAX_BOSS_LIFE		(100)		// �ő�̗�

//**************************************************************
// �\���̒�`
//**************************************************************
struct TBoss {
	XMFLOAT3	pos;		// ���݂̈ʒu
	XMFLOAT3	size;		// �傫��
	XMFLOAT3	rot;		// ���݂̌���
	XMFLOAT3	rotDest;	// �ړI�̌���
	XMFLOAT3	move;		// �ړ���
	int			life;		// �̗�
	bool		use;		// �g�p���Ă邩�ۂ�	ON:�g�p��

	XMFLOAT4X4	mtxWorld;	// ���[���h�}�g���b�N�X
};

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static CAssimpModel	g_model;			// ���f�����
static TBoss		g_boss;			// �G�@���

//**************************************************************
// ����������
//**************************************************************
HRESULT InitBoss(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���f���f�[�^�̓ǂݍ���-------------------------------
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_BOSS))
	{
		MessageBoxA(GetMainWnd(), "Boss���f���f�[�^�ǂݍ��݃G���[", "InitBoss", MB_OK);
		return E_FAIL;
	}

	// �ϐ��̏�����-----------------------------------------
	g_boss.pos  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_boss.size = XMFLOAT3(BOSS_SIZE_X, BOSS_SIZE_Y, 0.0f);
	g_boss.rot  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_boss.rotDest = g_boss.rot;
	g_boss.move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_boss.life = MAX_BOSS_LIFE;
	g_boss.use  = false;

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitBoss(void)
{
	// ���f���̉��
	g_model.Release();
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateBoss(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ������boss�̋���������
	//	��

// -------���[���h�}�g���N�X����------------------------------------------

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_boss.rot.x),
				XMConvertToRadians(g_boss.rot.y),
				XMConvertToRadians(g_boss.rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(
					g_boss.pos.x,
					g_boss.pos.y,
					g_boss.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&g_boss.mtxWorld, mtxWorld);
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawBoss(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	g_model.Draw(pDC, g_boss.mtxWorld, eOpacityOnly);
	

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	g_model.Draw(pDC, g_boss.mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	
}

//*******************************
//
//	�G�̔z�u����
//	
//	����:�z�u���������W x y z
//
//	�߂�l
//		:�g�p���Ă���G�̍ő吔
//
//*******************************
int SetBoss(XMFLOAT3 pos)
{
	// �߂�l�̏�����
	int Enemy = 1;

	g_boss.use = true;	// �g�p��ON
	g_boss.pos = pos;	// �w�肵�����W����

	return Enemy;
}