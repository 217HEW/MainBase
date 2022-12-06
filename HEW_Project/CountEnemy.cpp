//**************************************************************
//
//	CountEnemy.cpp
//	���l�\������
//
//--------------------------------------------------------------
//	����ҁF������
//--------------------------------------------------------------
//**************************************************************
//	�J������
//	2021/12/22	�R�����g�̕ҏW&�ǉ�
//	�ҏW�ҁF������
//**************************************************************
//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "EnemyMelee.h"
#include "EnemyRange.h"
#include "Texture.h"
#include "polygon.h"
#include "number.h"
#include "CountEnemy.h"
#include "Sound.h"
#include "SceneGameclear.h"
#include "SceneManager.h"
#include "Fade.h"
//**************************************************************
// �}�N����`
//**************************************************************
// �^�C�}�[�g
#define PATH_CountEnemyFRAMETEXTURE L"data/texture/GameScene/frame_CountEnemy.png"		//�J�E���^�[�g�e�N�X�`��
#define CountEnemyFRAME_WIDTH (399.0f)		//�J�E���^�[�g����
#define CountEnemyFRAME_HEIGHT (80.0f)		//�J�E���^�[�g�c��
#define CountEnemyFRAME_POS_X (400.0f)			//�J�E���^�[�g�������W
#define CountEnemyFRAME_POS_Y ((SCREEN_HEIGHT-CountEnemyFRAME_HEIGHT)*0.5f)	//�J�E���^�[�g�c�����W
//�J�E���^�[
#define CountEnemy_CHRCNT (2)		// �J�E���^�[�̕\������
#define CountEnemy_POS_X (CountEnemyFRAME_POS_X-CountEnemy_CHRCNT*NUMBER_SIZE_X+90.0f)	// �J�E���^�[�������W
#define CountEnemy_POS_Y (CountEnemyFRAME_POS_Y+50.0f)	//�J�E���^�[�c�����W
#define CountEnemy_NumSize_X 50.0f
#define CountEnemy_NumSize_Y 100.0f

//
#define MaxCountEnemy_POS_X (CountEnemyFRAME_POS_X-CountEnemy_CHRCNT*NUMBER_SIZE_X*-2.0f)	// �����J�E���^�[�������W
#define MaxCountEnemy_POS_Y (CountEnemyFRAME_POS_Y+16.0f)									//�����J�E���^�[�c�����W
//
#define MAX_ENEMY_NUM  99//�G�l�~�[�̏��
//
//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
static ID3D11ShaderResourceView* g_pTexture;	// �e�N�X�`���p�ϐ�
int g_nCountEnemy;		//�G�l�~�[���J�E���g����
int g_nMaxCountEnemy;	//���̃G���A�̃G�l�~�[�̍ő吔��ۊǂ���

//**************************************************************
// ����������
//**************************************************************
HRESULT CCountEnemy::InitCountEnemy()
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
	g_nCountEnemy=0;	
	g_nMaxCountEnemy=0;
	return hr;

}
//**************************************************************
// �I������
//**************************************************************
void CCountEnemy::UninitCountEnemy()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}
//**************************************************************
// �X�V����
//**************************************************************
void CCountEnemy::UpdateCountEnemy()
{
}
//**************************************************************
// �`�揈��
//**************************************************************
void CCountEnemy::DrawCountEnemy()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	
	// �^�C�}�[�g�\��
	SetPolygonSize(CountEnemyFRAME_WIDTH, CountEnemyFRAME_HEIGHT);
	SetPolygonPos(CountEnemyFRAME_POS_X, CountEnemyFRAME_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// �J�E���^�[�\��
	DrawNumber(XMFLOAT2(CountEnemy_POS_X, CountEnemy_POS_Y), g_nCountEnemy, CountEnemy_CHRCNT, CountEnemy_NumSize_X, CountEnemy_NumSize_Y);
	
	// ���ɖ߂�
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}
//**************************************************************
// �J�E���g�G�l�~�[���Z
//**************************************************************
void CCountEnemy::AddCountEnemy(int nCount)
{
	g_nCountEnemy += nCount;
	g_nMaxCountEnemy += nCount;
	if (nCount > MAX_ENEMY_NUM)
	{
		nCount = MAX_ENEMY_NUM;
		g_nMaxCountEnemy = MAX_ENEMY_NUM;
	}
}
//**************************************************************
// �J�E���g�G�l�~�[���Z
//**************************************************************
void CCountEnemy::DelCountEnemy(int nCount)
{
	g_nCountEnemy -= nCount;
	if (nCount < 0)
	{
		nCount = 0;
	}
	if (g_nCountEnemy == 0)
	{
		GameclearFlag();
	}
}
//**************************************************************
//�J�E���g�G�l�~�[�擾
//**************************************************************
int CCountEnemy::GetCountEnemy()
{
	return g_nCountEnemy;
}