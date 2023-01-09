//**************************************************************
//
//	Life.h
//	ライフ処理
//
//--------------------------------------------------------------
//	製作者：澤村瑠人
//--------------------------------------------------------------
//**************************************************************

#pragma once
//**************************************************************
// インクルード部
//**************************************************************
#include "main.h"

//**************************************************************
// 関数プロトタイプ宣言
//**************************************************************
HRESULT InitLife(); // 初期化
void UninitLife();	// 終了処理
void UpdateLife();	// 更新
void DrawLife();	// 描画

void AddLife(int nLife = 1);	// 体力加算
void DelLife(int nLife = 1);	// 体力減算
int GetLife();		// ライフ取得