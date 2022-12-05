//**************************************************************
//
//	CreateField.cpp
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
//	2021/12/21 マップチップ完成
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//　2021/12/22	マップチップのswith管理に変更
//　編集者：澤村瑠人
//　		コメントの編集&追加、switch文に「case 0」の追加
//　		何も処理せずにとばす
//			マップチップの縦横を定数で宣言
//　編集者：柴山凜太郎
//--------------------------------------------------------------
//　2021/12/24	ブロックサイズが統一化されたの為、61行にあった
//				サイズに関するコメントを削除しました。
//				マップサイズを25*25に変更しました
//　編集者：上月大地
//--------------------------------------------------------------
//　2021/12/28	エリアの量産のための改良開始
//				g_Mapの要素にエリアを切り替える要素の追加
//				エリアごとのマップチップ(仮)の作成
//				列挙型のグローバル変数を作成
//				InitCFieldに新しくエリアの切り替え用引数を追加
//				エリア状態を取得する関数の作成
//				エリア量産のための改良完了
//　編集者：柴山凜太郎
//--------------------------------------------------------------
//　2022/01/17	ブロックの仕様変更によりスイッチ文を変更しました
//				実装最優先に取り組んだ所、魔境になってしまいました
//				他人が見たら解読不可能な可能性があります。
//				ごめんなさい
//	編集者：上月大地
//--------------------------------------------------------------
//　2022/01/27	プレイヤーをエリアごとに設置出来るように関数を
//				追加しました
//	編集者：上月大地
//--------------------------------------------------------------
//　2022/01/28	近接敵が現環境で動くように引数(ID)を削除しました。
//	編集者：柴山凜太郎
//**************************************************************

//**************************************************************
//	コメント（意見）
//	2021/12/22	10 * 10 だと物足りないかも？	__澤村瑠人
//	2021/12/22	25 * 25 がレベデザの要望です	＿上月大地
//	2022/12/06	プレイヤーと敵キャラ全部ここでよんだらいかんのか？
//
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "CreateField.h"
#include "input.h"
#include "AssimpModel.h"
#include "Block.h"
#include "EnemyMelee.h"
#include "EnemyRange.h"
#include "player.h"
#include "CountEnemy.h"

#include "SceneGame.h"
//**************************************************************
// 列挙体宣言
//**************************************************************
enum CBLOCK
{	// ブロック呼び出しで使う
	// X or Y _??(ブロック何個分)_右 左(そのブロックが何処に寄っているか)

// 通常ブロック==================
	A = 10,			// X_6_右
	a,				// X_6
	B,				// X_6.5_左
	C,				// X_6.5_右
	D,				// X_7
	E,				// X_7.5_左
	F,				// X_7.5_右
	G,				// X_8_右
	g,				// X_8
	H,				// X_9.5_左
	J,				// X_9.5_右
	K,				// X_10
	L,				// X_11.5_左
	M,				// X_11.5_右

// 無敵ブロック===================
	// 縦
	N,				// Y_3
	P,				// Y_4
	R,				// Y_6
	S,				// Y_9
	// 横
	T,				// X_5
	U,				// X_6_右
	V,				// X_6
	W,				// X_7
	X,				// X_9.5_左
	Y,				// X_10
	Z,				// X_19

	// 急遽増えてしまったもの
	_,				// X_4
	I,				// X_4.5_左
	O,				// X_4.5_右
	Q,				// X_5_右
	e,				// 近接敵
	m,
	r,
	p,
	MAX
};

//**************************************************************
// マクロ定義
//**************************************************************
#define MAP_WIDTH  (25)	
#define MAP_HEIGHT (25)	

//**************************************************************
// グローバル変数
//**************************************************************

