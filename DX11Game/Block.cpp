//**************************************************************
//
//	Block.cpp
//	�u���b�N(�ǔ�)�̏���
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/21	����Wall.cpp����������Đ���	
//				�ʏ�u���b�N�A�Ђъ���u���b�N�̓����蔻��Ɛ؂�ւ�������
//				�������������s�v�Ȃ̂�FPS����ׁ̈A�R�����g�A�E�g(Draw)
//--------------------------------------------------------------
//	2021/12/21	�ʏ�u���b�N�A�Ђъ���u���b�N�̓����蔻��Ɛ؂�ւ�������	
//	2021/12/21	�ʏ�u���b�N�A�Ђъ���u���b�N�̓����蔻��Ɛ؂�ւ�������	
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/21	�u���b�N�̃T�C�Y���\���̗̂v�f����A�O���[�o���ϐ���
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2121/12/22	�R�����g�̕ҏW&�ǉ��A�s�v�ȃ\�[�X�̍폜
//**************************************************************
#include "Block.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_BLOCK			"data/model/Block.fbx"	// �ʏ�u���b�N
//#define TEXTURE_BLOCK		"data/model/Block.jpg"	// �ʏ�u���b�N
#define MODEL_CRACKS		"data/model/Block2.fbx"	// �Ђъ��ꂽ�u���b�N
#define MAX_LIFE			(2)			// �ǂ̑ϋv�l

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel	g_model[MAX_BLOCK];	// ���f��
static TBLOCK		g_block[MAX_BLOCK];	// �Ǐ��
static XMFLOAT3		g_BlockSize;		// ���݂̃T�C�Y

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBlock(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
 // ------�u���b�N�̏�����-----------------------------------------------------
	for (int i = 0; i < MAX_BLOCK; ++i)
 	{
		//X����{�ɂȂ��Y�̓񕪂̈�ɂ��Ă���
 		g_BlockSize = XMFLOAT3(20.0f, 40.0f, 10.0f);
 		// g_wall->m_pos = XMFLOAT3(0.0f, 50.0f, 150.0f);
		g_block[i].m_3Dmodel = MODEL_BLOCK;
 		g_block[i].m_nLife = MAX_LIFE;
 		g_block[i].m_use = false;
		g_block[i].m_invincible = false;
		// ���f���f�[�^�̓ǂݍ���
		//if (!g_model[i].Load(pDevice, pDeviceContext, g_block[i].m_3Dmodel))
		//{
		//	MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitBlock", MB_OK);
		//	return E_FAIL;
		//}
 	}
 
 	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBlock(void)
{
	// ���f���̉��
	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		g_model[i].Release();
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBlock(void)
{
 //------���[���h�}�g���N�X�Ƀu���b�N�̃f�[�^�𔽉f----------------------------

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// ���̃T�C�Y
		mtxWorld = XMMatrixScaling(g_BlockSize.x,
								   g_BlockSize.y, 
								   g_BlockSize.z);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(
			g_block[i].m_pos.x,
			g_block[i].m_pos.y,
			g_block[i].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&g_block[i].m_mtxWorld, mtxWorld);

	}

//------�u���b�N�ƃv���C���[�̓����蔻�菈��-------------------------------
	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		if (!g_block[i].m_use)
		{// ���g�p�Ȃ玟��
			continue;
		}
	
		// �ǂƃv���C���[���Փ˂��Ă�����
		 if (CollisionAABB(g_block[i].m_pos, g_BlockSize, GetPlayerPos(), XMFLOAT3(5.0f,5.0f,10.0f)))
		 {
			 // �v���C���[���Ƃ�ł�����

			 ID3D11Device* pDevice = GetDevice();
			 ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

			// �u���b�N�ɂЂт�����
			g_block[i].m_3Dmodel = MODEL_CRACKS;
			g_model[i].Load(pDevice, pDeviceContext, g_block[i].m_3Dmodel);
			g_block[i].m_nLife--;
		 	
		 	// �̗͂������Ȃ�����g��Ȃ�
		 	if (g_block[i].m_nLife <= 0)
		 	{
		 		g_block[i].m_use = false;
				g_model[i].Release();
		 	}
		 }
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBlock(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		// �g���Ă���u���b�N�̕`��
		if (!g_block[i].m_use)
		{
			continue;
		}
		// �u���b�N���f���`��
		g_model[i].Draw(pDC, g_block[i].m_mtxWorld, eOpacityOnly);
	}

	// ������������`��
	//for (int i = 0; i < MAX_BLOCK; ++i) {
	//	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	//	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	//	g_model[i].Draw(pDC, g_block[i].m_mtxWorld, eTransparentOnly);
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
 int SetBlock(XMFLOAT3 pos)
 {
	 ID3D11Device* pDevice = GetDevice();
	 ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
 	int Block = -1;
 
 	for (int cntBlock = 0; cntBlock < MAX_BLOCK; ++cntBlock) {
 		// �g�p���Ȃ�X�L�b�v
 		if (g_block[cntBlock].m_use) {
 			continue;
 		}
 		g_block[cntBlock].m_use = true;	// �g�p��
 		g_block[cntBlock].m_pos = pos;	// ���W�ݒ�
 
		Block = cntBlock +1;
 		break;
 	}
 
 	return Block;
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
XMFLOAT3 GetBlockSize()
{
	return g_BlockSize;
}

//***********************************
//
//		�u���b�N�z��擾
//		
//		�߂�l�F�u���b�N�̃T�C�Y
//
//***********************************
TBLOCK* GetBlockArray()
{
	return g_block;
}
