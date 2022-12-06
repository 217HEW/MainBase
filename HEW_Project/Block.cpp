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
//--------------------------------------------------------------
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
//--------------------------------------------------------------
//	2122/1/30	���f���̃f�[�^��z��ŊǗ��ł���悤�ɂ��܂����B
//				���f���̎�ގw��p�̗񋓑̂��쐬���܂����B
//				����ɔ����ꕔ��`���Ă����萔���폜���܂����B
//				�u���b�N�̊Ǘ���2�����z��ɂ��܂����B
//				����ɔ���for�����������Ƃ�1�ǉ����܂���
//				���f���̓Ǎ��݂��������̕����ɒu�������܂����B
//				SetBlock�֐�����for���Ńu���񂹂Ȃ������̂�
//				�u���b�N�̎�ނ��Ƃɏ������x�^�������܂����B
//	�ҏW�ҁF�ĎR꣑��Y
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
#define MAX_LIFE		 (1)						// �u���b�N�ϋv�l
#define BLOCK_SCALE		 (20.0f)					// �u���b�N�̃X�P�[���T�C�Y

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

// �u���b�N���f���i�[�p�z��
const char* c_aFileNameStageModel[MAX_BLOCK_TYPE] =
{
	"data/model/Block/Normal/Hew_3_3.fbx",	// �ʏ�u���b�N
	"data/model/Block/Cracks/Hew_2.fbx",	// �Ђъ��ꂽ�u���b�N
	"data/model/Block/Block.fbx",		// ���G�u���b�N
};

