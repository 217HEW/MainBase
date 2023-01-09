//**************************************************************
//
//	PlayEffect(for Effekseer).cpp
//	�G�t�F�N�g�`��
//
//--------------------------------------------------------------
//	����ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//��������������������������������������������������������������
// PlayEffect.cpp &.h �̎g������Effect�e�L�X�g�����邱��
//��������������������������������������������������������������
//**************************************************************

//********************
// �C���N���[�h��
//********************
#include "PlayEffect.h"
#include "Camera.h"
#include <string>


// �g�������G�t�F�N�g�̓o�^
const char16_t* g_EffectData[MAX_EFFECT] = {
	u"data/effect/fire.efk",
	u"data/effect/burning.efk",
};

Effect::Effect()
{

}

Effect::~Effect()
{

}

//********************
// �G�t�F�N�g�Ǎ���
//********************
HRESULT Effect::Load()
{
	// Effekseer������
	m_renderer = EffekseerRendererDX11::Renderer::Create(GetDevice(), GetDeviceContext(), 8000);
	m_manager = Effekseer::Manager::Create(8000);

	// �`�惂�W���[��
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());	// �X�v���C�g
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());	// ���{��
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());		// �����O
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());		// �g���b�N

	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());		// ���f��

	// �f�[�^�Z�b�g
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());		// �e�N�X�`��
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());			// ���f��
	m_manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());	// �}�e���A��
	m_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());// �J�[�u

	// �g���G�t�F�N�g�̓Ǎ���
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		m_effect[i] = Effekseer::Effect::Create(m_manager, g_EffectData[i]);
	}

	return S_OK;
}

//**********************
// �G�t�F�N�g�X�V
//**********************
void Effect::Update()
{
	CCamera* pCamera = CCamera::Get();

	XMFLOAT3 pos = pCamera->GetPos();
	XMFLOAT3 target = pCamera->GetTargetPos();
	XMFLOAT3 upp = pCamera->GetUp();
	Effekseer::Vector3D eye = ::Effekseer::Vector3D(pos.x, pos.y, pos.z);
	Effekseer::Vector3D look = ::Effekseer::Vector3D(target.x, target.y, target.z);
	Effekseer::Vector3D up = ::Effekseer::Vector3D(upp.x, upp.y, upp.z);

	// ���e�s��
	m_renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(pCamera->GetFov(), pCamera->GetAspect(), pCamera->GetNearClip(), pCamera->GetFarClip()));

	// �J�����s���ݒ�
	m_renderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtLH(eye, look, up));
}

//************************
// �G�t�F�N�g�`��
//************************
void Effect::Draw()
{

	SetZBuffer(false);

	static int time = 0;

	m_manager->Update();
	m_renderer->SetTime(time++ / 120);
	m_renderer->BeginRendering();
	m_manager->Draw();
	m_renderer->EndRendering();
	SetZBuffer(true);
}


//**************************************************************************
// �G�t�F�N�g�`��
//--------------------------------------------------------------------------
// �����F
// @no      ->�g���G�t�F�N�g[MAX_EFFECT]
// @pos     ->�ݒu���������W
// @scale   ->�傫��
// @speed ->�Đ����x(0.0f�`)
// @rot	  ->�p�x(0.0f�`360.0f)
//**************************************************************************
void Effect::Set(eEffect no,XMFLOAT3 pos,XMFLOAT3 scale,float speed,XMFLOAT3 rot)
{
	m_handle = m_manager->Play(m_effect[no], pos.x, pos.y, pos.z);
	m_manager->SetScale(m_handle, scale.x, scale.y, scale.z);
	m_manager->SetSpeed(m_handle, speed);
	m_manager->SetRotation(m_handle, rot.x, rot.y,rot.z);
}