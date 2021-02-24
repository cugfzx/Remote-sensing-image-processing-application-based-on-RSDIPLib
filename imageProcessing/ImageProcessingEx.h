#pragma once
#include "RSDIPLib.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <complex>
#include <valarray>
#include "FFT.h"//������ɢ����Ҷ�任��Դ��Project Nayuki
#include "MyDFT.h"//��ɢ����Ҷ�任�����Լ�ʵ�ֵ�
#include "CMyMatrix.h"//�����࣬�����Ի���CMatrix
#define PI 3.141592653589793

using namespace std;

class CImageProcessingEx :public CImageProcessing
{
public:
	CImageProcessingEx(void);
	~CImageProcessingEx(void);

	//ֱ��ͼ���⻯��������
	static BOOL histeq(CImageDataset &imgIn, CImageDataset &imgOut); 

	//ֱ��ͼƥ�亯������
	static BOOL histmatch(CImageDataset &imgIn, CImageDataset &img_standard, CImageDataset &imgOut); 

	//�������ʾ��DFT
	static BOOL DFT_show(CImageDataset &imgIn, CImageDataset &imgOut,int showWhat,bool visual_trans_option,bool isFFT);

	//�������ʾ��IDFT
	static BOOL DFT_IDFT(CImageDataset &imgIn, CImageDataset &imgOut,bool isFFT);

	//��ֵ�˲���������
	static BOOL medium_filter(CImageDataset &imgIn, CImageDataset &imgOut,int win_len);

	//˫���˲���������
	static BOOL bilateral_filter(CImageDataset &imgIn, CImageDataset &imgOut,int N,double sigema_r,double sigema_s);

	//������˹��
	static BOOL Laplace(CImageDataset &imgIn, CImageDataset &imgOut);

	//��ͨ�˲�
	static BOOL HighpassFilter(CImageDataset &imgIn, CImageDataset &imgOut,int filterType,double cut_off);

	//����ȥ��
	static BOOL StripRemove(CImageDataset& imgIn, CImageDataset& imgOut,double inclination,double angle);

	//���ɷֱ任
	static BOOL PCA_transform(CImageDataset& imgIn, CImageDataset& imgOut,CImageDataset& imgPC, int P, bool recommend);

	//RGBתHIS
	static BOOL stretch(CImageDataset& imgIn, CImageDataset& imgOut, double I_coe, double H_coe, double S_coe);

private:
	//ͼ��ߴ�
	static int n_band;//������
	static int n_row;//��
	static int n_col;//��
	static int n_pixel;//�����ܸ���
	static int n_area;//ÿ�����ε����ظ���
	static int row_mid;//�м��к�
	static int col_mid;//�м��к�
	//����ת����
	static inline int coo2ind(int band,int row,int col){return band*n_row*n_col+row*n_col+col;}

	//�������ľ���
	static inline double dist(int row,int col){return sqrt((double)(row-row_mid)*(row-row_mid)+(col-col_mid)*(col-col_mid));}

	//��ʼ����������
	static void init(CImageDataset &imgIn);

	//����ֱ��ͼ����
	static void getHist(const CImageDataset& img, int band, std::vector<int>& hist);

	//��������˹�˲��б����õĺ���
	static inline double Wr(int band,int r1,int c1,int r2,int c2,double sigma_r,const double* data)
	{
		double a=data[coo2ind(band,r1,c1)];
		double b=data[coo2ind(band,r2,c2)];
		return exp(-((a-b)*(a-b))/(2*sigma_r*sigma_r));
	}
	static inline double Ws(int r1,int c1,int r2,int c2,double sigma_s)
	{return exp(-((r1-r2)*(r1-r2)+(c1-c2)*(c1-c2))/(2*sigma_s*sigma_s));}
	static inline double W(int band,int r1,int c1,int r2,int c2,double sigma_r,double sigma_s,const double* data)
	{
		double s=Ws(r1,c1,r2,c2,sigma_s);
		double r=Wr(band,r1,c1,r2,c2,sigma_r,data);
		return s*r;
	}

	//��ɢ��ά����Ҷ�任
	static BOOL DFT(CImageDataset &imgIn,complex<double> *complex_data,bool isFFT);

	//��ɢ��ά����Ҷ���任
	static BOOL IDFT(complex<double> *complex_data,CImageDataset &imgOut, bool isIFFT);

	//ʹ����Ҷ�任�ĸ�������䵽0-255�ĺ���
	static void visualize(complex<double>* complexImg, double* outputImg,int n_pixel,int showWhat,bool trans_option);
	static inline double amplitude(const complex<double>& x)//����
	{
		double r=x.real();
		double m=abs(x);
		return sqrt(r*r+m*m);
	}
	static inline double angle(const complex<double>& x)//���
	{
		return atan(x.imag()/x.real());
	}
	static inline double energy(const complex<double>& x)//����
	{
		return x.imag()*x.imag()+x.real()*x.real();
	}

