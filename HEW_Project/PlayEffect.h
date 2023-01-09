//**************************************************************
//
//	PlayEffect(for Effekseer).h
//	エフェクト描画
//
//--------------------------------------------------------------
//	製作者：柴山凜太郎
//--------------------------------------------------------------
//**************************************************************

#pragma once

//*************************
// インクルード部
//*************************
#include "main.h"

#include "Effekseer.h"
#include "EffekseerRendererDX11.h"

//*************************
// ライブラリ切り替え処理
//*************************
#ifdef _DEBUG	
// デバッグ時
#pragma comment(lib,"EffekseerD.lib")
#pragma comment(lib,"EffekseerRendererDX11D.lib")

#else
// リリース時
#pragma comment(lib,"EffekseerR.lib")
#pragma comment(lib,"EffekseerRendererDX11R.lib")

#endif // _DEBUG

//*************************
// 列挙体宣言
//-------------------------
// ※EFFECT_〇〇で呼び出す
//*************************
enum eEffect
{
	EFFECT_FIRE = 0,	// 炎
	EFFECT_BURNING,		// ジェット噴射

	MAX_EFFECT			// 最大登録数
};

//*************************
// エフェクトクラス
//*************************
class Effect
{
public:
	Effect();
	~Effect();
	
	// エフェクト読込み
	HRESULT Load();

//**************************************************************************
// エフェクト設置
// 引数：
// @no      ->使うエフェクト[MAX_EFFECT]
// @pos     ->設置したい座標
// @scale   ->大きさ
// @speed ->再生速度(0.0f～)
// @rot	  ->角度(0.0f～360.0f)
//**************************************************************************
	void Set(eEffect no, XMFLOAT3 pos, XMFLOAT3 scale, float speed,XMFLOAT3 rot);

	// 更新
	void Update();

	// 描画
	void Draw();


private:
	EffekseerRendererDX11::RendererRef m_renderer;
	Effekseer::ManagerRef m_manager;	// 扱うエフェクトの制御？
	Effekseer::Handle m_handle;		// 扱うエフェクトの情報？
	Effekseer::EffectRef m_effect[MAX_EFFECT];	// 登録するエフェクトの数
}; 
#pragma once
