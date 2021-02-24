#pragma once
#include <complex>
#include <vector>

class MyDFT
{
public:
	static void dft(std::vector<std::complex<double> >& vec, bool inverse);
private:
	static std::vector<std::complex<double>> vec_copy;
};



