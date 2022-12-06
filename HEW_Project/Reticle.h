//=============================================================================
//
// レティクル処理 [Reticle.h]
// Author : 上月大地
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
class CReticle
{
public:
	/*CReticle();
	~CReticle();*/

	virtual HRESULT InitReticle(void);
	virtual void UninitReticle(void);
	virtual void UpdateReticle(void);
	virtual void DrawReticle(void);

private:

};




// 照準設置
int SetReticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT2 size, int timer);