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
//--------------------------------------------------------------
//	2022/01/31	�G���v���C���[�̕��p�֌����ȈՓI�ȏ�����ǉ����܂���
//	�ҏW�ҁF�㌎��n
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
#include "Texture.h"
#include "polygon.h"
#include "Reticle.h"
#include "Sound.h"
#include "CountEnemy.h"
//#include "SceneManager.h"



//**************************************************************
// �}�N����`
//**************************************************************
#define MODEL_ENEMY			"data/model/Range/Range.fbx"	// "data/model/helicopter000.fbx"
#define RETICLE_TEXTURE		L"data/texture/reticle000.png"

#define MAX_ENEMYRANGE			(10)		// �G�@�ő吔
#define SEARCH_RANGE			(200)		// �T���͈�
#define ENEMY_TIMER				(5)			// ��������
#define SCALE_E_RANGE		(XMFLOAT3(0.03f, 0.06f, 0.06f))
#define COLLAR_ENEMY		(XMFLOAT4(0.0f, 20.0f, 0.0f,1.0f))	// �v���C���[�J���[(��)������������ƃJ���[���ς��܂�
//////////////////////////////////////////////////////////////////
#define MODEL_ENEMY1			"data/model/Range/Range.fbx"	// "data/model/helicopter000.fbx"
#define MAX_ENEMYRANGE1			(10)		// �G�@�ő吔
#define SEARCH_RANGE1			(200)		// �T���͈�
#define ENEMY_TIMER1			(5)			// ��������
#define SCALE_E_RANGE1		(XMFLOAT3(0.05f, 0.1f, 0.1f))
#define COLLAR_ENEMY1		(XMFLOAT4(0.0f, 0.0f, 20.0f,1.0f))	// �v���C���[�J���[(��)������������ƃJ���[���ς��܂�
//////////////////////////////////////////////////////////////////
#define MODEL_ENEMY2			"data/model/Range/Range.fbx"	// "data/model/helicopter000.fbx"
#define MAX_ENEMYRANGE2			(10)		// �G�@�ő吔
#define SEARCH_RANGE2			(200)		// �T���͈�
#define ENEMY_TIMER2				(5)			// ��������
#define SCALE_E_RANGE2		(XMFLOAT3(0.05f, 0.1f, 0.1f))
#define COLLAR_ENEMY2		(XMFLOAT4(20.0f, 0.0f, 20.0f,1.0f))	// �v���C���[�J���[(��)������������ƃJ���[���ς��܂�


#define DEFAULT_COLLAR		(XMFLOAT4(1.0f, 1.0f, 0.0f,1.0f))
#define SCALE_E_RANGE		(XMFLOAT3(0.03f, 0.06f, 0.06f))
//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static CAssimpModel	g_model;			// ���f��
static TEnemyRange	g_ERange[MAX_ENEMYRANGE];	// �G�@���
static ID3D11ShaderResourceView* g_pTexture;
//////////////////////////////////////////////////////////////////
static CAssimpModel	g_model1;			// ���f��
static TEnemyRange1	g_ERange1[MAX_ENEMYRANGE1];	// �G�@���
//////////////////////////////////////////////////////////////////
static CAssimpModel	g_model2;			// ���f��
static TEnemyRange2	g_ERange2[MAX_ENEMYRANGE2];	// �G�@���

