//**************************************************************
//
//	EnemyBarrier.h
//	�o���A�G�l�~�[����
//
//--------------------------------------------------------------
//	����ҁF������
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//
//**************************************************************
#pragma once
//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "main.h"
#include < string >

//**************************************************************
// �\���̒�`
//**************************************************************
struct T {
	XMFLOAT3	m_pos;		// ���݂̈ʒu
	//XMFLOAT3    m_size;	// ���݂̃T�C�Y
	XMFLOAT4X4	m_mtxWorld;	// ���[���h�}�g���b�N�X
	std::string	m_3Dmodel;	// ���f�����
	int			m_nLife;	// �ǂ̑ϋv�l
	bool		m_use;		// �g�p���Ă��邩
	bool		m_invincible;// ���G���ǂ����@ON�F���G
};

//**************************************************************
// �}�N����`
//**************************************************************
#define BLOCK_X			(25)	// �u���b�N�ő吔X
#define BLOCK_Y			(25)	// �u���b�N�ő吔Y
#define MAX_BLOCK		(BLOCK_X * BLOCK_Y)	// �u���b�N�ő吔

//**************************************************************
// �v���g�^�C�v�錾
//**************************************************************
HRESULT InitEnemy(void);	// ������
void UninitEnemy(void);		// �I��
void UpdateEnemy(void);		// �X�V
void DrawEnemy(void);		// �`��

int SetEnemy(XMFLOAT3 pos);	// �G�z�u

int SetBlock(XMFLOAT3 pos, bool inv);	// �u���b�N�z�u 
XMFLOAT3 GetBlockSize();	// �u���b�N�T�C�Y�擾
TBLOCK* GetBlockArray();	// �u���b�N�z��擾

////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>

using std::vector;
using std::ostream;
using std::cout;
using std::endl;

template<typename T, size_t N>
inline size_t num_elms(const T(&)[N])
{
	return N;
}

typedef struct {
	double x;
	double y;
} POINT;

/*
* Polygon �̊e�_�� ostream �ɏo�͂���
*/
ostream &printPolygon(ostream &os, const vector<POINT> &pv)
{
	vector<POINT > ::const_iterator cit;
	os << "Polygon ";
	for (cit = pv.begin(); cit != pv.end(); ++cit)
	{
		os << "(" << cit->x << ", " << cit->y << ") ";
	}
	os << endl;
	return os;
}

/*
* pt �� pv �����̓_�����肷�鏈��
*/
bool PtInPolygon(const POINT &pt, const vector<POINT> &pv)
{
	size_t ui, uj; // ui: �n�_ index, uj : �I�_ index
	int cnt = 0; // pt �_�̉E���̕ӂ̐�
	for (uj = 0; uj < pv.size(); uj++)
	{
		ui = (uj > 0) ? uj - 1 : pv.size() - 1;
		// pt �_�̉E����ʉ߂���ӂ𐔂���
		if ((pv[ui].y - pt.y) * (pv[uj].y - pt.y) < 0)
		{
			double offset = 0;
			if (pv[ui].y != pv[uj].y)
			{
				offset = (pv[uj].x - pv[ui].x) * 
					(pt.y - pv[ui].y) / (pv[uj].y - pv[ui].y);
			}
			if (pv[ui].x + offset > pt.x)
			{
				++cnt;
			}
		}
	}
	return cnt % 2 ? true : false;
}

int main(void)
{
	POINT Polygon[] = { {1, 1}, {2, 1}, {4, 3}, {1, 3} };
	vector<POINT > pv(Polygon, Polygon + num_elms(Polygon)); // ���p�` vector
	POINT pt = { 0 }; // �w��_

	// ���p�`
	printPolygon(cout, pv);
	// �w��_�����ɂ���ꍇ
	pt.x = 2.5;
	pt.y = 2.0;
	cout << "pt=(" << pt.x << ", " << pt.y << ") ��";
	cout << (PtInPolygon(pt, pv) ? "���_�ł�" : "�O�_�ł�") << endl;
	// �w��_���O�ɂ���ꍇ
	pt.x = 3.5;
	pt.y = 2.0;
	cout << "pt=(" << pt.x << ", " << pt.y << ") ��";
	cout << (PtInPolygon(pt, pv) ? "���_�ł�" : "�O�_�ł�") << endl;

	return 0;
}