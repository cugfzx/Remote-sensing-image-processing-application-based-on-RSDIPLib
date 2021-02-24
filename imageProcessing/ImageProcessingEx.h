#pragma once
#include "RSDIPLib.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <complex>
#include <valarray>
#include "FFT.h"//快速离散傅里叶变换来源于Project Nayuki
#include "MyDFT.h"//离散傅里叶变换，是自己实现的
#include "CMyMatrix.h"//矩阵类，派生自基类CMatrix
#define PI 3.141592653589793

using namespace std;

class CImageProcessingEx :public CImageProcessing
{
public:
	CImageProcessingEx(void);
	~CImageProcessingEx(void);

	//直方图均衡化函数定义
	static BOOL histeq(CImageDataset &imgIn, CImageDataset &imgOut); 

	//直方图匹配函数定义
	static BOOL histmatch(CImageDataset &imgIn, CImageDataset &img_standard, CImageDataset &imgOut); 

	//输出可显示的DFT
	static BOOL DFT_show(CImageDataset &imgIn, CImageDataset &imgOut,int showWhat,bool visual_trans_option,bool isFFT);

	//输出可显示的IDFT
	static BOOL DFT_IDFT(CImageDataset &imgIn, CImageDataset &imgOut,bool isFFT);

	//中值滤波函数定义
	static BOOL medium_filter(CImageDataset &imgIn, CImageDataset &imgOut,int win_len);

	//双边滤波函数定义
	static BOOL bilateral_filter(CImageDataset &imgIn, CImageDataset &imgOut,int N,double sigema_r,double sigema_s);

	//拉普拉斯锐化
	static BOOL Laplace(CImageDataset &imgIn, CImageDataset &imgOut);

	//高通滤波
	static BOOL HighpassFilter(CImageDataset &imgIn, CImageDataset &imgOut,int filterType,double cut_off);

	//条带去除
	static BOOL StripRemove(CImageDataset& imgIn, CImageDataset& imgOut,double inclination,double angle);

	//主成分变换
	static BOOL PCA_transform(CImageDataset& imgIn, CImageDataset& imgOut,CImageDataset& imgPC, int P, bool recommend);

	//RGB转HIS
	static BOOL stretch(CImageDataset& imgIn, CImageDataset& imgOut, double I_coe, double H_coe, double S_coe);

private:
	//图像尺寸
	static int n_band;//波段数
	static int n_row;//宽
	static int n_col;//长
	static int n_pixel;//像素总个数
	static int n_area;//每个波段的像素个数
	static int row_mid;//中间行号
	static int col_mid;//中间列号
	//坐标转索引
	static inline int coo2ind(int band,int row,int col){return band*n_row*n_col+row*n_col+col;}

	//计算中心距离
	static inline double dist(int row,int col){return sqrt((double)(row-row_mid)*(row-row_mid)+(col-col_mid)*(col-col_mid));}

	//初始化长宽波段数
	static void init(CImageDataset &imgIn);

	//计算直方图函数
	static void getHist(const CImageDataset& img, int band, std::vector<int>& hist);

	//在拉普拉斯滤波中被调用的函数
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

	//离散二维傅里叶变换
	static BOOL DFT(CImageDataset &imgIn,complex<double> *complex_data,bool isFFT);

	//离散二维傅里叶反变换
	static BOOL IDFT(complex<double> *complex_data,CImageDataset &imgOut, bool isIFFT);

	//使傅里叶变换的复数结果变到0-255的函数
	static void visualize(complex<double>* complexImg, double* outputImg,int n_pixel,int showWhat,bool trans_option);
	static inline double amplitude(const complex<double>& x)//幅度
	{
		double r=x.real();
		double m=abs(x);
		return sqrt(r*r+m*m);
	}
	static inline double angle(const complex<double>& x)//相角
	{
		return atan(x.imag()/x.real());
	}
	static inline double energy(const complex<double>& x)//功率
	{
		return x.imag()*x.imag()+x.real()*x.real();
	}

	//频率域滤波器
	static inline double ideal_highpass(int row,int col,double cut_off){return dist(row,col)>cut_off?0:1;}
	static inline double butterworth(int row,int col,double cut_off){return 1/(1+pow(dist(row,col)/cut_off,4));}//默认2阶，暂不可更改
	static inline double Guassian_highpass(int row,int col,double cut_off)
	{
		double d=dist(row,col);
		return 1-exp(-d*d/(2*cut_off*cut_off));
	}

