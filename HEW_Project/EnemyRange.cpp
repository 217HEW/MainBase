//**************************************************************
//
//	EnemyRange.cpp
//	�������U���G�l�~�[
//
//--------------------------------------------------------------
//	����ҁF�Ό����l
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/28	�G�͈̔͂ɓ�������^�C�}�[�Ń_���[�W����炤����
//				�̎���
//	2021/01/03	�W�����v���͍U��������Ȃ������̎���
//	�ҏW�ҁF�Ό����l
//--------------------------------------------------------------
//	2022/01/14	�����������G�l�~�[���f���𓱓����܂����B
//				�ˌ��܂ł̃N�[���^�C���𑽏������_���ɂ��鏈����
//				�쐬��
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2022/01/22	���u�G�̏����擾����֐��̍쐬
//				�v���C���[�̃��C�t�����͂����ŉ�����ׂ��łȂ����ߍ폜
//	�ҏW�ҁF�ĎR꣑��Y
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "EnemyRange.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "explosion.h"
#include "life.h"
#include "timer.h"
//#include "SceneManager.h"



//**************************************************************
// �}�N����`
//**************************************************************
#define MODEL_ENEMY			"data/model/Range/Range.fbx"	// "data/model/helicopter000.fbx"

#define MAX_ENEMYRANGE			(10)		// �G�@�ő吔

#define SEARCH_RANGE			(200)		// �T���͈�

#define ENEMY_TIMER				(5)			// ��������

#define SCALE_E_RANGE		(XMFLOAT3(0.05f, 0.1f, 0.1f))
//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static CAssimpModel	g_model;			// ���f��
static TEnemyRange	g_ERange[MAX_ENEMYRANGE];	// �G�@���


//**************************************************************
// ����������
//**************************************************************
HRESULT InitEnemyRange(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	// ���f���f�[�^�̓ǂݍ���
	g_model.SetDif(XMFLOAT4(0.2f,5.0f,0.2f,1.0f));
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY))
	{
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < MAX_ENEMYRANGE; ++i)
	{// ���������������m������΂����Ɂ�
		g_ERange[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_ERange[i].m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_ERange[i].m_rotDest = g_ERange[i].m_rot;
		g_ERange[i].m_size = XMFLOAT3(5.0f, 5.0f, 5.0f);
		g_ERange[i].m_use = false;
		// g_ERange[i].m_Time = ENEMY_TIMER * 60 + 59;
		g_ERange[i].m_Time = (ENEMY_TIMER + rand() % 3) * 60 + 59;	// 3�`6�b�̃����_����
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

	//�v���C���[�̍��W�E�T�C�Y�擾
	XMFLOAT3 posPlayer = GetPlayerPos();
	float sizePlayer = GetPlayerRadSize();

	for (int i = 0; i < MAX_ENEMYRANGE; ++i)
	{
		//GetPlayerJump();
		//�G�ƃv���C���[�̋������߂Â�����
		//if (CollisionSphere(posPlayer, sizePlayer, g_ERange[i].m_pos, SEARCH_RANGE))
		//{
			// �g�p���Ȃ�X�L�b�v
			if (!g_ERange[i].m_use)
			{
				continue;
			}
			// �^�C�}�[�J�E���g�_�E��
			if (g_ERange[i].m_Time > 0)
			{
				--g_ERange[i].m_Time;
				if (g_ERange[i].m_Time == 0)
				{
					if (GetPlayerJump())
					{
						DelLife();
						StartExplosion(posPlayer, XMFLOAT2(40.0f, 40.0f));
					}
					else
					{
						continue;
					}
					g_ERange[i].m_Time += (ENEMY_TIMER + rand() % 3) * 60 + 59;	// ������x3�`6�b������
					
				}
			}
			/*else
			{
				g_ERange[i].m_Time;
			}*/

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_ERange[i].m_rot.x),
				XMConvertToRadians(g_ERange[i].m_rot.y),
				XMConvertToRadians(g_ERange[i].m_rot.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			mtxWorld = XMMatrixScaling(SCALE_E_RANGE.x, SCALE_E_RANGE.y, SCALE_E_RANGE.z);
			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(
				g_ERange[i].m_pos.x,
				g_ERange[i].m_pos.y,
				g_ERange[i].m_pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			XMStoreFloat4x4(&g_ERange[i].m_mtxWorld, mtxWorld);
		//}
	}
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawEnemyRange(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	for (int i = 0; i < MAX_ENEMYRANGE; ++i) {
		// �g���ĂȂ��Ȃ�X�L�b�v
		if (!g_ERange[i].m_use)
		{
			continue;
		}
		g_model.Draw(pDC, g_ERange[i].m_mtxWorld, eOpacityOnly);
	}

	// ������������`��
	for (int i = 0; i < MAX_ENEMYRANGE; ++i) {
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_model.Draw(pDC, g_ERange[i].m_mtxWorld, eTransparentOnly);
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

	for (int cntEnemy = 0; cntEnemy < MAX_ENEMYRANGE; ++cntEnemy)
	{
		// �g�p���Ȃ�X�L�b�v
		if (g_ERange[cntEnemy].m_use)
		{
			continue;
		}
		g_ERange[cntEnemy].m_use = true;	// �g�p��ON
		g_ERange[cntEnemy].m_pos = pos;	// �w�肵�����W����

		Enemy = cntEnemy + 1;	// �g�p���̓G������
		break;
	}

	return Enemy;
}

TEnemyRange* GetEnemyRange()
{
	return g_ERange;
}
