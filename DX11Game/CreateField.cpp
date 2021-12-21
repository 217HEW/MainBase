//=============================================================================
//
// �e [CreateField.cpp]
// Author : SAWAMURA RYUTO
// �J�������F12/20	�񎟌��z���p�����A�}�b�v�\�z
//					�����͈͖͂�����
//
//=============================================================================
#include "CreateField.h"
#include "input.h"
#include "AssimpModel.h"
#include "meshwall.h"

#define MAX_MODEL		110
#define STAGEHEIGHT		500	//�X�e�[�W�̏c��
#define STAGEWIDTH		500 //�X�e�[�W�̉���
#define	SETMAPCHIP_HEIGHT	10	//�}�b�v�`�b�v�̏c��
#define	SETMAPCHIP_WIDTH	10	//�}�b�v�`�b�v�̉���
//#define MODEL_SIZE_X	50
//#define MODEL_SIZE_Y	50

#define FIELD_TEST		"data/model/airplane000.fbx"
//#define MAP_SIZE_HEIGHT	(STAGEHEIGHT / SETMAPCHIP_HEIGHT)	//�`�悷�郂�f���P�̏c��
//#define MAP_SIZE_WIDTH	(STAGEWIDTH / SETMAPCHIP_WIDTH)		//�`�悷�郂�f���P�̉���

static CAssimpModel	g_model;		// ���f��
static XMFLOAT4X4	g_mtxWorld;		// ���[���h�}�g���b�N�X


struct MODEL
{
	XMFLOAT3	g_posModel;
	XMFLOAT3	g_sizeModel;
};
	MODEL g_block[MAX_MODEL];


bool g_Map[SETMAPCHIP_HEIGHT][SETMAPCHIP_WIDTH] =
{
	//*******500*******
	1,1,1,1,1,1,1,1,1,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//5
	1,0,0,0,0,0,0,0,0,1,//0
	1,0,0,0,0,0,0,0,0,1,//0
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,0,0,0,0,0,0,0,0,1,//
	1,1,1,1,1,1,1,1,1,1,//

};
//MAP g_Map;

//typedef struct
//{
//	XMFLOAT2 g_MapSize;	//�z��̒��g
//	XMFLOAT2 g_MapScale;//�}�b�v���̂̑傫��
//
//}MAP;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCField(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	TBLOCK* Mesh = GetMesh();
	
	//for (int i = 0; i < MAX_MODEL; i++)
	//{
	//	//g_block[i].g_posModel  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	//g_block[i].g_sizeModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//}
	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, FIELD_TEST)) 
	{
		MessageBoxA(GetMainWnd(), "���@���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
		return E_FAIL;
	}
	//{
	//	for (int Width = 0; Width < SETMAPCHIP_WIDTH; Width++)
	//	{
	//		
	//		//�}�b�v�`�b�v�ԍ�����
	//		bool chip_number = g_Map[Height][Width];
	//
	//		//if (g_Map[Height][Width] == 1)
	//		//�񎟌��z��Map���ŁA"1"�̏ꏊ�ɕ`�悷��
	//		if (chip_number)
	//		{
	//			// �����Ƀu���b�N��������
	//			SetMeshBlock(XMFLOAT3(Mesh->m_size.x * Width, Mesh->m_size.y * Height, 0.0f));
	//		}//0�ł���Ȃ�΁A���������Ȃ�
	//		else if (!chip_number)
	//		{
	//			continue;
	//		}
	//	}
	//}
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	//g_posModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);


	

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
	g_model.Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCField(void)
{

	

	
	
	// �ǍX�V
	UpdateMeshWall();
	
	XMMATRIX mtxWorld;
	XMFLOAT3 model;

	

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();
	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// �ۉe�̈ړ�
	//MoveShadow(g_nShadow, g_posModel);

}

//=============================================================================
// �`�揈��
// ��肽�����ƁF�}�b�v�`�b�v���O������Ԃ�500 * 500�ŕ`�悵����
// �o���Ă��Ȃ����ƁF���W�̊��蓖�āH
//=============================================================================
void DrawCField(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	//g_Map[10][10] =
	//����[�c10][��10]
	
	//�z��̒��g�̏���
	for (int Height = 0; Height < SETMAPCHIP_HEIGHT; Height++)
	{
		for (int Width = 0; Width < SETMAPCHIP_WIDTH; Width++)
		{
			//�}�b�v�`�b�v�ԍ�����
			int chip_number = g_Map[Height][Width];
			//if (g_Map[Height][Width] == 1)
			//�񎟌��z��Map���ŁA"1"�̏ꏊ�ɕ`�悷��
			if(chip_number == 1)
			{
				//0�ł���Ȃ�΁A���������Ȃ�
				if (chip_number == 0)
				{
					continue;
				}
	
	
				//�`�悷��I�u�W�F�N�g�̊֐����Ăяo��
				// �s����������`��
				g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);
	
				// ������������`��
				SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
				SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
				g_model.Draw(pDC, g_mtxWorld, eTransparentOnly);
				SetZWrite(true);				// Z�o�b�t�@�X�V����
				SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	
			}
		}
	}

	// �Ǖ`�� (�s��������)
	//DrawMeshWall(DRAWPART_OPAQUE);


}
