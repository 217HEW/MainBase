//**************************************************************
//
//	PlayEffect(for Effekseer).cpp
//	エフェクト描画
//
//--------------------------------------------------------------
//	製作者：柴山凜太郎
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2022/01/21	　エフェクシアのエフェクトの組み込み完了
//--------------------------------------------------------------
//**************************************************************

//********************
// インクルード部
//********************
#include "PlayEffect.h"
#include "Camera.h"
#include <string>

Effect::Effect()
{

}

Effect::~Effect()
{

}

//********************
// エフェクト読込み
//********************
HRESULT Effect::Load()
{
	// Effekseer初期化
	m_renderer = EffekseerRendererDX11::Renderer::Create(GetDevice(), GetDeviceContext(), 8000);
	m_manager = Effekseer::Manager::Create(8000);

	// 描画モジュール
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());	// スプライト
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());	// リボン
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());		// リング
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());		// トラック

	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());		// モデル

	// データセット
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());		// テクスチャ
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());			// モデル
	m_manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());	// マテリアル
	m_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// 使うエフェクトの登録
	m_effect[0] = Effekseer::Effect::Create(m_manager, u"data/effect/fire.efk");
	m_effect[1] = Effekseer::Effect::Create(m_manager, u"data/effect/burning.efk");

	return S_OK;
}

//**********************
// エフェクト更新
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

	// 投影行列
	m_renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(pCamera->GetFov(), pCamera->GetAspect(), pCamera->GetNearClip(), pCamera->GetFarClip()));

	// カメラ行列を設定
	m_renderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtLH(eye, look, up));
}

//************************
// エフェクト描画
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
// エフェクト描画
//--------------------------------------------------------------------------
// 引数：
// @no      ->使うエフェクト[MAX_EFFECT]
// @pos     ->設置したい座標
// @scale   ->大きさ
// @speed ->再生速度
// @rot	  ->角度
//**************************************************************************
void Effect::Set(int no,XMFLOAT3 pos,XMFLOAT3 scale,float speed,XMFLOAT3 rot)
{
	m_handle = m_manager->Play(m_effect[no], pos.x, pos.y, pos.z);
	m_manager->SetScale(m_handle, scale.x, scale.y, scale.z);
	m_manager->SetSpeed(m_handle, speed);
	m_manager->SetRotation(m_handle, rot.x, rot.y,rot.z);
}