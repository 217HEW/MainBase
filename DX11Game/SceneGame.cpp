//**************************************************************
//
//	SceneGame.cpp
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
#include "SceneGame.h"
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
#include "polyline.h"
#include "Sound.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define MAX_POLYLINE	(20)					// �|�����C����
#define THICKNESS		(10.0f)					// ���̑���


//**************************************************************
// �O���[�o���ϐ�
//**************************************************************
TPolyline					g_polyline[MAX_POLYLINE];	// �|�����C�����

//**************************************************************
// ����������
//**************************************************************
HRESULT InitGame()
{
	HRESULT hr = S_OK;

	// �|���S���\��������
	hr = InitPolygon(GetDevice());
	if (FAILED(hr))
		return hr;

	// �f�o�b�O������\��������
	hr = InitDebugProc();
	if (FAILED(hr))
		return hr;

	// ���͏���������
	hr = InitInput();
	if (FAILED(hr))
		return hr;

	// Assimp�p�V�F�[�_������
	if (!CAssimpModel::InitShader(GetDevice()))
		return E_FAIL;

	// ���b�V��������
	hr = InitMesh();
	if (FAILED(hr))
		return hr;

	// �ۉe������
	hr = InitShadow();
	if (FAILED(hr))
		return hr;

	// ���@������
	hr = InitPlayer();
	if (FAILED(hr))
		return hr;

	// �t�B�[���h������
	hr = InitMeshField(16, 16, 80.0f, 80.0f);
	if (FAILED(hr))
		return hr;

	// �w�i������
	hr = InitBG();
	if (FAILED(hr))
		return hr;

	// �r���{�[�h�e������
	hr = InitBullet();
	if (FAILED(hr))
		return hr;

	// ����������
	hr = InitExplosion();
	if (FAILED(hr))
		return hr;

	// �G�t�F�N�g������
	hr = InitEffect();
	if (FAILED(hr))
		return hr;

	// �G�t�F�N�g������
	hr = InitEffect();
	if (FAILED(hr))
		return hr;

	// ��������
	hr = InitSmoke();
	if (FAILED(hr))
		return hr;

	// �Ǐ�����
	 hr = InitMeshWall();
	 if (FAILED(hr))
	 	return hr;
	 SetMeshWall(XMFLOAT3(0.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, 30.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	 SetMeshWall(XMFLOAT3(-640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	 SetMeshWall(XMFLOAT3(640.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	 SetMeshWall(XMFLOAT3(0.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 180.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, XMFLOAT2(80.0f, 80.0f));
	 

	// �{�����C��������
	// hr = InitPolyline();
	// if (FAILED(hr))
	// 	return hr;
	// XMFLOAT4 vColor[8] = {
	// 	XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
	// 	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	// 	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	// 	XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
	// 	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	// 	XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
	// 	XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
	// 	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	// };
	// for (int i = 0; i < MAX_POLYLINE; ++i) {
	// 	hr = CreatePolyline(&g_polyline[i], THICKNESS, true, vColor[i % 7 + 1], BS_ADDITIVE);
	// 	if (FAILED(hr)) {
	// 		return hr;
	// 	}
	// 	XMFLOAT3 pos;
	// 	pos.x = rand() % 1240 - 620.0f;
	// 	pos.y = rand() % 140 + 10.0f;
	// 	pos.z = rand() % 1240 - 620.0f;
	// 	AddPolylinePoint(&g_polyline[i], pos);
	// 	pos.x = rand() % 1240 - 620.0f;
	// 	pos.y = rand() % 140 + 10.0f;
	// 	pos.z = rand() % 1240 - 620.0f;
	// 	AddPolylinePoint(&g_polyline[i], pos);
	// }

	// BGM�Đ�
	//CSound::Play(BGM_003);

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitGame()
{

	// BGM�Đ���~
	//CSound::Stop(BGM_003);

	// �|�����C���I������
	//UninitPolyline();

	// �ǏI������
	UninitMeshWall();

	// ���I������
	UninitSmoke();

	// �G�t�F�N�g�I������
	UninitEffect();

	// �����I������
	UninitExplosion();

	// �r���{�[�h�e�I������
	UninitBullet();

	// �w�i�I������
	UninitBG();

	// �t�B�[���h�I������
	UninitMeshField();

	// ���@�I������
	UninitPlayer();

	// �ۉe�I������
	UninitShadow();

	// ���b�V���I������
	UninitMesh();

	// Assimp�p�V�F�[�_�I������
	CAssimpModel::UninitShader();

	// ���͏����I������
	UninitInput();

	// �f�o�b�O������\���I������
	UninitDebugProc();

	// �|���S���\���I������
	UninitPolygon();
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateGame()
{
	// ���͏����X�V
	UpdateInput();	// �K��Update�֐��̐擪�Ŏ��s.

	// �f�o�b�O������\���X�V
	UpdateDebugProc();

	// �f�o�b�O������ݒ�
	StartDebugProc();
	
	// �|���S���\���X�V
	UpdatePolygon();

	// ���@�X�V
	UpdatePlayer();

	// �w�i�X�V
	UpdateBG();

	// �ǍX�V
	UpdateMeshWall();

	// �t�B�[���h�X�V
	UpdateMeshField();

	// �ۉe�X�V
	UpdateShadow();

	// �J�����X�V
	CCamera::Get()->Update();

	// �r���{�[�h�e�X�V
	UpdateBullet();

	// �����X�V
	UpdateExplosion();

	// �G�t�F�N�g�X�V
	UpdateEffect();

	// ���X�V
	UpdateSmoke();

	// �|�����C���X�V
	// for (int i = 0; i < MAX_POLYLINE; ++i) {
	// 	UpdatePolyline(&g_polyline[i]);
	// }

	
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawGame()
{
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	// �w�i�`��
	DrawBG();

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);

	// �t�B�[���h�`��
	DrawMeshField();

	// ���@�`��
	DrawPlayer();
	
	// �ۉe�`��
	DrawShadow();

	// �r���{�[�h�e�`��
	DrawBullet();

	// ���`��
	DrawSmoke();

	// �G�t�F�N�g�`��
	DrawEffect();

	// �Ǖ`�� (�s��������)
	DrawMeshWall(DRAWPART_OPAQUE);

	// �����`��
	DrawExplosion();

	// �|�����C���`��
	// for (int i = 0; i < MAX_POLYLINE; ++i)
	// {
	// 	DrawPolyline(&g_polyline[i]);
	// }

	// �Ǖ`�� (����������)
	 DrawMeshWall(DRAWPART_TRANSLUCENT);

	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	// �f�o�b�O������\��
	SetBlendState(BS_ALPHABLEND);
	DrawDebugProc();
	SetBlendState(BS_NONE);
}