//関数化してスケールサイズを指定できるようにする
int g_Map[MAX_AREA][MAP_WIDTH][MAP_HEIGHT] =
{
	//--------------------------------------------------
	//					デバッグエリア
	//--------------------------------------------------
	//{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// S,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,e,0,0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,5,0,e,6,e,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,e,0,0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// S,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	// 0,0,0,0,0,0,0,0,0,Z,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	//},

	//--------------------------------------------------
	//					エリア１
	//--------------------------------------------------
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,Z,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 S,0,0,0,0,0,6,0,0,0,0,0,6,0,0,e,0,0,S,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,p,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,Z,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	},

	//--------------------------------------------------
	//					エリア２
	//--------------------------------------------------
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,////
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,////
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,////
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,H,0,0,0,0,0,0,0,0,0,J,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,////
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 4,0,0,0,0,e,0,0,0,4,0,0,0,e,0,0,0,0,4,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,G,0,0,0,0,0,0,0,0,G,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,0,////
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 P,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,//
	 0,0,0,p,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,X,0,0,0,0,0,0,0,0,0,J,0,0,0,0,0,0,0,0,0,0,0,//
	},

	//--------------------------------------------------
	//					エリア３
	//--------------------------------------------------
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,////
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,////
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,////
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,B,0,0,0,0,0,0,a,0,0,0,0,0,0,C,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,////
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,e,0,0,0,0,0,e,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 6,0,0,0,0,0,6,0,0,0,0,0,6,0,0,0,0,0,6,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,////
	 0,0,0,e,0,0,0,0,0,0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,0,0,0,0,0,0,0,p,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 0,0,B,0,0,0,0,0,0,V,0,0,0,0,0,0,C,0,0,0,0,0,0,0,0,//
	},

	//--------------------------------------------------
	//					エリア４
	//--------------------------------------------------
	 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,Z,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,r,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  R,0,r,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,R,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,8,0,0,0,0,0,a,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,p,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,W,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	 },

	//--------------------------------------------------
	//					エリア５
	//--------------------------------------------------
	 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,E,0,0,0,0,0,_,0,0,0,0,0,F,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,r,0,0,0,0,0,0,r,0,0,0,0,0,0,r,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  6,0,0,0,0,0,0,6,0,0,0,6,0,0,0,0,0,0,6,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,r,0,0,0,0,0,0,0,0,0,0,0,0,0,r,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,p,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,E,0,0,0,0,0,_,0,0,0,0,0,F,0,0,0,0,0,0,0,0,0,//
	 },

	//--------------------------------------------------
	//					エリア６
	//--------------------------------------------------
	 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,I,0,0,0,0,0,Q,0,0,0,0,Q,0,0,0,0,O,0,0,0,0,0,0,0,//
	  0,e,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  1,0,0,0,1,0,0,e,0,1,0,0,0,0,1,e,0,0,1,0,0,0,0,0,0,//
	  0,I,0,0,0,0,0,0,0,Y,0,0,0,0,0,0,0,O,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  2,0,0,0,2,0,e,0,0,0,0,0,e,0,2,0,0,0,2,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,p,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,I,0,0,0,0,0,0,0,Y,0,0,0,0,0,0,0,O,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  1,0,0,e,1,0,0,0,0,1,0,0,0,0,1,0,0,e,1,0,0,0,0,0,0,//
	  0,I,0,0,0,0,0,Q,0,0,0,0,Q,0,0,0,0,O,0,0,0,0,0,0,0,//
	 },

	//--------------------------------------------------
	//					エリア７
	//--------------------------------------------------
	 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,8,0,0,0,0,0,0,0,0,M,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,e,0,0,0,0,0,0,0,r,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,//
	  R,r,0,0,0,0,3,0,0,e,0,0,e,0,0,e,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,8,0,0,0,0,0,0,0,0,M,0,0,0,0,0,0,0,0,0,0,0,0,//
	  0,0,r,0,0,0,0,0,0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,r,0,0,0,0,0,0,0,//
	  4,0,0,0,e,0,0,0,0,0,0,0,3,0,0,0,0,0,R,0,0,0,0,0,0,//
	  0,0,0,0,0,0,0,0,0,p,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,//
	  0,0,0,0,0,0,L,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,//
	 },

	//--------------------------------------------------
	//					エリア８
	//--------------------------------------------------
	  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,D,0,0,0,0,0,T,0,0,0,0,0,D,0,0,0,0,0,0,0,0,0,//
	   0,r,0,0,0,e,0,0,0,0,0,0,0,e,0,0,0,r,0,0,0,0,0,0,0,//
	   2,0,0,0,e,0,0,0,0,0,0,0,0,0,e,0,0,0,2,0,0,0,0,0,0,//
	   0,0,0,8,0,0,0,0,0,a,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   2,0,0,e,0,0,N,0,0,0,0,0,N,0,0,e,0,0,2,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,p,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,8,0,0,0,0,0,a,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,//
	   2,0,0,0,e,0,0,0,0,0,0,0,0,0,e,0,0,0,2,0,0,0,0,0,0,//
	   0,r,0,0,0,e,0,0,0,0,0,0,0,e,0,0,0,r,0,0,0,0,0,0,0,//
	   0,0,0,D,0,0,0,0,0,T,0,0,0,0,0,D,0,0,0,0,0,0,0,0,0,//
	  },

	//--------------------------------------------------
	//					エリア９
	//--------------------------------------------------
	  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,D,0,0,0,0,0,T,0,0,0,0,0,D,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   2,0,0,e,0,0,0,0,0,0,0,0,0,0,0,e,0,0,2,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,e,0,e,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   N,0,e,0,0,0,e,0,0,0,0,0,e,0,0,0,e,0,N,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   2,0,0,e,0,0,0,0,0,0,0,0,0,0,0,e,0,0,2,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,p,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,D,0,0,0,0,0,T,0,0,0,0,0,D,0,0,0,0,0,0,0,0,0,//
	  },

	//--------------------------------------------------
	//					エリア１０
	//--------------------------------------------------
	  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,A,0,0,0,0,0,A,0,0,0,0,0,U,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,r,0,0,0,0,0,0,0,r,0,0,0,0,0,0,0,0,0,0,0,//
	   P,0,r,0,0,0,0,0,0,0,0,0,0,0,0,0,r,0,0,0,0,0,0,0,0,//
	   0,0,0,0,e,0,0,0,0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,e,0,0,0,0,0,0,0,0,0,0,0,e,0,0,5,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,e,0,0,0,e,0,0,0,0,0,0,0,0,0,0,0,0,0,//
	   5,0,r,0,0,0,0,0,0,0,0,0,0,0,0,0,r,0,0,0,0,0,0,0,0,//
	   0,0,0,0,e,0,0,0,0,0,0,0,0,0,e,0,0,0,0,0,0,0,0,0,0,//
	   0,0,0,0,0,0,0,0,0,p,0,0,0,0,0,0,0,0,P,0,0,0,0,0,0,//
	   0,0,0,U,0,0,0,0,0,A,0,0,0,0,0,A,0,0,0,0,0,0,0,0,0,//
	  },
};

