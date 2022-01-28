//**************************************************************
//
//	EnemyMelee.h
//	�v���C���[��Ǐ]����ߐړG
//
//--------------------------------------------------------------
//	����ҁF�Ό����l
//--------------------------------------------------------------
//	�J������
//	2021/12/15 �v���C���[��Ǐ]���铮���̎���
//	2021/12/18 �G���m�A�ǁA�v���C���[�Ƃ̓����蔻��̎���
//	2021/12/21 �ǂƓ���������~�܂�(�S����)�����̎���
//	2021/12/13	�G�l�~�[�x�[�X�����ɒǉ�
//	�ҏW�ҁF??
//--------------------------------------------------------------
//�@2021/12/30	�v���C���[�̋��E�����a���擾����֐����̕ύX
//	�ҏW�ҁF�ĎR꣑��Y
//
//**************************************************************
//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "EnemyMelee.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "Block.h"
#include "explosion.h"
#include "life.h"
#include "SceneManager.h"
#include "PlayEffect.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define MODEL_ENEMY			"data/model/Melee/Melee.fbx"//
#define	VALUE_MOVE_ENEMY		(1.0f)		// �ړ����x
#define	VALUE_SCALE_ENEMY		4.0f, 8.0f, 4.0f
#define MAX_ENEMYMELEE			(10)		// �G�@�ő吔

#define	VALUE_ROTATE_ENEMY		(7.0f)		// ��]���x
#define	RATE_ROTATE_ENEMY		(0.20f)		// ��]�����W��

#define SEARCH_ENEMY			(250)		// �T���͈�
#define COLLAR_ENEMY		(XMFLOAT4(1.0f, 0.5f, 0.0f,1.0f))	//�� �v���C���[�J���[(��)������������ƃJ���[���ς��܂�

////////////////////////////////////////////////////////////////
#define MODEL_ENEMY1			"data/model/Melee/Melee.fbx"//
#define	VALUE_MOVE_ENEMY1		(1.0f)		// �ړ����x
#define	VALUE_SCALE_ENEMY1		4.0f, 8.0f, 4.0f
#define MAX_ENEMYMELEE1			(10)		// �G�@�ő吔

#define	VALUE_ROTATE_ENEMY1		(7.0f)		// ��]���x
#define	RATE_ROTATE_ENEMY1		(0.20f)		// ��]�����W��

#define SEARCH_ENEMY1			(250)		// �T���͈�
#define COLLAR_ENEMY1		(XMFLOAT4(10.0f, 10.0f, 10.0f,1.0f))	//�� �v���C���[�J���[(��)������������ƃJ���[���ς��܂�

////////////////////////////////////////////////////////////////
#define MODEL_ENEMY2			"data/model/Melee/Melee.fbx"
#define	VALUE_MOVE_ENEMY2		(1.0f)		// �ړ����x
#define	VALUE_SCALE_ENEMY2		4.0f, 8.0f, 4.0f
#define MAX_ENEMYMELEE2			(10)		// �G�@�ő吔

#define	VALUE_ROTATE_ENEMY2		(7.0f)		// ��]���x
#define	RATE_ROTATE_ENEMY2		(0.20f)		// ��]�����W��

#define SEARCH_ENEMY2			(250)		// �T���͈�
#define COLLAR_ENEMY2		(XMFLOAT4(1.0f, 1.0f, 0.0f,1.0f))	//���F�v���C���[�J���[(��)������������ƃJ���[���ς��܂�


#define DEFAULT_COLLAR		(XMFLOAT4(1.0f, 1.0f, 0.0f,1.0f))
//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static CAssimpModel		g_model;			// ���f�����
static TEnemyMelee		g_EMelee[MAX_ENEMYMELEE];	// �G�@���
Effect g_MeleeEffect[MAX_ENEMYMELEE];
static int g_EffectTimer = 0;		// �G�t�F�N�g����p�^�C�}�[

///////////////////////////////////////////////////////////////
static CAssimpModel		g_model1;			// ���f�����
static TEnemyMelee1		g_EMelee1[MAX_ENEMYMELEE1];	// �G�@���
Effect g_MeleeEffect1[MAX_ENEMYMELEE1];
static int g_EffectTimer1 = 0;		// �G�t�F�N�g����p�^�C�}�[

