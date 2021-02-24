#include "StdAfx.h"
#include "CMyMatrix.h"
#include <iostream>
#include <iomanip>
CMyMatrix CMyMatrix::average()
{
    CMyMatrix result(this->m_rows, 1);
    long double sum = 0;
    for (int i = 0; i < this->m_rows; ++i)
    {
        sum = 0;
        for (int j = 0; j < this->m_cols; ++j)
        {
            sum+= this->m_data[i * this->m_cols + j];
        }
        result.m_data[i]=sum/this->m_cols;
    }
    return result;
}

CMyMatrix CMyMatrix::operator*(CMyMatrix& B)
{
    //设(*this)为A，A为m行n列矩阵，B为n行p列矩阵
    //结果result矩阵为p行m列矩阵

    CMyMatrix& A = (*this);
    if (A.m_cols!=B.m_rows)//如果A的列数不等于B的行数，报错
        throw std::domain_error("进行矩阵乘法的两矩阵，A的行数不等于B的列数");

    int m = A.m_cols;//m是A的列数 B的行数
    int n = A.m_rows;//n是A的行数
    int p = B.m_cols;//p是B的列数
    CMyMatrix result(n,B.m_cols);
    
    //C[i][j]=A[i][k]*B[k][i]
    long double sum=0;
    for(int i=0;i< n;++i)
        for (int j = 0; j < p; ++j)
        {
            sum = 0;
            for (int k = 0; k < m; ++k)
            {
                sum += A.at(i, k) * B.at(k, j);
                //result.at(i, j) += A.at(i, k) * B.at(k, j);
            }
            result.at(i, j) = sum;//此处有截断，是和MATLAB计算结果不一致的原因
        }
    return result;
}

CMyMatrix CMyMatrix::transpose()
{
    //矩阵转置
    CMyMatrix result(this->m_cols, this->m_rows);

    //用i遍历完一维数组,利用除法和求余锁定步长
    for (int i = 0; i < this->m_cols * this->m_rows; ++i)
        result.m_data[i / this->m_cols + this->m_rows * (i % this->m_cols)] = this->m_data[i];
    return result;
}

CMyMatrix CMyMatrix::operator/(float val)
{
    CMyMatrix mtx(this->m_rows,this->m_cols);
    for (int i = 0; i < this->m_cols * this->m_rows; ++i)
        mtx.m_data[i] = this->m_data[i] / val;
    return mtx;
}

CMyMatrix CMyMatrix::operator*(float val)
{
    CMyMatrix mtx(this->m_rows, this->m_cols);

    for (int i = 0; i < this->m_cols * this->m_rows; ++i)
        mtx.m_data[i] = this->m_data[i] * val;
    return mtx;
}

CMyMatrix CMyMatrix::cov()
{
    int n_row = this->m_rows;
    int n_col = this->m_cols;

    //mtx是中间结果
    CMyMatrix mtx(n_row, n_col);

    //存放每个波段平均值的列向量
    CMyMatrix band_average = this->average();
  /*  cout << "各波段平均值" << endl;
    band_average.print();*/
    //每个波段数据减去每个波段平均值放入mtx中
    for (int band = 0; band < n_row; ++band)
    {
        for (int i = 0; i < n_col; ++i)
        {
            mtx.m_data[band * n_col + i] = this->m_data[band * n_col + i] - band_average.m_data[band];
        }
    }

    CMyMatrix mtx_T = mtx.transpose();//转置
    return mtx*mtx_T/(n_col-1);
}

void CMyMatrix::print()
{
    cout << setprecision(20);
    for (int i = 0; i < this->m_rows; ++i)
    {
        for (int j = 0; j < this->m_cols; ++j)
        {
            cout << this->m_data[i * m_cols + j] << "\t";
        }
        cout << endl;
    }
    cout << endl << endl;
}

CMyMatrix& CMyMatrix::operator=(const CMyMatrix& mtx)
{
    const_cast <CMyMatrix&>(mtx).duplicate(*this);
    return *this;
}

CMyMatrix CMyMatrix::operator+(CMyMatrix& mtx)
{
    int n_row = this->m_rows;
    int n_col = this->m_cols;

    if (n_row != mtx.m_rows || n_col != mtx.m_cols)
    {
        throw std::domain_error("AB行列数不相等");
    }

    CMyMatrix result(n_row, n_col);
    for (int i = 0; i < this->m_cols * this->m_rows; ++i)
        result.m_data[i] = this->m_data[i] + mtx.m_data[i];
    return result;
}

CMyMatrix CMyMatrix::operator+=(CMyMatrix& mtx)
{
    int n_row = this->m_rows;
    int n_col = this->m_cols;
    if (n_row != mtx.m_rows || n_col != mtx.m_cols)
    {
        throw std::domain_error("AB行列数不相等");
    }
   
    for (int i = 0; i < n_row * n_col; ++i)
        this->m_data[i]+= mtx.m_data[i];
    return (*this);

}