//**************************************************************
//
//	SceneGameover.cpp
//	�Q�[���I�[�o�[�V�[��
//
//--------------------------------------------------------------
//	����ҁF������
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/06	������N�̃v���O���������ɍ쐬
//--------------------------------------------------------------
//	2021/12/21	GetDevice�֐��i�[�p�|�C���^�ϐ����쐬���A�K���̕ύX
//				�t�F�[�h���ɕʂ̃t�F�[�h���������Ȃ��悤�␳
//				�|���S��4�又������(�`�揈���ȊO)
//	�ύX�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/22	�R�����g�̒ǉ�&�ҏW
//				�t�F�[�h�C���̋���������������������Draw�֐���
//				Z�o�b�t�@�̏�����ǉ�
//				�Q�[���I�[�o�[����^�C�g���ɍs�����K�{�^���̎���
//				�uEnter�v�uSpace�v
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/28	�f�o�b�O����őJ�ڂł���V�[���̒ǉ�(AREA2,3,BOSS)
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2022/1/5	�Q�[���I�[�o�[�W���O����ǉ����܂����B
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "SceneGameover.h"
#include "input.h"
#include "SceneManager.h"
#include "polygon.h"
#include "Fade.h"
#include "Texture.h"
#include "Sound.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define	NUM_GAMEOVER		 (3)		// �|�[�Y���j���[��
#define	GAMEOVER_MENU_WIDTH	 (320.0f)	// �|�[�Y���j���[��
#define	GAMEOVER_MENU_HEIGHT (60.0f)	// �|�[�Y���j���[����
#define	GAMEOVER_INTERVAL	 (100.0f)	// �|�[�Y���j���[�Ԋu
#define	GAMEOVER_MENU_POS_X	 (-450.0f)	// �|�[�Y���j���[�ʒu(X���W)
#define	GAMEOVER_MENU_POS_Y	 (GAMEOVER_INTERVAL)	// �|�[�Y���j���[�ʒu(Y���W)
#define	PLATE_WIDTH			 (360.0f)	// �v���[�g�̕�
#define	PLATE_HEIGHT		 (320.0f)	// �v���[�g�̕�
#define	PLATE_POS_X			 (-450.0f)	// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			 (0.0f)		// �v���[�g�̈ʒu(Y���W)

#define PATH_BGTEXTURE "data/texture/Gameover.png"	//�Q�[���I�[�o�[�e�N�X�`��
#define BG_POS_X (0.0f)	// �V�[���w�i�̉������W
#define BG_POS_Y (0.0f)	// �V�[���w�i�̉������W
#define BG_WIDTH SCREEN_WIDTH	// �e�N�X�`���̉���
#define BG_HEIGHT SCREEN_HEIGHT	// �e�N�X�`���̏c��
//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;	// �e�N�X�`���p�ϐ�

static LPCWSTR c_aFileNameGameover[NUM_GAMEOVER] =
{
	L"data/texture/pause000.png",	// ROGO
	L"data/texture/pause001.png",	// ���g���C
	L"data/texture/pause002.png",	// �N�C�b�g
};
//**************************************************************
// ����������
//**************************************************************
HRESULT InitGameover()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if (FAILED(hr))
		return hr;

	CSound::SetPlayVol(SE_GAMEOVER, 0.1f);

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitGameover()
{
	CSound::Stop(SE_GAMEOVER);
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateGameover()
{
	// �t�F�[�h�������Ă��Ȃ�������
	if (GetFadeState() == FADE_NONE)
	{
		if (GetKeyRelease(VK_1) || GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE))
		{
			// �^�C�g����
			StartFadeOut(SCENE_TITLE);
		}
		else if (GetKeyRelease(VK_2))
		{
			// �Q�[���V�[����
			StartFadeOut(SCENE_GAME);
		}
		else if (GetKeyRelease(VK_3))
		{
			StartFadeOut(SCENE_AREA2);
		}
		else if (GetKeyRelease(VK_4))
		{
			StartFadeOut(SCENE_AREA3);
		}
		else if (GetKeyRelease(VK_5))
		{
			StartFadeOut(SCENE_AREA_DEBUG);
		}
		else if (GetKeyRelease(VK_6))
		{
			StartFadeOut(SCENE_GAMEOVER);
		}
		else if (GetKeyRelease(VK_7))
		{
			StartFadeOut(SCENE_GAMECLEAR);
		}
	}

	// �|���S���\���X�V
	//UpdatePolygon();
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawGameover()
{
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);
	
}