///////////////////////////////////////////////////////////////
static CAssimpModel		g_model2;			// ���f�����
static TEnemyMelee2		g_EMelee2[MAX_ENEMYMELEE2];	// �G�@���
Effect g_MeleeEffect2[MAX_ENEMYMELEE2];
static int g_EffectTimer2 = 0;		// �G�t�F�N�g����p�^�C�}�[

static XMFLOAT3			Blocksize;

//**************************************************************
// ����������
//**************************************************************
HRESULT InitEnemyMelee(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���f���f�[�^�̓ǂݍ���
	g_model.SetDif(COLLAR_ENEMY); // ���f�����[�h�O�ɃJ���[���w��
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_ENEMY))
	{
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return E_FAIL;
	}
	g_model.SetDif(DEFAULT_COLLAR);
	for (int i = 0; i < MAX_ENEMYMELEE; ++i)
	{// ���������������m������΂����Ɂ�
		g_EMelee[i].m_pos = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EMelee[i].m_size = (XMFLOAT3(10.0f, 10.0f, 10.0f));
		g_EMelee[i].m_move = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EMelee[i].m_rot = (XMFLOAT3(90.0f, 90.0f, 0.0f));
		g_EMelee[i].m_rotDest = g_EMelee[i].m_rot;
		g_EMelee[i].m_use = false;
	}
	////////////////////////////////////////////////////////////////////////////////
	// ���f���f�[�^�̓ǂݍ���
	g_model1.SetDif(COLLAR_ENEMY1); // ���f�����[�h�O�ɃJ���[���w��
	if (!g_model1.Load(pDevice, pDeviceContext, MODEL_ENEMY1))
	{
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return E_FAIL;
	}
	g_model1.SetDif(DEFAULT_COLLAR);
	for (int i = 0; i < MAX_ENEMYMELEE1; ++i)
	{// ���������������m������΂����Ɂ�
		g_EMelee1[i].m_pos = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EMelee1[i].m_size = (XMFLOAT3(10.0f, 10.0f, 10.0f));
		g_EMelee1[i].m_move = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EMelee1[i].m_rot = (XMFLOAT3(90.0f, 90.0f, 0.0f));
		g_EMelee1[i].m_rotDest = g_EMelee1[i].m_rot;
		g_EMelee1[i].m_use = false;
	}
	////////////////////////////////////////////////////////////////////////////////
	// ���f���f�[�^�̓ǂݍ���
	g_model2.SetDif(COLLAR_ENEMY2); // ���f�����[�h�O�ɃJ���[���w��
	if (!g_model2.Load(pDevice, pDeviceContext, MODEL_ENEMY2))
	{
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return E_FAIL;
	}
	g_model2.SetDif(DEFAULT_COLLAR);
	for (int i = 0; i < MAX_ENEMYMELEE2; ++i)
	{// ���������������m������΂����Ɂ�
		g_EMelee2[i].m_pos = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EMelee2[i].m_size = (XMFLOAT3(10.0f, 10.0f, 10.0f));
		g_EMelee2[i].m_move = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		g_EMelee2[i].m_rot = (XMFLOAT3(90.0f, 90.0f, 0.0f));
		g_EMelee2[i].m_rotDest = g_EMelee2[i].m_rot;
		g_EMelee2[i].m_use = false;
	}

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitEnemyMelee(void)
{
	// ���f���̉��
	g_model.Release();
	/////////////////////////////////////////////////////////
	// ���f���̉��
	g_model1.Release();
	/////////////////////////////////////////////////////////
// ���f���̉��
	g_model2.Release();
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateEnemyMelee(void)
{
	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	XMMATRIX mtxWorld, mtxRot, mtxTranslate,mtxScale;

	//�u���b�N�z��擾
	TBLOCK *Block = GetBlockArray();
	
	//�v���C���[�̍��W�E�T�C�Y�擾
	XMFLOAT3 posPlayer = GetPlayerPos();
	float sizePlayer = GetPlayerRadSize();

	for (int i = 0; i < MAX_ENEMYMELEE; ++i)
	{
		if (!g_EMelee[i].m_use)
		{//���g�p�Ȃ玟��
			continue;
		}

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
			for (int j = 0; j < MAX_ENEMYMELEE; ++j)
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
				if (CollisionAABB(g_EMelee[i].m_pos, g_EMelee[i].m_size, Block->m_pos, Block->m_Halfsize))
				{
					//�ǂɓ�����Ǝ~�܂�
					//�E
					if (Block->m_pos.x + Block->m_Halfsize.x  <= g_EMelee[i].m_pos.x - g_EMelee[i].m_size.x/2)
					{
						g_EMelee[i].m_pos.x = (Block->m_pos.x + Block->m_Halfsize.x + g_EMelee[i].m_size.x);
					}
					//��
					else if (Block->m_pos.x - Block->m_Halfsize.x  >= g_EMelee[i].m_pos.x + g_EMelee[i].m_size.x / 2)
					{
						g_EMelee[i].m_pos.x = (Block->m_pos.x - Block->m_Halfsize.x - g_EMelee[i].m_size.x);
					}
					//��
					else if (Block->m_pos.y - Block->m_Halfsize.y >= g_EMelee[i].m_pos.y + g_EMelee[i].m_size.y / 2)
					{
						g_EMelee[i].m_pos.y = (Block->m_pos.y - Block->m_Halfsize.y - g_EMelee[i].m_size.y);
					}
					//��
					else if (Block->m_pos.y + Block->m_Halfsize.y <= g_EMelee[i].m_pos.y - g_EMelee[i].m_size.y / 2)
					{
						g_EMelee[i].m_pos.y = (Block->m_pos.y + Block->m_Halfsize.y + g_EMelee[i].m_size.y);
					}
				}
			}
			for (int k = 0; k < MAX_BLOCK; ++k, --Block)
			//// �G�t�F�N�g�Z�b�g������
			//if (g_EffectTimer == 0)
			//{
			//	g_MeleeEffect[i].Set(EFFECT_FIRE, g_EMelee[i].m_pos, XMFLOAT3(1.0f, 1.0f, 1.0f), 0.5f, XMFLOAT3(1.0f,1.0f,1.0f));
			//	g_EffectTimer = 30;
			//}
			//--g_EffectTimer;

			// �G�ƃv���C���[�̓����蔻��
			if (!g_EMelee[i].m_use)
			{// ���g�p�Ȃ玟��
				continue;
			}
			if (CollisionSphere(g_EMelee[i].m_pos, g_EMelee[i].m_size.x, posPlayer, sizePlayer))
			{
				//DelLife();
				//if (GetLife() == 0)
				//{
				//	SetScene(SCENE_GAMEOVER);
				//}
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
			mtxScale = XMMatrixScaling(VALUE_SCALE_ENEMY);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			XMStoreFloat4x4(&g_EMelee[i].m_mtxWorld, mtxWorld);

		}
	}


	for (int i = 0; i < MAX_ENEMYMELEE1; ++i)
	{
		if (!g_EMelee1[i].m_use)
		{//���g�p�Ȃ玟��
			continue;
		}

		//�G�ƃv���C���[�̋������߂Â�����
		if (CollisionSphere(posPlayer, sizePlayer, g_EMelee1[i].m_pos, SEARCH_ENEMY1))
		{
			if (!g_EMelee1[i].m_use)
			{//���g�p�Ȃ玟��
				continue;
			}

			//��Ƀv���C���[�̕���������
			/*g_EMelee[i].m_rotDest = posPlayer;
			g_EMelee[i].m_rot = g_EMelee[i].m_rotDest;
			g_EMelee[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/

			//�G��x���W���v���C���[�����傫��������
			if (g_EMelee1[i].m_pos.x >= posPlayer.x)
			{
				g_EMelee1[i].m_pos.x += -VALUE_MOVE_ENEMY1;
				g_EMelee1[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f));

				g_EMelee1[i].m_rotDest.y = rotCamera.y - 90.0f;
			}
			//�G��x���W���v���C���[����������������
			if (g_EMelee1[i].m_pos.x <= posPlayer.x)
			{
				g_EMelee1[i].m_pos.x += VALUE_MOVE_ENEMY1;
				g_EMelee1[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 90.0f));
				g_EMelee1[i].m_rotDest.y = rotCamera.y + 90.0f;

			}
			//�G��y���W���v���C���[�����傫��������
			if (g_EMelee1[i].m_pos.y >= posPlayer.y)
			{
				g_EMelee1[i].m_pos.y += -VALUE_MOVE_ENEMY1;

			}
			//�G��y���W���v���C���[����������������
			if (g_EMelee1[i].m_pos.y <= posPlayer.y)
			{
				g_EMelee1[i].m_pos.y += VALUE_MOVE_ENEMY1;

			}

			//**************************************************************************************
			//		�����蔻��
			//**************************************************************************************

			// �G���m�̓����蔻��
			for (int j = 0; j < MAX_ENEMYMELEE1; ++j)
			{
				if (CollisionSphere(g_EMelee1[i].m_pos, g_EMelee1[i].m_size.x, g_EMelee1[j].m_pos, g_EMelee1[j].m_size.x))
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
				if (CollisionAABB(g_EMelee1[i].m_pos, g_EMelee1[i].m_size, Block->m_pos, Block->m_Halfsize))
				{
					//�ǂɓ�����Ǝ~�܂�
					//�E
					if (Block->m_pos.x + Block->m_Halfsize.x <= g_EMelee1[i].m_pos.x - g_EMelee1[i].m_size.x / 2)
					{
						g_EMelee1[i].m_pos.x = (Block->m_pos.x + Block->m_Halfsize.x + g_EMelee1[i].m_size.x);
					}
					//��
					else if (Block->m_pos.x - Block->m_Halfsize.x >= g_EMelee1[i].m_pos.x + g_EMelee1[i].m_size.x / 2)
					{
						g_EMelee1[i].m_pos.x = (Block->m_pos.x - Block->m_Halfsize.x - g_EMelee1[i].m_size.x);
					}
					//��
					else if (Block->m_pos.y - Block->m_Halfsize.y >= g_EMelee1[i].m_pos.y + g_EMelee1[i].m_size.y / 2)
					{
						g_EMelee1[i].m_pos.y = (Block->m_pos.y - Block->m_Halfsize.y - g_EMelee1[i].m_size.y);
					}
					//��
					else if (Block->m_pos.y + Block->m_Halfsize.y <= g_EMelee1[i].m_pos.y - g_EMelee1[i].m_size.y / 2)
					{
						g_EMelee1[i].m_pos.y = (Block->m_pos.y + Block->m_Halfsize.y + g_EMelee1[i].m_size.y);
					}
				}
			}
			for (int k = 0; k < MAX_BLOCK; ++k, --Block)
			// �G�t�F�N�g�Z�b�g������
			//if (g_EffectTimer1 == 0)
			//{
			//	g_MeleeEffect1[i].Set(EFFECT_FIRE, g_EMelee1[i].m_pos, XMFLOAT3(1.0f, 1.0f, 1.0f), 0.5f, XMFLOAT3(1.0f, 1.0f, 1.0f));
			//	g_EffectTimer1 = 30;
			//}
			//--g_EffectTimer1;

			// �G�ƃv���C���[�̓����蔻��
			if (!g_EMelee1[i].m_use)
			{// ���g�p�Ȃ玟��
				continue;
			}
			if (CollisionSphere(g_EMelee1[i].m_pos, g_EMelee1[i].m_size.x, posPlayer, sizePlayer))
			{
				DelLife();
				//if (GetLife() == 0)
				//{
				//	SetScene(SCENE_GAMEOVER);
				//}
				g_EMelee1[i].m_use = false;
			}

			//�ړI�̊p�x�܂ł̍���
			float fDiffRotY = g_EMelee1[i].m_rotDest.y - g_EMelee1[i].m_rot.y;
			if (fDiffRotY >= 180.0f) {
				fDiffRotY -= 360.0f;
			}
			if (fDiffRotY < -180.0f) {
				fDiffRotY += 360.0f;
			}

			// �ړI�̊p�x�܂Ŋ�����������
			g_EMelee1[i].m_rot.y += fDiffRotY * RATE_ROTATE_ENEMY1;
			if (g_EMelee1[i].m_rot.y >= 180.0f) {
				g_EMelee1[i].m_rot.y -= 360.0f;
			}
			if (g_EMelee1[i].m_rot.y < -180.0f) {
				g_EMelee1[i].m_rot.y += 360.0f;
			}
			g_EMelee1[i].m_move = XMFLOAT3(
				-SinDeg(g_EMelee1[i].m_rot.y) * VALUE_MOVE_ENEMY1,
				0.0f,
				-CosDeg(g_EMelee1[i].m_rot.y) * VALUE_MOVE_ENEMY1);

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_EMelee1[i].m_rot.x),
				XMConvertToRadians(g_EMelee1[i].m_rot.y),
				XMConvertToRadians(g_EMelee1[i].m_rot.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(
				g_EMelee1[i].m_pos.x,
				g_EMelee1[i].m_pos.y,
				g_EMelee1[i].m_pos.z);
			mtxScale = XMMatrixScaling(VALUE_SCALE_ENEMY1);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			XMStoreFloat4x4(&g_EMelee1[i].m_mtxWorld, mtxWorld);

		}
	}


	//�u���b�N�z��擾
	TBLOCK *Block2 = GetBlockArray();

	for (int i = 0; i < MAX_ENEMYMELEE2; ++i)
	{
		if (!g_EMelee2[i].m_use)
		{//���g�p�Ȃ玟��
			continue;
		}

		//�G�ƃv���C���[�̋������߂Â�����
		if (CollisionSphere(posPlayer, sizePlayer, g_EMelee2[i].m_pos, SEARCH_ENEMY2))
		{
			if (!g_EMelee2[i].m_use)
			{//���g�p�Ȃ玟��
				continue;
			}

			//��Ƀv���C���[�̕���������
			/*g_EMelee[i].m_rotDest = posPlayer;
			g_EMelee[i].m_rot = g_EMelee[i].m_rotDest;
			g_EMelee[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/

			//�G��x���W���v���C���[�����傫��������
			if (g_EMelee2[i].m_pos.x >= posPlayer.x)
			{
				g_EMelee2[i].m_pos.x += -VALUE_MOVE_ENEMY2;
				g_EMelee2[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 0.0f));

				g_EMelee2[i].m_rotDest.y = rotCamera.y - 90.0f;
			}
			//�G��x���W���v���C���[����������������
			if (g_EMelee2[i].m_pos.x <= posPlayer.x)
			{
				g_EMelee2[i].m_pos.x += VALUE_MOVE_ENEMY2;
				g_EMelee2[i].m_rot = (XMFLOAT3(90.0f, 0.0f, 90.0f));
				g_EMelee2[i].m_rotDest.y = rotCamera.y + 90.0f;

			}
			//�G��y���W���v���C���[�����傫��������
			if (g_EMelee2[i].m_pos.y >= posPlayer.y)
			{
				g_EMelee2[i].m_pos.y += -VALUE_MOVE_ENEMY2;

			}
			//�G��y���W���v���C���[����������������
			if (g_EMelee2[i].m_pos.y <= posPlayer.y)
			{
				g_EMelee2[i].m_pos.y += VALUE_MOVE_ENEMY2;

			}

			//**************************************************************************************
			//		�����蔻��
			//**************************************************************************************

			// �G���m�̓����蔻��
			for (int j = 0; j < MAX_ENEMYMELEE2; ++j)
			{
				if (CollisionSphere(g_EMelee2[i].m_pos, g_EMelee2[i].m_size.x, g_EMelee2[j].m_pos, g_EMelee2[j].m_size.x))
				{
					if (i == j)
					{//�����G�Ȃ�Ƃ΂�
						continue;
					}
					//�G���m���d�Ȃ�Ȃ�����

				}
			}

			// �G�ƕǂ̓����蔻��
			for (int j = 0; j < MAX_BLOCK; ++j, ++Block2)
			{
				if (!Block->m_use)
				{// ���g�p�Ȃ玟��
					continue;
				}
				if (CollisionAABB(g_EMelee2[i].m_pos, g_EMelee2[i].m_size, Block2->m_pos, Block2->m_Halfsize))
				{
					//�ǂɓ�����Ǝ~�܂�
					//�E
					if (Block2->m_pos.x + Block2->m_Halfsize.x <= g_EMelee2[i].m_pos.x - g_EMelee2[i].m_size.x / 2)
					{
						g_EMelee2[i].m_pos.x = (Block2->m_pos.x + Block2->m_Halfsize.x + g_EMelee2[i].m_size.x);
					}
					//��
					else if (Block2->m_pos.x - Block2->m_Halfsize.x >= g_EMelee2[i].m_pos.x + g_EMelee2[i].m_size.x / 2)
					{
						g_EMelee2[i].m_pos.x = (Block2->m_pos.x - Block2->m_Halfsize.x - g_EMelee2[i].m_size.x);
					}
					//��
					else if (Block2->m_pos.y - Block2->m_Halfsize.y >= g_EMelee2[i].m_pos.y + g_EMelee2[i].m_size.y / 2)
					{
						g_EMelee2[i].m_pos.y = (Block2->m_pos.y - Block2->m_Halfsize.y - g_EMelee2[i].m_size.y);
					}
					//��
					else if (Block2->m_pos.y + Block2->m_Halfsize.y <= g_EMelee2[i].m_pos.y - g_EMelee2[i].m_size.y / 2)
					{
						g_EMelee2[i].m_pos.y = (Block2->m_pos.y + Block2->m_Halfsize.y + g_EMelee2[i].m_size.y);
					}
				}
			}
			for (int k = 0; k < MAX_BLOCK; ++k, --Block2)

			// �G�t�F�N�g�Z�b�g������
			//if (g_EffectTimer == 0)
			//{
			//	g_MeleeEffect1[i].Set(EFFECT_FIRE, g_EMelee1[i].m_pos, XMFLOAT3(1.0f, 1.0f, 1.0f), 0.5f, XMFLOAT3(1.0f, 1.0f, 1.0f));
			//	g_EffectTimer = 30;
			//}
			//--g_EffectTimer;

			// �G�ƃv���C���[�̓����蔻��
			if (!g_EMelee2[i].m_use)
			{// ���g�p�Ȃ玟��
				continue;
			}
			if (CollisionSphere(g_EMelee2[i].m_pos, g_EMelee2[i].m_size.x, posPlayer, sizePlayer))
			{
				//DelLife();
				//if (GetLife() == 0)
				//{
				//	SetScene(SCENE_GAMEOVER);
				//}
				g_EMelee2[i].m_use = false;
			}

			//�ړI�̊p�x�܂ł̍���
			float fDiffRotY = g_EMelee2[i].m_rotDest.y - g_EMelee2[i].m_rot.y;
			if (fDiffRotY >= 180.0f) {
				fDiffRotY -= 360.0f;
			}
			if (fDiffRotY < -180.0f) {
				fDiffRotY += 360.0f;
			}

			// �ړI�̊p�x�܂Ŋ�����������
			g_EMelee2[i].m_rot.y += fDiffRotY * RATE_ROTATE_ENEMY2;
			if (g_EMelee2[i].m_rot.y >= 180.0f) {
				g_EMelee2[i].m_rot.y -= 360.0f;
			}
			if (g_EMelee2[i].m_rot.y < -180.0f) {
				g_EMelee2[i].m_rot.y += 360.0f;
			}
			g_EMelee2[i].m_move = XMFLOAT3(
				-SinDeg(g_EMelee2[i].m_rot.y) * VALUE_MOVE_ENEMY2,
				0.0f,
				-CosDeg(g_EMelee2[i].m_rot.y) * VALUE_MOVE_ENEMY2);

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_EMelee2[i].m_rot.x),
				XMConvertToRadians(g_EMelee2[i].m_rot.y),
				XMConvertToRadians(g_EMelee2[i].m_rot.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(
				g_EMelee2[i].m_pos.x,
				g_EMelee2[i].m_pos.y,
				g_EMelee2[i].m_pos.z);
			mtxScale = XMMatrixScaling(VALUE_SCALE_ENEMY2);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			XMStoreFloat4x4(&g_EMelee2[i].m_mtxWorld, mtxWorld);

		}
	}
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawEnemyMelee(void)
{

	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	for (int i = 0; i < MAX_ENEMYMELEE; ++i) {
		if (!g_EMelee[i].m_use)
		{
			continue;
		}
		g_model.Draw(pDC, g_EMelee[i].m_mtxWorld, eOpacityOnly);
	}
	////////////////////////////////////////////////////////////
	// �s����������`��
	for (int i = 0; i < MAX_ENEMYMELEE1; ++i) {
		if (!g_EMelee1[i].m_use)
		{
			continue;
		}
		g_model1.Draw(pDC, g_EMelee1[i].m_mtxWorld, eOpacityOnly);
	}
	////////////////////////////////////////////////////////////
	// �s����������`��
	for (int i = 0; i < MAX_ENEMYMELEE2; ++i) {
		if (!g_EMelee2[i].m_use)
		{
			continue;
		}
		g_model2.Draw(pDC, g_EMelee2[i].m_mtxWorld, eOpacityOnly);
	}
	// // ������������`��
	// for (int i = 0; i < MAX_ENEMYMELEE; ++i) {
	// 	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	// 	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	// 	g_model.Draw(pDC, g_EMelee[i].m_mtxWorld, eTransparentOnly);
	// 	SetZWrite(true);				// Z�o�b�t�@�X�V����
	// 	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	// }
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
int SetEnemyMelee(XMFLOAT3(pos), int(id))
{
	if (id == 0)
	{
		// �߂�l�̏�����
		int EnemyMelee = -1;

		for (int cntEnemyMelee = 0; cntEnemyMelee < MAX_ENEMYMELEE; ++cntEnemyMelee)
		{
			// �g�p���Ȃ�X�L�b�v
			if (g_EMelee[cntEnemyMelee].m_use)
			{
				continue;
			}
			g_EMelee[cntEnemyMelee].m_use = true;	// �g�p��ON
			g_EMelee[cntEnemyMelee].m_pos = pos;	// �w�肵�����W����

			EnemyMelee = cntEnemyMelee + 1;	// �g�p���̓G������
			break;
		}

		return EnemyMelee;
	}
	else if (id == 1)
	{
		// �߂�l�̏�����
		int EnemyMelee1 = -1;

		for (int cntEnemyMelee1 = 0; cntEnemyMelee1 < MAX_ENEMYMELEE1; ++cntEnemyMelee1)
		{
			// �g�p���Ȃ�X�L�b�v
			if (g_EMelee1[cntEnemyMelee1].m_use)
			{
				continue;
			}
			g_EMelee1[cntEnemyMelee1].m_use = true;	// �g�p��ON
			g_EMelee1[cntEnemyMelee1].m_pos = pos;	// �w�肵�����W����

			EnemyMelee1 = cntEnemyMelee1 + 1;	// �g�p���̓G������
			break;
		}

		return EnemyMelee1;
	}
	else  if (id == 2)
	{
		// �߂�l�̏�����
		int EnemyMelee2 = -1;

		for (int cntEnemyMelee2 = 0; cntEnemyMelee2 < MAX_ENEMYMELEE2; ++cntEnemyMelee2)
		{
			// �g�p���Ȃ�X�L�b�v
			if (g_EMelee2[cntEnemyMelee2].m_use)
			{
				continue;
			}
			g_EMelee2[cntEnemyMelee2].m_use = true;	// �g�p��ON
			g_EMelee2[cntEnemyMelee2].m_pos = pos;	// �w�肵�����W����

			EnemyMelee2 = cntEnemyMelee2 + 1;	// �g�p���̓G������
			break;
		}

		return EnemyMelee2;
	}
	return 0;
}

//**************************************************************
// �ߐړG���擾
//
// �߂�l�F�ߐړG�\���̕ϐ�
//**************************************************************
TEnemyMelee* GetEnemyMelee()
{
	return g_EMelee;
}