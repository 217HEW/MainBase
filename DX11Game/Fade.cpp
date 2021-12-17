//**************************************************************
//
//	Fade.cpp
//	�t�F�[�h����
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//	�J������
//	2021/11/30	�t�F�[�h��������J�n		�F�ĎR꣑��Y
//	2021/12/03	�t�F�[�h����x�[�X��������	�F�ĎR꣑��Y
//	2021/12/06	�ĎR꣑��Y�N�̃v���O���������ɕύX	�F�㌎��n	
//
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "Fade.h"
#include "polygon.h"
#include "Sound.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define FADE_RATE 0.02f // �t�F�[�h�C��/�A�E�g�̍���

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static float g_fRed = 0.0f;			// ��:0.0f~255.0f�g�F
static float g_fGreen = 0.0f;		// ��:0.0f~255.0f
static float g_fBlue = 0.0f;		// ��:0.0f~255.0f
static float g_fAlpha = 1.0f;		// ���l:0.0f~1.0f
static float g_fFadeRate = FADE_RATE;	// �t�F�[�h�C��/�A�E�g�̍���

static E_FADE_STATE g_eState = FADE_IN;	// �t�F�[�h���
static EScene g_eNext = SCENE_TITLE;	// ���̃V�[��

//**************************************************************
// ����������
//**************************************************************
HRESULT InitFade()
{
	HRESULT hr = S_OK;

	g_fFadeRate = FADE_RATE;	//1.0f���ƈ�u�ł����
	g_eState = FADE_IN;
	g_eNext = SCENE_TITLE;

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitFade()
{
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
		g_fAlpha += g_fFadeRate; // �s�����x�𑝂�
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
		g_fAlpha -= g_fFadeRate; // �����x�𑝂�
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
}

//**************************************************************
// �I������
//**************************************************************
void DrawFade()
{
	// ��ʑS�̂ɔ������̋�`��`��
	// ID3D11DeviceContext* pDC = GetDeviceContext();
	// SetPolygonPos(0.0f, 0.0f);				// ���W
	// SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);	// �z���T�C�Y
	// SetPolygonUV(0.0f, 0.0f);				// �e�N�X�`�����W
	// SetPolygonFrameSize(1.0f, 1.0f);		// �e�N�X�`���T�C�Y
	// SetPolygonTexture(nullptr);				// �e�N�X�`�����
	// SetPolygonColor(g_fRed, g_fGreen, g_fBlue);		// �F���
	// SetPolygonAlpha(g_fAlpha);				// �����x
	// DrawPolygon(pDC);
	// 
	// // ���ɖ߂�
	// SetPolygonColor(1.0f, 1.0f, 1.0f);
	// SetPolygonAlpha(1.0f);

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
void StartFadeOut(EScene eNext)
{
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
