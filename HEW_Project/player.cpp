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
//--------------------------------------------------------------
//	2022/01/16	���f���J���[��ǉ����܂���
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2022/01/23	�G�t�F�N�g�`�揈���ǉ�
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2022/01/24	�v���C���[�̈ړ��������������܂��������؂��ł͂���܂���
//				�R���g���[���[�̕��͗ǂ������ł��B
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2022/01/25	�v���C���[�̈ʒu�����Z�b�g���鏈���̍폜
//				�v���C���[�̈ꕔ�ړ������̍폜(�R�����g�A�E�g)
//				�v���C���[�̈ړ�����(���[)��ǉ�
//				�v���C���[�̈ʒu���Z�b�g�����̍폜
//				�ꕔ�R�����g�̒ǉ�
//				�R�����g�A�E�g���ꂽ����Ȃ��ł��낤�ꕔ�f�o�b�O�\�������̍폜
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2022/01/25	�v���C���[�̕�����ς��鏈����ǉ����܂���
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2022/01/29	���삷��܂Ŗ��G�A�Ɣ��f���鏈����ǉ����܂���
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2022/01/29	�v���C���[�̕����w�肷��ϐ���񋓑̕ϐ��Ő錾�������܂���
//				����ɂ��A�֐��̌^���񋓑̐錾���̌^���ɕύX���܂����B
//	�ҏW�ҁF�ĎR꣑��Y
//**************************************************************

//**************************************************************
//	�ۑ�F���󂻂ꂼ��̃N���X��Init~Update~��ǂݍ���ł��ďd���̂�
//		���܂Ƃ߂�Init,Update�ŌĂׂ�`�ɕς���
//**************************************************************



//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "player.h"
#include <xinput.h>		// �R���g���[���[���擾�ɕK�v
#include "AssimpModel.h"
#include "debugproc.h"
#include "effect.h"
#include "collision.h"
#include "explosion.h"
#include "life.h"
#include "Fade.h"
#include "CreateStage.h"
#include "Block.h"
#include "Sound.h"
#include "PlayEffect.h"
#include "Texture.h"
#include "polygon.h"

#pragma comment (lib, "xinput.lib")	// �R���g���[���[���擾�ɕK�v
//**************************************************************
// �}�N����`
//**************************************************************
#define MODEL_PLAYER	"data/model/Hero/NewAllMotion02.fbx"	// "data/model/test.fbx"

#define	VALUE_MOVE_PLAYER	(0.155f)	// �ړ����x
#define	SPEED_MOVE_PLAYER	(5)			// �������x
#define	RATE_MOVE_PLAYER	(0.025f)	// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(4.5f)		// ��]���x
#define	RATE_ROTATE_PLAYER	(0.1f)		// ��]�����W��
#define SCALE_PLAYER		(XMFLOAT3(1.0f, 1.5f, 1.0f))	//	�v���C���[�̃��f���X�P�[��
#define COLLAR_PLAYER		(XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f))	// �v���C���[�J���[(��)������������ƃJ���[���ς��܂�
#define SIZE_PLAYER			XMFLOAT3(6.0f, 10.0f, 0.5f)		// �v���C���[�̃��f���T�C�Y

#define	PLAYER_RADIUS		(5.0f)		// ���E�����a
#define DAMAGE_TIMER		(120)		// �_���[�W��̖��G����

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static CAssimpModel	g_model;		// ���f��

static XMFLOAT3		g_posModel;		// ���݂̈ʒu
static XMFLOAT3		g_rotModel;		// ���݂̌���
static XMFLOAT3		g_rotDestModel;	// �ړI�̌���
static XMFLOAT3		g_moveModel;	// �ړ���
static XMFLOAT3		g_ScaleModel;	// �g�k����
static XMFLOAT3		g_SizeModel;	// �����蔻��p�T�C�Y

static XMFLOAT4X4	g_mtxWorld;		// ���[���h�}�g���b�N�X

int	g_nDamage;		// �_�Œ�
PlayerDir	g_eDir;	// �ǂƐڂ��Ă������	0:�����ĂȂ� 1:�E,2:��,3:��,4:��
bool g_bInv;		// �_���[�W���̖��G����	true:���G
bool g_bLand;		// �n�ʔ���	true:���łȂ�
bool g_bkabe;		// �ǂɐG��Ă܂� true�G��Ă܂�

