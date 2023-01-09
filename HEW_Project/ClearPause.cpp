//**************************************************************
//
//	ClearC_Pause.cpp
//	�|�[�Y�@�\
//
//--------------------------------------------------------------
//	����ҁF�V���ڐl
//--------------------------------------------------------------
//**************************************************************

#include "ClearPause.h"
#include "input.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"
#include "Fade.h"
#include "SceneManager.h"
#include "StageSelect.h"
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
} C_PCROSS;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_C_PAUSE_MENU		(3)			// �|�[�Y���j���[��
#define	C_PAUSE_MENU_WIDTH	(320.0f)	// �|�[�Y���j���[��
#define	C_PAUSE_MENU_HEIGHT	(60.0f)		// �|�[�Y���j���[����
#define	C_PAUSE_MENU_POS_X	(0.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	C_PAUSE_MENU_INTERVAL	(100.0f)	// �|�[�Y���j���[�Ԋu
#define	C_PAUSE_MENU_POS_Y	(C_PAUSE_MENU_INTERVAL)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(320.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)

#define WAIT_CPTIME			(15)			// �I��ҋ@����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static C_PAUSE_MENU g_nSelectMenu = C_PAUSE_MENU_NEXTSTAGE;		// �I�𒆂̃��j���[No.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static DWORD	Joystate;	// �ڑ��m�F�p
static DWORD	Joycon;		// �R���g���[���[���

bool g_bcPTime;	// �ҋ@�p
int g_bcPJoySelect;	// �R���g���[���I��p

// �ҋ@�^�C�}�[�J�E���g�_�E��
int g_ncPTime;

bool g_SetClearPause;


static LPCWSTR c_aFileNameC_PauseMenu[NUM_C_PAUSE_MENU] =
{
	L"data/texture/ClearNextStage.png",	// �l�N�X�e�[�W
	L"data/texture/ClearSelect.png",	// �Z���N�g
	L"data/texture/ClearQuit.png",	// �N�C�b�g
};

static ID3D11ShaderResourceView* g_pSTexture;//�����p
//=============================================================================
// ����������
//=============================================================================
HRESULT InitC_Pause(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntC_PauseMenu = 0; nCntC_PauseMenu < NUM_C_PAUSE_MENU; ++nCntC_PauseMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			c_aFileNameC_PauseMenu[nCntC_PauseMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntC_PauseMenu]);			// �ǂݍ��ރ������[
	}
	g_bcPTime = false;
	g_ncPTime = WAIT_CPTIME;
	g_nSelectMenu = C_PAUSE_MENU_NEXTSTAGE;
	g_fCurve = 0.0f;

	g_SetClearPause = false;
	return hr;

}

//=============================================================================
// �I������
//=============================================================================
void UninitC_Pause(void)
{
	// �e�N�X�`���̊J��
	for (int nCntC_PauseMenu = 0; nCntC_PauseMenu < NUM_C_PAUSE_MENU; ++nCntC_PauseMenu)
	{
		SAFE_RELEASE(g_pTextures[nCntC_PauseMenu]);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateC_Pause(void)
{
	//�Q�[���p�b�h�̏�Ԃ��擾
	XINPUT_STATE state;

	// �R���g���[���[���
	GetJoyState(Joycon);

	// �I��ҋ@����
	if (g_ncPTime < 0)
	{
		g_bcPTime = true;
	}
	g_bcPJoySelect = NOT;	// �R���g���[���p�I��������
	// -------�R���g���[���[����------------------------------------------
	GetJoyState(Joycon);
	// �R���g���[���[�̐ڑ��󋵊m�F
	Joystate = XInputGetState(0, &state);

	if (Joystate == ERROR_SUCCESS)
	{	// �ڑ��L�聫
		if (GetJoyDpadUp(Joycon)) {		g_bcPJoySelect = UP; }
		if (GetJoyDpadDown(Joycon)) {	g_bcPJoySelect = DOWN; }
		if (GetJoyDpadRight(Joycon)) {	g_bcPJoySelect = RIGHT; }
		if (GetJoyDpadLeft(Joycon)) {	g_bcPJoySelect = LEFT; }
	}
	else
	{	// �ڑ�������
		g_bcPJoySelect = NOT;
	}

	if (g_bcPTime == true) {
		if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP) || (g_bcPJoySelect == UP))
		{
			CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g��

			g_nSelectMenu = (C_PAUSE_MENU)((g_nSelectMenu + NUM_C_PAUSE_MENU - 1) % NUM_C_PAUSE_MENU);
			SetC_PauseMenu();
			g_bcPTime = false;
			g_ncPTime = WAIT_CPTIME;
		}
		else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN) || (g_bcPJoySelect == DOWN))
		{
			CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g��

			g_nSelectMenu = (C_PAUSE_MENU)((g_nSelectMenu + 1) % NUM_C_PAUSE_MENU);
			SetC_PauseMenu();
			g_bcPTime = false;
			g_ncPTime = WAIT_CPTIME;
		}
	}
	--g_ncPTime;
	// ���ˌ��̐ݒ�
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawC_Pause(void)
{
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetPolygonTexture(nullptr);
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(g_fCol, g_fCol, g_fCol);
	SetPolygonAlpha(1.0f);
	DrawPolygon(pDeviceContext);

	SetPolygonSize(C_PAUSE_MENU_WIDTH, C_PAUSE_MENU_HEIGHT);
	for (int nCntC_PauseMenu = 0; nCntC_PauseMenu < NUM_C_PAUSE_MENU; ++nCntC_PauseMenu) 
	{

		SetPolygonPos(C_PAUSE_MENU_POS_X, C_PAUSE_MENU_POS_Y - nCntC_PauseMenu * C_PAUSE_MENU_INTERVAL);

		if (nCntC_PauseMenu == g_nSelectMenu) {
			SetPolygonColor(1.0f, 1.0f, 0.1f);
		}
		else {
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[nCntC_PauseMenu]);
		// �|���S���̕`��
		DrawPolygon(pDeviceContext);

	}

	

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}

//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetC_PauseMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
C_PAUSE_MENU GetC_PauseMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetC_PauseMenu(void)
{
	g_nSelectMenu = C_PAUSE_MENU_NEXTSTAGE;
	// CSound::Play(SE_SELECT);
	// CSound::SetVolume(SE_SELECT, 0.02f);
	SetC_PauseMenu();
}

bool GetClearPause()
{
	return g_SetClearPause;
}

void SetClearPause(bool Setpause)
{
	g_SetClearPause = Setpause;
}
