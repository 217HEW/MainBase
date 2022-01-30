#include "EnemyMelee.h"
#include "EnemyRange.h"
// ��ʑJ�� [CountEnemy.cpp]
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

//���Ԃ̌v�Z�͍s���Ă��邪�`�悪����Ă��Ȃ��������͌����Ȃ�
//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "Texture.h"
#include "polygon.h"
#include "number.h"
#include "CountEnemy.h"
#include "Sound.h"

//**************************************************************
// �}�N����`
//**************************************************************

// �^�C�}�[�g
#define PATH_CountEnemyFRAMETEXTURE L"data/texture/frame_CountEnemy.png"		// �^�C�}�[�g�e�N�X�`��
#define CountEnemyFRAME_WIDTH (140.0f)		// �^�C�}�[�g����
#define CountEnemyFRAME_HEIGHT (80.0f)		// �^�C�}�[�g�c��
#define CountEnemyFRAME_POS_X (0.0f)			// �^�C�}�[�g�������W
#define CountEnemyFRAME_POS_Y ((SCREEN_HEIGHT-CountEnemyFRAME_HEIGHT)*0.5f)	// �^�C�}�[�g�c�����W

//�J�E���^�[
#define CountEnemy_CHRCNT (3)		// �J�E���^�[�̕\������
#define CountEnemy_POS_X (CountEnemyFRAME_POS_X-CountEnemy_CHRCNT*NUMBER_SIZE_X*0.5f)	// �J�E���^�[�������W
#define CountEnemy_POS_Y (CountEnemyFRAME_POS_Y+16.0f)	//�J�E���^�[�c�����W

#define CountEnemy_START (30)// ��������

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;	// �e�N�X�`���p�ϐ�
int g_nCountEnemy;		// ���Ԃ��J�E���g����

//**************************************************************
// ����������
//**************************************************************
HRESULT InitCountEnemy()
{
	HRESULT hr = S_OK;

	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_CountEnemyFRAMETEXTURE, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}
	// �ϐ�������
	g_nCountEnemy = CountEnemy_START * 60 + 59;
	return hr;

}
//**************************************************************
// �I������
//**************************************************************
void UninitCountEnemy()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}
//**************************************************************
// �X�V����
//**************************************************************
void UpdateCountEnemy()
{
	// �^�C�}�[�J�E���g�_�E��
	if (g_nCountEnemy > 0)
	{
		--g_nCountEnemy;
	}
}
//**************************************************************
// �`�揈��
//**************************************************************
void DrawCountEnemy()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// �����Ԃɐݒ�
	if (g_nCountEnemy < 10 * 60)
	{
		if (g_nCountEnemy == 9.7 * 60)
		{
			CSound::SetPlayVol(SE_BEEP, 0.07f); // �x����
		}
		SetPolygonColor(1.0f, 0.0f, 0.0f);//�c��10�b�ŐԂ��Ȃ�
	}
	else {
		SetPolygonColor(1.0f, 1.0f, 1.0f);
	}
	// �^�C�}�[�g�\��
	SetPolygonSize(CountEnemyFRAME_WIDTH, CountEnemyFRAME_HEIGHT);
	SetPolygonPos(CountEnemyFRAME_POS_X, CountEnemyFRAME_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// �^�C�}�[�\��
	DrawNumber(XMFLOAT2(CountEnemy_POS_X, CountEnemy_POS_Y), (unsigned)(g_nCountEnemy / 60), CountEnemy_CHRCNT);
	// ���ɖ߂�
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}
//**************************************************************
// �^�C�}�[���Z
//**************************************************************
void AddCountEnemy(int nSec)
{
	g_nCountEnemy += nSec * 60;
}
//**************************************************************
// �^�C�}�[�擾
//**************************************************************
int GetCountEnemy()
{
	return g_nCountEnemy / 60;
}