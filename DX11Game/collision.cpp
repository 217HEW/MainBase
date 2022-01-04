//**************************************************************
//
//	collision.cpp
//	�Փ˔��菈��
//
//--------------------------------------------------------------
//	����ҁF??
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/12/??	
//	�ҏW�ҁF??
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "collision.h"

//**************************************************************
//
//	�����m�̏Փ˔���
//
//	�����F
//			Apos:A�̍��W�AAr:A�̔��a�ABpos:B�̍��W�ABr:B�̔��a
//
//	�߂�l�F
//			�����������ǂ���
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
//	AABB���m�̏Փ˔���(��`���m)
//
//	�����F
//			Apos:A�̍��W�AAsize:A�̃T�C�Y�A
//			Bpos:B�̍��W�ABsize:B�̃T�C�Y
//
//	�߂�l�F
//			�����������ǂ���
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
//	��`�Ƌ�`�̓����蔻��
//	
//	����:
//		��`�P�̒��S���W
//		��`�Q�̒��S���W
//		��`�P�̃T�C�Y
//		��`�Q�̃T�C�Y
//		��`�̏c�̔��肩���̔��肩
//
//	�߂�l
//		false�F�ڐG���Ă��Ȃ�
//		true�F�ڐG���Ă���
//
//*******************************
bool CheckCollisionRectToRect(XMFLOAT3 centerPos1, XMFLOAT3 centerPos2, XMFLOAT3 size1, XMFLOAT3 size2, Way way)
{
	// ���f���̔����̃T�C�Y���擾
	XMFLOAT3 halfSize1 = XMFLOAT3(size1.x / 2.0f, size1.y / 2.0f,size1.z /2.0f);
	XMFLOAT3 halfSize2 = XMFLOAT3(size2.x / 2.0f, size2.y / 2.0f, size2.z / 2.0f);
	switch (way)
	{
	case VERTICAL:
		// ���f�������̊Ԃɂ��Ȃ������珈�����Ȃ�(�����[or�E�[��)
		if (centerPos1.x + halfSize1.x < centerPos2.x - halfSize2.x ||
			centerPos1.x - halfSize1.x > centerPos2.x + halfSize2.x)	//���f���̕�����
			break;
		//=== ���f�����m�̔�ђʔ��� ===
		if (centerPos1.y - halfSize1.y < centerPos2.y + halfSize2.y)
		{
			// ���f��1��[��<���f��2���[��
			centerPos1.y = (centerPos2.y + halfSize2.y) + halfSize1.y;
			return true;
		}
		if (centerPos1.y + halfSize1.y > centerPos2.y - halfSize2.y)
		{
			// ���f��1���[��>���f��2��[��
			centerPos1.y = (centerPos2.y - halfSize2.y) - halfSize1.y;
			return true;
		}
		break;
	case HORIZON:
		// ���f���c���̊Ԃɂ��Ȃ������珈�����Ȃ�(����[or���[��)
		if (centerPos1.y - halfSize1.y > centerPos2.y + halfSize2.y ||
			centerPos1.y + halfSize1.y < centerPos2.y - halfSize2.y)
			break;
		//=== ���ꑤ�ʔ�ђʔ��� ===
		if (centerPos1.x + halfSize1.x < centerPos2.x - halfSize2.x)	//������
		{
			// ���f��1���[��<���f��2�E�[��
			centerPos1.x = (centerPos2.x - halfSize2.x) - halfSize1.x;
			return true;
		}
		if (centerPos1.x - halfSize1.x > centerPos2.x + halfSize2.x)	//�E����
		{
			// ���f��1�E�[��>���f��2���[��
			centerPos1.x = (centerPos2.x + halfSize2.x) + halfSize1.x;
			return true;
		}
		break;
	default:
		break;
	}
	
	//if ((centerPos2.x - halfSize2.x) < (centerPos1.x + halfSize1.x) &&		// 2�̍��[ < 1�̉E�[ 
	//	(centerPos2.x + halfSize2.x) > (centerPos1.x - halfSize1.x))		// 2�̉E�[ > 1�̍��[
	//{
	//	if ((centerPos2.y + halfSize2.y) > (centerPos1.y - halfSize1.y) &&	// 2�̏�[ < 1�̉��[
	//		(centerPos2.y - halfSize2.y) < (centerPos1.y + halfSize1.y))	// 1�̏�[ < 2�̉��[
	//	{
	//		return true;
	//	}
	//}

	return false;
}
