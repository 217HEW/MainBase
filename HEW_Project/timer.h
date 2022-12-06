// 画面遷移 [timer.h]
#pragma once
#include "main.h"

// 関数プロトタイプ
class CTimer
{
public:
	/*Timer();
	~Timer();*/
	virtual HRESULT InitTimer(); // 初期化
	virtual void UninitTimer(); // 終了処理
	virtual void UpdateTimer(); // 更新
	virtual void DrawTimer(); // 描画

private:

};


void AddTimer(int nSec = 30);// 切替_使ってない
int GetTimer();