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

// �摜�t�@�C���l�[��
static LPCWSTR g_pszTexFName[FADE_TEX_MAX] = {
	L"data/texture/Loading/NowLoading.png",
	L"data/texture/Loading/Logo.png",
};


CFade::CFade()
{

}
CFade::~CFade()
{

}

//**************************************************************
// ����������
//**************************************************************
HRESULT CFade::Init()
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
	m_color.r = m_color.g = m_color.b = m_color.a = 1.0f;
	m_eState = FADE_IN;
	m_nNext = SCENE_TITLE;
	m_SManager = GetSManager();

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void CFade::Uninit()
{
	// �����̃e�N�X�`�����
	for (int i = 0; i < FADE_TEX_MAX; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}

	// �I�����ɏ�����
	m_color.a = 0.0f;
	m_eState = FADE_NONE;
}

//**************************************************************
// �X�V����
//**************************************************************
void CFade::Update()
{
	// �t�F�[�h�C���E�A�E�g
	switch (m_eState)
	{
	case FADE_NONE:
		break;

	case FADE_OUT:
		// �t�F�[�h�A�E�g---------------------------------------
		m_color.a += FADE_RATE; // �s�����x�𑝂�
		if (m_color.a >= 1.0f)
		{
			// �t�F�[�h�C�������ɐ؂�ւ�
			m_color.a = 1.0f;
			m_eState = FADE_IN;

			// �V�[���ؑ�
			m_SManager->Set(m_nNext);
		}
		// �{�����[�����t�F�[�h�A�E�g
		CSound::SetVolume(1.0f - m_color.a);
		break;
		// -----------------------------------------------------

	case FADE_IN:
		// �t�F�[�h�C��-----------------------------------------
		m_color.a -= FADE_RATE; // �����x�𑝂�
		if (m_color.a <= 0.0f)
		{
			// �t�F�[�h�C���I��
			m_color.a = 0.0f;
			m_eState = FADE_NONE;
		}
		// �{�����[�����t�F�[�h�C��
		CSound::SetVolume(1.0f - m_color.a);
		break;
		// -----------------------------------------------------
	}
	if (m_color.r <= 0.0f)
	{
		m_color.r = m_color.g = m_color.b += FADE_RATE;
	}
	else
	{
		m_color.r = m_color.g = m_color.b -= FADE_RATE;
	}

	if (m_nNext == SCENE_GAMECLEAR || m_nNext == SCENE_GAMEOVER)
	{
		m_color.r = m_color.g = m_color.b = 0.0f;
	}
	else
	{
		m_color.r = m_color.g = m_color.b = 1.0f;
	}
}

//**************************************************************
// �I������
//**************************************************************
void CFade::Draw()
{
	// ��ʑS�̂ɔ������̋�`��`��
	SetBlendState(BS_ALPHABLEND);
	 ID3D11DeviceContext* pDC = GetDeviceContext();
	 SetPolygonPos(0.0f, 0.0f);		// ���W�ݒ�
	 SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);	// �z���T�C�Y�ݒ�
	 SetPolygonUV(0.0f, 0.0f);		// �e�N�X�`�����W�ݒ�
	 SetPolygonFrameSize(1.0f, 1.0f);	// �e�N�X�`���T�C�Y�ݒ�
	 if(m_nNext == SCENE_TITLE)		// �e�N�X�`�����ݒ�
	 { SetPolygonTexture(g_pTexture[LOGO]);} 
	 else {SetPolygonTexture(g_pTexture[LOAD]);}
	 SetPolygonColor(m_color.r, m_color.g, m_color.b);// �F���ݒ�
	 SetPolygonAlpha(m_color.a);		// �����x�ݒ�
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
void CFade::StartFadeOut(int eNext)
{
	// �t�F�[�h�A�E�g���Ă��Ȃ���Ώ���
	if (m_eState != FADE_OUT)
	{
		m_eState = FADE_OUT;
		m_color.a = 0.0f;
		m_nNext = eNext;
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
E_FADE_STATE CFade::GetFadeState()
{
	return m_eState;
}
