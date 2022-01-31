//**************************************************************
//
//	StageSelect.cpp
//	�X�e�[�W�Z���N�g�@�\
//
//--------------------------------------------------------------
//	����ҁF�Ό����l
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2022/01/21	�v10�X�e�[�W�̃Z���N�g�{�^���̔z�u�ƃV�[���J�ڂ̎���
//	2022/01/23	�^�C�g���ɖ߂�{�^���̐ݒu
//	2022/01/24	�I�𒆂̉摜�̍����ւ��Ƒ傫���̒���
//--------------------------------------------------------------
//	2021/1/24	�R���g���[���[�̐����ǉ��A�I��ҋ@���Ԃ�ǉ�
//				�K�v�Œ���̉���ǉ����܂���
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/1/25	�R���g���[���[�������ꍇ�ɋN����s��𖳂����܂���
//	�ҏW�ҁF�㌎��n
//**************************************************************

#include "StageSelect.h"
#include "input.h"
#include "fade.h"
#include "SceneManager.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"
#include "debugproc.h"
#include "input.h"
#include "number.h"
#include "SceneGameclear.h"

#include <xinput.h>		// �R���g���[���[���擾�ɕK�v

#pragma comment (lib, "xinput.lib")	// �R���g���[���[���擾�ɕK�v

// �R���g���[���\���L�[�p
typedef enum {
	NOT = 0,	// ���������ĂȂ�
	UP,			// ��
	DOWN,		// ��
	RIGHT,		// �E
	LEFT,		// ��

	CROSS_MAX
} CROSS;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PATH_BGTEXTURE	L"data/texture/haikei000.png"	// �w�i�e�N�X�`��
#define BG_POS_X		(0.0f)	// �w�iX���W
#define BG_POS_Y		(0.0f)	// �w�iY���W
#define BG_WIDTH		SCREEN_WIDTH	// �e�N�X�`���̉���
#define BG_HEIGHT		SCREEN_HEIGHT	// �e�N�X�`���̏c��

#define	NUM_SELECT_MENU		(12)			// �Z���N�g���j���[��
#define	SELECT_MENU_WIDTH	(150.0f)		// �Z���N�g���j���[��
#define	SELECT_MENU_HEIGHT	(150.0f)		// �Z���N�g���j���[����
#define	SELECT_MENU_POS_X	(-360.0f)		// �Z���N�g���j���[�ʒu(X���W)
#define	SELECT_MENU_POS_Y	(130.0f)		// �Z���N�g���j���[�ʒu(Y���W)
#define	SELECT_MENU_INTERVAL (180.0f)		// �Z���N�g���j���[�Ԋu

#define	PLATE_WIDTH			(150.0f)		// �v���[�g�̕�
#define	PLATE_HEIGHT		(100.0f)		// �v���[�g�̕�
#define	PLATE_POS_X			(-500.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(300.0f)		// �v���[�g�̈ʒu(Y���W)

#define WAIT_TIME			(15)			// �I��ҋ@����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[12] = { nullptr };	// �e�N�X�`���ւ̃|�C���^
static ID3D11ShaderResourceView*	g_pBGTexture; // �w�i

static DWORD	Joystate;	// �ڑ��m�F�p
static DWORD	Joycon;		// �R���g���[���[���
static SELECT_MENU g_nSelectMenu = STAGE_1;		// �I�𒆂̃��j���[No.
float g_fCurve = 0.0f;
float g_fCol = 0.0f;

int g_StageClear = 0;

bool g_bTime;		// �ҋ@�p
int g_bJoySelect;	// �R���g���[���I��p

// �ҋ@�^�C�}�[�J�E���g�_�E��
int g_nTime;

static LPCWSTR c_aFileNameStageMenu[NUM_SELECT_MENU] =
{
	L"data/texture/hero002.jpg",// hero001 �� �I��
	L"data/texture/hero002.jpg",// hero002 �� �I���ł��Ȃ�
	L"data/texture/hero002.jpg",// hero003 �� �I���ł��邪�I�����Ă��Ȃ�
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/hero002.jpg",
	L"data/texture/STitle.png",// �^�C�g���ɖ߂�
	L"data/texture/hero001.jpg",
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSelect(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;
	InitNumber();

	for (int nCntStageMenu = 0; nCntStageMenu < NUM_SELECT_MENU; ++nCntStageMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			c_aFileNameStageMenu[nCntStageMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntStageMenu]);			// �ǂݍ��ރ������[
	}

	g_nSelectMenu = STAGE_1;
	g_fCurve = 0.0f;
	g_bTime = false;
	g_nTime = WAIT_TIME;
	g_bJoySelect = NOT;	// �����I�����Ă��Ȃ�

	CSound::SetPlayVol(BGM_SELECT, 0.1f); // �Z���N�g�V�[��BGM

	// �w�i�e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pBGTexture);
	return hr;

}

//=============================================================================
// �I������
//=============================================================================
void UninitSelect(void)
{
	CSound::Stop(BGM_SELECT);
	UninitNumber();

	// �e�N�X�`���̊J��
	for (int nCntStageMenu = 0; nCntStageMenu < NUM_SELECT_MENU; ++nCntStageMenu)
	{
		SAFE_RELEASE(g_pTextures[nCntStageMenu]);
	}
	// �w�i�e�N�X�`�����
	SAFE_RELEASE(g_pBGTexture);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSelect(void)
{
	//�Q�[���p�b�h�̏�Ԃ��擾
	XINPUT_STATE state;

	// �R���g���[���[���
	GetJoyState(Joycon);

	// �I��ҋ@����
	if (g_nTime < 0)
	{
		g_bTime = true;
	}
	g_bJoySelect = NOT;	// �R���g���[���p�I��������

	// -------�R���g���[���[����------------------------------------------
	GetJoyState(Joycon);
	// �R���g���[���[�̐ڑ��󋵊m�F
	Joystate = XInputGetState(0, &state);

	if (Joystate == ERROR_SUCCESS)
	{	// �ڑ��L�聫
		if (GetJoyDpadUp(Joycon)) { g_bJoySelect = UP; }
		if (GetJoyDpadDown(Joycon)) { g_bJoySelect = DOWN; }
		if (GetJoyDpadRight(Joycon)) { g_bJoySelect = RIGHT; }
		if (GetJoyDpadLeft(Joycon)) { g_bJoySelect = LEFT; }
	}
	else
	{	// �ڑ�������
		g_bJoySelect = NOT;
	}

	if (g_StageClear < Getclear())
		g_StageClear = Getclear();

	if (g_bTime == true) {
		if (GetKeyRepeat(VK_UP) || (g_bJoySelect == UP))
		{
			// �J�[�\����
			CSound::SetPlayVol(SE_SELECT, 0.3f);
			g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + (NUM_SELECT_MENU - 2) - 5) % (NUM_SELECT_MENU - 2));
			SetSelectMenu();
			g_bTime = false;
			g_nTime = WAIT_TIME;
		}
		else if (GetKeyRepeat(VK_DOWN) || (g_bJoySelect == DOWN))
		{
			// �J�[�\����
			CSound::SetPlayVol(SE_SELECT, 0.3f);
			g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + 5) % (NUM_SELECT_MENU - 2));
			SetSelectMenu();
			g_bTime = false;
			g_nTime = WAIT_TIME;
		}
		else if (GetKeyRepeat(VK_RIGHT) || (g_bJoySelect == RIGHT))
		{
			// �J�[�\����
			CSound::SetPlayVol(SE_SELECT, 0.3f);
			g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + 1) % (NUM_SELECT_MENU - 2));
			SetSelectMenu();
			g_bTime = false;
			g_nTime = WAIT_TIME;
		}
		else if (GetKeyRepeat(VK_LEFT) || (g_bJoySelect == LEFT))
		{
			if (g_nSelectMenu == STAGE_1)
			{
				g_nSelectMenu = SELECT;
			}
			// �J�[�\����
			CSound::SetPlayVol(SE_SELECT, 0.3f);
			g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + NUM_SELECT_MENU - 1) % (NUM_SELECT_MENU));
			SetSelectMenu();
			g_bTime = false;
			g_nTime = WAIT_TIME;
		}

		// ���ˌ��̐ݒ�
		g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

		//[ENTER]�������ꂽ
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(Joycon, JOYSTICKID1))
		{
			if (g_StageClear >= GetSelectMenu())
			{
				CSound::SetPlayVol(SE_DECISION, 0.7f);
				g_bTime = false;
				g_nTime = WAIT_TIME * WAIT_TIME;
			}
			else
			{
				CSound::SetPlayVol(SE_CANCEL, 0.4f);
			}

			//�I�𒆂̃��j���[���ڂɂ�蕪��
			switch (GetSelectMenu())
			{
			case STAGE_1:	// �X���[�W1
				StartFadeOut(SCENE_GAME);
				break;
			case STAGE_2:	// �X�e�[�W2
				if (g_StageClear >= 1)
					StartFadeOut(SCENE_AREA2);
				break;
			case STAGE_3:	// �X�e�[�W3
				if (g_StageClear >= 2)
					StartFadeOut(SCENE_AREA3);
				break;
			case STAGE_4:	// �X�e�[�W4
				if (g_StageClear >= 3)
					StartFadeOut(SCENE_AREA4);
				break;
			case STAGE_5:	// �X�e�[�W5
				if (g_StageClear >= 4)
					StartFadeOut(SCENE_AREA5);
				break;
			case STAGE_6:	// �X�e�[�W6
				if (g_StageClear >= 5)
					StartFadeOut(SCENE_AREA6);
				break;
			case STAGE_7:	// �X�e�[�W7
				if (g_StageClear >= 6)
					StartFadeOut(SCENE_AREA7);
				break;
			case STAGE_8:	// �X�e�[�W8
				if (g_StageClear >= 7)
					StartFadeOut(SCENE_AREA8);
				break;
			case STAGE_9:	// �X�e�[�W9
				if (g_StageClear >= 8)
					StartFadeOut(SCENE_AREA9);
				break;
			case STAGE_10:	// �X�e�[�W10
				if (g_StageClear >= 9)
					StartFadeOut(SCENE_AREA10);
				break;
			case SELECT_TITLE:
				StartFadeOut(SCENE_TITLE); // �^�C�g����
				break;
			}
		}
	}
	--g_nTime;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSelect(void)
{
	int nCntStageMenu;
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	// �w�i
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pBGTexture);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �v���[�g
	/*SetPolygonTexture(nullptr);
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(g_fCol, g_fCol, g_fCol);
	SetPolygonAlpha(1.0f);
	DrawPolygon(pDeviceContext);*/

	// �ð��1�`10
	for (nCntStageMenu = 0; nCntStageMenu < NUM_SELECT_MENU - 2; ++nCntStageMenu)
	{
		//�ð��5�܂ł���������s
		if (nCntStageMenu <= 4)
		{
			SetPolygonPos(SELECT_MENU_POS_X + nCntStageMenu * SELECT_MENU_INTERVAL,
				SELECT_MENU_POS_Y);
		}
		else if (!nCntStageMenu <= 4)
		{
			SetPolygonPos(SELECT_MENU_POS_X + (nCntStageMenu - 5) * SELECT_MENU_INTERVAL,
				SELECT_MENU_POS_Y - SELECT_MENU_INTERVAL * 1.5);
		}

		// �e�N�X�`���̐ݒ�
		// �I�΂�Ă���X�e�[�W�̉摜�̓���ւ�
		if (nCntStageMenu == g_nSelectMenu) {
			if (g_StageClear >= GetSelectMenu())
				SetPolygonTexture(g_pTextures[SELECT]);
			else
				SetPolygonTexture(g_pTextures[nCntStageMenu]);

			SetPolygonSize(SELECT_MENU_WIDTH + 25, SELECT_MENU_HEIGHT + 25);
		}
		else
		{
			SetPolygonTexture(g_pTextures[nCntStageMenu]);
			SetPolygonSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
		}

		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}
	//����
	SetPolygonColor(10.0f, 10.0f, 0.0f);
	for (int i = 1; i < 6; i++)
	{
		DrawNumber(XMFLOAT2(-585 + (180 * i), 175), i, 2, 50.0f, 100.0f);

	}
	for (int i = 6; i < 11; i++)
	{
		DrawNumber(XMFLOAT2(-585 + (180 * (i - 5)), -95), i, 2, 50.0f, 100.0f);

	}
	// �^�C�g��
	SetPolygonSize(SELECT_MENU_WIDTH / 2, SELECT_MENU_HEIGHT);
	SetPolygonPos(-500.0f, 0.0f);
	if (g_nSelectMenu == SELECT_TITLE) {
		SetPolygonColor(1.0f, 1.0f, 0.1f);
	}
	else {
		SetPolygonColor(0.3f, 0.3f, 0.3f);
	}
	SetPolygonTexture(g_pTextures[SELECT_TITLE]);
	DrawPolygon(pDeviceContext);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �Z���N�g���j���[�̐ݒ�
//=============================================================================
void SetSelectMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �Z���N�g���j���[�̎擾
//=============================================================================
SELECT_MENU GetSelectMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// �X�e�[�W�Z���N�g�̃��Z�b�g
//=============================================================================
void ResetSelectMenu(void)
{
	g_nSelectMenu = STAGE_1;
	SetSelectMenu();
}
