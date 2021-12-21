//**************************************************************
//
//	CreateField.cpp
//	�}�b�v����
//
//==============================================================
//	����ҁF�V���ڐl
//==============================================================
//	�J������
//	2021/12/20	�񎟌��z���p�����A�}�b�v�\�z
//				�����͈͖͂�����
//	�ύX�ҁF�V���ڐl
//--------------------------------------------------------------
//	2021/12/21	
//**************************************************************

#include "CreateField.h"
#include "input.h"
#include "AssimpModel.h"
#include "Block.h"

//#define STAGEWIDTH		500
//#define STAGEHEIGHT		500
//#define FIELD_TEST		"data/model/airplane000.fbx"

//static CAssimpModel	g_model;		// ���f��
//static XMFLOAT4X4	g_mtxWorld;		// ���[���h�}�g���b�N�X
//MAP g_Map;


//�֐������ăX�P�[���T�C�Y���w��ł���悤�ɂ���
	//g_Map[10][10] =
bool Map[10][10] =
{
	//		100
	1,1,1,1,1,1,1,1,1,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//1	
	1,0,0,0,0,0,0,0,0,1,//0
	1,0,0,0,0,0,0,0,0,1,//0
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,1,1,1,1,1,1,1,1,1,//

};

XMFLOAT3 g_MapPosOrizin = XMFLOAT3(0.0f, 0.0f, 100.0f);	// �}�b�v�����J�n���W

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCField(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// �u���b�N������
	hr = InitBlock();
	if (FAILED(hr))
		return hr;
	
	XMFLOAT3 BlockSize = GetBlockSize();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	//g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// ���f���f�[�^�̓ǂݍ���
	//if (!g_model.Load(pDevice, pDeviceContext, FIELD_TEST)) {
	//	MessageBoxA(GetMainWnd(), "���@���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
	//	return E_FAIL;
	//}

	//�z��̒��g�̏���
	for (int Height = 0; Height < 10; Height++)
	{
		for (int Width = 0; Width < 10; Width++)
		{
			//if (g_Map[Height][Width] == 1)
			if (Map[Height][Width])
			{
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x), 
								  g_MapPosOrizin.y + (Height * BlockSize.y),
								  g_MapPosOrizin.z));
				//�񎟌��z��Map���ŁA"1"�̏ꏊ�ɕ`�悷��
				//�`�悷��I�u�W�F�N�g�̊֐����Ăяo��
				// �s����������`��
				//g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);

				// ������������`��
				//SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
				//SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
				//g_model.Draw(pDC, g_mtxWorld, eTransparentOnly);
				//SetZWrite(true);				// Z�o�b�t�@�X�V����
				//SetBlendState(BS_NONE);			// �A���t�@�u�����h����

			}
		}
	}

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCField(void)
{
	// �ۉe�̉��
	//ReleaseShadow(g_nShadow);

	// ���f���̉��
	//g_model.Release();

	// �u���b�N�I��
	UninitBlock();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCField(void)
{
	//XMMATRIX mtxWorld;
	XMFLOAT3 model;

	

	// ���[���h�}�g���b�N�X�̏�����
	//mtxWorld = XMMatrixIdentity();
	// ���[���h�}�g���b�N�X�ݒ�
	//XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// �ۉe�̈ړ�
	//MoveShadow(g_nShadow, g_posModel);

	// �u���b�N�X�V
	UpdateBlock();

}

//=============================================================================
// �`�揈��
// ��肽�����ƁF�}�b�v�`�b�v���O������Ԃ�500 * 500�ŕ`�悵����
// �o���Ă��Ȃ����ƁF���W�̊��蓖�āH
//=============================================================================
void DrawCField(void)
{
	//ID3D11DeviceContext* pDC = GetDeviceContext();
	
	// �u���b�N�`��
	DrawBlock();
}
