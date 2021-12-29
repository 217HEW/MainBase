//**************************************************************
//
//	CreateField.h
//	マップ生成
//
//--------------------------------------------------------------
//	製作者：澤村瑠人
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/20	二次元配列を用いた、マップ構築
//				生成範囲は未実装
//	編集者：澤村瑠人
//--------------------------------------------------------------
//	2021/12/22	コメントの編集&追加
//--------------------------------------------------------------
//	2021/12/28	列挙体を作ることによって
//**************************************************************
#pragma once
//**************************************************************
// インクルード部
//**************************************************************
#include "main.h"

//**************************************************************
// 列挙体宣言
//**************************************************************

// マップ配列の中身
enum E_MAP_CONTENTS
{
	__ = 0,		// ・ 何も接地しない
	LJ,			// □,壊れる壁
	NN,			// ■,無敵の壁
	゛,			// 近,近接エネミー
	｜JJ,			// 遠,遠距離エネミー
	｜KK,			// 爆,自爆型エネミー
	｜LL,			// バ,バリアエネミー

	MAX_MAP
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCField(void);	// 初期化
void UninitCField(void);	// 終了
void UpdateCField(void);	// 更新
void DrawCField(void);		// 描画