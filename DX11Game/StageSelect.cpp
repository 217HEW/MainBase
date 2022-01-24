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
//**************************************************************

#include "StageSelect.h"
#include "input.h"
#include "fade.h"
#include "SceneManager.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PATH_BGTEXTURE	L"data/texture/selectBG.PNG"	// �w�i�e�N�X�`��
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

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[12] = { nullptr };	// �e�N�X�`���ւ̃|�C���^
static ID3D11ShaderResourceView*	g_pBGTexture; // �w�i


static SELECT_MENU g_nSelectMenu = STAGE_1;		// �I�𒆂̃��j���[No.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;

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

	for (int nCntStageMenu = 0; nCntStageMenu < NUM_SELECT_MENU; ++nCntStageMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			c_aFileNameStageMenu[nCntStageMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntStageMenu]);			// �ǂݍ��ރ������[
	}

	g_nSelectMenu = STAGE_1;
	g_fCurve = 0.0f;

	// ���ʉ�������
	//g_pSE_Select = CreateSound(SE_SELECT_PATH);

	// �w�i�e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pBGTexture);
	return hr;

}

//=============================================================================
// �I������
//=============================================================================
void UninitSelect(void)
{
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
	if (GetKeyRepeat(VK_UP))
	{
		CSound::Play(SE_SELECT);
		CSound::SetVolume(SE_SELECT, 0.02f);
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + (NUM_SELECT_MENU - 2) - 5) % (NUM_SELECT_MENU - 2));
		SetSelectMenu();
	}
	else if (GetKeyRepeat(VK_DOWN))
	{
		CSound::Play(SE_SELECT);
		CSound::SetVolume(SE_SELECT, 0.02f);
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + 5) % (NUM_SELECT_MENU - 2));
		SetSelectMenu();
	}
	else if (GetKeyRepeat(VK_RIGHT))
	{
		CSound::Play(SE_SELECT);
		CSound::SetVolume(SE_SELECT, 0.02f);
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + 1) % (NUM_SELECT_MENU - 1));
		SetSelectMenu();
	}
	else if (GetKeyRepeat(VK_LEFT))
	{
		CSound::Play(SE_SELECT);
		CSound::SetVolume(SE_SELECT, 0.02f);
		g_nSelectMenu = (SELECT_MENU)((g_nSelectMenu + NUM_SELECT_MENU - 1) % (NUM_SELECT_MENU));
		
		SetSelectMenu();
	}
	

	//g_fCurve += XM_PI * 0.01f;//�s�J�s�J�̌���
	//if (g_fCurve > XM_PI) {
	//	g_fCurve -= XM_2PI;
	//}

	// ���ˌ��̐ݒ�
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;


	//[ENTER]�������ꂽ
	if (GetKeyTrigger(VK_RETURN))
	{
		//�I�𒆂̃��j���[���ڂɂ�蕪��
		switch (GetSelectMenu())
		{
		case STAGE_1:	// �X���[�W1
			StartFadeOut(SCENE_GAME);
			break;
		case STAGE_2:	// �X�e�[�W2
			StartFadeOut(SCENE_AREA2);
			break;
		case STAGE_3:	// �X�e�[�W3
			StartFadeOut(SCENE_AREA3);
			break;
		case STAGE_4:	// �X�e�[�W4
			StartFadeOut(SCENE_AREA4);
			break;
		case STAGE_5:	// �X�e�[�W5
			StartFadeOut(SCENE_AREA5);
			break;
		case STAGE_6:	// �X�e�[�W6
			StartFadeOut(SCENE_AREA6);
			break;
		case STAGE_7:	// �X�e�[�W7
			StartFadeOut(SCENE_AREA7);
			break;
		case STAGE_8:	// �X�e�[�W8
			StartFadeOut(SCENE_AREA8);
			break;
		case STAGE_9:	// �X�e�[�W9
			StartFadeOut(SCENE_AREA9);
			break;
		case STAGE_10:	// �X�e�[�W10
			StartFadeOut(SCENE_AREA10);
			break;
		case SELECT_TITLE:
			StartFadeOut(SCENE_TITLE); // �^�C�g����
			break;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSelect(void)
{
	int nCntStageMenu;

	// �w�i
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pBGTexture);
	SetPolygonUV(0.0f, 0.0f);
	DrawPolygon(pDC);

	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
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
		if(nCntStageMenu <= 4)
		{ 
			SetPolygonPos(SELECT_MENU_POS_X+ nCntStageMenu * SELECT_MENU_INTERVAL,
				SELECT_MENU_POS_Y);
		}
		else if(!nCntStageMenu <= 4)
		{
			
			SetPolygonPos(SELECT_MENU_POS_X + (nCntStageMenu-5) * SELECT_MENU_INTERVAL,
				SELECT_MENU_POS_Y - SELECT_MENU_INTERVAL * 1.5);
		}
		
		if (nCntStageMenu == g_nSelectMenu) {
			//SetPolygonColor(1.0f, 1.0f, 0.1f);
		}
		else {
			//SetPolygonColor(0.3f, 0.3f, 0.3f);
		}

		// �e�N�X�`���̐ݒ�
		// �I�΂�Ă���X�e�[�W�̉摜�̓���ւ�
		if (nCntStageMenu == g_nSelectMenu) {
			SetPolygonTexture(g_pTextures[SELECT]);
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
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetSelectMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
SELECT_MENU GetSelectMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetSelectMenu(void)
{
	g_nSelectMenu = STAGE_1;
	// CSound::Play(SE_SELECT);
	// CSound::SetVolume(SE_SELECT, 0.02f);
	SetSelectMenu();
}