	//Ƶ�����˲���
	static inline double ideal_highpass(int row,int col,double cut_off){return dist(row,col)>cut_off?0:1;}
	static inline double butterworth(int row,int col,double cut_off){return 1/(1+pow(dist(row,col)/cut_off,4));}//Ĭ��2�ף��ݲ��ɸ���
	static inline double Guassian_highpass(int row,int col,double cut_off)
	{
		double d=dist(row,col);
		return 1-exp(-d*d/(2*cut_off*cut_off));
	}

	//Ш���˲���
	static inline double WedgeFilter(int row, int col, double inclination, double angle)
	{//���أ�row,col����Wedgeģ���ֵ
		if (col == col_mid||col==0) return 1;
		double ang[9];
		ang[0] = row/col;					//���Ͻ�
		ang[1] = row/ (col - col_mid);		//���е�
		ang[2] = row / (col - 2*col_mid);	//���Ͻ�
		ang[3] = (row - row_mid) / col;		//����е�
		ang[4] = (row - row_mid) / (col - col_mid);		//������
		ang[5] = (row - row_mid) / (col - 2*col_mid);	//�ұ��е�
		ang[6] = (row - 2 * row_mid) / col;				//���½�
		ang[7] = (row - 2 * row_mid) / (col - col_mid);		//�±��е�
		ang[8] = (row - 2 * row_mid) / (col - 2 * col_mid);	//���½�
		
		for (int i = 0; i < 9; ++i)
		{
			if (ang[i] < tan((inclination + 90 + angle / 2) / 180 * PI) && (ang[i] > tan((inclination + 90 - angle / 2) / 180 * PI)))
				return 0;
		}
		return 1;
	}

	//�߳�
	static DWORD WINAPI medium_filter_thread(LPVOID lpParamter);//��ֵ�˲��߳�
	static DWORD WINAPI bilateral_filter_thread(LPVOID lpParamter);//˫���˲��߳�

	//����Ӱ�����ݾ���ÿһ����һ������
	static void BuiltDataMtx(CImageDataset& imgIn, CMyMatrix& mtx);

	//�ؽ�Ӱ�����ݾ���
	static void RebuiltDataMtx(CImageDataset& imgOut, CMyMatrix& mtx);

	//����ģ����������Ĳ�������
	static int insertionSort(float* arr, int* link, int length);

	//����ֵ��һ��
	static void Normalize(float* eigvectors, int n,int len);

	//RGBתIHS
	inline static void RGB2IHS(double R, double G, double B, double& I, double& H, double& S)
	{
		I = (R + G + B) / 3;
		S = R+G+B==0?1:(1 - 3 * Min(R, G, B) / (R + G + B));//�������0��Ϊ����
		H = (B <= G ? theta(R, G, B) : 360 - theta(R, G, B));
	}
	//������������С
	inline static double Min(double a, double b, double c)
	{ return min(a, min(b, c)); }
	//�����theta ���ص��ǽǶ�
	inline static double theta(double R, double G, double B) 
	{ return acos((0.5*((R-G)+(R-B)))/sqrt((R-G)*(R-G)+(R-B)*(G-B))) / PI * 180; }
	//cos(�Ƕ�)  ����Ϊ�Ƕ�
	inline static double COS(double angle) { return cos(angle / 180 * PI); }
	//IHSתRGB
	inline static void IHS2RGB(double I, double H, double S, double& R, double& G, double& B)
	{
		if (0 <= H && H < 120)
		{
			R = I * (1 + S * COS(H) / COS(60 - H));
			B = I * (1 - S);
			G = 3 * I - (R + B);//����R��B������G
		}
		else if (120 <= H && H < 240)
		{
			R = I * (1 - S);
			G = I * (1 + S * COS(H - 120) / COS(180 - H));
			B = 3 * I - (R + G);//����R��G������B
		}
		else if (240 <= H && H <= 360)
		{
			G = I * (1 - S);
			B = I * (1 + S * COS(H - 240) / COS(300 - H));
			R = 3 * I - (G + B);//����G��B������R
		}
		else
			throw std::domain_error("Hֵ�쳣");

	}
	//��õ�ǰ���RGB
	inline static void getRGB(double* data,int row,int col,double& band1,double& band2,double& band3)
	{
		band1 = data[coo2ind(0, row, col)];
		band2 = data[coo2ind(1, row, col)];
		band3 = data[coo2ind(2, row, col)];
	}
	//���õ�ǰ���RGB
	inline static void setRGB(double* data, int row, int col, double band1, double band2, double band3)
	{
		data[coo2ind(0, row, col)]=band1;
		data[coo2ind(1, row, col)]=band2;
		data[coo2ind(2, row, col)]=band3;
	}

};

//�������࣬ͬʱҲ����ֵ�˲�����
class parameters
{
public:
	int win_len;

	double* input;
	double* output;

	int start;
	int num_of_threads;
};

//˫���˲�����
class bilateral_filter_parameters:public parameters//�̳���parameters
{
public:
	int N;
	double sigma_r;
	double sigma_s;
};

