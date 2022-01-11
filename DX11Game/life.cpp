//**************************************************************
//
//	Life.cpp
//	���C�t����
//
//--------------------------------------------------------------
//	����ҁF�V���ڐl
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/22	�R�����g�̒ǉ��A�e�N�X�`���p�񋓑̐錾�ǉ�
//				�s�v�ȃ\�[�X�̍폜
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "Texture.h"
#include "life.h"
#include "polygon.h"

//**************************************************************
// �}�N����`
//**************************************************************

// ���C�t�g
#define PATH_LIFEFRAMETEXTURE L"data/texture/frame_life.png"	//���C�t�g�e�N�X�`��
#define LIFEFRAME_WIDTH (200.0f)		// ���C�t�g�̉���
#define LIFEFRAME_HEIGHT (80.0f)		// ���C�t�g�̏c��
#define LIFEFRAME_POS_X ((SCREEN_WIDTH-LIFEFRAME_WIDTH)*-0.5f)	// ���C�t�g�������W
#define LIFEFRAME_POS_Y ((SCREEN_HEIGHT-LIFEFRAME_HEIGHT)*0.5f)	// ���C�t�g�c�����W

// ���C�t�A�C�R��
#define PATH_LIFETEXTURE L"data/texture/hart.tga"	//���C�t�A�C�R���e�N�X�`��
#define MAX_LIFE (5)		// ���C�t�ő�l
#define LIFE_SIZE_X (40.0f)	// ���e�N�X�`���T�C�Y
#define LIFE_SIZE_Y (40.0f)	// �c�e�N�X�`���T�C�Y
#define LIFE_POS_X (LIFEFRAME_POS_X-LIFE_SIZE_X*(MAX_LIFE*0.5f-0.5f))	// ���C�t�������W
#define LIFE_POS_Y (LIFEFRAME_POS_Y-6.0f)		// ���C�t�c�����W

//#define LIFE_COUNT_X	3
//#define LIFE_COUNT_Y	4
//#define LIFE_START 3
//**************************************************************
// �񋓑̐錾
//**************************************************************
enum TEXTURE
{
	TEX_FRAME = 0,	// ���C�t�g
	TEX_LIFE,		// ���C�t�A�C�R��

	MAX_TEXTURE		// ���C�t�Ŏg�����e�N�X�`����
};


//**************************************************************
//�\���̒�`
//**************************************************************

//struct ANIM_PAT
//{
//	int nFrame;		//�\���gNo.(-1�ŏI��)
//	int nCount;		//�\���t���[����
//};
//
//struct LIFE
//{//�G���
//	XMFLOAT2 vPos;
//	int nAnimFrame;	//�\���gNo.
//	int nFrameCount;		//�\���t���[����
//	int nAnimPat;			//�p�^�[��No.
//	int nDir;				//����(0:��,1:��,2:�E)
//};


//**************************************************************
// �O���[�o���ϐ�
//**************************************************************

//static ANIM_PAT g_animPat[4][5] =
//{
//	//�������A�j���[�V�����J�E���g
//	//�E�����t���[���J�E���g
//		{{ 0, 5}, { 0, 2}, { 0, 5}, { 0, 2}, {-1, -1}},
//		{{ 4, 5}, { 4, 2}, { 4, 5}, { 4, 2}, {-1, -1}},
//		{{ 8, 5}, { 8, 2}, {8, 5}, { 8, 2}, {-1, -1}},
//		{{12, 5}, {12, 2}, {12, 5}, {12, 2}, {-1, -1}},
//
//};

static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];	// �e�N�X�`�����
static int g_nLife;	// �̗�



//**************************************************************
// ����������
//**************************************************************
HRESULT InitLife()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	LPCWSTR pszTexFName[] = { PATH_LIFEFRAMETEXTURE, PATH_LIFETEXTURE };
	for (int i = 0; i < MAX_TEXTURE; ++i) 
	{
		hr = CreateTextureFromFile(pDevice, pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) 
		{
			return hr;
		}
	}
	// �ϐ�������
	g_nLife = MAX_LIFE;
	
	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitLife()
{
	// �e�N�X�`�����
	for (int i = 0; i < MAX_TEXTURE; ++i) 
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
}
//**************************************************************
// �X�V����
//**************************************************************
void UpdateLife()
{

}
//**************************************************************
// �`�揈��
//**************************************************************
void DrawLife()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	// �e�N�X�`���Ɠ��F�ɐݒ�
	SetPolygonColor(0.929412f, 0.152941f, 0.564706f);

	// ���C�t�g�\��
	SetPolygonSize(LIFEFRAME_WIDTH, LIFEFRAME_HEIGHT);
	SetPolygonPos(LIFEFRAME_POS_X, LIFEFRAME_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_FRAME]);
	DrawPolygon(pDC);

	// ���ɖ߂�
	SetPolygonColor(1.0f, 1.0f, 1.0f);

	// ���C�t�\��
	SetPolygonSize(LIFE_SIZE_X, LIFE_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_LIFE]);

	//Life���A�j���[�V����������ɂ́AUV,FramSize���K�v
	for(int i = 0; i < g_nLife; ++i) 
	{
		SetPolygonPos(LIFE_POS_X + i * LIFE_SIZE_X, LIFE_POS_Y);
		DrawPolygon(pDC);
	}
}
//**************************************************************
// ���C�t���Z
//**************************************************************
void AddLife(int nLife)
{
	g_nLife += nLife;
	if (g_nLife > MAX_LIFE) 
	{
		g_nLife = MAX_LIFE;
	}
}
//**************************************************************
// ���C�t���Z
//**************************************************************
void DelLife(int nLife)
{
	g_nLife -= nLife;
	if (g_nLife < 0) 
	{
		g_nLife = 0;
	}
}
//**************************************************************
// ���C�t�擾
//**************************************************************
int GetLife()
{
	return g_nLife;
}