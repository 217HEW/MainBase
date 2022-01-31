//**************************************************************
//
//	SceneGameclear.h
//	タイトルシーンのヘッダ
//
//--------------------------------------------------------------
//	製作者：小嶋悟
//--------------------------------------------------------------
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
// 関数宣言
//**************************************************************
HRESULT InitGameclear(); // 初期化
void UninitGameclear(); // 終了処理
void UpdateGameclear(); // 更新
void DrawGameclear(); // 描画
void GameclearFlag();