// �R���g���[���[
static DWORD	Joycon;		// �R���g���[���[���
static DWORD	JoyState;	// �ڑ��m�F�p
static XMFLOAT2 Stick;		// �X�e�B�b�N�̌X���p	

Effect g_PlayerEffect;		// �v���C���[�̃G�t�F�N�g
int g_PEffectTimer = 0;		// �v���C���[�G�t�F�N�g����p�^�C�}�[

//**************************************************************
// ����������
//**************************************************************
HRESULT CPlayer::InitPlayer()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posModel = XMFLOAT3(10.0f, -320.0f, 0.0f);
	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_ScaleModel = SCALE_PLAYER;
	g_SizeModel = SIZE_PLAYER;
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f); 
	Stick = XMFLOAT2(0.0f, 0.0f);

	// ���f���f�[�^�̓ǂݍ���
	g_model.SetDif(COLLAR_PLAYER); // ���f�����[�h�O�ɃJ���[���w��
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_PLAYER)) {
		MessageBoxA(GetMainWnd(), "���@���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
		return E_FAIL;
	}

	g_PlayerEffect.Load();
	
	// �ǐڐG,���G���菉����
	g_bLand = true;
	g_bInv = true;
	g_bkabe = true;

	g_eDir = DIR_DOWN;	// ���Ɛڂ��Ă���

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void CPlayer::UninitPlayer()
{
	// ���f���̉��
	g_model.Release();
}

//**************************************************************
// �X�V����
//**************************************************************
void CPlayer::UpdatePlayer()
{
	//�Q�[���p�b�h�̏�Ԃ��擾
	XINPUT_STATE state;

	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();
	CSceneManager* SManager = CSceneManager::CGet();
	CFade* fade = SManager->GetCFade();
	// �̗͂�0�Ȃ�Q�[���I�[�o�[��
	if (GetLife() <= 0)
	{
		fade->StartFadeOut(SManager->Get());
	}

	// ���삵���疳�G������
	if (g_bLand == false)
	{
		g_bInv = false;
	}

	// -------�R���g���[���[����------------------------------------------
	GetJoyState(Joycon);
	// �R���g���[���[�̐ڑ��󋵊m�F
	JoyState = XInputGetState(0, &state);

	g_bkabe = true;
	if (JoyState == ERROR_SUCCESS)
	{	// �ڑ��L�聫
		if (g_bLand && g_bkabe)
		{
			// �X�e�B�b�N���͎�����
			if (GetJoyX(Joycon) != 0 || GetJoyY(Joycon) != 0)
			{
				// �X�e�B�b�N�̒l�|1�`1�ő��
				Stick.x = static_cast<FLOAT>(GetJoyX(Joycon) / 32767.0);
				Stick.y = -static_cast<FLOAT>(GetJoyY(Joycon) / 32767.0);

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
					// �����w��(�R���g���[���p)
					switch (g_eDir)
					{
					case DIR_RIGHT: if (Stick.x < 0.4f)	{ g_bkabe = false; }
						break;
					case DIR_LEFT: if (Stick.x > -0.4f) { g_bkabe = false; }
						break;
					case DIR_UP: if (Stick.y > -0.4f) { g_bkabe = false; }
						break;
					case DIR_DOWN: if (Stick.y < 0.4f) { g_bkabe = false; }
						break;
					default:
						break;
					}

					// �K�C�h�p
					for (int i = 0; i < 5; i++)
					{
						if(g_bkabe == true)
						SetEffect(XMFLOAT3((g_posModel.x + ((60.0f / 6 * i) * (Stick.x / 2))), (g_posModel.y + (60.0f / 6 * i) * Stick.y), 0.0f),
							XMFLOAT3(0.0f, 0.0f, 0.0f),
							XMFLOAT4(1.0f, 0.0f, 0.0f, 0.80f),
							XMFLOAT2(4.5f, 9.0f), 5);
					}
					// A�{�^���������ꂽ��
					if (GetJoyTrigger(Joycon, JOYSTICKID1) && g_bkabe)
					{
						g_moveModel.x += SPEED_MOVE_PLAYER *Stick.x;
						g_moveModel.y += SPEED_MOVE_PLAYER *Stick.y*1.5;
						g_eDir = DIR_NONE;
						g_rotModel.z = Stick.x * -90;
						g_rotModel.y = Stick.y * -90;
						g_bLand = false;				 // �ݒu����I�t
						CSound::SetPlayVol(SE_JUMP,0.1f); // �W�����v��
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

	// -------�v���C���[����------------------------------------------
		// �Ǔ��ɐڐG���Ă���Ƃ�
	if (g_bLand)
	{
		// ������
		g_moveModel.x = 0;
		g_moveModel.y = 0;
		g_moveModel.z = 0;

		// �㉺�ړ�
		if (GetKeyTrigger(VK_UP) && !(g_eDir == DIR_UP)) {
			StartExplosion(g_posModel, XMFLOAT2(40.0f, 40.0f));
			g_moveModel.y += SPEED_MOVE_PLAYER * 1.5;
			CSound::SetPlayVol(SE_JUMP, 0.1f); // �W�����v��
			g_bLand = false;	// �ݒu����I�t
		}
		else if (GetKeyTrigger(VK_DOWN) && !(g_eDir == DIR_DOWN)) {
			StartExplosion(g_posModel, XMFLOAT2(40.0f, 40.0f));
			g_moveModel.y -= SPEED_MOVE_PLAYER * 1.5f;
			CSound::SetPlayVol(SE_JUMP, 0.1f); // �W�����v��
			g_bLand = false;	// �ݒu����I�t
		}

		//���݉������̕ǂɐU��Ă���Ƌt���ɂ͔�ׂȂ�
		// �ǐڐG�����E�ړ����Ȃ�
		if (!g_bLand)
		{
			// ���E�ړ�
			if (GetKeyPress(VK_LEFT) && !(g_eDir == DIR_RIGHT)) {
				g_moveModel.x -= SPEED_MOVE_PLAYER;
			}
			else if (GetKeyPress(VK_RIGHT) && !(g_eDir == DIR_LEFT)) {
				g_moveModel.x += SPEED_MOVE_PLAYER;
			}
		}
	}
	
	// -------�ړ��̐���&����------------------------------------------

	// �ʒu�ړ�
	g_posModel.x += g_moveModel.x;
	g_posModel.y += g_moveModel.y;

	// �Q�[���I�[�o�[�p����
	if (g_posModel.x > 630.0f)	{ fade->StartFadeOut(SManager->Get()); }	// �E
	if (g_posModel.x < -430.0f) { fade->StartFadeOut(SManager->Get()); }	// ��
	if (g_posModel.y > 0.0f)	{ fade->StartFadeOut(SManager->Get()); }	// ��
	if (g_posModel.y < -400.0f) { fade->StartFadeOut(SManager->Get()); }	// ��

	// �v���C���[��������
	switch (g_eDir)
	{
	case DIR_NONE: g_rotModel.x = 0.0f;
			//g_rotModel.y = 0.0f;
		break;

	case DIR_RIGHT: g_rotModel.x = 90.0f;
			g_rotModel.y = 90.0f;
			g_rotModel.z = 0.0f;
		break;
	case DIR_LEFT:	g_rotModel.x = 90.0f;
			g_rotModel.y = -90.0f;
			g_rotModel.z = 0.0f;
		break;
	case DIR_UP: g_rotModel.x = 0.0f;
			g_rotModel.y = 0.0f;
			g_rotModel.z = 180.0f;
		break;
	case DIR_DOWN: g_rotModel.x = 0.0f;
			g_rotModel.y = 0.0f; 
			g_rotModel.z = 0.0f;
		break;
	default:
		break;
	}

// -------�G�t�F�N�g����------------------------------------------
	g_PlayerEffect.Update();

	if ((g_moveModel.x * g_moveModel.x
	   + g_moveModel.y * g_moveModel.y
	   + g_moveModel.z * g_moveModel.z) > 1.0f) {
		XMFLOAT3 pos;

		pos.x = g_posModel.x;
		pos.y = g_posModel.y - 10.0f;
		pos.z = g_posModel.z;

		// �G�t�F�N�g�̐ݒ�
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
					   XMFLOAT4(0.85f, 0.85f, 0.05f, 0.50f),
					   XMFLOAT2(14.0f, 14.0f), 20);
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
					   XMFLOAT4(0.65f, 0.65f, 0.05f, 0.30f),
					   XMFLOAT2(10.0f, 10.0f), 20);
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
					   XMFLOAT4(0.45f, 0.45f, 0.05f, 0.15f),
					   XMFLOAT2(5.0f, 5.0f), 20);
	}

#ifdef _DEBUG
	PrintDebugProc("StickX : %f\n", Stick.x);
	PrintDebugProc("StickY : %f\n", Stick.y);
	PrintDebugProc("PosX : %f\n", g_posModel.x);
	PrintDebugProc("PosY : %f\n", g_posModel.y);
	PrintDebugProc("Dir : %d\n", g_eDir);
#endif // _DEBUG

	

	// -------���f���A�j���[�V��������------------------------------------------
	switch (g_eDir)
	{
	case DIR_NONE: g_model.SetAnimIndex(2);
				   g_model.SetAnimTime(0.4);
				   break;
	case DIR_RIGHT: g_model.SetAnimIndex(4);
					g_model.SetAnimTime(0.4);
					break;
	case DIR_LEFT: g_model.SetAnimIndex(4);
				   g_model.SetAnimTime(0.4);
				   break;
	case DIR_UP: g_model.SetAnimIndex(2);
				 g_model.SetAnimTime(10);
				 break;
	case DIR_DOWN: g_model.SetAnimIndex(2);
				   g_model.SetAnimTime(10);
				   break;
	default:
		break;
	}

	// -------���[���h�}�g���N�X����------------------------------------------

	// 1.���[���h�}�g���N�X�A2.��]�ړ��A3.���s�ړ��A4.�g�k
	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
		XMConvertToRadians(g_rotModel.y),
		XMConvertToRadians(g_rotModel.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// ���f���̃T�C�Y
	mtxScale = XMMatrixScaling(g_ScaleModel.x, g_ScaleModel.y, g_ScaleModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);

	// �ړ��𔽉f
	if(g_eDir == DIR_UP) { mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y + 10, g_posModel.z); } // �V��ɓ����Ă����烂�f�����W�C��
	else if(g_eDir == DIR_RIGHT) { mtxTranslate = XMMatrixTranslation(g_posModel.x -6, g_posModel.y - 10, g_posModel.z); }
	else if(g_eDir == DIR_LEFT) { mtxTranslate = XMMatrixTranslation(g_posModel.x +3, g_posModel.y - 10, g_posModel.z); }
	else{ mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y - 10, g_posModel.z); }
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);
}

