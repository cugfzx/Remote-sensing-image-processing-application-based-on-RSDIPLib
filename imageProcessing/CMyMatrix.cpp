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
    //��(*this)ΪA��AΪm��n�о���BΪn��p�о���
    //���result����Ϊp��m�о���

    CMyMatrix& A = (*this);
    if (A.m_cols!=B.m_rows)//���A������������B������������
        throw std::domain_error("���о���˷���������A������������B������");

    int m = A.m_cols;//m��A������ B������
    int n = A.m_rows;//n��A������
    int p = B.m_cols;//p��B������
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
            result.at(i, j) = sum;//�˴��нضϣ��Ǻ�MATLAB��������һ�µ�ԭ��
        }
    return result;
}

CMyMatrix CMyMatrix::transpose()
{
    //����ת��
    CMyMatrix result(this->m_cols, this->m_rows);

    //��i������һά����,���ó�����������������
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

    //mtx���м���
    CMyMatrix mtx(n_row, n_col);

    //���ÿ������ƽ��ֵ��������
    CMyMatrix band_average = this->average();
  /*  cout << "������ƽ��ֵ" << endl;
    band_average.print();*/
    //ÿ���������ݼ�ȥÿ������ƽ��ֵ����mtx��
    for (int band = 0; band < n_row; ++band)
    {
        for (int i = 0; i < n_col; ++i)
        {
            mtx.m_data[band * n_col + i] = this->m_data[band * n_col + i] - band_average.m_data[band];
        }
    }

    CMyMatrix mtx_T = mtx.transpose();//ת��
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
        throw std::domain_error("AB�����������");
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
        throw std::domain_error("AB�����������");
    }
   
    for (int i = 0; i < n_row * n_col; ++i)
        this->m_data[i]+= mtx.m_data[i];
    return (*this);

}