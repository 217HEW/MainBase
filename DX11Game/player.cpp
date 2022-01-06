//**************************************************************
//
//	Player.cpp
//	�v���C���[����
//
//--------------------------------------------------------------
//	����ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/22	�v���C���[�̒����������(��)
//				�v���C���[��Z�����W�Œ�
//				�ړ��͈͍Đݒ�A�s�v�ȏ����̍폜
//				�R�����g�̕ҏW
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/22	GetPlayerSize���쐬
//				���̏��œ����蔻�肪�K�v�ɂȂ�����
//				GetPlayerJump���쐬
//				�u���b�N�̑ϋv�l�����炷�ۂɔ��ł邩�m�肽����
//				SetPlayerJump���쐬
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/24	�R���g���[���[�̏�����ǉ�
//				�X�e�B�b�N�̒l���擾����ɂ��Ă��܂��B
//				�R���g���[���[�������ꍇ�A�������܂���
//				�X�e�B�b�N���X���ă{�^����������
//				�v���C���[�����̕����ɃW�����v����悤�ɂ��܂����B
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/25	�X�e�B�b�N���X����Ɣ�ԕ������\�������悤�ɂ��܂���
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/27	�T�E���h��ǉ����܂����B�W�����v��
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/28	�G���A�N���A���A���̃G���A�ɑJ�ڂ���悤�ύX
//				CreateField.h���C���N���[�h
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/30	�R���g���[���[�����������甭�����Ă��܂��o�O��
//				��菜���܂����B�����ꍇ�̓X�e�B�b�N�̒l���[����
//				�Ȃ�܂��B
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/30	���f���̃X�P�[�����f�t�@�C���ɂ��܂����B
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/30	�����蔻��p�A�v���C���[�̋��E�����a���擾����֐��̖��O��ύX
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2022/01/07	�v���C���[�̃T�C�Y���擾���邽�߂̊֐��쐬
//				���̂��߁A�T�C�Y�p�̕ϐ��ug_sizeModel�v���O���[�o���ϐ��ɐ錾
//	�ҏW�ҁF�ĎR꣑��Y
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "player.h"
#include <xinput.h>		// �R���g���[���[���擾�ɕK�v
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "collision.h"
#include "explosion.h"
#include "life.h"
#include "Fade.h"
#include "CreateField.h"
#include "Sound.h"

#pragma comment (lib, "xinput.lib")	// �R���g���[���[���擾�ɕK�v
//**************************************************************
// �}�N����`
//**************************************************************
#define MODEL_PLAYER	 "data/model/Character02.fbx"

#define	VALUE_MOVE_PLAYER	(0.155f)	// �ړ����x
#define	SPEED_MOVE_PLAYER	(50)		// �������x
#define	RATE_MOVE_PLAYER	(0.025f)	// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(4.5f)		// ��]���x
#define	RATE_ROTATE_PLAYER	(0.1f)		// ��]�����W��
#define SCALE_PLAYER		XMFLOAT3(2.0f, 1.5f, 1.0f) //	�v���C���[�̃��f���X�P�[��

#define	PLAYER_RADIUS		(10.0f)		// ���E�����a
#define DAMAGE_TIMER		(120)		// �_���[�W��̖��G����

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static CAssimpModel	g_model;		// ���f��

static XMFLOAT3		g_posModel;		// ���݂̈ʒu
static XMFLOAT3		g_rotModel;		// ���݂̌���
static XMFLOAT3		g_rotDestModel;	// �ړI�̌���
static XMFLOAT3		g_moveModel;	// �ړ���
static XMFLOAT3		g_sizeModel;	// �ړ���

static XMFLOAT4X4	g_mtxWorld;		// ���[���h�}�g���b�N�X

//static int		g_nShadow;		// �ۉe�ԍ�
static int			g_nDamage;		// �_�Œ�
static bool			g_bInv;			// �_���[�W���̖��G����	true:���G
static bool			g_bLand;		// �n�ʔ���	true:���łȂ�

// �R���g���[���[
static DWORD	Joycon;		// �R���g���[���[���
static DWORD	JoyState;	// �ڑ��m�F�p
static XMFLOAT2 Stick;		// �X�e�B�b�N�̌X���p	

//**************************************************************
// ����������
//**************************************************************
HRESULT InitPlayer(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	g_nDamage = 0;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posModel = XMFLOAT3(50.0f, -800.0f, 0.0f);
	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sizeModel = SCALE_PLAYER;
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	 Stick = XMFLOAT2(0.0f, 0.0f);

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_PLAYER)) {
		MessageBoxA(GetMainWnd(), "���@���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
		return E_FAIL;
	}

	// �ۉe�̐���
	//g_nShadow = CreateShadow(g_posModel, 12.0f);

	// �ǐڐG,���G���菉����
	g_bLand = true;
	g_bInv = false;

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitPlayer(void)
{
	// �ۉe�̉��
	//ReleaseShadow(g_nShadow);

	// ���f���̉��
	g_model.Release();
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdatePlayer(void)
{
	//�Q�[���p�b�h�̏�Ԃ��擾
	XINPUT_STATE state;

	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	// -------�R���g���[���[����------------------------------------------
	GetJoyState(Joycon);
	// �R���g���[���[�̐ڑ��󋵊m�F
	JoyState = XInputGetState(0, &state);

	if (JoyState == ERROR_SUCCESS)
	{	// �ڑ��L�聫
		if (g_bLand)
		{
			// �X�e�B�b�N���͎�����
			if (GetJoyX(Joycon) != 0 || GetJoyY(Joycon) != 0)
			{
				// �X�e�B�b�N�̃f�b�h�]�[������
				if ((GetJoyX(Joycon) < JOYSTICK_DEADZONE && GetJoyX(Joycon) > -JOYSTICK_DEADZONE) &&
					(GetJoyY(Joycon) < JOYSTICK_DEADZONE && GetJoyY(Joycon) > -JOYSTICK_DEADZONE))
				{	// �X�������Ȃ��ꍇ��
					// �X�e�B�b�N�̒l���[���ɂ���
					Stick.x = 0.0f;
					Stick.y = 0.0f;
				}
				else
				{	// �傫���X�����ꍇ��
					// �X�e�B�b�N�̒l�|1�`1�ő��
					Stick.x = static_cast<FLOAT>(GetJoyX(Joycon) / 32767.0);
					Stick.y = -static_cast<FLOAT>(GetJoyY(Joycon) / 32767.0);
					SetEffect(XMFLOAT3((g_posModel.x + (60.0f * (Stick.x /2))), (g_posModel.y + (60.0f * Stick.y)),0.0f),
						XMFLOAT3(0.0f, 0.0f, 0.0f),
						XMFLOAT4(1.0f, 0.05f, 0.05f, 0.80f),
						XMFLOAT2(9.0f, 18.0f), 5);
					for (int i = 0; i < 5; i++)
					{
						SetEffect(XMFLOAT3((g_posModel.x + ((60.0f / 6 * i) * (Stick.x / 2))), (g_posModel.y + ((60.0f / 6 * i) * Stick.y)), 0.0f),
							XMFLOAT3(0.0f, 0.0f, 0.0f),
							XMFLOAT4(1.0f, 0.05f, 0.05f, 0.80f),
							XMFLOAT2(4.5f, 9.0f), 5);
					}
					// B�{�^���������ꂽ��
					if (GetJoyTrigger(Joycon, JOYSTICKID2))
					{
						g_moveModel.x = Stick.x *7.5f;
						g_moveModel.y = Stick.y *15;
						g_bLand = false;
						CSound::Play(SE_JUMP);
						CSound::SetVolume(SE_JUMP, 0.01f);
					}
				}
			}
		}
	}
	else
	{	// �ڑ�������
		Stick.x = 0.0f;
		Stick.y = 0.0f;
	}
	
	/*do
	{*/
	if (g_nDamage > 0) {
		--g_nDamage;
		if (g_nDamage <= 0) {
			g_bInv = false;
			g_nDamage = 0;
		}
		//break;
	}

	// -------�v���C���[����------------------------------------------
		// �Ǔ��ɐڐG���Ă���Ƃ�
	if (g_bLand)
	{
		// ������
		g_moveModel.x = 0;
		g_moveModel.y = 0;
		g_moveModel.z = 0;

		// �㉺�ړ�
		if (GetKeyTrigger(VK_UP)) {
			StartExplosion(g_posModel, XMFLOAT2(40.0f, 40.0f));
			g_moveModel.y += SPEED_MOVE_PLAYER;
			CSound::Play(SE_JUMP);
			CSound::SetVolume(SE_JUMP, 0.01f);
			g_bLand = false;
		}
		else if (GetKeyTrigger(VK_DOWN)) {
			StartExplosion(g_posModel, XMFLOAT2(40.0f, 40.0f));
			g_moveModel.y -= SPEED_MOVE_PLAYER;
			CSound::Play(SE_JUMP);
			CSound::SetVolume(SE_JUMP, 0.01f);
			g_bLand = false;
		}

		//���݉������̕ǂɐU��Ă���Ƌt���ɂ͔�ׂȂ�
		// �ǐڐG�����E�ړ����Ȃ�
		if (!g_bLand)
		{
			// ���E�ړ�
			if (GetKeyPress(VK_LEFT)) {
				g_moveModel.x -= SPEED_MOVE_PLAYER;
			}
			else if (GetKeyPress(VK_RIGHT)) {
				g_moveModel.x += SPEED_MOVE_PLAYER;
			}
			// ����O�ړ�
			//if (GetKeyPress(VK_UP)) {
			//	g_moveModel.z += SPEED_MOVE_PLAYER;
			//}
			//else if (GetKeyPress(VK_DOWN)) {
			//	g_moveModel.z -= SPEED_MOVE_PLAYER;
			//}
		}
	}
	//if (GetKeyPress(VK_LEFT)) {
	//	if (GetKeyPress(VK_UP)) {
	//		// ���O�ړ�
	//		g_moveModel.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y + 135.0f;
	//	} else if (GetKeyPress(VK_DOWN)) {
	//		// ����ړ�
	//		g_moveModel.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y + 45.0f;
	//	} else {
	//		// ���ړ�
	//		g_moveModel.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y + 90.0f;
	//	}
	//} else if (GetKeyPress(VK_RIGHT)) {
	//	if (GetKeyPress(VK_UP)) {
	//		// �E�O�ړ�
	//		g_moveModel.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y - 135.0f;
	//	} else if (GetKeyPress(VK_DOWN)) {
	//		// �E��ړ�
	//		g_moveModel.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y - 45.0f;
	//	} else {
	//		// �E�ړ�
	//		g_moveModel.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y - 90.0f;
	//	}
	//} else if (GetKeyPress(VK_UP)) {
	//	// �O�ړ�
	//	g_moveModel.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;
	//	//g_moveModel.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;
	//
	//	g_rotDestModel.y = 180.0f + rotCamera.y;
	//} else if (GetKeyPress(VK_DOWN)) {
	//	// ��ړ�
	//	g_moveModel.x -= SinDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
	//	//g_moveModel.z -= CosDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
	//
	//	g_rotDestModel.y = rotCamera.y;
	//}

	if (GetKeyPress(VK_I)) {
		// ��Ɉړ�
		g_moveModel.y += VALUE_MOVE_PLAYER;
	}
	if (GetKeyPress(VK_K)) {
		// ���Ɉړ�
		g_moveModel.y -= VALUE_MOVE_PLAYER;
	}

	if (GetKeyPress(VK_J)) {
		// ����]
		g_rotDestModel.y -= VALUE_ROTATE_PLAYER;
		if (g_rotDestModel.y < -180.0f) {
			g_rotDestModel.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_L)) {
		// �E��]
		g_rotDestModel.y += VALUE_ROTATE_PLAYER;
		if (g_rotDestModel.y >= 180.0f) {
			g_rotDestModel.y -= 360.0f;
		}
	}

	// -------�ړ��̐���&����------------------------------------------

		// �ړI�̊p�x�܂ł̍���
	float fDiffRotY = g_rotDestModel.y - g_rotModel.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	g_rotModel.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (g_rotModel.y >= 180.0f) {
		g_rotModel.y -= 360.0f;
	}
	if (g_rotModel.y < -180.0f) {
		g_rotModel.y += 360.0f;
	}

	// �ʒu�ړ�
	g_posModel.x += g_moveModel.x;
	g_posModel.y += g_moveModel.y;
	//g_posModel.z += g_moveModel.z;

	// �ړ��ʂɊ�����������
	// g_moveModel.x += (0.0f - g_moveModel.x) * RATE_MOVE_PLAYER;
	// g_moveModel.y += (0.0f - g_moveModel.y) * RATE_MOVE_PLAYER;
	// //g_moveModel.z += (0.0f - g_moveModel.z) * RATE_MOVE_PLAYER;
	// 
	if (g_posModel.x < -630.0f) {
		g_posModel.x = -630.0f;

		StartFadeOut(SCENE_GAMEOVER);
	}
	if (g_posModel.x > 1630.0f) {
		g_posModel.x = 1630.0f;

		StartFadeOut(SCENE_GAMEOVER);
	}
	//  if (g_posModel.z < 0.0f) {
	//  	g_posModel.z = 0.0f;
	//  }
	//  if (g_posModel.z > 0.0f) {
	//  	g_posModel.z = 0.0f;
	//  }
	//  if (g_posModel.y < -199.0f) {
	//  	g_posModel.y = -200.0f;
	//	StartFadeOut(SCENE_GAMEOVER);
	//  }
	if (g_posModel.y > 800.0f) {
		g_posModel.y = 800.0f;
		// ���̃G���A���玟�̃G���A��
		switch (GetAreaState())
		{
		case AREA_1:
			StartFadeOut(SCENE_AREA2);
			break;
		case AREA_2:
			StartFadeOut(SCENE_AREA3);
			break;
		case AREA_3:
			StartFadeOut(SCENE_AREA_BOSS);
			break;
		case AREA_BOSS:
			StartFadeOut(SCENE_GAMECLEAR);
			break;
		default:
			break;
		}
	}
	// 
	if (GetKeyPress(VK_RETURN)) {
		// ���Z�b�g
		g_posModel = XMFLOAT3(0.0f, -800.0f, 0.0f);
		g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	// -------���[���h�}�g���N�X����------------------------------------------

		// 1.���[���h�}�g���N�X�A2.��]�ړ��A3.���s�ړ�
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
										  XMConvertToRadians(g_rotModel.y),
										  XMConvertToRadians(g_rotModel.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// ���f���̃T�C�Y
	mtxWorld = XMMatrixScaling(g_sizeModel.x, g_sizeModel.y, g_sizeModel.z);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y, g_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// �ۉe�̈ړ�
	//MoveShadow(g_nShadow, g_posModel);

// -------�G�t�F�N�g����------------------------------------------
	if ((g_moveModel.x * g_moveModel.x
	   + g_moveModel.y * g_moveModel.y
	   + g_moveModel.z * g_moveModel.z) > 1.0f) {
		XMFLOAT3 pos;

		pos.x = g_posModel.x + SinDeg(g_rotModel.y) * 10.0f;
		pos.y = g_posModel.y + 2.0f;
		pos.z = g_posModel.z + CosDeg(g_rotModel.y) * 10.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
					   XMFLOAT4(0.85f, 0.05f, 0.65f, 0.50f),
					   XMFLOAT2(14.0f, 14.0f), 20);
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
					   XMFLOAT4(0.65f, 0.85f, 0.05f, 0.30f),
					   XMFLOAT2(10.0f, 10.0f), 20);
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
					   XMFLOAT4(0.45f, 0.45f, 0.05f, 0.15f),
					   XMFLOAT2(5.0f, 5.0f), 20);
	}

	// �e����
	//if (GetKeyRepeat(VK_SPACE)) {
	//	FireBullet(g_posModel, XMFLOAT3(-g_mtxWorld._31, -g_mtxWorld._32, -g_mtxWorld._33),
	//		BULLETTYPE_PLAYER);
	//}
// -------�e�X�g����------------------------------------------
	// �_���[�W�e�X�g
	if (GetKeyRepeat(VK_D))
	{
		g_bInv = true;
		DelLife();
		g_nDamage = DAMAGE_TIMER;

	}
	// �񕜃e�X�g
	if (GetKeyRepeat(VK_H))
	{
		AddLife();
		//g_nDamage = DAMAGE_TIMER;

	}
	//} while (0);
	//�����蔻��
	//�e�X�g�ǂƂ̓����蔻���life������
	//XMFLOAT2 vRect(COLLISION_WIDTH, COLLISION_HEIGHT);
	//if (CollisionEnemy(g_vPos, vRect, 0.0f) >= 0)
	//{
	//	//life�����Z
	//	DelLife();
	//	g_nDamage = DAMAGE_TIMER;
	//	CSound::Play(SE_HIT);
	//}

	// PrintDebugProc("[˺�� �� : (%f : %f : %f)]\n", g_posModel.x, g_posModel.y, g_posModel.z);
	// PrintDebugProc("[˺�� ѷ : (%f) < Ӹ÷ ��:(%f) >]\n", g_rotModel.y, g_rotDestModel.y);
	// PrintDebugProc("\n");

	// PrintDebugProc("*** ˺�� ��� ***\n");
	// PrintDebugProc("ϴ   ��޳ : \x1e\n");//��
	// PrintDebugProc("���  ��޳ : \x1f\n");//��
	// PrintDebugProc("���� ��޳ : \x1d\n");//��
	// PrintDebugProc("з�  ��޳ : \x1c\n");//��
	// PrintDebugProc("�ޮ����   : I\n");
	// PrintDebugProc("���       : K\n");
	// PrintDebugProc("���� �ݶ� : J\n");
	// PrintDebugProc("з�  �ݶ� : L\n");
	// PrintDebugProc("��   ʯ�� : Space\n");
	// PrintDebugProc("\n");
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawPlayer(void)
{
	// ���f���_�ŏ���
	if (g_nDamage > 0) {
		if (g_nDamage & 4)	// 4�t���[�������Ƃɕ`�悵�Ȃ�
			return;
	}
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);

	// // ������������`��
	// SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	// SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	// g_model.Draw(pDC, g_mtxWorld, eTransparentOnly);
	// SetZWrite(true);				// Z�o�b�t�@�X�V����
	// SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}

//*******************************
//
//		�ʒu���擾
//	
//	�߂�l
//		�v���C���[�̈ʒu
//
//*******************************
XMFLOAT3& GetPlayerPos()
{
	return g_posModel;
}

//*******************************
//
//		�ʒu���擾
//	
//	�߂�l
//		�v���C���[�̈ʒu
//
//*******************************
XMFLOAT3& GetPlayerSize()
{
	return g_sizeModel;
}

//*******************************
//
//		�T�C�Y���擾
//	
//	�߂�l
//		�v���C���[�̃T�C�Y(����)
//
//*******************************
float GetPlayerRadSize()
{
	return PLAYER_RADIUS;
}

//*******************************
//
//		�W�����v��ԏ��擾
//	 �W�����v��bool���擾����
//
//	�߂�l
//		�W�����v��� true:���ł��Ȃ�
//
//*******************************
bool GetPlayerJump()
{
	return g_bLand;
}

//*******************************
//
//		�W�����v��ԃZ�b�g
// �W�����v��bool���O����ύX����
//
//	�����F
//		jump:�W�����v��Ԃ�bool
//		true:���ł��Ȃ�
//
//	�߂�l
//		����
//
//*******************************
void SetPlayerJump(bool jump)
{
	g_bLand = jump;
}
//*******************************
//
//		�v���C���[�Ƃ̏Փ˔���
//	
//	����:
//		pos:�v���C���[3�����W�Aradius:���a�Adamage:�_���[�W��
//
//	�߂�l
//		bool:�����������ǂ���
//		true:���ł��Ȃ�
//
//***********************************************************
bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage)
{
	bool hit = CollisionSphere(g_posModel, PLAYER_RADIUS, pos, radius);
	if (hit) {
		// �����J�n
		int nExp = -1;
		if (damage > 0.0f) {
			nExp = StartExplosion(g_posModel, XMFLOAT2(40.0f, 40.0f));
			// 
		}
		else {
			nExp = StartExplosion(g_posModel, XMFLOAT2(20.0f, 20.0f));
		}
		// �����G�t�F�N�g�̐F��ݒ�
		SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
	}

	return hit;
}
