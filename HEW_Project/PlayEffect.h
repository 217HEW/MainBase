//**************************************************************
//
//	PlayEffect(for Effekseer).h
//	�G�t�F�N�g�`��
//
//--------------------------------------------------------------
//	����ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//**************************************************************

#pragma once

//*************************
// �C���N���[�h��
//*************************
#include "main.h"

#include "Effekseer.h"
#include "EffekseerRendererDX11.h"

//*************************
// ���C�u�����؂�ւ�����
//*************************
#ifdef _DEBUG	
// �f�o�b�O��
#pragma comment(lib,"EffekseerD.lib")
#pragma comment(lib,"EffekseerRendererDX11D.lib")

#else
// �����[�X��
#pragma comment(lib,"EffekseerR.lib")
#pragma comment(lib,"EffekseerRendererDX11R.lib")

#endif // _DEBUG

//*************************
// �񋓑̐錾
//-------------------------
// ��EFFECT_�Z�Z�ŌĂяo��
//*************************
enum eEffect
{
	EFFECT_FIRE = 0,	// ��
	EFFECT_BURNING,		// �W�F�b�g����

	MAX_EFFECT			// �ő�o�^��
};

//*************************
// �G�t�F�N�g�N���X
//*************************
class Effect
{
public:
	Effect();
	~Effect();
	
	// �G�t�F�N�g�Ǎ���
	HRESULT Load();

//**************************************************************************
// �G�t�F�N�g�ݒu
// �����F
// @no      ->�g���G�t�F�N�g[MAX_EFFECT]
// @pos     ->�ݒu���������W
// @scale   ->�傫��
// @speed ->�Đ����x(0.0f�`)
// @rot	  ->�p�x(0.0f�`360.0f)
//**************************************************************************
	void Set(eEffect no, XMFLOAT3 pos, XMFLOAT3 scale, float speed,XMFLOAT3 rot);

	// �X�V
	void Update();

	// �`��
	void Draw();


private:
	EffekseerRendererDX11::RendererRef m_renderer;
	Effekseer::ManagerRef m_manager;	// �����G�t�F�N�g�̐���H
	Effekseer::Handle m_handle;		// �����G�t�F�N�g�̏��H
	Effekseer::EffectRef m_effect[MAX_EFFECT];	// �o�^����G�t�F�N�g�̐�
}; 
#pragma once
