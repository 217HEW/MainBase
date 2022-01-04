//**************************************************************
//
//	collision.cpp
//	衝突判定処理
//
//--------------------------------------------------------------
//	製作者：??
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/??	
//	編集者：??
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "collision.h"

//**************************************************************
//
//	球同士の衝突判定
//
//	引数：
//			Apos:Aの座標、Ar:Aの半径、Bpos:Bの座標、Br:Bの半径
//
//	戻り値：
//			当たったかどうか
//**************************************************************
bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br)
{
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

//**************************************************************
//
//	AABB同士の衝突判定(矩形同士)
//
//	引数：
//			Apos:Aの座標、Asize:Aのサイズ、
//			Bpos:Bの座標、Bsize:Bのサイズ
//
//	戻り値：
//			当たったかどうか
//**************************************************************
bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize)
{
	return (Apos.x - Asize.x <= Bpos.x + Bsize.x) &&
		   (Bpos.x - Bsize.x <= Apos.x + Asize.x) &&
		   (Apos.y - Asize.y <= Bpos.y + Bsize.y) &&
		   (Bpos.y - Bsize.y <= Apos.y + Asize.y) &&
		   (Apos.z - Asize.z <= Bpos.z + Bsize.z) &&
		   (Bpos.z - Bsize.z <= Apos.z + Asize.z);
}
//*******************************
//
//	矩形と矩形の当たり判定
//	
//	引数:
//		矩形１の中心座標
//		矩形２の中心座標
//		矩形１のサイズ
//		矩形２のサイズ
//		矩形の縦の判定か横の判定か
//
//	戻り値
//		false：接触していない
//		true：接触している
//
//*******************************
bool CheckCollisionRectToRect(XMFLOAT3 centerPos1, XMFLOAT3 centerPos2, XMFLOAT3 size1, XMFLOAT3 size2, Way way)
{
	// モデルの半分のサイズを取得
	XMFLOAT3 halfSize1 = XMFLOAT3(size1.x / 2.0f, size1.y / 2.0f,size1.z /2.0f);
	XMFLOAT3 halfSize2 = XMFLOAT3(size2.x / 2.0f, size2.y / 2.0f, size2.z / 2.0f);
	switch (way)
	{
	case VERTICAL:
		// モデル横幅の間にいなかったら処理しない(＜左端or右端＜)
		if (centerPos1.x + halfSize1.x < centerPos2.x - halfSize2.x ||
			centerPos1.x - halfSize1.x > centerPos2.x + halfSize2.x)	//モデルの幅判定
			break;
		//=== モデル同士の非貫通判定 ===
		if (centerPos1.y - halfSize1.y < centerPos2.y + halfSize2.y)
		{
			// モデル1上端側<モデル2下端側
			centerPos1.y = (centerPos2.y + halfSize2.y) + halfSize1.y;
			return true;
		}
		if (centerPos1.y + halfSize1.y > centerPos2.y - halfSize2.y)
		{
			// モデル1下端側>モデル2上端側
			centerPos1.y = (centerPos2.y - halfSize2.y) - halfSize1.y;
			return true;
		}
		break;
	case HORIZON:
		// モデル縦幅の間にいなかったら処理しない(＞上端or下端＞)
		if (centerPos1.y - halfSize1.y > centerPos2.y + halfSize2.y ||
			centerPos1.y + halfSize1.y < centerPos2.y - halfSize2.y)
			break;
		//=== 足場側面非貫通判定 ===
		if (centerPos1.x + halfSize1.x < centerPos2.x - halfSize2.x)	//左側面
		{
			// モデル1左端側<モデル2右端側
			centerPos1.x = (centerPos2.x - halfSize2.x) - halfSize1.x;
			return true;
		}
		if (centerPos1.x - halfSize1.x > centerPos2.x + halfSize2.x)	//右側面
		{
			// モデル1右端側>モデル2左端側
			centerPos1.x = (centerPos2.x + halfSize2.x) + halfSize1.x;
			return true;
		}
		break;
	default:
		break;
	}
	
	//if ((centerPos2.x - halfSize2.x) < (centerPos1.x + halfSize1.x) &&		// 2の左端 < 1の右端 
	//	(centerPos2.x + halfSize2.x) > (centerPos1.x - halfSize1.x))		// 2の右端 > 1の左端
	//{
	//	if ((centerPos2.y + halfSize2.y) > (centerPos1.y - halfSize1.y) &&	// 2の上端 < 1の下端
	//		(centerPos2.y - halfSize2.y) < (centerPos1.y + halfSize1.y))	// 1の上端 < 2の下端
	//	{
	//		return true;
	//	}
	//}

	return false;
}
