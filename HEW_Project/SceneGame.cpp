//**************************************************************
//
//	SceneGame.cpp
//	�^�C�g���V�[�����g
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/06	������N�̃v���O���������ɍ쐬
//	�ҏW�ҁF??
//--------------------------------------------------------------
//	2021/12/17	Timer��Life��O���DX��i�����Ƃɐ���
//				�`��ɂ܂���肠��
//	�ҏW�ҁF??
//--------------------------------------------------------------
//	2021/12/19	�`�抮��
//	�ҏW�ҁF??
//--------------------------------------------------------------
//	2021/12/21	GetDevice�֐��i�[�p�|�C���^�ϐ����쐬���A�K���̕ύX
//				�t�F�[�h���ɕʂ̃t�F�[�h���������Ȃ��悤�␳
//				�|���S���A�f�o�b�O�\���A���͏�����4�又���폜(�|���S���͕`�揈���ȊO)
//				�|�����C���֘A�폜
//				�`��֐��̍폜
//														�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/22	�K�v���������m�̕`��A�X�V�𒆒f���Ă��܂��B
//														�ύX�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/22	�|�[�Y�Ɉڂ�L�[����폜(VK_PAUSE)
//														�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/27	���f�[�^��ǉ������̂Ŏ���(BGM)
//														�ύX�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/28	�f�o�b�O�p�V�[���؂�ւ��ɂɃ{�X�G���A�̒ǉ�
//				InitGame�̈����ɃG���A�p�̈����ǉ�
//				BGM�Đ������̎�O�ɃG���A����BGM��ς������ꍇ�̎菇�R�����g��ǉ�
//				�f�o�b�O����őJ�ڂł���V�[���̒ǉ�(AREA2,3,BOSS)
//				CreateField.h��SceneGame.h�Ɉړ�
//														�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/30	�t�B�[���h�������R�����g�A�E�g���Ă��܂��Ă����̂�
//				���ɖ߂��܂����B
//														�ύX�ҁF�㌎��n
//--------------------------------------------------------------
//	2022/01/22	Effekseer�ō쐬�����G�t�F�N�g���Đ����鏈���̒ǉ�
//														�ύX�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2022/01/25	�G�t�F�N�g�N���X�p�̕ϐ��̖��O��ύX
//				����Ȃ��I������(MeshuField,MeshWall,Bullet,Smoke)���R�����g�A�E�g���폜���܂���
//				UpdateBG�̌Ăяo�����Ԃ�ύX���܂���
//														�ύX�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2022/01/30	�|�[�Y���Ƀ��g���C�������ƕςȃG���A�ɔ��
//				�s����Ȃ����܂���
//														�ύX�ҁF�㌎��n
//--------------------------------------------------------------
//	2022/01/31	����Ȃ��������R�����g�����̍폜
//														�ύX�ҁF�ĎR꣑��Y
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "SceneGame.h"
#include "SceneManager.h"
#include "Fade.h"
#include "AssimpModel.h"
#include "input.h"
#include "polygon.h"
#include "debugproc.h"
#include "mesh.h"
#include "player.h"
#include "bg.h"
#include "explosion.h"
#include "effect.h"
#include "Reticle.h"
#include "Sound.h"
#include "timer.h"
#include "life.h"
#include "number.h"
#include "Block.h"
#include "EnemyMelee.h"
#include "EnemyRange.h"
#include "Pause.h"
#include "PlayEffect.h"
#include "ClearPause.h"	//�e�X�g�C���N���[�h
#include "StageSelect.h"
#include "CountEnemy.h"


//**************************************************************
// �}�N����`
//**************************************************************
#define MAX_POLYLINE	(20)		// �|�����C����
#define THICKNESS		(10.0f)		// ���̑���

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static DWORD Joycon;	// �R���g���[���[���
bool g_bPause;			// �ꎞ��~��
bool g_bC_Pause;		// �N���A�\���t���O
int g_nNowScene;		// ���݂̃V�[��

Effect g_GameEffect;	// �G�t�F�N�g�ϐ�
static int g_EffectTimer = 0;	// �G�t�F�N�g����p�^�C�}�[