//**************************************************************
// ����������
//**************************************************************
HRESULT InitEnemyRange(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	//// �e�N�X�`���Ǎ�
	//hr = CreateTextureFromFile(pDevice, RETICLE_TEXTURE, &g_pTexture);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}

	// ���f���f�[�^�̓ǂݍ���
	g_model.SetDif(XMFLOAT4(0.2f,20.0f,10.0f,1.0f));
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
		g_ERange[i].ReticleSize = g_ERange[i].m_Time / 3;
	}
	//////////////////////////////////////////////////////////////////////////////////
	//// ���f���f�[�^�̓ǂݍ���
	//g_model1.SetDif(COLLAR_ENEMY1); // ���f�����[�h�O�ɃJ���[���w��
	//if (!g_model1.Load(pDevice, pDeviceContext, MODEL_ENEMY1))
	//{
	//	MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
	//	return E_FAIL;
	//}
	//g_model1.SetDif(DEFAULT_COLLAR);
	//for (int i = 0; i < MAX_ENEMYRANGE1; ++i)
	//{// ���������������m������΂����Ɂ�
	//	g_ERange1[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_ERange1[i].m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_ERange1[i].m_rotDest = g_ERange1[i].m_rot;
	//	g_ERange1[i].m_size = XMFLOAT3(5.0f, 5.0f, 5.0f);
	//	g_ERange1[i].m_use = false;
	//	// g_ERange1[i].m_Time = ENEMY_TIMER1 * 60 + 59;
	//	g_ERange1[i].m_Time = (ENEMY_TIMER + rand() % 3) * 60 + 59;	// 3�`6�b�̃����_����
	//}
	//////////////////////////////////////////////////////////////////////////////////
	//// ���f���f�[�^�̓ǂݍ���
	//g_model2.SetDif(COLLAR_ENEMY2); // ���f�����[�h�O�ɃJ���[���w��
	//if (!g_model2.Load(pDevice, pDeviceContext, MODEL_ENEMY2))
	//{
	//	MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
	//	return E_FAIL;
	//}
	//g_model2.SetDif(DEFAULT_COLLAR);
	//for (int i = 0; i < MAX_ENEMYRANGE2; ++i)
	//{// ���������������m������΂����Ɂ�
	//	g_ERange2[i].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_ERange2[i].m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_ERange2[i].m_rotDest = g_ERange2[i].m_rot;
	//	g_ERange2[i].m_size = XMFLOAT3(5.0f, 5.0f, 5.0f);
	//	g_ERange2[i].m_use = false;
	//	// g_ERange2[i].m_Time = ENEMY_TIMER2 * 60 + 59;
	//	g_ERange2[i].m_Time = (ENEMY_TIMER + rand() % 3) * 60 + 59;	// 3�`6�b�̃����_����
	//}


	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitEnemyRange(void)
{
	// ���f���̉��
	g_model.Release();

	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
	// ���f���̉��
	g_model1.Release();
	// ���f���̉��
	g_model2.Release();

}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateEnemyRange(void)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxSize, mtxScale;
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//�v���C���[�̍��W�E�T�C�Y�擾
	XMFLOAT3 posPlayer = GetPlayerPos();
	float sizePlayer = GetPlayerRadSize();

	for (int i = 0; i < MAX_ENEMYRANGE; ++i)
	{
		// ���g�p�Ȃ�
		if (!g_ERange[i].m_use)
		{
			continue;
		}
		if (g_ERange[i].m_pos.x >= posPlayer.x)
		{
			g_ERange[i].m_rot = (XMFLOAT3(0.0f, 180.0f, 0.0f));
		}	
		//�G���W���v���C���[����������������
		if (g_ERange[i].m_pos.x <= posPlayer.x)
		{	
			g_ERange[i].m_rot = (XMFLOAT3(0.0f, 0.0f, 0.0f));
		}
			// �g�p���Ȃ�X�L�b�v
			if (!g_ERange[i].m_use)
			{
				continue;
			}
			if (CollisionSphere(g_ERange[i].m_pos, g_ERange[i].m_size.x, posPlayer, sizePlayer))
			{
				//DelLife();
				//if (GetLife() == 0)
				//{
				//	SetScene(SCENE_GAMEOVER);
				//}
				DelCountEnemy();
				g_ERange[i].m_use = false;
			}
			//��Ƀv���C���[�̕���������
			/*g_ERange[i].m_rotDest = posPlayer;
			g_ERange[i].m_rot = g_ERange[i].m_rotDest;
			g_ERange[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/
			// �^�C�}�[�J�E���g�_�E��
			if ((g_ERange[i].m_Time > 0) && (!GetPlayerInv()))
			{
				--g_ERange[i].m_Time;
				g_ERange[i].ReticleSize -= 0.25f;
				SetReticle(posPlayer, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					XMFLOAT2(g_ERange[i].ReticleSize, g_ERange[i].ReticleSize), 2);
				if (g_ERange[i].m_Time == 0)
				{
					CSound::SetPlayVol(SE_LASER, 0.1f);
					StartExplosion(posPlayer, XMFLOAT2(40.0f, 40.0f));
					if (GetPlayerJump())
					{
						DelLife();
					}
					g_ERange[i].m_Time += (ENEMY_TIMER + rand() % 3) * 60 + 59;	// ������x3�`6�b������
					g_ERange[i].ReticleSize = g_ERange[i].m_Time / 3;
				}
			}

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_ERange[i].m_rot.x),
				XMConvertToRadians(g_ERange[i].m_rot.y),
				XMConvertToRadians(g_ERange[i].m_rot.z));
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			mtxSize = XMMatrixScaling(SCALE_E_RANGE.x, SCALE_E_RANGE.y, SCALE_E_RANGE.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxSize);
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
	//for (int i = 0; i < MAX_ENEMYRANGE1; ++i)
	//{
	//	//GetPlayerJump();
	//	//�G�ƃv���C���[�̋������߂Â�����
	//	//if (CollisionSphere(posPlayer, sizePlayer, g_ERange1[i].m_pos, SEARCH_RANGE))
	//	//{
	//		// �g�p���Ȃ�X�L�b�v
	//	if (!g_ERange1[i].m_use)
	//	{
	//		continue;
	//	}
	//	if (CollisionSphere(g_ERange1[i].m_pos, g_ERange1[i].m_size.x, posPlayer, sizePlayer))
	//	{
	//		//DelLife();
	//		//if (GetLife() == 0)
	//		//{
	//		//	SetScene(SCENE_GAMEOVER);
	//		//}
	//		DelCountEnemy();
	//		g_ERange1[i].m_use = false;
	//	}
	//	//��Ƀv���C���[�̕���������
	//	/*g_ERange1[i].m_rotDest = posPlayer;
	//	g_ERange1[i].m_rot = g_ERange1[i].m_rotDest;
	//	g_ERange1[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/
	//	// �^�C�}�[�J�E���g�_�E��
	//	if (g_ERange1[i].m_Time > 0)
	//	{
	//		--g_ERange1[i].m_Time;
	//		if (g_ERange1[i].m_Time == 0)
	//		{
	//			if (GetPlayerJump())
	//			{
	//				//DelLife();
	//				StartExplosion(posPlayer, XMFLOAT2(40.0f, 40.0f));
	//			}
	//			else
	//			{
	//				continue;
	//			}
	//			g_ERange1[i].m_Time += (ENEMY_TIMER2 + rand() % 3) * 60 + 59;	// ������x3�`6�b������

	//		}
	//	}
	//	/*else
	//	{
	//		g_ERange1[i].m_Time;
	//	}*/

	//	// ���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();

	//	// ��]�𔽉f
	//	mtxRot = XMMatrixRotationRollPitchYaw(
	//		XMConvertToRadians(g_ERange1[i].m_rot.x),
	//		XMConvertToRadians(g_ERange1[i].m_rot.y),
	//		XMConvertToRadians(g_ERange1[i].m_rot.z));
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	mtxWorld = XMMatrixScaling(SCALE_E_RANGE1.x, SCALE_E_RANGE1.y, SCALE_E_RANGE1.z);
	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(
	//		g_ERange1[i].m_pos.x,
	//		g_ERange1[i].m_pos.y,
	//		g_ERange1[i].m_pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ���[���h�}�g���b�N�X�ݒ�
	//	XMStoreFloat4x4(&g_ERange1[i].m_mtxWorld, mtxWorld);
	//	//}
	//}
	//for (int i = 0; i < MAX_ENEMYRANGE2; ++i)
	//{
	//	//GetPlayerJump();
	//	//�G�ƃv���C���[�̋������߂Â�����
	//	//if (CollisionSphere(posPlayer, sizePlayer, g_ERange2[i].m_pos, SEARCH_RANGE2))
	//	//{
	//		// �g�p���Ȃ�X�L�b�v
	//	if (!g_ERange2[i].m_use)
	//	{
	//		continue;
	//	}
	//	if (CollisionSphere(g_ERange2[i].m_pos, g_ERange2[i].m_size.x, posPlayer, sizePlayer))
	//	{
	//		//DelLife();
	//		//if (GetLife() == 0)
	//		//{
	//		//	SetScene(SCENE_GAMEOVER);
	//		//}
	//		DelCountEnemy();
	//		g_ERange2[i].m_use = false;
	//	}
	//	//��Ƀv���C���[�̕���������
	//	/*g_ERange2[i].m_rotDest = posPlayer;
	//	g_ERange2[i].m_rot = g_ERange2[i].m_rotDest;
	//	g_ERange2[i].m_rot = XMFLOAT3(posPlayer.x, posPlayer.y, posPlayer.z);*/
	//	// �^�C�}�[�J�E���g�_�E��
	//	if (g_ERange2[i].m_Time > 0)
	//	{
	//		--g_ERange2[i].m_Time;
	//		if (g_ERange2[i].m_Time == 0)
	//		{
	//			if (GetPlayerJump())
	//			{
	//				//DelLife();
	//				StartExplosion(posPlayer, XMFLOAT2(40.0f, 40.0f));
	//			}
	//			else
	//			{
	//				continue;
	//			}
	//			g_ERange2[i].m_Time += (ENEMY_TIMER2 + rand() % 3) * 60 + 59;	// ������x3�`6�b������

	//		}
	//	}
	//	/*else
	//	{
	//		g_ERange2[i].m_Time;
	//	}*/

	//	// ���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();

	//	// ��]�𔽉f
	//	mtxRot = XMMatrixRotationRollPitchYaw(
	//		XMConvertToRadians(g_ERange2[i].m_rot.x),
	//		XMConvertToRadians(g_ERange2[i].m_rot.y),
	//		XMConvertToRadians(g_ERange2[i].m_rot.z));
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	mtxWorld = XMMatrixScaling(SCALE_E_RANGE2.x, SCALE_E_RANGE2.y, SCALE_E_RANGE2.z);
	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(
	//		g_ERange2[i].m_pos.x,
	//		g_ERange2[i].m_pos.y,
	//		g_ERange2[i].m_pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ���[���h�}�g���b�N�X�ݒ�
	//	XMStoreFloat4x4(&g_ERange2[i].m_mtxWorld, mtxWorld);
	//	//}
	//}
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
	

		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_model.Draw(pDC, g_ERange[i].m_mtxWorld, eTransparentOnly);
		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
	/////////////////////////////////////////////////////////////////////
	//// �s����������`��
	//for (int i = 0; i < MAX_ENEMYRANGE1; ++i) {
	//	// �g���ĂȂ��Ȃ�X�L�b�v
	//	if (!g_ERange1[i].m_use)
	//	{
	//		continue;
	//	}
	//	g_model1.Draw(pDC, g_ERange1[i].m_mtxWorld, eOpacityOnly);
	//}
	////// ������������`��
	//for (int i = 0; i < MAX_ENEMYRANGE1; ++i) {
	//	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	//	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	//	g_model1.Draw(pDC, g_ERange1[i].m_mtxWorld, eTransparentOnly);
	//	SetZWrite(true);				// Z�o�b�t�@�X�V����
	//	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	//}
	/////////////////////////////////////////////////////////////////////
	// //�s����������`��
	//for (int i = 0; i < MAX_ENEMYRANGE2; ++i) {
	//	// �g���ĂȂ��Ȃ�X�L�b�v
	//	if (!g_ERange2[i].m_use)
	//	{
	//		continue;
	//	}
	//	g_model2.Draw(pDC, g_ERange2[i].m_mtxWorld, eOpacityOnly);
	//}
	//// ������������`��
	//for (int i = 0; i < MAX_ENEMYRANGE2; ++i) {
	//	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	//	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	//	g_model2.Draw(pDC, g_ERange2[i].m_mtxWorld, eTransparentOnly);
	//	SetZWrite(true);				// Z�o�b�t�@�X�V����
	//	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	//}
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
int SetEnemyRange(XMFLOAT3 pos,int id)
{
	if (id == 0)
	{
		// �߂�l�̏�����
		int EnemyRange = -1;

		for (int cntEnemy = 0; cntEnemy < MAX_ENEMYRANGE; ++cntEnemy)
		{
			// �g�p���Ȃ�X�L�b�v
			if (g_ERange[cntEnemy].m_use)
			{
				continue;
			}
			g_ERange[cntEnemy].m_use = true;	// �g�p��ON
			g_ERange[cntEnemy].m_pos = pos;	// �w�肵�����W����

			EnemyRange = cntEnemy + 1;	// �g�p���̓G������
			break;
		}

		return EnemyRange;
	}
	//else if (id == 1)
	//{
	//	// �߂�l�̏�����
	//	int EnemyRange1 = -1;

	//	for (int cntEnemy1 = 0; cntEnemy1 < MAX_ENEMYRANGE1; ++cntEnemy1)
	//	{
	//		// �g�p���Ȃ�X�L�b�v
	//		if (g_ERange1[cntEnemy1].m_use)
	//		{
	//			continue;
	//		}
	//		g_ERange1[cntEnemy1].m_use = true;	// �g�p��ON
	//		g_ERange1[cntEnemy1].m_pos = pos;	// �w�肵�����W����

	//		EnemyRange1 = cntEnemy1 + 1;	// �g�p���̓G������
	//		break;
	//	}

	//	return EnemyRange1;
	//}
	//else if (id == 2)
	//{
	//	// �߂�l�̏�����
	//	int EnemyRange2 = -1;

	//	for (int cntEnemy2 = 0; cntEnemy2 < MAX_ENEMYRANGE2; ++cntEnemy2)
	//	{
	//		// �g�p���Ȃ�X�L�b�v
	//		if (g_ERange2[cntEnemy2].m_use)
	//		{
	//			continue;
	//		}
	//		g_ERange2[cntEnemy2].m_use = true;	// �g�p��ON
	//		g_ERange2[cntEnemy2].m_pos = pos;	// �w�肵�����W����

	//		EnemyRange2 = cntEnemy2 + 1;	// �g�p���̓G������
	//		break;
	//	}

	//	return EnemyRange2;
	//}
	return 0;
}

TEnemyRange* GetEnemyRange()
{
	return g_ERange;	
}
