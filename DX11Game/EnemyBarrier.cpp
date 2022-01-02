//**************************************************************
//
//	EnemyBarrier
//	�v���C���[���͈͓��ɓ����Ă�����o���A��\��G��|���܂ŏo��Ȃ�������
//
//--------------------------------------------------------------
//	����ҁF������
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/27
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "EnemyBarrier.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "Block.h"
#include "explosion.h"
#include "life.h"
#include "SceneManager.h"

#include "player.h"
//**************************************************************
// �\���̒�`
//**************************************************************
struct TEnemyBarrier {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_size;		// ���݂̃T�C�Y
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	bool		m_use;		// �g�p���Ă邩�ۂ�	ON:�g�p��

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
	int			m_speed;	// �X�s�[�h
};

//**************************************************************
// �}�N����`
//**************************************************************
#define MODEL_ENEMY			"data/model/Hew4.fbx"//�o���A�G�l�~�[
#define MAX_EnemyBarrier			(10)// �G�@�ő吔

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static CAssimpModel		g_model;			// ���f�����
static TEnemyBarrier		g_EMelee[MAX_EnemyBarrier];	// �G�@���
static XMFLOAT3			Blocksize;

//**************************************************************
// ����������
//**************************************************************
HRESULT InitEnemyBarrier(void)
{
	HRESULT hr = S_OK;//
	ID3D11Device* pDevice = GetDevice();//
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �u���b�N�̃T�C�Y�擾
	Blocksize = GetBlockSize();

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY))
	{
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < MAX_EnemyBarrier; ++i)
	{// ���������������m������΂����Ɂ�
		g_EMelee[i].m_pos = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EMelee[i].m_size = (XMFLOAT3(10.0f, 10.0f, 10.0f));
		g_EMelee[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f));
		g_EMelee[i].m_rotDest = g_EMelee[i].m_rot;
		g_EMelee[i].m_use = false;
	}

	return hr;//
}

