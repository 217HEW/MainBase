//**************************************************************
//
//	SceneGameover.h
//	ゲームオーバーシーン処理
//
//--------------------------------------------------------------
//	製作者：小嶋悟
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/06	小嶋悟君のプログラムを元に作成	
//
//**************************************************************
#pragma once

//**************************************************************
// インクルード部
//**************************************************************
#include "main.h"

//**************************************************************
// プロトタイプ宣言
//**************************************************************
HRESULT InitGameover(); // 初期化
void UninitGameover(); // 終了処理
void UpdateGameover(); // 更新
void DrawGameover(); // 描画