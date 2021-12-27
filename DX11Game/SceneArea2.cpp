//**************************************************************
//
//	SceneArea3.cpp
//	�^�C�g���V�[�����g
//
//--------------------------------------------------------------
//	����ҁF������
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/22	�Q�[���G���A3�̎���
//	�ҏW�ҁF������
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "SceneArea2.h"
#include "SceneManager.h"
#include "Fade.h"
#include "AssimpModel.h"
#include "input.h"
#include "polygon.h"
#include "debugproc.h"
#include "mesh.h"
#include "meshfield.h"
#include "player.h"
#include "shadow.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "smoke.h"
#include "meshwall.h"
//#include "polyline.h"
#include "Sound.h"
#include "timer.h"
#include "life.h"
#include "number.h"
#include "CreateField.h"
#include "Block.h"
#include "EnemyMelee.h"
#include "Pause.h"
#include "Texture.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define MAX_POLYLINE	(20)					// �|�����C����
#define THICKNESS		(10.0f)					// ���̑���

#define PATH_BGTEXTURE "data/texture/Area2.png"
#define BG_POS_X 0.0f
#define BG_POS_Y 0.0f
#define BG_WIDTH SCREEN_WIDTH
#define BG_HEIGHT SCREEN_HEIGHT

//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
//TPolyline					g_polyline[MAX_POLYLINE];	// �|�����C�����

static bool g_bPause;								//�ꎞ��~��
static ID3D11ShaderResourceView* g_pTexture;

//**************************************************************
// ����������
//**************************************************************
HRESULT InitArea2()
{
	HRESULT hr = S_OK;

	//// ���b�V��������
	//hr = InitMesh();
	//if (FAILED(hr))
	//	return hr;

	//// �ۉe������
	//hr = InitShadow();
	//if (FAILED(hr))
	//	return hr;

	//// ��������
	//hr = InitNumber();
	//if (FAILED(hr))
	//	return hr;


	////*12/17�V���ڐl�ǉ�
	//// �^�C�}�[�\��������
	//hr = InitTimer();
	//if (FAILED(hr))
	//{
	//	MessageBox(GetMainWnd(), _T("�^�C�}�[�\�����������s"), NULL, MB_OK | MB_ICONSTOP);
	//	return hr;
	//}

	//// ���C�t�\��������
	//hr = InitLife();
	//if (FAILED(hr))
	//{
	//	MessageBox(GetMainWnd(), _T("���C�t�\�����������s"), NULL, MB_OK | MB_ICONSTOP);
	//	return hr;
	//}

	//// ���@������
	//hr = InitPlayer();
	//if (FAILED(hr))
	//	return hr;

	//// �t�B�[���h������
	//hr = InitMeshField(16, 16, 80.0f, 80.0f);
	//if (FAILED(hr))
	//	return hr;

	////�񎟌��z��}�b�v
	//hr = InitCField();
	//if (FAILED(hr))
	//	return hr;

	//// �w�i������
	//hr = InitBG();
	//if (FAILED(hr))
	//	return hr;

	//// �r���{�[�h�e������
	//hr = InitBullet();
	//if (FAILED(hr))
	//	return hr;

	//// ����������
	//hr = InitExplosion();
	//if (FAILED(hr))
	//	return hr;

	//// �G�t�F�N�g������
	//hr = InitEffect();
	//if (FAILED(hr))
	//	return hr;

	//// ��������
	//hr = InitSmoke();
	//if (FAILED(hr))
	//	return hr;

	//// �u���b�N������
	////hr = InitBlock();
	////if (FAILED(hr))
	////return hr;

	//// �G�l�~�[�����[������
	//hr = InitEnemyMelee();
	//if (FAILED(hr))
	//	return hr;

	//// �����[�Ăяo��
	//SetEnemyMelee(XMFLOAT3(30.0f, 30.0f, 0.0f));

	//// ���b�V���Ǐ�����
	//hr = InitMeshWall();
	//if (FAILED(hr))
	//	return hr;

	//// �|�[�Y������
	//hr = InitPause();
	//g_bPause = false;
	//if (FAILED(hr))
	//	return hr;
	//// SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(40.0f, 40.0f));
	//// SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//// SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	//// SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));

	//// �u���b�N�Z�b�g
	//// for (int y = 0; y < 10; ++y)
	//// {
	////	 for (int x = 0; x < 12; ++x)
	////	 {
	////		 SetBlock(XMFLOAT3(21.0f * x, 41.0f * y, 100.0f));
	////	 }
	//// }
	//// �{�����C��������
	//// hr = InitPolyline();
	//// if (FAILED(hr))
	//// 	return hr;
	//// XMFLOAT4 vColor[8] = {
	//// 	XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
	//// 	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	//// 	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	//// 	XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
	//// 	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	//// 	XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
	//// 	XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
	//// 	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//// };
	//// for (int i = 0; i < MAX_POLYLINE; ++i) {
	//// 	hr = CreatePolyline(&g_polyline[i], THICKNESS, true, vColor[i % 7 + 1], BS_ADDITIVE);
	//// 	if (FAILED(hr)) {
	//// 		return hr;
	//// 	}
	//// 	XMFLOAT3 pos;
	//// 	pos.x = rand() % 1240 - 620.0f;
	//// 	pos.y = rand() % 140 + 10.0f;
	//// 	pos.z = rand() % 1240 - 620.0f;
	//// 	AddPolylinePoint(&g_polyline[i], pos);
	//// 	pos.x = rand() % 1240 - 620.0f;
	//// 	pos.y = rand() % 140 + 10.0f;
	//// 	pos.z = rand() % 1240 - 620.0f;
	//// 	AddPolylinePoint(&g_polyline[i], pos);
	//// }
	//return hr;
	// // BGM�Đ��J�n
	//CSound::Play(BGM_000);
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
void UninitArea2()
{
	//�|�[�Y�I������
	UninitPause();
	//// BGM�Đ���~
	//CSound::Stop(BGM_000);

	//// �|�����C���I������
	////UninitPolyline();

	//// �ǏI��
	//UninitMeshWall();

	//// �G�l�~�[�����[�I��
	//UninitEnemyMelee();

	//// �u���b�N�I��
	////UninitBlock();

	//// ���I��
	//UninitSmoke();

	//// �G�t�F�N�g�I��
	//UninitEffect();

	//// �����I��
	//UninitExplosion();

	//// �r���{�[�h�e�I��
	//UninitBullet();

	//// �w�i�I��
	//UninitBG();

	//// �t�B�[���h�I��
	//UninitMeshField();

	//// �񎟌��z��}�b�v�I��
	//UninitCField();

	//// ���@�I��
	//UninitPlayer();

	//// �ۉe�I��
	//UninitShadow();

	////�i���o�[�I��
	//UninitNumber();

	////���C�t�I��
	//UninitLife();

	////�^�C�}�[�I��
	//UninitTimer();

	//// ���b�V���I��
	//UninitMesh();
	// ���g����
	//// BGM�Đ���~
	//CSound::Stop(BGM_001);
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateArea2()
{
	// ���͏����X�V
	//UpdateInput();	// �K��Update�֐��̐擪�Ŏ��s.

	//�ꎞ��~��?
	if (g_bPause) {
		//�ꎞ��~�X�V
		UpdatePause();
	}
	else
	{
		if (GetFadeState() == FADE_NONE)
		{
			if (GetKeyRelease(VK_1))
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
			else if (GetKeyRelease(VK_5))
			{
				StartFadeOut(SCENE_GAMEOVER);
			}
			else if (GetKeyRelease(VK_6))
			{
				StartFadeOut(SCENE_GAMECLEAR);
			}
		}




		// �f�o�b�O������\���X�V
		UpdateDebugProc();

		// �f�o�b�O������ݒ�
		StartDebugProc();

		//// �|���S���\���X�V
		////UpdatePolygon();

		//// ���@�X�V
		//UpdatePlayer();

		//// �G�l�~�[�����[
		//UpdateEnemyMelee();

		//// �w�i�X�V
		//UpdateBG();

		//// �ǍX�V
		//UpdateMeshWall();

		//// �t�B�[���h�X�V
		//UpdateMeshField();

		//// �񎟌��z��}�b�v�X�V
		//UpdateCField();

		//*12/17�V���ڐl�ǉ�
		// �^�C�}�[�X�V
		UpdateTimer();

		//// �ۉe�X�V
		//UpdateShadow();

		//// �J�����X�V
		//CCamera::Get()->Update();

		//// �r���{�[�h�e�X�V
		//UpdateBullet();

		//// �����X�V
		//UpdateExplosion();

		//// �G�t�F�N�g�X�V
		//UpdateEffect();

		//// �u���b�N�X�V
		//// UpdateBlock();

		//// ���X�V
		//UpdateSmoke();

		//// �u���b�N�X�V
		//// �|�����C���X�V
		//// for (int i = 0; i < MAX_POLYLINE; ++i) {
		//// 	UpdatePolyline(&g_polyline[i]);
		//// }


	}
	//�ꎞ��~ON/OFF
	if (GetKeyTrigger(VK_P) || GetKeyTrigger(VK_PAUSE))
	{
		if (GetFadeState() == FADE_NONE)
		{
			g_bPause = !g_bPause;
			if (g_bPause) {
				//CSound::Pause();
				//CSound::Play(SE_DECIDE);
				ResetPauseMenu();
			}
			else
			{
				//CSound::Play(SE_CANCEL);
				//CSound::Resume();
			}
		}
	}

	//�ꎞ��~���j���[�̑I��
	if (g_bPause && GetFadeState() == FADE_NONE)
	{
		//[ENTER]�������ꂽ?
		if (GetKeyTrigger(VK_RETURN))
		{
			//�I�𒆂̃��j���[���ڂɂ�蕪��
			switch (GetPauseMenu())
			{
			case PAUSE_MENU_CONTINUE:
				g_bPause = false;
				//CSound::Play(SE_CANCEL);
				//CSound::Resume();
				break;
			case PAUSE_MENU_RETRY:
				StartFadeOut(SCENE_GAME);
				break;
			case PAUSE_MENU_QUIT:
				StartFadeOut(SCENE_TITLE);
				break;
			}
		}
	}
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawArea2()
{
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	//// �w�i�`��
	//DrawBG();

	//// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	//SetZBuffer(true);

	//// �t�B�[���h�`��
	////DrawMeshField();

	//// �񎟌��z��}�b�v�`��
	//DrawCField();

	//// ���@�`��
	//DrawPlayer();

	//// �G�l�~�[�����[
	//DrawEnemyMelee();

	//// �ۉe�`��
	//DrawShadow();

	//// �r���{�[�h�e�`��
	//DrawBullet();

	//// ���`��
	//DrawSmoke();

	//// �G�t�F�N�g�`��
	//DrawEffect();

	//// �Ǖ`�� (�s��������)
	//DrawMeshWall(DRAWPART_OPAQUE);

	//// �u���b�N�`��
	////DrawBlock();

	//// �����`��
	//DrawExplosion();

	//// �|�����C���`��
	//// for (int i = 0; i < MAX_POLYLINE; ++i)
	//// {
	//// 	DrawPolyline(&g_polyline[i]);
	//// }

	//// �Ǖ`�� (����������)
	//DrawMeshWall(DRAWPART_TRANSLUCENT);

	SetZBuffer(false);
	//�ꎞ��~�`��
	if (g_bPause) {
		DrawPause();
	}
	SetZBuffer(true);

	//// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	//SetZBuffer(false);

	//// �f�o�b�O������\��(����)
	//SetBlendState(BS_ALPHABLEND);
	//// �^�C�}�[�\��
	//DrawTimer();
	//// ���C�t�\��(����)
	//DrawLife();
	//DrawDebugProc();

		// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);
	SetBlendState(BS_NONE);
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);
}


