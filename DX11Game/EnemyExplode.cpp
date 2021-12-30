//**************************************************************
//
//	EnemyExplode.cpp
//	�v���C���[���ڋ߂���Ɣ�������G
//
//--------------------------------------------------------------
//	����ҁF�V���ڐl
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//**************************************************************
//	2021/12/24�@EnemyExplode�쐬
//	�ҏW�ҁF�V���ڐl
//--------------------------------------------------------------
//�@2021/12/25	EnemyExplode�͈͔̔��芮��	
//	�ҏW�ҁF�V���ڐl
//--------------------------------------------------------------
//�@2021/12/29	�����܂ł̗P�\(5�b)�̊���	
//	�ҏW�ҁF�V���ڐl
//--------------------------------------------------------------
//�@2021/12/31	���f���̃X�P�[����g�ݍ��݂܂����B347�s	
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�R�����g�i�ӌ��j
//	2021/12/29	�ǂ𔇂��̂͂ނ��������ł�... _�V���ڐl
//	2021/12/29	�v�����i�[�ɑ��k���Ă݂܂�	  _�㌎��n
//**************************************************************




//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "EnemyExplode.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "Block.h"
#include "explosion.h"
#include "life.h"
//#include "timer.h"
#include "SceneManager.h"
#include <stdlib.h>

//**************************************************************
// �\���̒�`
//**************************************************************
struct TEnemyExplode {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	XMFLOAT3	m_rot;		// ���݂̌���
	XMFLOAT3	m_size;		// ���݂̃T�C�Y
	XMFLOAT3	m_rotDest;	// �ړI�̌���
	XMFLOAT3	m_move;		// �ړ���
	bool		m_use;		// �g�p���Ă邩�ۂ�	ON:�g�p��

	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
	int			m_speed;	// �X�s�[�h
};

//**************************************************************
// �}�N����`
//**************************************************************
#define MODEL_ENEMY			"data/model/Explode/Explode.fbx"//"data/model/sentouki.fbx"
//#define MODEL_ENEMY			"data/model/enemy3.fbx"

#define	VALUE_MOVE_ENEMY		(1.0f)		// �ړ����x
#define MAX_ENEMYEXPLODE		(10)		// �G�@�ő吔

#define	VALUE_ROTATE_ENEMY		(7.0f)		// ��]���x
#define	RATE_ROTATE_ENEMY		(0.20f)		// ��]�����W��

#define SEARCH_ENEMY			(250)		// �T���͈�

#define ENEMY_EXPLODE_TIMER_START (5)// ��������



//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static CAssimpModel			g_model;	// ���f�����
static TEnemyExplode		g_EExplode[MAX_ENEMYEXPLODE];	// �G�@���
static XMFLOAT3				Blocksize;
static int g_nEETimer;		// ���Ԃ��J�E���g����

