//**************************************************************
//
//	SceneGameclear.cpp
//	�Q�[���N���A�V�[��
//
//--------------------------------------------------------------
//	����ҁF������
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/22	�Q�[���N���A�V�[���̎���
//	�ҏW�ҁF������
//				�Q�[���I�[�o�[����^�C�g���ɍs�����K�{�^���̎���
//				�uEnter�v�uSpace�v
//				�R�����g�̕ҏW&�ǉ�
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/28	�f�o�b�O����őJ�ڂł���V�[���̒ǉ�(AREA2,3,BOSS)
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//�@2022/1/26	�R���e�j���[���I�邩�̏�������J�n
//	�ҏW�ҁF�V���ڐl
//
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "SceneGameclear.h"
#include "input.h"
#include "SceneManager.h"
#include "polygon.h"
#include "Fade.h"
#include "Texture.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define PATH_BGTEXTURE "data/texture/Gameclear.png"		// �Q�[���N���A�e�N�X�`��
#define BG_POS_X (0.0f)				// �w�i���WX
#define BG_POS_Y (0.0f)				// �w�i���WY
#define BG_WIDTH (SCREEN_WIDTH)		// �w�i����
#define BG_HEIGHT (SCREEN_HEIGHT)	// �w�i�c��
//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;	// �e�N�X�`���p�ϐ�

int SceneClearFlag=0;//�V�[���N���A�t���O
//**************************************************************
// ����������
//**************************************************************
HRESULT InitGameclear()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if (FAILED(hr))
		return hr;

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitGameclear()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateGameclear()
{
	if (GetFadeState() == FADE_NONE)
	{
		if (GetKeyRelease(VK_1))// || GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE))
		{
			StartFadeOut(SCENE_TITLE);
		}
		else if (GetKeyRelease(VK_2))
		{
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
	//	else if (GetKeyRelease(VK_5))
	//	{
	//		StartFadeOut(SCENE_AREA_DEBUG);
	//	}
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
void DrawGameclear()
{
	// ���ɂȂ�
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);
}
void GameclearFlag()
{
	switch (GetScene())
	{
	case SCENE_GAME:  // �Q�[����ʃG���A1
		SceneClearFlag = 1;
		break;
	case SCENE_AREA2: // �Q�[����ʃG���A2
		SceneClearFlag = 2;
		break;
	case SCENE_AREA3: // �Q�[����ʃG���A3
		SceneClearFlag = 3;
		break;
	case SCENE_AREA4: // �Q�[����ʃG���A4
		SceneClearFlag = 4;
		break;
	case SCENE_AREA5: // �Q�[����ʃG���A5
		SceneClearFlag = 5;
		break;
	case SCENE_AREA6: // �Q�[����ʃG���A6
		SceneClearFlag = 6;
		break;
	case SCENE_AREA7: // �Q�[����ʃG���A7
		SceneClearFlag = 7;
		break;
	case SCENE_AREA8: // �Q�[����ʃG���A8
		SceneClearFlag = 8;
		break;
	case SCENE_AREA9: // �Q�[����ʃG���A9
		SceneClearFlag = 9;
		break;
	case SCENE_AREA10: // �Q�[����ʃG���A10
		SceneClearFlag = 10;
		break;
	}
	StartFadeOut(SCENE_GAMECLEAR);
}
