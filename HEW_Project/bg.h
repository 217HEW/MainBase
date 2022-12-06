// 背景描画 [bg.h]
#pragma once
#include "main.h"

// 関数プロトタイプ
class CBG
{
public:
	/*CBG();
	~CBG();*/

	virtual HRESULT InitBG();	// 初期化
	virtual void UninitBG();	// 終了処理
	virtual void UpdateBG();	// 更新
	virtual void DrawBG();		// 描画

private:

};

