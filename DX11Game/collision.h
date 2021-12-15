// è’ìÀîªíË [collision.h]

#pragma once
#include "main.h"

bool CollisionSphere(XMFLOAT3 Apos, float Ar, XMFLOAT3 Bpos, float Br);
bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize);
//bool CheckCollisionRectToRect(XMFLOAT3 centerPos1, XMFLOAT3 centerPos2, XMFLOAT2 size1, XMFLOAT2 size2);