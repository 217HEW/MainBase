//**************************************************************
//
//	Gate.cpp
//	�Q�[���V�[���J�n���̃Q�[�g�̏���
//
//--------------------------------------------------------------
//	����ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/21	�Q�[�g�̍쐬�J�n
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/30	for�����񂷂��߂̕ϐ��̏����l��0��1�ɕύX
//				���̂���SetGate�̖߂�l�̌v�Z�uGate = cntGate + 1�v
//				����u+1�v���폜
//	�ҏW�ҁF�ĎR꣑��Y
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "Gate.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define MODEL_GATE		"data/model/Block.fbx"	// �Q�[�g���f��
#define GATE_SIZE		(XMFLOAT3(20.0f,40.0f,10.0f))	// �Q�[�g�̑傫��
#define MAX_GATE		(2)		// 


//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static CAssimpModel	g_model;	// ���f��
static TGATE		g_gate[MAX_GATE];	// �Ǐ��
static XMFLOAT3		g_GateSize;		// ���݂̃T�C�Y

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGate(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// ------�u���b�N�̏�����-----------------------------------------------------
	for (int i = 1; i < MAX_GATE; ++i)
	{
		//X����{�ɂȂ�ׁAX��Y�̓񕪂̈�ɂ��Ă���
		//g_gate[i].model3D = MODEL_GATE;
		g_GateSize = GATE_SIZE;
		g_gate[i].use = false;
		g_gate[i].invincible = false;
		
	}
	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_GATE))
	{
		MessageBoxA(GetMainWnd(), "�Q�[�g���f���f�[�^�ǂݍ��݃G���[", "InitGate", MB_OK);
		return E_FAIL;
	}
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGate(void)
{
	g_model.Release();	
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGate(void)
{
	//------���[���h�}�g���N�X�Ƀu���b�N�̃f�[�^�𔽉f----------------------------

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	for (int i = 0; i < MAX_GATE; ++i)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// ���̃T�C�Y
		mtxWorld = XMMatrixScaling(g_GateSize.x,
								   g_GateSize.y,
								   g_GateSize.z);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(
					   g_gate[i].pos.x,
					   g_gate[i].pos.y,
					   g_gate[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_gate[i].mtxWorld, mtxWorld);

	}

	XMFLOAT3 PlayerSize = GetPlayerSize();
	XMFLOAT3 PlayerPos = GetPlayerPos();

	//------�u���b�N�ƃv���C���[�̓����蔻�菈��-------------------------------
	for (int i = 0; i < MAX_GATE; ++i)
	{
		if (!g_gate[i].use)
		// ���g�p�Ȃ玟��
			continue;
		
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGate(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	for (int i = 0; i < MAX_GATE; ++i)
	{
		// �g���Ă���u���b�N�̕`��
		if (!g_gate[i].use)
		{
			continue;
		}
		// �u���b�N���f���`��
		g_model.Draw(pDC, g_gate[i].mtxWorld, eOpacityOnly);
	}

	// ������������`��
	//for (int i = 0; i < MAX_GATE; ++i) {
	//	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	//	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	//	g_model[i].Draw(pDC, g_gate[i].m_mtxWorld, eTransparentOnly);
	//	SetZWrite(true);				// Z�o�b�t�@�X�V����
	//	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	//}
}

//*******************************
//
//		�u���b�N�̔z�u����
//	
//	����:
//		�u���������W
//
//	�߂�l:
//		�g�p�����u���b�N�̑���
//
//*******************************
int SetGate(XMFLOAT3 pos)
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	int Gate = -1;

	for (int cntGate = 1; cntGate < MAX_GATE; ++cntGate) {
		// �g�p���Ȃ�X�L�b�v
		if (g_gate[cntGate].use) {
			continue;
		}
		g_gate[cntGate].use = true;	// �g�p��
		g_gate[cntGate].pos = pos;	// ���W�ݒ�

		Gate = cntGate;
		break;
	}

	return Gate;
}

//*******************************
//
//		�u���b�N�̃T�C�Y�擾
//	
//	����:�Ȃ�
//
//	�߂�l:
//		�u���b�N�̃T�C�Y
//
//*******************************
XMFLOAT3 GetGateSize()
{
	return g_GateSize;
}

//***********************************
//
//		�u���b�N�z��擾
//		
//		�߂�l�F�u���b�N�̃T�C�Y
//
//***********************************
TGATE* GetGateArray()
{
	return g_gate;
}