//**************************************************************
// �`�揈��
//**************************************************************
void CPlayer::DrawPlayer()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);
	g_PlayerEffect.Draw();

	// // ������������`��
	 SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	 SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	 g_model.Draw(pDC, g_mtxWorld, eTransparentOnly);
	 SetZWrite(true);				// Z�o�b�t�@�X�V����
	 SetBlendState(BS_NONE);		// �A���t�@�u�����h����
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
	return g_ScaleModel;
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
//		���G��ԏ��擾
//	 ���G��bool���擾����
//	�Q�[���J�n���A�G��܂Ŗ��G�ɂ����
//	�߂�l
//		true:���G
//
//*******************************
bool GetPlayerInv()
{
	return g_bInv;
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
//*******************************
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

//*******************************
//
//		�v���C���[�ړ������p�֐�
//	
//	����:
//		int dir : �ǂ̕��p�ɓ����Ă��邩
//
//	�߂�l
//		����
//
//*******************************
void SetPlayerDir(PlayerDir dir)
{
	g_eDir = dir;
}

//*******************************
//
//		�v���C���[�ݒu�֐�
//	
//	����:
//		�ݒu���������W
//
//	�߂�l
//		����
//
//*******************************
void SetPlayer(XMFLOAT3 pos)
{
	g_posModel = pos;
}