static CAssimpModel	g_model[MAX_BLOCK_TYPE];	// ���f�����
static TBLOCK		g_block[MAX_BLOCK][MAX_BLOCK_TYPE];	// �u���b�N���
static XMFLOAT3		g_BlockScale;		// ���݂̃T�C�Y
static XMFLOAT3		g_BlockHalfScale;	// �u���b�N�̔����̃T�C�Y
static XMMATRIX		mtxWorldinv;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBlock(void)
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	HRESULT hr = S_OK;

	for (int nCntBlockType = 0; nCntBlockType < MAX_BLOCK_TYPE; ++nCntBlockType)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; ++nCntBlock)
		{
			//X����{�ɂȂ��Y�̓񕪂̈�ɂ��Ă���
			g_block[nCntBlock][nCntBlockType].m_size = g_BlockScale = XMFLOAT3(10.0f, 10.0f, 10.0f);
			g_block[nCntBlock][nCntBlockType].m_Halfsize = g_BlockHalfScale = XMFLOAT3(5.0f, 10.0f, 10.0f);
			g_block[nCntBlock][nCntBlockType].m_nLife = MAX_LIFE;
			g_block[nCntBlock][nCntBlockType].m_use = false;
			g_block[nCntBlock][nCntBlockType].m_invincible = false;
		}
		switch (nCntBlockType)
		{// ���f�����[�h�O�ɃJ���[���w��
		case BLOCK_NORMAL:
			g_model[nCntBlockType].SetDif(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		case BLOCK_CRACKS:
			g_model[nCntBlockType].SetDif(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		case BLOCK_INVINCIBLE:
			g_model[nCntBlockType].SetDif(XMFLOAT4(1.0f, 0.8f, 0.1f, 1.0f));
			break;
		default:
			break;
		}
		// ���f���f�[�^�̓ǂݍ���
		if (!g_model[nCntBlockType].Load(pDevice, pDeviceContext, c_aFileNameStageModel[nCntBlockType]))
		{
			MessageBoxA(GetMainWnd(), "�u���b�N���f���ǂݍ��݃G���[", "InitBlock", MB_OK);
			return E_FAIL;
		}
	}
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBlock(void)
{
	// ���f���̉��
	for (int i = 0; i < MAX_BLOCK_TYPE; ++i)
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
	for (int nCntBlockType = 0; nCntBlockType < MAX_BLOCK_TYPE; ++nCntBlockType)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; ++nCntBlock)
		{
			XMMATRIX mtxWorld, mtxRot, mtxTranslate;

			if (!g_block[nCntBlock][nCntBlockType].m_use)
			{// ���g�p�Ȃ玟��
				continue;
			}
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			mtxWorld = XMMatrixScaling(g_block[nCntBlock][nCntBlockType].m_size.x,
									   g_block[nCntBlock][nCntBlockType].m_size.y * 2,
									   g_block[nCntBlock][nCntBlockType].m_size.z);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(
									   g_block[nCntBlock][nCntBlockType].m_pos.x,
									   g_block[nCntBlock][nCntBlockType].m_pos.y,
									   g_block[nCntBlock][nCntBlockType].m_pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			XMStoreFloat4x4(&g_block[nCntBlock][nCntBlockType].m_mtxWorld, mtxWorld);

		}
	}

	//------�u���b�N�ƃv���C���[�̓����蔻�菈��-------------------------------

	for (int nCntBlockType = 0; nCntBlockType < MAX_BLOCK_TYPE; nCntBlockType++)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; ++nCntBlock)
		{
			if (!g_block[nCntBlock][nCntBlockType].m_use)
			{// ���g�p�Ȃ玟��
				continue;
			}

			// �ǂƃv���C���[���Փ˂��Ă�����
			if (CollisionAABB(g_block[nCntBlock][nCntBlockType].m_pos, g_block[nCntBlock][nCntBlockType].m_Halfsize,
								GetPlayerPos(), XMFLOAT3(7.0f, 10.0f, 0.5f)))//�v���C���[�̃T�C�Y 
			{
				// �v���C���[���Ƃ�ł�����
				if (GetPlayerJump() == false)
				{

					// �̗͂������Ȃ�����g��Ȃ�
					if (g_block[nCntBlock][nCntBlockType].m_nLife <= 0)
					{
						SetExplosionColor(StartExplosion(g_block[nCntBlock][nCntBlockType].m_pos, XMFLOAT2(80.0f, 80.0f)),XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
						CSound::SetPlayVol(SE_WALLBREAK, 0.05f); // �ǔj��
						g_block[nCntBlock][nCntBlockType].m_use = false;
						//g_model[i].Release();
						break;
					}
					CSound::SetPlayVol(SE_LANDING, 0.1f); // ���n��

					// ���G�u���b�N��������o��
					if (g_block[nCntBlock][nCntBlockType].m_invincible)
					{
						// �����G�t�F�N�g�Đ�
						StartExplosion(GetPlayerPos(), XMFLOAT2(60.0f, 30.0f));
						// �ڒn���ON
						SetPlayerJump(true);
						break;
					}
					// �u���b�N�ɂЂт�����
					g_block[nCntBlock][nCntBlockType].m_nLife--;
					
					// �ڒn���ON
					SetPlayerJump(true);

					// �����G�t�F�N�g�Đ�
					StartExplosion(GetPlayerPos(), XMFLOAT2(50.0f, 30.0f));
				}
				// �ǂ̉E�ɐG��Ă�����
				if (g_block[nCntBlock][nCntBlockType].m_pos.x + g_block[nCntBlock][nCntBlockType].m_Halfsize.x <= GetPlayerPos().x - (3.0f / 2))
				{
					SetPlayerDir(DIR_RIGHT);
				}
				// �ǂ̍��ɐG��Ă�����
				else if (g_block[nCntBlock][nCntBlockType].m_pos.x - g_block[nCntBlock][nCntBlockType].m_Halfsize.x >= GetPlayerPos().x + (3.0f / 2))
				{
					SetPlayerDir(DIR_LEFT);
				}
				// �ǂ̏�ɐG��Ă�����
				else if (g_block[nCntBlock][nCntBlockType].m_pos.y - g_block[nCntBlock][nCntBlockType].m_Halfsize.y >= GetPlayerPos().y + (5.0f / 2))
				{
					SetPlayerDir(DIR_UP);
				}
				// �ǂ̉��ɐG��Ă�����
				else if (g_block[nCntBlock][nCntBlockType].m_pos.y + g_block[nCntBlock][nCntBlockType].m_Halfsize.y <= GetPlayerPos().y - (5.0f / 2))
				{
					SetPlayerDir(DIR_DOWN);
				}
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
	for (int nCntBlockType = 0; nCntBlockType < MAX_BLOCK_TYPE; ++nCntBlockType)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; ++nCntBlock)
		{
			// �g���Ă���u���b�N�̕`��
			if (!g_block[nCntBlock][nCntBlockType].m_use){
				continue;
			}
			// �u���b�N���f���`��
			g_model[nCntBlockType].Draw(pDC, g_block[nCntBlock][nCntBlockType].m_mtxWorld, eOpacityOnly);
		}
	}

	// ������������`��
	for (int i = 0; i < MAX_BLOCK_TYPE; ++i) {
		for (int j = 0; j < MAX_BLOCK; j++)
		{
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
			g_model[i].Draw(pDC, g_block[i][j].m_mtxWorld, eTransparentOnly);
			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����
		}
		
	}
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
		if (inv)
		{// -----------------���G�ǂ̐ݒu------------------------------------------------------------
			// �g�p���Ȃ�X�L�b�v
			if (g_block[cntBlock][BLOCK_INVINCIBLE].m_use)
				continue;
			g_block[cntBlock][BLOCK_INVINCIBLE].m_invincible = true;
			// �g�p���n�m
			g_block[cntBlock][BLOCK_INVINCIBLE].m_use = true;
			g_block[cntBlock][BLOCK_INVINCIBLE].m_pos = pos;
			g_block[cntBlock][BLOCK_INVINCIBLE].m_pos.x += cpos.x;
			g_block[cntBlock][BLOCK_INVINCIBLE].m_pos.y += cpos.y;

			// �w�肳�ꂽ�T�C�Y�ɕύX
			g_block[cntBlock][BLOCK_INVINCIBLE].m_size.x *= size.x;
			g_block[cntBlock][BLOCK_INVINCIBLE].m_size.y *= size.y;


			if (((g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.x * size.x) > g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.x = 
			   (g_block[cntBlock][BLOCK_INVINCIBLE].m_size.x / 2);
			}
			else if (((g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.x * size.x) < g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.x = 
			   (g_block[cntBlock][BLOCK_INVINCIBLE].m_size.x / 2);
			}
			else { g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.x = g_BlockHalfScale.x; }

			if (((g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.y * size.y) > g_BlockHalfScale.y))
			{
				g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.y = 
			   (g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.y * size.y);// -(g_BlockHalfScale.y * 2.0f) + g_BlockHalfScale.y;
			}
			else
			{
				g_block[cntBlock][BLOCK_INVINCIBLE].m_Halfsize.y = g_BlockHalfScale.y;
			}
		}
		else 
		{// --------------�ʏ�ǂ̐ݒu------------------------------------------------------------
			if (g_block[cntBlock][BLOCK_NORMAL].m_use)
				continue;	// �g�p���Ȃ�X�L�b�v

			// �g�p���n�m
			g_block[cntBlock][BLOCK_NORMAL].m_use = true;
			
			// �G�ꂽ�u�Ԃɏ�����悤�ɂ���
			g_block[cntBlock][BLOCK_NORMAL].m_nLife = 0;
			
			// ���W�ݒ�
			g_block[cntBlock][BLOCK_NORMAL].m_pos = pos;
			g_block[cntBlock][BLOCK_NORMAL].m_pos.x += cpos.x;
			g_block[cntBlock][BLOCK_NORMAL].m_pos.y += cpos.y;

			// �w�肳�ꂽ�T�C�Y�ɕύX
			g_block[cntBlock][BLOCK_NORMAL].m_size.x *= size.x;
			g_block[cntBlock][BLOCK_NORMAL].m_size.y *= size.y;


			if (((g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.x * size.x) > g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.x =
			   (g_block[cntBlock][BLOCK_NORMAL].m_size.x / 2);
			}
			else if (((g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.x * size.x) < g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.x =
			   (g_block[cntBlock][BLOCK_NORMAL].m_size.x / 2);
			}
			else { g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.x = g_BlockHalfScale.x; }

			if (((g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.y * size.y) > g_BlockHalfScale.y))
			{
				g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.y =
			   (g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.y * size.y);// -(g_BlockHalfScale.y * 2.0f) + g_BlockHalfScale.y;
			}
			else
			{
				g_block[cntBlock][BLOCK_NORMAL].m_Halfsize.y = g_BlockHalfScale.y;
			}

			// --------------�q�r�ǂ̐ݒu------------------------------------------------------------
			if (g_block[cntBlock][BLOCK_CRACKS].m_use)
				continue;	// �g�p���Ȃ�X�L�b�v

			// �g�p���n�m
			g_block[cntBlock][BLOCK_CRACKS].m_use = true;
			
			// ���W�ݒ�
			g_block[cntBlock][BLOCK_CRACKS].m_pos = pos;
			g_block[cntBlock][BLOCK_CRACKS].m_pos.x += cpos.x;
			g_block[cntBlock][BLOCK_CRACKS].m_pos.y += cpos.y;

			// �w�肳�ꂽ�T�C�Y�ɕύX
			g_block[cntBlock][BLOCK_CRACKS].m_size.x *= size.x;
			g_block[cntBlock][BLOCK_CRACKS].m_size.y *= size.y;


			if (((g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.x * size.x) > g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.x =
			   (g_block[cntBlock][BLOCK_CRACKS].m_size.x / 2);
			}
			else if (((g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.x * size.x) < g_BlockHalfScale.x))
			{
				g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.x =
			   (g_block[cntBlock][BLOCK_CRACKS].m_size.x / 2);
			}
			else { g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.x = g_BlockHalfScale.x; }

			if (((g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.y * size.y) > g_BlockHalfScale.y))
			{
				g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.y =
			   (g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.y * size.y);// -(g_BlockHalfScale.y * 2.0f) + g_BlockHalfScale.y;
			}
			else
			{
				g_block[cntBlock][BLOCK_CRACKS].m_Halfsize.y = g_BlockHalfScale.y;
			}
		}
		
		// �g�p���Ă���u���b�N���̃J�E���g
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
//		�߂�l�F�u���b�N�̏��
//
//***********************************
TBLOCK* GetBlockArray()
{
	return *g_block;
}
