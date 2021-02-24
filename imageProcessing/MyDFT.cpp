#include "StdAfx.h"
#include "MyDFT.h"

using std::complex;
using std::size_t;
using std::vector;

#define PI 3.1415926535

std::vector<std::complex<double>> MyDFT::vec_copy;

void MyDFT::dft(std::vector<std::complex<double>>& vec, bool inverse)
{
	size_t n = vec.size();
	vec_copy.assign(vec.begin(), vec.end());

	std::complex<double> temp;
	for (size_t i = 0; i < n; ++i)
	{
		temp=0;
		for (size_t j = 0; j < n; ++j)
		{
			temp += vec_copy[j] * std::polar(1.0,(inverse?1:-1)*2*PI*i*j/n);//由欧拉公式
		}
		vec[i] = (inverse ? temp/(double)n : temp);
	}
}
