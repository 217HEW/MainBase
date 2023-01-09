//**************************************************************
//
//	Pause.cpp
//	�|�[�Y�@�\
//
//--------------------------------------------------------------
//	����ҁF������
//--------------------------------------------------------------
//**************************************************************


#include "Pause.h"
#include "input.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"
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
} PCROSS;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_PAUSE_MENU		(3)			// �|�[�Y���j���[��
#define	PAUSE_MENU_WIDTH	(320.0f)	// �|�[�Y���j���[��
#define	PAUSE_MENU_HEIGHT	(60.0f)		// �|�[�Y���j���[����
#define	PAUSE_MENU_POS_X	(-450.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	PAUSE_MENU_POS_Y	(PAUSE_MENU_INTERVAL)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PAUSE_MENU_INTERVAL	(100.0f)	// �|�[�Y���j���[�Ԋu
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(320.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(-450.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)


#define WAIT_PTIME			(15)			// �I��ҋ@����

//�����p
#define PATH_STEXTURE "data/texture/pause003.png"//�����摜
#define S_POS_X 200.0f
#define S_POS_Y 0.0f
#define S_WIDTH 809
#define S_HEIGHT 500
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static PAUSE_MENU g_nSelectMenu = PAUSE_MENU_CONTINUE;		// �I�𒆂̃��j���[No.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static DWORD	Joystate;	// �ڑ��m�F�p
static DWORD	Joycon;		// �R���g���[���[���

bool g_bpTime;	// �ҋ@�p
int g_bpJoySelect;	// �R���g���[���I��p

// �ҋ@�^�C�}�[�J�E���g�_�E��
int g_npTime;

static LPCWSTR c_aFileNamePauseMenu[NUM_PAUSE_MENU] =
{
	L"data/texture/pause000.png",	// �R���e�B�j���[
	L"data/texture/pause001.png",	// ���g���C
	L"data/texture/pause002.png",	// �N�C�b�g
};

static ID3D11ShaderResourceView* g_pSTexture;//�����p
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPause(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			c_aFileNamePauseMenu[nCntPauseMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntPauseMenu]);			// �ǂݍ��ރ������[
	}
	g_bpTime = false;
	g_npTime = WAIT_PTIME;
	g_bpJoySelect = NOT;	// �����I�����Ă��Ȃ�

	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	g_fCurve = 0.0f;

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_STEXTURE, &g_pSTexture);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;

}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{
	// �e�N�X�`���̊J��
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu)
	{
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
	// �e�N�X�`�����
	SAFE_RELEASE(g_pSTexture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{
	//�Q�[���p�b�h�̏�Ԃ��擾
	XINPUT_STATE state;

	// �R���g���[���[���
	GetJoyState(Joycon);

	// �I��ҋ@����
	if (g_npTime < 0)
	{
		g_bpTime = true;
	}
	g_bpJoySelect = NOT;	// �R���g���[���p�I��������

	// -------�R���g���[���[����------------------------------------------
	GetJoyState(Joycon);
	// �R���g���[���[�̐ڑ��󋵊m�F
	Joystate = XInputGetState(0, &state);

	if (Joystate == ERROR_SUCCESS)
	{	// �ڑ��L�聫
		if (GetJoyDpadUp(Joycon)) { g_bpJoySelect = UP; }
		if (GetJoyDpadDown(Joycon)) { g_bpJoySelect = DOWN; }
		if (GetJoyDpadRight(Joycon)) { g_bpJoySelect = RIGHT; }
		if (GetJoyDpadLeft(Joycon)) { g_bpJoySelect = LEFT; }
	}
	else
	{	// �ڑ�������
		g_bpJoySelect = NOT;
	}

	if (g_bpTime == true) {
		if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP) || (g_bpJoySelect == UP))
		{
			CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g��

			g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
			g_bpTime = false;
			g_npTime = WAIT_PTIME;
		}
		else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN) || (g_bpJoySelect == DOWN))
		{
			CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g��

			g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
			g_bpTime = false;
			g_npTime = WAIT_PTIME;
		}
	}

	// ���ˌ��̐ݒ�
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

	--g_npTime;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
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

	SetPolygonSize(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) 
	{
		SetPolygonPos(PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y - nCntPauseMenu * PAUSE_MENU_INTERVAL);

		if (nCntPauseMenu == g_nSelectMenu) {
			SetPolygonColor(1.0f, 1.0f, 0.1f);
		}
		else {
			SetPolygonColor(0.3f, 0.3f, 0.3f);
		}
		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[nCntPauseMenu]);
		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}

	//�����p
	SetPolygonColor(3.0f, 3.0f, 3.0f);//1�ȏ�ɂ��邱�ƂŖ��邳�ύX�\
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(S_WIDTH, S_HEIGHT);
	SetPolygonPos(S_POS_X, S_POS_Y);
	SetPolygonTexture(g_pSTexture);
	DrawPolygon(pDC);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
	//SetPolygonColor(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetPauseMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
PAUSE_MENU GetPauseMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetPauseMenu(void)
{
	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	SetPauseMenu();
}
