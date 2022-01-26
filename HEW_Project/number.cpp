//**************************************************************
//
//	number.cpp
//	���l�\������
//
//--------------------------------------------------------------
//	����ҁF�V���ڐl
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/22	�R�����g�̕ҏW&�ǉ�
//	�ҏW�ҁF�ĎR꣑��Y
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "number.h"
#include "Texture.h"
#include "polygon.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define PATH_NUMBERTEXTURE L"data/texture/number000.png"	// �����e�N�X�`��
#define NUMBER_COUNT_X (3)	// �e�N�X�`�����R�}��
#define NUMBER_COUNT_Y (4)	// �e�N�X�`���c�R�}��
//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;		// �e�N�X�`���p�ϐ�
//**************************************************************
// ����������
//**************************************************************
HRESULT InitNumber()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_NUMBERTEXTURE, &g_pTexture);
	return hr;
}
//**************************************************************
// �I������
//**************************************************************
void UninitNumber()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}
//**************************************************************
// �`�揈��
//**************************************************************
void DrawNumber(XMFLOAT2 vPos,	// �\���ʒu(����)
				unsigned uNumber,	// �\�����l
				int nWidth,		// �\������
				float fSizeX,	// �\������
				float fSizeY)	// �\���c��
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(NUMBER_SIZE_X, NUMBER_SIZE_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonFrameSize(1.0f / NUMBER_COUNT_X,
		1.0f / NUMBER_COUNT_Y);
	// �@
	vPos.x += (nWidth - 0.5f) * NUMBER_SIZE_X;
	vPos.y -= NUMBER_SIZE_Y * 0.5f;
	// �D
	for (; nWidth > 0; --nWidth) 
	{
		// �A
		unsigned n = uNumber % 10;
		SetPolygonPos(vPos.x, vPos.y);
		SetPolygonUV
		(
			(n % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
			(n / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y
		);
		DrawPolygon(pDC);
		// �B
		uNumber /= 10;
		// �C
		vPos.x -= NUMBER_SIZE_X* 1.1;
	}
	// ���ɖ߂�
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}