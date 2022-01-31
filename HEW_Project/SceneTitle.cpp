//**************************************************************
//
//	SceneTitle.cpp
//	�^�C�g���V�[�����g
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/06	������N�̃v���O���������ɍ쐬	
//--------------------------------------------------------------
//	2021/12/21	GetDevice�֐��i�[�p�|�C���^�ϐ����쐬���A�K���̕ύX
//				�t�F�[�h���ɕʂ̃t�F�[�h���������Ȃ��悤�␳
//				�|���S��4�又������
//	�ύX�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/21	�t�F�[�h�������o����悤��Draw������Z�o�b�t�@��
//				�ǉ����܂����B97�`106�s
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/22	�^�C�g������Q�[���ɍs�����K�{�^���̎���
//				�uEnter�v�uSpace�v
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/27	�^�C�g��BGM��ǉ����܂����B���ʂ��ƂĂ��傫���̂�
//				�蓮�ŉ����Ă��܂��B
//				�Z���N�g���������Ă��܂�
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/28	�f�o�b�O����őJ�ڂł���V�[���̒ǉ�(AREA2,3,BOSS)
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2022/1/5	�R���g���[���[�ł��V�[���J�ڏo����悤�ɂ��܂���
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2022/1/22	�uOO�������ĉ������I�v��ǉ��ł���悤�ɂ��܂���
//				�摜�����Ε`��o���܂�
//	�ҏW�ҁF�㌎��n
//
//	2022/01/24	�^�C�g������Z���N�g��ʂɍs���悤�ɂ��܂���
//	�ҏW��: �Ό����l
//--------------------------------------------------------------
//	2022/1/27	�^�C�g���𐳎��ȉ摜�ɕύX���܂���
//	�ҏW�ҁF�㌎��n
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "SceneTitle.h"
#include "input.h"
#include "SceneManager.h"
#include "polygon.h"
#include "Fade.h"
#include "Texture.h"
#include "Sound.h"

//**************************************************************
// �}�N����`
//**************************************************************
// �w�i
#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT

// �^�C�g����
#define TITLE_POS_X 0.0f
#define TITLE_POS_Y 70.0f
#define TITLE_WIDTH 1040
#define TITLE_HEIGHT 560

// ����A�C�R��
#define ENTER_WIDTH	 550
#define ENTER_HEIGHT 80
#define ENTER_POS_X	 0.0f
#define ENTER_POS_Y	 (SCREEN_HEIGHT-ENTER_HEIGHT) * -0.5f

#define BLINK_TIMER 60
#define BLINK_START_TIMER 5

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************

// �摜�t�@�C���l�[��
static LPCWSTR g_pszTexFName[TITLE_TEX_MAX] = {
 L"data/texture/Title/TitleBG000.png",	// �^�C�g���w�i�e�N�X�`��
 L"data/texture/Title/TitleLogo.png",		// �^�C�g�����e�N�X�`��
 L"data/texture/Title/TitlePush.png",			// ����A�C�R���e�N�X�`��
};

static ID3D11ShaderResourceView* g_pTexture[TITLE_TEX_MAX];
static DWORD	Joycon;		// �R���g���[���[���

// �_�Ń^�C�}�[�p
static int g_nBlink;
static bool g_bStart;

//**************************************************************
// ����������
//**************************************************************
HRESULT InitTitle()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �����̃e�N�X�`���Ǎ�
	for (int i = 0; i < TITLE_TEX_MAX; ++i)
	{
		hr = CreateTextureFromFile(pDevice, g_pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	// �ϐ�������
	g_nBlink = BLINK_TIMER;
	g_bStart = false;

	// �^�C�g��BGM�Đ�
	CSound::SetPlayVol(BGM_TITLE, 0.1f);

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitTitle()
{
	// �^�C�g��BGM�I��
	CSound::Stop(BGM_TITLE);

	// �ϐ�������
	g_nBlink = BLINK_TIMER;
	g_bStart = false;

	// �����̃e�N�X�`�����
	for (int i = 0; i < TITLE_TEX_MAX; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateTitle()
{
	// �R���g���[���[���
	GetJoyState(Joycon);

	// �_�Ń^�C�}�[�X�V
	--g_nBlink;
	if (g_nBlink <= 0)
	{
		g_nBlink = (g_bStart) ? BLINK_START_TIMER : BLINK_TIMER;
	}

	// �L�[���͂ŃV�[���J��
	if (GetFadeState() == FADE_NONE)
	{
		if (GetKeyRelease(VK_T))
		{
			StartFadeOut(SCENE_TITLE);
		}
		else if (GetKeyRelease(VK_1) || GetKeyRelease(VK_SPACE) || GetKeyRelease(VK_RETURN)|| GetJoyTrigger(Joycon, JOYSTICKID1))
		{
			g_bStart = true;
			g_nBlink = BLINK_START_TIMER;

			// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			StartFadeOut(SCENE_SELECT);
		}
		else if (GetKeyRelease(VK_2))
		{
			// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			StartFadeOut(SCENE_AREA2);
		}
		else if (GetKeyRelease(VK_3))
		{
			// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			StartFadeOut(SCENE_AREA3);
		}
		else if (GetKeyRelease(VK_4))
		{
			// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			StartFadeOut(SCENE_AREA4);
		}
		else if (GetKeyRelease(VK_5))
		{
			// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			StartFadeOut(SCENE_AREA5);
		}
		else if (GetKeyRelease(VK_6))
		{
			// ���艹
				// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			StartFadeOut(SCENE_AREA6);
		}
		else if (GetKeyRelease(VK_7))
		{
			// ���艹
				// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			StartFadeOut(SCENE_AREA7);
		}
		else if (GetKeyRelease(VK_8))
		{
			// ���艹
				// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			StartFadeOut(SCENE_AREA8);
		}
		else if (GetKeyRelease(VK_9))
		{
			// ���艹
				// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			StartFadeOut(SCENE_AREA9);
		}
		else if (GetKeyRelease(VK_0))
		{
			// ���艹
				// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			StartFadeOut(SCENE_AREA10);
		}
		else if (GetKeyRelease(VK_D))
		{
			// ���艹
				// ���艹
			CSound::SetPlayVol(SE_DECISION, 1.0f);
			//StartFadeOut(SCENE_AREA_DEBUG);
		}
	}
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawTitle()
{
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);

	// �^�C�g���w�i�`��
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture[TITLE_TEX_BG]);
	DrawPolygon(pDC);
	
	// �^�C�g�����`��
	SetPolygonSize(TITLE_WIDTH, TITLE_HEIGHT);
	SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
	SetPolygonTexture(g_pTexture[TITLE_TEX_NAME]);
	DrawPolygon(pDC);

	// �_�Ń^�C�}�[�p
	if (g_bStart)
	{
		if (g_nBlink < BLINK_START_TIMER / 2)
			return;
	}
	else
	{
		if (g_nBlink < BLINK_TIMER / 2)
			return;
	}

	// ����A�C�R���`��
	SetPolygonSize(ENTER_WIDTH, ENTER_HEIGHT);
	SetPolygonPos(ENTER_POS_X, ENTER_POS_Y);
	SetPolygonTexture(g_pTexture[TITLE_TEX_ENTER]);
	DrawPolygon(pDC);
	
	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
	SetBlendState(BS_NONE);	// �A���t�@�u�����h����

}