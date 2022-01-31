//**************************************************************
//
//	Fade.cpp
//	�t�F�[�h����
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/11/30	�t�F�[�h��������J�n
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/03	�t�F�[�h����x�[�X��������
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/06	�ĎR꣑��Y�N�̃v���O���������ɕύX
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/19	�ĎR꣑��Y�N�̃v���O���������ɕύX
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/21	g_fFadeRate�ϐ��̍폜
//				�O���[�o���ϐ��������ʒu���u�O���[�o���̈恨�������֐����v�ɕύX
//				Draw�֐�����SetBlendState�֐��̒ǉ�
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/22	�R�����g�̕ҏW&�ǉ�
//--------------------------------------------------------------
//	2022/1/27	�����ȉ摜�ɕύX���܂���
//	�ҏW�ҁF�㌎��n
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "Fade.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define FADE_RATE 0.02f // �t�F�[�h�C��/�A�E�g�̍���
#define FADE_TEX	
#define FADE_TEX_2	

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static float g_fRed;		// ��:0.0f~255.0f�g�F
static float g_fGreen;		// ��:0.0f~255.0f
static float g_fBlue;		// ��:0.0f~255.0f
static float g_fAlpha;		// ���l:0.0f~1.0f

// �摜�t�@�C���l�[��
static LPCWSTR g_pszTexFName[FADE_TEX_MAX] = {
	L"data/texture/Loading/NowLoading.png",
	L"data/texture/Loading/Logo.png",
};

static ID3D11ShaderResourceView* g_pTexture[FADE_TEX_MAX];
static E_FADE_STATE g_eState;	// �t�F�[�h���
static int g_eNext;			// ���̃V�[��

//**************************************************************
// ����������
//**************************************************************
HRESULT InitFade()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	for (int i = 0; i < FADE_TEX_MAX; ++i)
	{
		hr = CreateTextureFromFile(pDevice, g_pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	// �ϐ��̏�����
	g_fRed = 1.0f;
	g_fGreen = 1.0f;
	g_fBlue = 1.0f;
	g_fAlpha = 1.0f;
	g_eState = FADE_IN;
	g_eNext = SCENE_TITLE;

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitFade()
{
	// �����̃e�N�X�`�����
	for (int i = 0; i < FADE_TEX_MAX; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}

	// �I�����ɏ�����
	g_fAlpha = 0.0f;
	g_eState = FADE_NONE;
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateFade()
{
	// �t�F�[�h�C���E�A�E�g
	switch (g_eState)
	{
	case FADE_NONE:
		break;

	case FADE_OUT:
		// �t�F�[�h�A�E�g---------------------------------------
		g_fAlpha += FADE_RATE; // �s�����x�𑝂�
		if (g_fAlpha >= 1.0f)
		{
			// �t�F�[�h�C�������ɐ؂�ւ�
			g_fAlpha = 1.0f;
			g_eState = FADE_IN;

			// �V�[���ؑ�
			SetScene(g_eNext);
		}
		// �{�����[�����t�F�[�h�A�E�g
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
		// -----------------------------------------------------

	case FADE_IN:
		// �t�F�[�h�C��-----------------------------------------
		g_fAlpha -= FADE_RATE; // �����x�𑝂�
		if (g_fAlpha <= 0.0f)
		{
			// �t�F�[�h�C���I��
			g_fAlpha = 0.0f;
			g_eState = FADE_NONE;
		}
		// �{�����[�����t�F�[�h�C��
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
		// -----------------------------------------------------
	}
	if (g_fRed <= 0.0f)
	{
		g_fRed = g_fGreen = g_fBlue += FADE_RATE;
	}
	else
	{
		g_fRed = g_fGreen = g_fBlue -= FADE_RATE;
	}

	if (g_eNext == SCENE_GAMECLEAR || g_eNext == SCENE_GAMEOVER)
	{
		g_fRed = g_fGreen = g_fBlue = 0.0f;
	}
	else
	{
		g_fRed = g_fGreen = g_fBlue = 1.0f;
	}
}

//**************************************************************
// �I������
//**************************************************************
void DrawFade()
{
	// ��ʑS�̂ɔ������̋�`��`��
	SetBlendState(BS_ALPHABLEND);
	 ID3D11DeviceContext* pDC = GetDeviceContext();
	 SetPolygonPos(0.0f, 0.0f);		// ���W�ݒ�
	 SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);	// �z���T�C�Y�ݒ�
	 SetPolygonUV(0.0f, 0.0f);		// �e�N�X�`�����W�ݒ�
	 SetPolygonFrameSize(1.0f, 1.0f);	// �e�N�X�`���T�C�Y�ݒ�
	 if(g_eNext == SCENE_TITLE)		// �e�N�X�`�����ݒ�
	 { SetPolygonTexture(g_pTexture[LOGO]);} 
	 else {SetPolygonTexture(g_pTexture[LOAD]);}
	 SetPolygonColor(g_fRed, g_fGreen, g_fBlue);// �F���ݒ�
	 SetPolygonAlpha(g_fAlpha);		// �����x�ݒ�
	 DrawPolygon(pDC);
	 
	  // ���ɖ߂�
	 SetBlendState(BS_NONE);// ����(�A���t�@�l)��������Ƃ��̓Z�b�g�u�����h�ł͂���
	 SetPolygonColor(1.0f, 1.0f, 1.0f);
	 SetPolygonAlpha(1.0f);

}

//*******************************
//
//	�t�F�[�h�A�E�g����
//	
//	����:
//		���̃V�[���ԍ�
//
//	�߂�l
//		����
//
//*******************************
void StartFadeOut(int eNext)
{
	// �t�F�[�h�A�E�g���Ă��Ȃ���Ώ���
	if (g_eState != FADE_OUT)
	{
		g_eState = FADE_OUT;
		g_fAlpha = 0.0f;
		g_eNext = eNext;
	}
}

//*******************************
//
//	�t�F�[�h��Ԏ擾
//	
//	����:
//		����
//
//	�߂�l
//		�t�F�[�h�̏��
//		1:�C��
//		2:�A�E�g
//
//*******************************
E_FADE_STATE GetFadeState()
{
	return g_eState;
}
