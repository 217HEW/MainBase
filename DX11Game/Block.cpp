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
//	2021/12/21	�@����Wall.cpp����������Đ���	
//				�@�ʏ�u���b�N�A�Ђъ���u���b�N�̓����蔻��Ɛ؂�ւ�������
//				�@�������������s�v�Ȃ̂�FPS����ׁ̈A�R�����g�A�E�g(Draw)
//																		||�ύX�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/21	�@�ʏ�u���b�N�A�Ђъ���u���b�N�̓����蔻��Ɛ؂�ւ�������		
//	2021/12/21	�@�u���b�N�̃T�C�Y���\���̗̂v�f����A�O���[�o���ϐ��ց@||�ύX�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/22	�@���f���f�[�^�̃e�N�X�`����ǂݍ��߂�悤�ɂ��܂����B
//				�@���G�u���b�N���Ăяo����悤�ɂ��܂����B				
//				�@�W�����v��Ԃ̃v���C���[����������ϋv�l������
//				�@�v���C���[��ڒn������悤�ɂ��܂��� 123�`147			||�ύX�ҁF�㌎��n
//	2021/12/21	�u���b�N�̃T�C�Y���\���̗̂v�f����A�O���[�o���ϐ���
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2121/12/22	�R�����g�̕ҏW&�ǉ��A�s�v�ȃ\�[�X�̍폜
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2121/12/28	���f���̓Ǎ������̌������J�n
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2122/1/4	���ʉ���ǉ�
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2122/01/07	�u���b�N�̃T�C�Y�̕ϐ����̈ꕔ��size��scale�ɕύX
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2122/1/8	�T�C�Y�ƃX�P�[���𕪂��܂���
//				�X�P�[������Y����2�{���Ă��܂�
//				���W���Y���Ă���^�f������ו␳
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2122/1/22	���f���J���[�̐؂�ւ���N���G�C�g�t�B�[���h�ŌĂюw���ۂ�
//				������ǉ����Ă܂���
//	�ҏW�ҁF�㌎��n
//**************************************************************

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "Block.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_BLOCK		 "data/model/Hew_3_3.fbx"	// "data/model/Block.fbx"  �ʏ�u���b�N			�e�N�X�`���� Block1.jpg
#define MODEL_CRACKS	 "data/model/Block.fbx"		// "data/model2/Hew_2.fbx"	 �Ђъ��ꂽ�u���b�N	�e�N�X�`���� Block1.jpg�����̓t�H���_��ς��Ĕ��f���Ă��܂�
#define MODEL_INVINCIBLE "data/model/Block.fbx"		// "data/model/Hew_3_3.fbx"	 ���G�u���b�N			�e�N�X�`������
#define MAX_LIFE		 (1)						// �u���b�N�ϋv�l
#define BLOCK_SCALE		 (20.0f)					// �u���b�N�̃X�P�[���T�C�Y

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAssimpModel	g_model[MAX_BLOCK];	// ���f��
static TBLOCK		g_block[MAX_BLOCK];	// �u���b�N���
static XMFLOAT3		g_BlockScale;		// ���݂̃T�C�Y
static XMFLOAT3		g_BlockHalfScale;	// �u���b�N�̔����̃T�C�Y
static XMMATRIX		mtxWorldinv;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBlock(void)
{
	HRESULT hr = S_OK;
	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		//X����{�ɂȂ��Y�̓񕪂̈�ɂ��Ă���
		g_block[i].m_size = g_BlockScale = XMFLOAT3(10.0f, 10.0f, 10.0f);
		g_block[i].m_Halfsize = g_BlockHalfScale = XMFLOAT3(5.0f, 10.0f, 10.0f);
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
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_BLOCK; ++i)
	{
		XMMATRIX mtxWorld, mtxRot, mtxTranslate;

		if (!g_block[i].m_use)
		{// ���g�p�Ȃ玟��
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		//// ���̃T�C�Y
		//mtxWorld = XMMatrixScaling(g_BlockScale.x,
		//	g_BlockScale.y * 2,
		//	g_BlockScale.z);
		mtxWorld = XMMatrixScaling(g_block[i].m_size.x,
			g_block[i].m_size.y * 2,
			g_block[i].m_size.z);

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
		if (CollisionAABB(g_block[i].m_pos, g_block[i].m_Halfsize, GetPlayerPos(), XMFLOAT3(7.0f, 10.0f, 0.5f)))//�v���C���[�̃T�C�Y 
		{
			// �v���C���[���Ƃ�ł�����
			if (GetPlayerJump() == false)
			{
				// �̗͂������Ȃ�����g��Ȃ�
				if (g_block[i].m_nLife <= 0)
				{
					StartExplosion(GetPlayerPos(), XMFLOAT2(80.0f, 80.0f));

					CSound::SetPlayVol(SE_WALLBREAK, 0.05f); // �ǔj��
					g_block[i].m_use = false;
					//g_model[i].Release();
					break;
				}
				CSound::SetPlayVol(SE_LANDING, 0.1f); // ���n��

				// ���G�u���b�N��������o��
				if (g_block[i].m_invincible)
				{
					StartExplosion(GetPlayerPos(), XMFLOAT2(80.0f, 80.0f));
					// �ڒn���ON
					SetPlayerJump(true);
					break;
				}
				// �u���b�N�ɂЂт�����
				g_block[i].m_3Dmodel = MODEL_CRACKS;
				g_model[i].SetDif(XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f));	// ���f�����[�h�O�ɃJ���[���w��
				g_model[i].Load(pDevice, pDeviceContext, g_block[i].m_3Dmodel);
				g_block[i].m_nLife--;
				g_model[i].SetDif(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));	// �J���[��߂�(���̂��ς���Ă��܂�����)
				// �ڒn���ON
				SetPlayerJump(true);

				StartExplosion(GetPlayerPos(), XMFLOAT2(80.0f, 80.0f));
			}
			//�E
			if (g_block[i].m_pos.x + g_block[i].m_Halfsize.x <= GetPlayerPos().x - (7.0f / 2))
			{
				SetPlayerDir(1);
			}
			//��
			else if (g_block[i].m_pos.x - g_block[i].m_Halfsize.x >= GetPlayerPos().x + (7.0f / 2))
			{
				SetPlayerDir(2);
			}
			//��
			else if (g_block[i].m_pos.y - g_block[i].m_Halfsize.y >= GetPlayerPos().y + (10.0f / 2))
			{
				SetPlayerDir(3);
			}
			//��
			else if (g_block[i].m_pos.y + g_block[i].m_Halfsize.y <= GetPlayerPos().y - (10.0f / 2))
			{
				SetPlayerDir(4);
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
//		�u���������W,
//		���G���H,true:���G
//		�u���b�N�̃T�C�Y
//		���W�␳�l
//	�߂�l:
//		�g�p�����u���b�N�̑���
//
//*******************************
int SetBlock(XMFLOAT3 pos, bool inv,XMFLOAT2 size, XMFLOAT2 cpos)
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	int Block = -1;

	for (int cntBlock = 0; cntBlock < MAX_BLOCK; ++cntBlock)
	{
		// �g�p���Ȃ�X�L�b�v
		if (g_block[cntBlock].m_use) {
			continue;
		}

		// ���f���f�[�^�̓ǂݍ���if��
		if (inv == true)
		{
			// ���G�u���b�N�̃��f���f�[�^
			g_block[cntBlock].m_3Dmodel = MODEL_INVINCIBLE;

			// ���G�I��
			g_block[cntBlock].m_invincible = true;
			g_model[cntBlock].SetDif(XMFLOAT4(1.0f, 0.8f, 0.1f, 1.0f));	// ���f�����[�h�O�ɃJ���[���w��
		}
		else
		{	// �ʏ�u���b�N�̃��f���f�[�^
			g_block[cntBlock].m_3Dmodel = MODEL_BLOCK;
			g_model[cntBlock].SetDif(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		g_model[cntBlock].Load(pDevice, pDeviceContext, g_block[cntBlock].m_3Dmodel);

		// �g�p���n�m
		g_block[cntBlock].m_use = true;
		g_block[cntBlock].m_pos = pos;
		g_block[cntBlock].m_pos.x += cpos.x;
		g_block[cntBlock].m_pos.y += cpos.y;

		// �w�肳�ꂽ�T�C�Y�ɕύX
		g_block[cntBlock].m_size.x *= size.x;
		g_block[cntBlock].m_size.y *= size.y;
		if(((g_block[cntBlock].m_Halfsize.x * size.x) > g_BlockHalfScale.x))
		{
			g_block[cntBlock].m_Halfsize.x = (g_block[cntBlock].m_size.x /2);
		}
		else
		{
			g_block[cntBlock].m_Halfsize.x = g_BlockHalfScale.x;
		}

		if (((g_block[cntBlock].m_Halfsize.y * size.y) > g_BlockHalfScale.y))
		{
			g_block[cntBlock].m_Halfsize.y = (g_block[cntBlock].m_Halfsize.y * size.y) - (g_BlockHalfScale.y * 2.0f);
		}
		else
		{
			g_block[cntBlock].m_Halfsize.y *= size.y;

		}

		Block = cntBlock + 1;
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
	return g_BlockScale;
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