static XMFLOAT3 g_MapPosOrizin = XMFLOAT3(-40.0f, 130.0f, 0.0f);	// マップ生成開始座標
AREA eArea;		// エリア切り替え用変数
//=============================================================================
// 初期化処理
// 引数：エリアの切り替え
//=============================================================================
HRESULT InitCField(AREA Area)
{
	HRESULT hr = S_OK;
	
	// ブロック初期化
	hr = InitBlock();
	if (FAILED(hr))
		return hr;
	
	// 変数の初期化
	eArea = Area;

	// ブロックのサイズ取得
	XMFLOAT3 BlockSize = GetBlockSize();

	
	//配列の中身の処理
	//二次元配列Map内で、"1"の場所に描画する
	//描画するオブジェクトの関数を呼び出す
	for (int Height = 0; Height < MAP_HEIGHT; Height++)
	{
		for (int Width = 0; Width < MAP_WIDTH; Width++)
		{
			
			switch (g_Map[eArea][Height][Width])
			{
			//	何もしない
			case 0:{
				// 何もしない
				break;} 
			// =============== 縦
			//	通常Y_2
			case 1:{
				//マップチップ"1"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) *2,
					g_MapPosOrizin.z), false,XMFLOAT2(0.5f, 2.0f), XMFLOAT2(1.0f, 11.0f));
				break; }
			//	通常Y_3
			case 2:{
				//マップチップ"2"の場所に描画するもの

				// 無敵ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) *2,
					g_MapPosOrizin.z), false, XMFLOAT2(0.5f, 3.0f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	通常Y_4
			case 3:{
				//マップチップ"3"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(0.5f, 4.0f), XMFLOAT2(1.0f, 11.0f));
				break; }
			//	通常Y_5
			case 4:{
				//マップチップ"4"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(0.5f, 5.0f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	通常Y_6	
			case 5:{
				//マップチップ"5"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(0.5f, 6.0f), XMFLOAT2(1.0f, 11.0f));
				break; }
			//	通常Y_9
			case 6:{
				//マップチップ"6"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(0.5f, 9.0f), XMFLOAT2(1.0f, 1.0f));
				break; }
			// =============== 横
			//	通常X_5.0
			case 7:{
				//マップチップ"7"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(5.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	通常X_5.5(左)
			case 8:{
				//マップチップ"8"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(5.5f, 0.5f), XMFLOAT2(3.5f, 1.0f));
				break; }
			//	通常X_5.5(右)
			case 9:{
				//マップチップ"1"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(5.5f, 0.5f), XMFLOAT2(-1.5f, 1.0f));
				break; }
			//	通常X_6.0(右)
			case A:{
				//マップチップ"A"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(6.0f, 0.5f), XMFLOAT2(-4.0f, 1.0f));
				break; }
			//	通常X_6.0(中)
			case a: {
				//マップチップ"a"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(6.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	通常X_6.5(左)
			case B:{
				//マップチップ"B"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(6.5f, 0.5f), XMFLOAT2(8.25f, 1.0f));
				break; }
			//	通常X_6.5(右)
			case C:{
				//マップチップ"C"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(6.5f, 0.5f), XMFLOAT2(-6.25f, 1.0f));
				break; }
			//	通常X_7.0
			case D:{
				//マップチップ"D"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(7.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	通常X_7.5(左)
			case E:{
				//マップチップ"E"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(7.5f, 0.5f), XMFLOAT2(3.5f, 1.0f));
				break; }
			//	通常X_7.5(右)
			case F:{
				//マップチップ"F"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(7.5f, 0.5f), XMFLOAT2(-1.5f, 1.0f));
				break; }
			//	通常X_8.0(右)
			case G:{
				//マップチップ"G"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(8.0f, 0.5f), XMFLOAT2(-4.0f, 1.0f));
				break; }
			//	通常X_8.0(中)
			case g:{
				//マップチップ"G"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(8.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	通常X_9.5(左)
			case H:{
				//マップチップ"H"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(9.5f, 0.5f), XMFLOAT2(4.0f, 1.0f));
				break;}
			//	通常X_9.5(右)
			case J:{
				//マップチップ"J"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(9.5f, 0.5f), XMFLOAT2(-1.5f, 1.0f));
				break; }
			//	通常X_10.0
			case K:{
				//マップチップ"K"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(10.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	通常X_11.5(左)
			case L:{
				//マップチップ"L"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(11.5f, 0.5f), XMFLOAT2(3.5f, 1.0f));
				break; }
			//	通常X_11.5(右)
			case M:{
				//マップチップ"M"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(11.5f, 0.5f), XMFLOAT2(-1.5f, 1.0f));
				break; }
			// =============== 無敵
			// =============== 縦
			//	無敵Y_3
			case N:{
				//マップチップ"N"の場所に描画するもの

				// 無敵ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(0.5f, 3.0f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	無敵Y_4
			case P:{
				//マップチップ"P"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(0.5f, 4.0f), XMFLOAT2(1.0f, 11.0f));
				break; }
			//	無敵Y_6	
			case R: {
				//マップチップ"5"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(0.5f, 6.0f), XMFLOAT2(1.0f, 11.0f));
				break; }
			//	無敵Y_9
			case S: {
				//マップチップ"6"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(0.5f, 9.0f), XMFLOAT2(1.0f, 1.0f));
				break; }
			// =============== 横
			//	無敵X_5.0
			case T: {
				//マップチップ"T"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(5.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	無敵X_6.0(右)
			case U: {
				//マップチップ"A"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(6.0f, 0.5f), XMFLOAT2(-4.0f, 1.0f));
				break; }
			//	無敵X_6.0(中)
			case V: {
				//マップチップ"a"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(6.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	無敵X_7.0
			case W: {
				//マップチップ"D"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(7.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	無敵X_9.5(左)
			case X: {
				//マップチップ"X"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(9.5f, 0.5f), XMFLOAT2(3.5f, 1.0f));
				break; }
			//	無敵X_10.0(中)
			case Y: {
				//マップチップ"Y"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(10.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	無敵X_19.0
			case Z:{
				//マップチップ"N"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(19.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			// =============== 他(急遽増やしたもの)
			//	無敵X_4.0
			case _:{
				//マップチップ"N"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), true, XMFLOAT2(4.0f, 0.5f), XMFLOAT2(1.0f, 1.0f));
				break; }
			//	通常X_4.5(左)
			case I:{
				//マップチップ"N"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(4.5f, 0.5f), XMFLOAT2(8.0f, 1.0f));
				break;}
			//	通常X_4.5(右)
			case O:{
				//マップチップ"N"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(4.5f, 0.5f), XMFLOAT2(-6.25f, 1.0f));
				break; }
			//	通常X_5.0(右)
			case Q:{
				//マップチップ"Q"の場所に描画するもの

				// 通常ブロック
				SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width  * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z), false, XMFLOAT2(5.0f, 0.5f), XMFLOAT2(-4.0f, 1.0f));
				break; }
			//	近接敵
			case e: {
				//マップチップ"e"の場所に描画するもの
				AddCountEnemy();
				//	近接敵
				SetEnemyMelee(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z),0);
				break; }
			case m: {
				//マップチップ"e"の場所に描画するもの
				AddCountEnemy();
				//	近接敵
				SetEnemyMelee(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z),1);
				break; }
			case r: {
				//マップチップ"r"の場所に描画するもの
				AddCountEnemy();
				// 遠距離敵にする
				SetEnemyRange(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x),
					g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
					g_MapPosOrizin.z),0);
				break; }
			case p: {
				//マップチップ"p"の場所に描画するもの

				// プレイヤー
				SetPlayer(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x),
					(g_MapPosOrizin.y - (Height * BlockSize.y) * 2) -3.0f,
					0.0f));
				break; }
			//遠距離敵
			//case f: {
			//	//マップチップ"f"の場所に描画するもの
					AddCountEnemy();

			//	//遠距離敵
			//	SetEnemyRange(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x),
			//		g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
			//		g_MapPosOrizin.z), 0);
			//	break; }
			//case h: {
			//	//マップチップ"h"の場所に描画するもの
					AddCountEnemy();

			//	//遠距離敵
			//	SetEnemyRange(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x),
			//		g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
			//		g_MapPosOrizin.z), 1);
			//	break; }
			//case i: {
			//	//マップチップ"i"の場所に描画するもの
					AddCountEnemy();

			//	//遠距離敵
			//	SetEnemyRange(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x),
			//		g_MapPosOrizin.y - (Height * BlockSize.y) * 2,
			//		g_MapPosOrizin.z), 2);
			//	break; }

			default:
				break;
			}

			//if (Map[Height][Width] == 1)
			//{
			//	
			//	/*SetBlock(XMFLOAT3(g_MapPosOrizin.x + (Width * BlockSize.x), 
			//					  g_MapPosOrizin.y + (Height * BlockSize.y),
			//					  g_MapPosOrizin.z));*/
			//	
			//}
		}
	}

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCField(void)
{
	// ブロック終了
	UninitBlock();

	// 近接敵終了
	UninitEnemyMelee();

	// 遠隔敵終了
	UninitEnemyRange();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCField(void)
{
	
	// ブロック更新
	UpdateBlock();

	// 近接敵更新
	UpdateEnemyMelee();

	// 遠隔敵更新
	UpdateEnemyRange();
}

//=============================================================================
// 描画処理
// やりたいこと：マップチップを三次元空間に500 * 500で描画したい
// 出来ていないこと：座標の割り当て？
//=============================================================================
void DrawCField(void)
{
	// ブロック描画
	DrawBlock();

	// 近接敵描画
	DrawEnemyMelee();

	// 遠隔敵描画
	DrawEnemyRange();
}

//=============================================================================
// エリア状態の取得
// 戻り値：現在のエリア
//=============================================================================
AREA GetAreaState()
{
	return eArea;
}