//**************************************************************
// ����������
//**************************************************************
HRESULT InitEnemyExplode(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �u���b�N�̃T�C�Y�擾
	Blocksize = GetBlockSize();

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY))
	{
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < MAX_ENEMYEXPLODE; ++i)
	{// ���������������m������΂����Ɂ�
		g_EExplode[i].m_pos = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EExplode[i].m_size = (XMFLOAT3(20.0f, 10.0f, 10.0f));
		g_EExplode[i].m_move = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EExplode[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f));
		g_EExplode[i].m_rotDest = g_EExplode[i].m_rot;
		g_EExplode[i].m_use = false;
	}

	//�ϐ�������
	g_nEETimer = ENEMY_EXPLODE_TIMER_START * 60 + 59;

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitEnemyExplode(void)
{
	// ���f���̉��
	g_model.Release();
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateEnemyExplode(void)
{
	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	//�v���C���[�̍��W�E�T�C�Y�擾
	XMFLOAT3 posPlayer = GetPlayerPos();
	float sizePlayer = GetPlayerSize();

	// �^�C�}�[�J�E���g�_�E��
	if (g_nEETimer > 0)
	{
		--g_nEETimer;
	}
	else
	{
		g_nEETimer;
	}

	for (int i = 0; i < MAX_ENEMYEXPLODE; ++i)
	{
		//�u���b�N�z��擾
		TBLOCK *Block = GetBlockArray();

		//�G�ƃv���C���[�̋������߂Â�����
		if (CollisionSphere(posPlayer, sizePlayer, g_EExplode[i].m_pos, SEARCH_ENEMY))
		{
			if (!g_EExplode[i].m_use)
			{//���g�p�Ȃ玟��
				continue;
			}

			//��Ƀv���C���[�̕���������
			/*g_EExplode[i].m_rotDest = posPlayer;
			g_EExplode[i].m_rot = g_EExplode[i].m_rotDest;
			g_EExplode[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/

			//*********************************
			//�Ǐ]����
			//*********************************

			//�G��x���W���v���C���[�����傫��������
			if (g_EExplode[i].m_pos.x >= posPlayer.x)
			{
				g_EExplode[i].m_pos.x += -VALUE_MOVE_ENEMY;
				g_EExplode[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f) );

				g_EExplode[i].m_rotDest.y = rotCamera.y - 90.0f;
			}
			//�G��x���W���v���C���[����������������
			if (g_EExplode[i].m_pos.x <= posPlayer.x)
			{
				g_EExplode[i].m_pos.x += VALUE_MOVE_ENEMY;
				g_EExplode[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 90.0f));
				g_EExplode[i].m_rotDest.y = rotCamera.y + 90.0f;

			}
			//�G��y���W���v���C���[�����傫��������
			if (g_EExplode[i].m_pos.y >= posPlayer.y)
			{
				g_EExplode[i].m_pos.y += -VALUE_MOVE_ENEMY;

			}
			//�G��y���W���v���C���[����������������
			if (g_EExplode[i].m_pos.y <= posPlayer.y)
			{
				g_EExplode[i].m_pos.y += VALUE_MOVE_ENEMY;

			}
			
			
			
			



			//**************************************************************************************
			//		�����蔻��
			//**************************************************************************************

			// �G���m�̓����蔻��
			for (int j = 0; j < MAX_ENEMYEXPLODE; ++j)
			{
				if (CollisionSphere(g_EExplode[i].m_pos, g_EExplode[i].m_size.x, g_EExplode[j].m_pos, g_EExplode[j].m_size.x))
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
				if (CollisionAABB(g_EExplode[i].m_pos, g_EExplode[i].m_size, Block->m_pos,Blocksize))
				{
					//�ǂɓ�����Ǝ~�܂�
					//�E
					if (Block->m_pos.x + Blocksize.x / 2 <= g_EExplode[i].m_pos.x - g_EExplode[i].m_size.x / 2)
					{
					 	g_EExplode[i].m_pos.x = (Block->m_pos.x + Blocksize.x + g_EExplode[i].m_size.x);
					}
					//��
					else if (Block->m_pos.x - Blocksize.x / 2 >= g_EExplode[i].m_pos.x + g_EExplode[i].m_size.x / 2)
					{
					 	g_EExplode[i].m_pos.x = (Block->m_pos.x - Blocksize.x - g_EExplode[i].m_size.x);
					}
					//��
					else if (Block->m_pos.y - Blocksize.y / 2 >= g_EExplode[i].m_pos.y + g_EExplode[i].m_size.y / 2)
					{
						g_EExplode[i].m_pos.y = (Block->m_pos.y - Blocksize.y - g_EExplode[i].m_size.y);
					}
					//��
					else if (Block->m_pos.y + Blocksize.y / 2 <= g_EExplode[i].m_pos.y - g_EExplode[i].m_size.y / 2)
					{
						g_EExplode[i].m_pos.y = (Block->m_pos.y + Blocksize.y + g_EExplode[i].m_size.y);
					}
				}
			}

			// �G�ƃv���C���[�̓����蔻��
			if (!g_EExplode[i].m_use)
			{// ���g�p�Ȃ玟��
				continue;
			}

			//*********************************
			//�G�ƃv���C���[�̋������菈��
			//*********************************
			//��)����SetScene��ǂݍ��ޕK�v�͖�����������u
			//��)�������Ă��G���A�ɓ������u�Ԕ�����������ׁA�����͕K�v
			//��)XMStoreMatrix�������܂��Ƃ����邪�����炭�A�����Ȃ��Βl��p���Ă��邩��Ɛ��������
			//�G�ƃv���C���[��X���W��"10.0f"�ȓ��ł���Ȃ�Δ���(����)����
			//if (g_EExplode[i].m_pos.x - posPlayer.x <= 10.0f)
			
				if (abs(g_EExplode[i].m_pos.x - posPlayer.x <= 10.0f))
				{
					// �G�ƃv���C���[��Y���W��"10.0f"�ȓ��ł���Ȃ�Δ���(����)����
						//if (g_EExplode[i].m_pos.y - posPlayer.y <= 10.0f)
					if (abs(g_EExplode[i].m_pos.y - posPlayer.y <= 10.0f))
					{
						if (g_nEETimer <= 0)
						{
							DelLife();
							if (GetLife() == 0)
							{
								SetScene(SCENE_GAMEOVER);
							}
							g_EExplode[i].m_use = false;
						}
					}
				}

			

			//if (CollisionSphere(g_EExplode[i].m_pos, g_EExplode[i].m_size.x, posPlayer, sizePlayer))
			//{
			//	DelLife();
			//	if (GetLife() == 0)
			//	{
			//		SetScene(SCENE_GAMEOVER);
			//	}
			//	g_EExplode[i].m_use = false;
			//}

			 //�ړI�̊p�x�܂ł̍���
			float fDiffRotY = g_EExplode[i].m_rotDest.y - g_EExplode[i].m_rot.y;
			if (fDiffRotY >= 180.0f) {
				fDiffRotY -= 360.0f;
			}
			if (fDiffRotY < -180.0f) {
				fDiffRotY += 360.0f;
			}

			// �ړI�̊p�x�܂Ŋ�����������
			g_EExplode[i].m_rot.y += fDiffRotY * RATE_ROTATE_ENEMY;
			if (g_EExplode[i].m_rot.y >= 180.0f) {
				g_EExplode[i].m_rot.y -= 360.0f;
			}
			if (g_EExplode[i].m_rot.y < -180.0f) {
				g_EExplode[i].m_rot.y += 360.0f;
			}
			g_EExplode[i].m_move = XMFLOAT3(
				-SinDeg(g_EExplode[i].m_rot.y) * VALUE_MOVE_ENEMY,
				0.0f,
				-CosDeg(g_EExplode[i].m_rot.y) * VALUE_MOVE_ENEMY);

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_EExplode[i].m_rot.x),
				XMConvertToRadians(g_EExplode[i].m_rot.y),
				XMConvertToRadians(g_EExplode[i].m_rot.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// ���f���̃T�C�Y
			mtxWorld = XMMatrixScaling(0.15f, 0.15f, 0.15f);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(
				g_EExplode[i].m_pos.x,
				g_EExplode[i].m_pos.y,
				g_EExplode[i].m_pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			XMStoreFloat4x4(&g_EExplode[i].m_mtxWorld, mtxWorld);

		}
	}
	PrintDebugProc("[˺�� �� : (%f : %f : %f)]\n", g_EExplode->m_pos.x, g_EExplode->m_pos.y, g_EExplode->m_pos.z);
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawEnemyExplode(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	for (int i = 0; i < MAX_ENEMYEXPLODE; ++i) {
		if (!g_EExplode[i].m_use)
		{
			continue;
		}
		g_model.Draw(pDC, g_EExplode[i].m_mtxWorld, eOpacityOnly);
	}

	// ������������`��
	for (int i = 0; i < MAX_ENEMYEXPLODE; ++i) {
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_model.Draw(pDC, g_EExplode[i].m_mtxWorld, eTransparentOnly);
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
int SetEnemyExplode(XMFLOAT3 pos)
{
	// �߂�l�̏�����
	int EnemyExplode = -1;

	for (int cntEnemyExplode = 0; cntEnemyExplode < MAX_ENEMYEXPLODE; ++cntEnemyExplode)
	{
		// �g�p���Ȃ�X�L�b�v
		if (g_EExplode[cntEnemyExplode].m_use)
		{
			continue;
		}
		g_EExplode[cntEnemyExplode].m_use = true;	// �g�p��ON
		g_EExplode[cntEnemyExplode].m_pos = pos;	// �w�肵�����W����

		EnemyExplode = cntEnemyExplode + 1;	// �g�p���̓G������
		break;
	}

	return EnemyExplode;
}