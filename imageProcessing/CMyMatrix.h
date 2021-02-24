#pragma once

#include "RSDIPLib.h"
#include <stdexcept>

using namespace std;
class CMyMatrix :public CMatrix
{
public:
	//构造函数
	CMyMatrix(CMatrix& mtx) { mtx.duplicate(*this); }
	CMyMatrix(CMyMatrix& mtx) { mtx.duplicate(*this); }
	CMyMatrix(CMyMatrix&& mtx) { mtx.duplicate(*this); }
	CMyMatrix(int n_row, int n_col) { this->create(n_row, n_col); }
	CMyMatrix() { }
	
	//重载乘号
	CMyMatrix operator*(CMyMatrix& mtx);

	//重载加号
	CMyMatrix operator+(CMyMatrix& mtx);

	//重载加号等于
	CMyMatrix operator+=(CMyMatrix& mtx);

	//转置
	CMyMatrix transpose();

	//赋值
	CMyMatrix& operator=(const CMyMatrix& mtx);

	//除法
	CMyMatrix operator/(float val);

	//乘法
	CMyMatrix operator*(float val);

	//计算协方差矩阵
	CMyMatrix cov();

	void print();//打印矩阵，用于调试

private:
	//寻址
	inline float& at(int row, int col) { return this->m_data[row*this->m_cols+col]; }

	//计算各行平均值，返回为列向量
	CMyMatrix average();
};