//**************************************************************
// �I������
//**************************************************************
void UninitEnemyBarrier(void)
{
	// ���f���̉��
	g_model.Release();
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateEnemyBarrier(void)
{
	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	//�v���C���[�̍��W�E�T�C�Y�擾
	XMFLOAT3 posPlayer = GetPlayerPos();
	float sizePlayer = GetPlayerSize();

	for (int i = 0; i < MAX_EnemyBarrier; ++i)
	{
		//�u���b�N�z��擾
		TBLOCK *Block = GetBlockArray();

		//�G�ƃv���C���[�̋������߂Â�����
		if (CollisionSphere(posPlayer, sizePlayer, g_EMelee[i].m_pos, SEARCH_ENEMY))
		{
			if (!g_EMelee[i].m_use)
			{//���g�p�Ȃ玟��
				continue;
			}

			//��Ƀv���C���[�̕���������
			/*g_EMelee[i].m_rotDest = posPlayer;
			g_EMelee[i].m_rot = g_EMelee[i].m_rotDest;
			g_EMelee[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/

			//�G��x���W���v���C���[�����傫��������
			if (g_EMelee[i].m_pos.x >= posPlayer.x)
			{
				g_EMelee[i].m_pos.x += -VALUE_MOVE_ENEMY;
				g_EMelee[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f));

				g_EMelee[i].m_rotDest.y = rotCamera.y - 90.0f;
			}
			//�G��x���W���v���C���[����������������
			if (g_EMelee[i].m_pos.x <= posPlayer.x)
			{
				g_EMelee[i].m_pos.x += VALUE_MOVE_ENEMY;
				g_EMelee[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 90.0f));
				g_EMelee[i].m_rotDest.y = rotCamera.y + 90.0f;

			}
			//�G��y���W���v���C���[�����傫��������
			if (g_EMelee[i].m_pos.y >= posPlayer.y)
			{
				g_EMelee[i].m_pos.y += -VALUE_MOVE_ENEMY;

			}
			//�G��y���W���v���C���[����������������
			if (g_EMelee[i].m_pos.y <= posPlayer.y)
			{
				g_EMelee[i].m_pos.y += VALUE_MOVE_ENEMY;

			}

			//**************************************************************************************
			//		�����蔻��
			//**************************************************************************************

			// �G���m�̓����蔻��
			for (int j = 0; j < MAX_EnemyBarrier; ++j)
			{
				if (CollisionSphere(g_EMelee[i].m_pos, g_EMelee[i].m_size.x, g_EMelee[j].m_pos, g_EMelee[j].m_size.x))
				{
					if (i == j)
					{//�����G�Ȃ�Ƃ΂�
						continue;
					}
					//�G���m���d�Ȃ�Ȃ�����

				}
			}

			// �G�ƕǂ̓����蔻��
			for (int j = 0; j < MAX_BLOCK; ++j, ++Block)
			{
				if (!Block->m_use)
				{// ���g�p�Ȃ玟��
					continue;
				}
				if (CollisionAABB(g_EMelee[i].m_pos, g_EMelee[i].m_size, Block->m_pos, Blocksize))
				{
					//�ǂɓ�����Ǝ~�܂�
					//�E
					if (Block->m_pos.x + Blocksize.x / 2 <= g_EMelee[i].m_pos.x - g_EMelee[i].m_size.x / 2)
					{
						g_EMelee[i].m_pos.x = (Block->m_pos.x + Blocksize.x + g_EMelee[i].m_size.x);
					}
					//��
					else if (Block->m_pos.x - Blocksize.x / 2 >= g_EMelee[i].m_pos.x + g_EMelee[i].m_size.x / 2)
					{
						g_EMelee[i].m_pos.x = (Block->m_pos.x - Blocksize.x - g_EMelee[i].m_size.x);
					}
					//��
					else if (Block->m_pos.y - Blocksize.y / 2 >= g_EMelee[i].m_pos.y + g_EMelee[i].m_size.y / 2)
					{
						g_EMelee[i].m_pos.y = (Block->m_pos.y - Blocksize.y - g_EMelee[i].m_size.y);
					}
					//��
					else if (Block->m_pos.y + Blocksize.y / 2 <= g_EMelee[i].m_pos.y - g_EMelee[i].m_size.y / 2)
					{
						g_EMelee[i].m_pos.y = (Block->m_pos.y + Blocksize.y + g_EMelee[i].m_size.y);
					}
				}
			}

			// �G�ƃv���C���[�̓����蔻��
			if (!g_EMelee[i].m_use)
			{// ���g�p�Ȃ玟��
				continue;
			}
			if (CollisionSphere(g_EMelee[i].m_pos, g_EMelee[i].m_size.x, posPlayer, sizePlayer))
			{
				DelLife();
				if (GetLife() == 0)
				{
					SetScene(SCENE_GAMEOVER);
				}
				g_EMelee[i].m_use = false;
			}

			//�ړI�̊p�x�܂ł̍���
			float fDiffRotY = g_EMelee[i].m_rotDest.y - g_EMelee[i].m_rot.y;
			if (fDiffRotY >= 180.0f) {
				fDiffRotY -= 360.0f;
			}
			if (fDiffRotY < -180.0f) {
				fDiffRotY += 360.0f;
			}

			// �ړI�̊p�x�܂Ŋ�����������
			g_EMelee[i].m_rot.y += fDiffRotY * RATE_ROTATE_ENEMY;
			if (g_EMelee[i].m_rot.y >= 180.0f) {
				g_EMelee[i].m_rot.y -= 360.0f;
			}
			if (g_EMelee[i].m_rot.y < -180.0f) {
				g_EMelee[i].m_rot.y += 360.0f;
			}
			g_EMelee[i].m_move = XMFLOAT3(
				-SinDeg(g_EMelee[i].m_rot.y) * VALUE_MOVE_ENEMY,
				0.0f,
				-CosDeg(g_EMelee[i].m_rot.y) * VALUE_MOVE_ENEMY);

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_EMelee[i].m_rot.x),
				XMConvertToRadians(g_EMelee[i].m_rot.y),
				XMConvertToRadians(g_EMelee[i].m_rot.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(
				g_EMelee[i].m_pos.x,
				g_EMelee[i].m_pos.y,
				g_EMelee[i].m_pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			XMStoreFloat4x4(&g_EMelee[i].m_mtxWorld, mtxWorld);

		}
	}
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawEnemyBarrier(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	for (int i = 0; i < MAX_EnemyBarrier; ++i) {
		if (!g_EMelee[i].m_use)
		{
			continue;
		}
		g_model.Draw(pDC, g_EMelee[i].m_mtxWorld, eOpacityOnly);
	}

	// ������������`��
	for (int i = 0; i < MAX_EnemyBarrier; ++i) {
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_model.Draw(pDC, g_EMelee[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
}

//*****************************************************************
//
//	�G�̔z�u����
//	
//	����:�z�u���������W x y z
//
//	�߂�l
//		:�g�p���Ă���G�̍ő吔
//
//****************************************************************
int SetEnemyBarrier(XMFLOAT3 pos)
{
	// �߂�l�̏�����
	int EnemyBarrier = -1;

	for (int cntEnemyBarrier = 0; cntEnemyBarrier < MAX_EnemyBarrier; ++cntEnemyBarrier)
	{
		// �g�p���Ȃ�X�L�b�v
		if (g_EMelee[cntEnemyBarrier].m_use)
		{
			continue;
		}
		g_EMelee[cntEnemyBarrier].m_use = true;	// �g�p��ON
		g_EMelee[cntEnemyBarrier].m_pos = pos;	// �w�肵�����W����

		EnemyBarrier = cntEnemyBarrier + 1;	// �g�p���̓G������
		break;
	}

	return EnemyBarrier;
}