//**************************************************************
// ����������
//	�����F�J�ڐ�̃G���A
//**************************************************************
HRESULT InitGame(AREA Area)
{
	HRESULT hr = S_OK;

	// ���b�V��������
	hr = InitMesh();
	if (FAILED(hr))
		return hr;

	// ��������
	hr = InitNumber();
	if (FAILED(hr))
		return hr;

	//*12/17�V���ڐl�ǉ�
	// �^�C�}�[�\��������
	hr = InitTimer();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("�^�C�}�[�\�����������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}
	// �J�E���g�G�l�~�[�\��������
	hr = InitCountEnemy();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("�J�E���g�G�l�~�[�\�����������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// ���C�t�\��������
	hr = InitLife();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("���C�t�\�����������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// ���@������
	hr = InitPlayer();
	if (FAILED(hr))
		return hr;

	// �w�i������
	hr = InitBG();
	if (FAILED(hr))
		return hr;

	// ����������
	hr = InitExplosion();
	if (FAILED(hr))
		return hr;

	// �G�t�F�N�g������
	hr = InitEffect();
	if (FAILED(hr))
		return hr;

	// ���e�B�N��������
	hr = InitReticle();
	if (FAILED(hr))
		return hr;

	// �G�l�~�[�����[������
	hr = InitEnemyMelee();
	if (FAILED(hr))
		return hr;

	// �����W������
	hr = InitEnemyRange();
	if (FAILED(hr))
		return hr;

	//�񎟌��z��}�b�v
	hr = InitCField(Area);
	if (FAILED(hr))
		return hr;

	// �G�t�F�N�g(for Effekseer)������
	hr = g_GameEffect.Load();
	if (FAILED(hr))
		return hr;

	// �|�[�Y������
	hr = InitPause();
	g_bPause = false;
	if (FAILED(hr))
		return hr;

	// �e�X�g
	hr = InitC_Pause();
	g_bC_Pause = false;
	if (FAILED(hr))
		return hr;



	g_nNowScene = GetScene();
	
	// BGM�Đ��J�n
	// �G���A����BGM��ς��������͂�����switch���Ő؂�ւ���悤�ɂ���
	CSound::SetPlayVol(BGM_GAME000, 0.1f); // �Q�[���{��BGM

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitGame()
{
	// BGM�Đ���~
	CSound::Stop(BGM_GAME000);

	//�|�[�Y�I������
	UninitPause();

	//�e�X�g
	UninitC_Pause();

	// �G�t�F�N�g�I��
	UninitEffect();

	// ���e�B�N���I��
	UninitReticle();

	// �����I��
	UninitExplosion();

	// �w�i�I��
	UninitBG();

	// �񎟌��z��}�b�v�I��
	UninitCField();

	// ���@�I��
	UninitPlayer();

	//�i���o�[�I��
	UninitNumber();

	//���C�t�I��
	UninitLife();

	//�^�C�}�[�I��
	UninitTimer();

	//�J�E���g�G�l�~�[�I��
	UninitCountEnemy();

	// ���b�V���I��
	UninitMesh();
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateGame()
{
	// �R���g���[���[���
	GetJoyState(Joycon);

	TEnemyMelee* pEMelee = GetEnemyMelee();

	if (GetClearPause())
	{
		UpdateC_Pause();
	}
	else
	{



		//if (g_bC_Pause) {
		//	//�ꎞ��~�X�V
		//	UpdateC_Pause();
		//}
		//else if (GetFadeState() == FADE_NONE)
		//{
		//	int Timer = GetTimer();
		//	if (Timer <= 0)
		//	{
		//		StartFadeOut(SCENE_GAMEOVER);
		//	}
		//}

		//�ꎞ��~��?
		if (g_bPause) {
			//�ꎞ��~�X�V
			UpdatePause();
		}
		else
		{
			// �t�F�[�h���͏������Ȃ�
			if (GetFadeState() == FADE_NONE)
			{

#ifdef _DEBUG
				if (GetKeyRelease(VK_1))
				{
					StartFadeOut(SCENE_TITLE);
				}
				else if (GetKeyRelease(VK_2))
				{
					StartFadeOut(SCENE_SELECT);
				}
				else if (GetKeyRelease(VK_3))
				{
					StartFadeOut(SCENE_GAME);
				}
				else if (GetKeyRelease(VK_4))
				{
					StartFadeOut(SCENE_AREA2);
				}
				else if (GetKeyRelease(VK_5))
				{
					StartFadeOut(SCENE_AREA3);
				}
				else if (GetKeyRelease(VK_6))
				{
					StartFadeOut(SCENE_GAMEOVER);
				}
				else if (GetKeyRelease(VK_7))
				{
					StartFadeOut(SCENE_GAMECLEAR);
				}
				else if (GetKeyRelease(VK_8))
				{
					StartFadeOut(SCENE_SELECT);
				}
#endif

				int Timer = GetTimer();
				if (Timer <= 0)
				{
					StartFadeOut(SCENE_GAMEOVER);
				}
			}
			// �w�i�X�V
			UpdateBG();

			// ���@�X�V
			UpdatePlayer();

			// �񎟌��z��}�b�v�X�V
			UpdateCField();

			//*12/17�V���ڐl�ǉ�
			// �^�C�}�[�X�V
			if (!GetPlayerInv())
				UpdateTimer();

			// �^�C�}�[�X�V
			if (!GetPlayerInv())
				UpdateCountEnemy();

		// �J�����X�V
		CCamera::Get()->Update();

			// �����X�V
			UpdateExplosion();

		// �G�t�F�N�g�X�V
		UpdateEffect();

		// �Ə��X�V
		UpdateReticle();

			// �G�t�F�N�g(for Effekseer)�X�V
			if (g_EffectTimer == 0)
			{
				g_GameEffect.Set(EFFECT_FIRE, XMFLOAT3(-50, -50, 0), XMFLOAT3(10.0f, 10.0f, 10.0f), 0.1f, XMFLOAT3(1.0f, 1.0f, 1.0f));
				g_EffectTimer = 30;
			}
			--g_EffectTimer;
			g_GameEffect.Update();

			// ���X�V
			// UpdateSmoke();

		}
		//�ꎞ��~ON/OFF
		if (GetKeyTrigger(VK_P) || GetJoyTrigger(Joycon, JOYSTICKID4))
		{
			if (GetFadeState() == FADE_NONE)
			{
				g_bPause = !g_bPause;
				if (g_bPause) {
					//CSound::Pause();
					CSound::SetVolume(BGM_GAME000, 0.03f);
					CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g

					ResetPauseMenu();
				}
				else
				{
					CSound::SetVolume(BGM_GAME000, 0.1f);
					CSound::SetPlayVol(SE_CANCEL, 0.1f); // �L�����Z��
					//CSound::Resume();
				}
			}
		}

		//�ꎞ��~ON/OFF
		//if (GetKeyTrigger(VK_M))
		//{
		//	if (GetFadeState() == FADE_NONE)
		//	{
		//		g_bC_Pause = !g_bC_Pause;
		//		if (g_bC_Pause) {
		//			//CSound::Pause();
		//			CSound::SetVolume(BGM_GAME000, 0.06f);
		//			CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g
		//
		//			ResetPauseMenu();
		//		}
		//		else
		//		{
		//			CSound::SetVolume(BGM_GAME000, 0.1f);
		//			CSound::SetPlayVol(SE_CANCEL, 0.1f); // �L�����Z��
		//			//CSound::Resume();
		//		}
		//	}
		//}

		//�ꎞ��~���j���[�̑I��
		if (g_bPause && GetFadeState() == FADE_NONE)
		{
			//[ENTER]�������ꂽ?
			if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(Joycon, JOYSTICKID1))
			{
				//�I�𒆂̃��j���[���ڂɂ�蕪��
				switch (GetPauseMenu())
				{
				case PAUSE_MENU_CONTINUE:	// �R���e�j���[
					g_bPause = false;
					CSound::SetVolume(BGM_GAME000, 0.1f);
					CSound::SetPlayVol(SE_CANCEL, 0.1f); // �L�����Z��
					//CSound::Resume();
					break;
				case PAUSE_MENU_RETRY:		// ���g���C
					StartFadeOut(GetScene());
					CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g
					break;
				case PAUSE_MENU_QUIT:		// �Q�[�������߂�
					StartFadeOut(SCENE_TITLE);
					CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g
					break;
				}
			}
		}


	}
		//�e�X�g
		if (GetClearPause() && GetFadeState() == FADE_NONE)
		{
			//[ENTER]�������ꂽ?
			if (GetKeyTrigger(VK_RETURN))
			{
				//�I�𒆂̃��j���[���ڂɂ�蕪��
				switch (GetC_PauseMenu())
				{
				case C_PAUSE_MENU_NEXTSTAGE:	// �l�N�X�e�[�W
					StartFadeOut(g_nNowScene + 1);
					SetClearPause(false);
					CSound::SetVolume(BGM_GAME000, 0.1f);
					CSound::SetPlayVol(SE_SELECT, 0.1f); // �L�����Z��
					//CSound::Resume();
					break;
				case C_PAUSE_MENU_SELECT:		// �Z���N�g���
					StartFadeOut(SCENE_SELECT);
					CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g
					break;
				case C_PAUSE_MENU_QUIT:		// �Q�[�������߂�
					StartFadeOut(SCENE_TITLE);
					CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g
					break;
				}
			}
		}
	
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawGame()
{
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	// �w�i�`��
	DrawBG();

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);

	// �񎟌��z��}�b�v�`��
	DrawCField();

	// ���@�`��
	DrawPlayer();

	// �����`��
	DrawExplosion();

	SetZBuffer(false);

	// �G�t�F�N�g�`��
	DrawEffect();

	// �Ə��`��
	DrawReticle();

	SetZBuffer(true);

	//�ꎞ��~�`��
	if (g_bPause) {
		DrawPause();
	}

	//�ꎞ��~�`��
	if (GetClearPause()) {
		DrawC_Pause();
	}

	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	// �f�o�b�O������\��(����)
	SetBlendState(BS_ALPHABLEND);
	SetZBuffer(false);

	// �^�C�}�[�\��
	DrawTimer();

	DrawCountEnemy();

	// ���C�t�\��(����)
	//DrawLife();
	
	SetBlendState(BS_NONE);
}


