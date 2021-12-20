//**************************************************************
//
//	SceneGameover.cpp
//	�^�C�g���V�[�����g
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//	�J������
//	2021/12/06	������N�̃v���O���������ɍ쐬	
//
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
// �}�N����`
#define PATH_BGTEXTURE "data/texture/Gameover.png"
#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT
// �O���[�o���ϐ�
static ID3D11ShaderResourceView* g_pTexture;
//**************************************************************
// ����������
//**************************************************************
HRESULT InitGameover()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �|���S���\��������
	hr = InitPolygon(GetDevice());
	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if (FAILED(hr))
		return hr;

	// ���g�͂܂��Ȃ�

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitGameover()
{
	// ���g����
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
		// �|���S���\���I������
	UninitPolygon();
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateGameover()
{
	if (GetKeyRelease(VK_2))
	{
		StartFadeOut(SCENE_TITLE);
	}
	// �|���S���\���X�V
	UpdatePolygon();

}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawGameover()
{
	// ���ɂȂ�
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);
	
}