//背景描画[Number.h]
#pragma once
#include "main.h"

#define NUMBER_SIZE_X 32
#define NUMBER_SIZE_Y 32

//関数のプロトタイプ
class CNumber
{
public:
	/*CNumber();
	~CNumber();*/
	virtual HRESULT InitNumber();	//初期化
	virtual void UninitNumber();	//終了処理
	virtual void DrawNumber(XMFLOAT2 vPos, unsigned, int nWidth, float fSizeX, float fSizeY);		//描画

private:

};



