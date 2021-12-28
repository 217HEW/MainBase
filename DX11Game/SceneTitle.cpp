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
#define PATH_BGTEXTURE "data/texture/Title.png"
#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;

//**************************************************************
// ����������
//**************************************************************
HRESULT InitTitle()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if (FAILED(hr))
		return hr;

	// �^�C�g��BGM�Đ�
	CSound::Play(BGM_TITLE);

	// ���ʂ��傫���̂ŏ���������
	CSound::SetVolume(BGM_TITLE, 0.01f);	// �������������Ɖ������(����1.0f���ő�ł�)

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitTitle()
{
	// �^�C�g��BGM�I��
	CSound::Stop(BGM_TITLE);

	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateTitle()
{
	// �L�[���͂ŃV�[���J��
	if (GetFadeState() == FADE_NONE)
	{
		if (GetKeyRelease(VK_1))
		{
			StartFadeOut(SCENE_TITLE);
		}
		else if (GetKeyRelease(VK_2) || GetKeyRelease(VK_SPACE) || GetKeyRelease(VK_RETURN))
		{
			// ���艹
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);	// �܂����ʂ̑S�̃{�����[����������������킩���Ė����̂Ŏ�ł�

			StartFadeOut(SCENE_GAME);
		}
		else if (GetKeyRelease(VK_3))
		{
			// ���艹
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA2);
		}
		else if (GetKeyRelease(VK_4))
		{
			// ���艹
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_AREA3);
		}
		else if (GetKeyRelease(VK_5))
		{
			// ���艹
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);
			
			StartFadeOut(SCENE_AREA_BOSS);
		}
		else if (GetKeyRelease(VK_6))
		{
			// ���艹
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_GAMEOVER);
		}
		else if (GetKeyRelease(VK_7))
		{
			// ���艹
			CSound::Play(SE_DECISION);
			CSound::SetVolume(SE_DECISION, 0.03f);

			StartFadeOut(SCENE_GAMECLEAR);
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

	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
}