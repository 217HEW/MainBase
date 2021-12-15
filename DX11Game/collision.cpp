// 衝突判定 [collision.cpp]

#include "collision.h"

// 球同士の衝突判定
bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br)
{
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

// AABB同士の衝突判定
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
//
//	戻り値
//		false：接触していない
//		true：接触している
//
//*******************************
//bool CheckCollisionRectToRect(XMFLOAT3 centerPos1, XMFLOAT3 centerPos2, XMFLOAT3 size1, XMFLOAT3 size2)
//{
//	XMFLOAT3 halfSize1 = XMFLOAT3(size1.x / 2.0f, size1.y / 2.0f,size1.z /2.0f);
//	XMFLOAT3 halfSize2 = XMFLOAT3(size2.x / 2.0f, size2.y / 2.0f, size2.z / 2.0f);
//
//	if ((centerPos2.x - halfSize2.x) < (centerPos1.x + halfSize1.x) &&		// 2の左端 < 1の右端 
//		(centerPos2.x + halfSize2.x) > (centerPos1.x - halfSize1.x))		// 2の右端 > 1の左端
//	{
//		if ((centerPos2.y + halfSize2.y) > (centerPos1.y - halfSize1.y) &&	// 2の上端 < 1の下端
//			(centerPos2.y - halfSize2.y) < (centerPos1.y + halfSize1.y))	// 1の上端 < 2の下端
//		{
//			return true;
//		}
//	}
//
//	return false;
//}