	//楔形滤波器
	static inline double WedgeFilter(int row, int col, double inclination, double angle)
	{//返回（row,col）处Wedge模板的值
		if (col == col_mid||col==0) return 1;
		double ang[9];
		ang[0] = row/col;					//左上角
		ang[1] = row/ (col - col_mid);		//上中点
		ang[2] = row / (col - 2*col_mid);	//右上角
		ang[3] = (row - row_mid) / col;		//左边中点
		ang[4] = (row - row_mid) / (col - col_mid);		//正中心
		ang[5] = (row - row_mid) / (col - 2*col_mid);	//右边中点
		ang[6] = (row - 2 * row_mid) / col;				//左下角
		ang[7] = (row - 2 * row_mid) / (col - col_mid);		//下边中点
		ang[8] = (row - 2 * row_mid) / (col - 2 * col_mid);	//右下角
		
		for (int i = 0; i < 9; ++i)
		{
			if (ang[i] < tan((inclination + 90 + angle / 2) / 180 * PI) && (ang[i] > tan((inclination + 90 - angle / 2) / 180 * PI)))
				return 0;
		}
		return 1;
	}

	//线程
	static DWORD WINAPI medium_filter_thread(LPVOID lpParamter);//中值滤波线程
	static DWORD WINAPI bilateral_filter_thread(LPVOID lpParamter);//双边滤波线程

	//构建影像数据矩阵，每一行是一个波段
	static void BuiltDataMtx(CImageDataset& imgIn, CMyMatrix& mtx);

	//重建影像数据矩阵
	static void RebuiltDataMtx(CImageDataset& imgOut, CMyMatrix& mtx);

	//基于模拟链表数组的插入排序
	static int insertionSort(float* arr, int* link, int length);

	//特征值归一化
	static void Normalize(float* eigvectors, int n,int len);

	//RGB转IHS
	inline static void RGB2IHS(double R, double G, double B, double& I, double& H, double& S)
	{
		I = (R + G + B) / 3;
		S = R+G+B==0?1:(1 - 3 * Min(R, G, B) / (R + G + B));//这里避免0作为除数
		H = (B <= G ? theta(R, G, B) : 360 - theta(R, G, B));
	}
	//找三个数中最小
	inline static double Min(double a, double b, double c)
	{ return min(a, min(b, c)); }
	//计算角theta 返回的是角度
	inline static double theta(double R, double G, double B) 
	{ return acos((0.5*((R-G)+(R-B)))/sqrt((R-G)*(R-G)+(R-B)*(G-B))) / PI * 180; }
	//cos(角度)  参数为角度
	inline static double COS(double angle) { return cos(angle / 180 * PI); }
	//IHS转RGB
	inline static void IHS2RGB(double I, double H, double S, double& R, double& G, double& B)
	{
		if (0 <= H && H < 120)
		{
			R = I * (1 + S * COS(H) / COS(60 - H));
			B = I * (1 - S);
			G = 3 * I - (R + B);//先求R和B，再求G
		}
		else if (120 <= H && H < 240)
		{
			R = I * (1 - S);
			G = I * (1 + S * COS(H - 120) / COS(180 - H));
			B = 3 * I - (R + G);//先求R和G，再求B
		}
		else if (240 <= H && H <= 360)
		{
			G = I * (1 - S);
			B = I * (1 + S * COS(H - 240) / COS(300 - H));
			R = 3 * I - (G + B);//先求G和B，再求R
		}
		else
			throw std::domain_error("H值异常");

	}
	//获得当前点的RGB
	inline static void getRGB(double* data,int row,int col,double& band1,double& band2,double& band3)
	{
		band1 = data[coo2ind(0, row, col)];
		band2 = data[coo2ind(1, row, col)];
		band3 = data[coo2ind(2, row, col)];
	}
	//设置当前点的RGB
	inline static void setRGB(double* data, int row, int col, double band1, double band2, double band3)
	{
		data[coo2ind(0, row, col)]=band1;
		data[coo2ind(1, row, col)]=band2;
		data[coo2ind(2, row, col)]=band3;
	}

};

//参数基类，同时也是中值滤波参数
class parameters
{
public:
	int win_len;

	double* input;
	double* output;

	int start;
	int num_of_threads;
};

//双边滤波参数
class bilateral_filter_parameters:public parameters//继承自parameters
{
public:
	int N;
	double sigma_r;
	double sigma_s;
};

