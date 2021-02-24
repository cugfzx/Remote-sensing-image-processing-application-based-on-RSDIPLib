#if !defined(AFX_MATRIX_H__9346F951_C8A6_4ca7_A9F8_08C21389A92E__INCLUDED_)
#define AFX_MATRIX_H__9346F951_C8A6_4ca7_A9F8_08C21389A92E__INCLUDED_

//DEFINE_GUID(<<name>>, 
//0x9346f951, 0xc8a6, 0x4ca7, 0xa9, 0xf8, 0x8, 0xc2, 0x13, 0x89, 0xa9, 0x2e);

#pragma once

class RSDIPLIB_API CMatrix
{
public:
	CMatrix(void);
	~CMatrix(void);

	bool empty();	// whether the matrix is empty
	void clear();	// clear the matrix
	bool duplicate(CMatrix &mtx);	//copy the source matrix
	BOOL create(int, int, float value=0.0);	// create a matrix
	
	//calculate eigvalues and eigvectors
	bool eig(float *eigvalues, float *eigvectors);

	int m_rows;		//number of row
	int m_cols;		//number of column

	//ex: the second row and the third column element is m_data[1*m_cols+2]
	float *m_data;	
};

#endif
