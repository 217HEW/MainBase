//**************************************************************
//
//	ClearC_Pause.cpp
//	�|�[�Y�@�\
//
//--------------------------------------------------------------
//	����ҁF�V���ڐl
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/01/30	
//	�ҏW�ҁF�V���ڐl
//--------------------------------------------------------------
//	2021/01/30
//														�ύX�ҁF�V���ڐl
//**************************************************************

#include "ClearPause.h"
#include "input.h"
//#include "fade.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"
#include "Fade.h"
#include "SceneManager.h"
#include "StageSelect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_C_PAUSE_MENU		(3)			// �|�[�Y���j���[��
#define	C_PAUSE_MENU_WIDTH	(320.0f)	// �|�[�Y���j���[��
#define	C_PAUSE_MENU_HEIGHT	(60.0f)		// �|�[�Y���j���[����
#define	C_PAUSE_MENU_POS_X	(0.0f)		// �|�[�Y���j���[�ʒu(X���W)
#define	C_PAUSE_MENU_POS_Y	(C_PAUSE_MENU_INTERVAL)	// �|�[�Y���j���[�ʒu(Y���W)
#define	C_PAUSE_MENU_INTERVAL	(100.0f)	// �|�[�Y���j���[�Ԋu
#define	PLATE_WIDTH			(360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(320.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(0.0f)		// �v���[�g�̈ʒu(Y���W)

//�����p
//#define PATH_STEXTURE "data/texture/pause003.png"//�����摜
//#define S_POS_X 200.0f
//#define S_POS_Y 0.0f
//#define S_WIDTH 809
//#define S_HEIGHT 500
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static C_PAUSE_MENU g_nSelectMenu = C_PAUSE_MENU_NEXTSTAGE;		// �I�𒆂̃��j���[No.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;

static LPCWSTR c_aFileNameC_PauseMenu[NUM_C_PAUSE_MENU] =
{
	L"data/texture/NextStage.png",	// �l�N�X�e�[�W
	L"data/texture/select.png",	// �Z���N�g
	L"data/texture/pause002.png",	// �N�C�b�g
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

	g_nSelectMenu = C_PAUSE_MENU_NEXTSTAGE;
	g_fCurve = 0.0f;

	// ���ʉ�������
	//g_pSE_Select = CreateSound(SE_SELECT_PATH);

	// �e�N�X�`���Ǎ�
	//hr = CreateTextureFromFile(pDevice, PATH_STEXTURE, &g_pSTexture);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}
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
	// �e�N�X�`�����
	SAFE_RELEASE(g_pSTexture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateC_Pause(void)
{
	if (GetKeyRepeat(VK_W) || GetKeyRepeat(VK_UP)) 
	{
		CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g��

		g_nSelectMenu = (C_PAUSE_MENU)((g_nSelectMenu + NUM_C_PAUSE_MENU - 1) % NUM_C_PAUSE_MENU);
		SetC_PauseMenu();
	}
	else if (GetKeyRepeat(VK_S) || GetKeyRepeat(VK_DOWN))
	{
		CSound::SetPlayVol(SE_SELECT, 0.1f); // �Z���N�g��

		g_nSelectMenu = (C_PAUSE_MENU)((g_nSelectMenu + 1) % NUM_C_PAUSE_MENU);
		SetC_PauseMenu();
	}

	//if (GetFadeState() == FADE_NONE)
	//{
	//	if (C_PAUSE_MENU_NEXTSTAGE)
	//	{
	//		GetSelectMenu() + 1;
	//
	//	}
	//}

	//g_fCurve += XM_PI * 0.01f;//�s�J�s�J�̌���
	//if (g_fCurve > XM_PI) {
	//	g_fCurve -= XM_2PI;
	//}

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

	//�����p
	//SetPolygonColor(3.0f, 3.0f, 3.0f);//1�ȏ�ɂ��邱�ƂŖ��邳�ύX�\
	//ID3D11DeviceContext* pDC = GetDeviceContext();
	//SetPolygonSize(S_WIDTH, S_HEIGHT);
	//SetPolygonPos(S_POS_X, S_POS_Y);
	//SetPolygonTexture(g_pSTexture);
	//DrawPolygon(pDC);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);
	//SetPolygonColor(1.0f, 1.0f, 1.0f);
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
