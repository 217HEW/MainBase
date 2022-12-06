//**************************************************************
//
//	SceneGame.h
//	�Q�[���V�[���̃w�b�_
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/06	������N�̃v���O���������ɍ쐬	
//	�ҏW�ҁF??
//	2021/12/28	CreateField.h�̃C���N���[�h
//				�������̈����ɃG���A�؂�ւ��p�̈����ǉ�
//	�ҏW�ҁF�ĎR꣑��Y
//**************************************************************
#pragma once

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "main.h"
#include "CreateStage.h"
#include "player.h"
#include "timer.h"
#include "CountEnemy.h"
#include "life.h"
#include "bg.h"
#include "explosion.h"
#include "Pause.h"
#include "ClearPause.h"	//�e�X�g�C���N���[�h
#include "Reticle.h"
#include "effect.h"
#include "SceneManager.h"
#include "mesh.h"
#include "number.h"

//**************************************************************
// �v���g�^�C�v�錾
//**************************************************************
class CSceneManager;
class CFade;
class CSceneGame
{
public:
	CSceneGame();
	~CSceneGame();

	virtual HRESULT Init(STAGE Stage); // ������
	virtual void Uninit();	// �I������
	virtual void Update();	// �X�V
	virtual void Draw();	// �`��


private:
	//�{����Player��Enemy�����������͕̂�cpp�Ȃǂł܂Ƃ߂Ĉꊇ�ŌĂт���
	//�|�C���^�ϐ�������m_p�`�ƌ�ŕς���B�R�[�h�̌��₷���ӎ�����
	CPlayer* m_pPlayer;
	CStage* m_pStage;
	CTimer* m_pTimer;
	CCountEnemy* m_pCountEnemy;
	CLife* m_pLife;
	CBG* m_pBG;
	CExplosion* m_pExplosion;
	CPause* m_pPause;
	CClearPause* m_pCPause;
	CReticle* m_pReticle;
	CEffect* m_pEffect;
	CMesh* m_pMesh;
	CNumber* m_pNumber;
	CPlayer* m_Player;
	CStage* m_Stage;
	CSceneManager* m_SManager;
	CFade* m_fade;
};
