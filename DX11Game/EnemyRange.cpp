//**************************************************************
//
//	EnemyRange.cpp
//	�������U���G�l�~�[
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//	�J������
//	2021/12/13	�G�l�~�[�̌��^���R�s�[���Ēǉ�
//
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "EnemyBase.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"

//**************************************************************
// �\���̒�`
//**************************************************************
struct TEnemy {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_move;		// �ړ���
	bool		m_use;		// �g�p���Ă邩�ۂ�	ON:�g�p��

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
};

//**************************************************************
// �}�N����`
//**************************************************************
#define MODEL_ENEMY			"data/model/helicopter000.fbx"

#define	VALUE_MOVE_ENEMY	(0.40f)		// �ړ����x
#define MAX_ENEMY			(10)		// �G�@�ő吔

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static CAssimpModel	g_model;			// ���f��
static TEnemy		g_enemy[MAX_ENEMY];	// �G�@���

//**************************************************************
// ����������
//**************************************************************
HRESULT InitEnemyRange(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY))
	{
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < MAX_ENEMY; ++i)
	{// ���������������m������΂����Ɂ�

		g_enemy[i].m_use = false;
	}

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitEnemyRange(void)
{
	// ���f���̉��
	g_model.Release();
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateEnemyRange(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	for (int i = 0; i < MAX_ENEMY; ++i)
	{
		// �����ɓG�̋���������
		//	��

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(g_enemy[i].m_rot.x),
			XMConvertToRadians(g_enemy[i].m_rot.y),
			XMConvertToRadians(g_enemy[i].m_rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(
			g_enemy[i].m_pos.x,
			g_enemy[i].m_pos.y,
			g_enemy[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_enemy[i].m_mtxWorld, mtxWorld);
	}
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawEnemyRange(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	for (int i = 0; i < MAX_ENEMY; ++i) {
		g_model.Draw(pDC, g_enemy[i].m_mtxWorld, eOpacityOnly);
	}

	// ������������`��
	for (int i = 0; i < MAX_ENEMY; ++i) {
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_model.Draw(pDC, g_enemy[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
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
int SetEnemyRange(XMFLOAT3 pos)
{
	// �߂�l�̏�����
	int Enemy = -1;

	for (int cntEnemy = 0; cntEnemy < MAX_ENEMY; ++cntEnemy)
	{
		// �g�p���Ȃ�X�L�b�v
		if (g_enemy[cntEnemy].m_use)
		{
			continue;
		}
		g_enemy[cntEnemy].m_use = true;	// �g�p��ON
		g_enemy[cntEnemy].m_pos = pos;	// �w�肵�����W����

		Enemy = cntEnemy;	// �g�p���̓G������
		break;
	}

	return Enemy;
}
