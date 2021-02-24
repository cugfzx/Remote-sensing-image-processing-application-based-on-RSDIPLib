#pragma once

#include "RSDIPLib.h"
#include <stdexcept>

using namespace std;
class CMyMatrix :public CMatrix
{
public:
	//���캯��
	CMyMatrix(CMatrix& mtx) { mtx.duplicate(*this); }
	CMyMatrix(CMyMatrix& mtx) { mtx.duplicate(*this); }
	CMyMatrix(CMyMatrix&& mtx) { mtx.duplicate(*this); }
	CMyMatrix(int n_row, int n_col) { this->create(n_row, n_col); }
	CMyMatrix() { }
	
	//���س˺�
	CMyMatrix operator*(CMyMatrix& mtx);

	//���ؼӺ�
	CMyMatrix operator+(CMyMatrix& mtx);

	//���ؼӺŵ���
	CMyMatrix operator+=(CMyMatrix& mtx);

	//ת��
	CMyMatrix transpose();

	//��ֵ
	CMyMatrix& operator=(const CMyMatrix& mtx);

	//����
	CMyMatrix operator/(float val);

	//�˷�
	CMyMatrix operator*(float val);

	//����Э�������
	CMyMatrix cov();

	void print();//��ӡ�������ڵ���

private:
	//Ѱַ
	inline float& at(int row, int col) { return this->m_data[row*this->m_cols+col]; }

	//�������ƽ��ֵ������Ϊ������
	CMyMatrix average();
};

