#include "StdAfx.h"
#include "ImageProcessingEx.h"
#include <iostream>
#include <iomanip>

//定义静态成员变量
int CImageProcessingEx::n_band = 0;
int CImageProcessingEx::n_row = 0;
int CImageProcessingEx::n_col = 0;
int CImageProcessingEx::n_pixel = 0;
int CImageProcessingEx::n_area = 0;
int CImageProcessingEx::row_mid = 0;
int CImageProcessingEx::col_mid = 0;

CImageProcessingEx::CImageProcessingEx(void)
{
}


CImageProcessingEx::~CImageProcessingEx(void)
{
}

//初始化
void CImageProcessingEx::init(CImageDataset &imgIn)
{
	n_band = imgIn.m_rastercount;
	n_row = imgIn.m_ysize;
	n_col = imgIn.m_xsize;
	n_pixel = n_row*n_col*n_band;
	n_area = n_row * n_col;
	row_mid=n_row/2;
	col_mid=n_col/2;
}

//计算直方图函数
void CImageProcessingEx::getHist(const CImageDataset& img,int band,std::vector<int>& hist)
{
	int value;
	int N_row = img.m_ysize;
	int N_col = img.m_xsize;
	const double* imgData = img.m_data;
	for(int row=0; row<N_row; ++row)  //对图像逐行逐列进行扫描，计算完成直方图
	{ 
		for(int col=0; col<N_col; ++col) 
		{ 
			int index = band * N_row * N_col + row * N_col + col;
			value=(UINT8)imgData[index];
			hist[value]++; 
		} 
	} 
}

//直方图均衡
BOOL CImageProcessingEx::histeq(CImageDataset &imgIn, CImageDataset &imgOut) 
{ 
	const int LEVEL = 256; //灰度值级数
	int k, row, col; //用于循环
	//double hist[LEVEL], accuHist[LEVEL]; //直方图和累积直方图
	std::vector<int> hist(LEVEL);
	std::vector<double> accuHist(LEVEL);

	/* 确保输入图像非空 */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//记录输入图像的尺寸
	init(imgIn);

	/*创建输入图像*/ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	//输入和输出图像的数据区指针
	const double *input_data=imgIn.m_data;
	double *output_data = imgOut.m_data;
	int img_area = n_col * n_row;//面积，即单个波段的像素个数

	for (int band = 0; band < n_band; band++)
	{/*  步骤 1：计算输入图像的直方图  */
		getHist(imgIn, band,hist);

		/*  步骤 2：计算累积直方图  */
		accuHist[0] = hist[0] / img_area;
		for (k = 1; k < LEVEL - 1; k++) //对直方图索引进行扫描，计算完成累积直方图
		{
			accuHist[k] = accuHist[k - 1] + double(hist[k]) / img_area;
		}
		accuHist[255] = 1;

		/*  步骤 3：根据用累积直方图建立灰度变换函数，并逐行逐列进行像素的灰度变换 */
		int index, gray_value;
		for (row = 0; row < n_row; ++row)
		{
			for (col = 0; col < n_col; ++col)
			{
				//for(k=0;k<LEVEL;++k)//多此一举   多此一举  多此一举
				//{
				//	if(input_data[row*n_col+col] == k )
				//	{
				//		data[row*n_col+col] = int((LEVEL-1)*accuHist[k]+0.5);//灰度变换
				//		k=LEVEL;
				//	}
				//}
				//output_data[row*n_col+col] = int((LEVEL-1)*accuHist[gray_value]+0.5);//灰度变换

				index = coo2ind(band,row,col);
				gray_value = (int)input_data[index];

				output_data[index] = (LEVEL - 1) * accuHist[gray_value];//灰度变换
			}
		}
	}
	return TRUE;
}

//直方图匹配
BOOL CImageProcessingEx::histmatch(CImageDataset &imgIn, CImageDataset &imgRef, CImageDataset &imgOut)
{
	const int LEVEL = 256; //灰度值个数

	/* 确保输入和参考图像不为空或者两张图像波段数不等 */ 
	if(imgIn.empty()||imgRef.empty()||imgIn.m_rastercount!=imgRef.m_rastercount) 
	{ 
		return FALSE; 
	} 

	int band,row,col,i,j;//用于遍历

	/*统计表*/
	std::vector<int> input_hist(LEVEL), ref_hist(LEVEL);//输入图直方图和参考图直方图
	std::vector<double> input_accuHist(LEVEL),ref_accuHist(LEVEL); //输入图累积直方图和参考累积直方图
	
	//记录输入图像的尺寸
	init(imgIn);

	/* 创建输出图像 */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 
	double *output_data = imgOut.m_data; 
	const double *input_data=imgIn.m_data;
	const double *ref_data=imgRef.m_data;
	

	for (band = 0; band < n_band; band++)
	{
		/*  步骤 1：计算输入图像和参考图像的直方图  */
		getHist(imgIn, band,input_hist);
		getHist(imgRef, band,ref_hist);

		/*  步骤 2：计算输入图像和参考图像的累积直方图  */
		input_accuHist[0] = input_hist[0] / n_area;
		ref_accuHist[0] = ref_hist[0] / n_area;
		for (i = 1; i < LEVEL - 1; ++i) //计算完成累积直方图
		{
			input_accuHist[i] = input_accuHist[i - 1] + double(input_hist[i]) / n_area;
			ref_accuHist[i] = ref_accuHist[i - 1] + double(ref_hist[i]) / n_area;
		}
		input_accuHist[255] = 1;
		ref_accuHist[255] = 1;

		/*  步骤 3：根据用累积直方图建立灰度变换函数，并逐行逐列进行像素的灰度变换 */

		//建立映射
		vector<double> deviation(LEVEL);//中间临时数据
		int mapping_table[LEVEL];//记录映射的表
		vector<double>::iterator minElement;

		for (i = 0; i < LEVEL; ++i)//在输入图的累积直方图中遍历
		{
			for (j = 0; j < LEVEL; ++j)//在参考图的累积直方图中遍历
			{
				deviation[j] = abs(input_accuHist[i] - ref_accuHist[j]);//对于输入累积直方图中的每个值与参考图累积直方图中的所有值作差
			}
			minElement = min_element(deviation.begin(), deviation.end());//找到最小的差
			mapping_table[i] = distance(deviation.begin(), minElement);//最小差的下标放入映射表

		}

		//灰度变换
		int index;
		for (row = 0; row < n_row; ++row)
		{
			for (col = 0; col < n_col; ++col)
			{
				index = coo2ind(band,row,col);
				output_data[index] = mapping_table[int(input_data[index])];
			}
		}
	}
	return TRUE;
}

//多线程中值滤波                                                                            
BOOL CImageProcessingEx::medium_filter(CImageDataset &imgIn, CImageDataset &imgOut,int win_len)
{
	/* 确保输入图像不为空 */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//记录输入图像的尺寸
	init(imgIn);

	/* 创建输出图像 */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	int win_area=win_len*win_len;   //窗口面积
	int border=win_len/2;			//边框宽度
	int num_of_threads = 8;

	//给传入进程的参数结构体数组赋值
	parameters* p = new parameters[num_of_threads];
	for (int i = 0; i < num_of_threads; ++i)
	{
		p[i].win_len = win_len;
		p[i].output = imgOut.m_data;
		p[i].input = imgIn.m_data;
		p[i].num_of_threads = num_of_threads;//线程数，也是处理的间隔
		//上面的每个进程都一样

		p[i].start = i;//每个进程的开始处理的位置不一样
	}

	//////////////////////////////////多线程并行///////////////////////

	HANDLE* Threads=new HANDLE[num_of_threads];//线程池
	for(int i=0;i< num_of_threads;++i)
	{
		Threads[i] = CreateThread(NULL, 0, medium_filter_thread, p+i, 0, NULL);
		SetThreadAffinityMask(Threads[i], (DWORD_PTR)pow(2.0,i));//附加到第i个CPU内核
	}

	DWORD state;
	do{//等待全部进程结束
		state=WaitForMultipleObjects(num_of_threads, Threads,TRUE,INFINITE);
	}while(state!=WAIT_OBJECT_0);

	//关闭进程
	for(int i=0;i< num_of_threads;++i)
		CloseHandle(Threads[i]);

	//释放空间
	delete[]p;
	delete[]Threads;

	return TRUE;
	
}
DWORD WINAPI CImageProcessingEx::medium_filter_thread(LPVOID lpParamter)
{
	parameters*p = (parameters*)lpParamter;

	int index,band,row,col,i,j,k=0;		//用于遍历

	int win_len=p->win_len;
	int win_area=win_len*win_len;   //窗口面积
	int border=p->win_len/2;		//边框宽度

	double* input_data=p->input;
	double* output_data=p->output;

	int start=p->start;
	int num_of_threads = p->num_of_threads;

	vector<double> window(win_area);//窗口

	for (band = 0; band < n_band; band++) {
		for (row = border + start; row < n_row - border; row += num_of_threads) {//每隔num_of_threads行处理一行，遍历全图
			for (col = border; col < n_col - border; ++col) {

				for (i = row - win_len / 2; i <= row + win_len / 2; ++i) {
					for (j = col - win_len / 2; j <= col + win_len / 2; ++j)//遍历窗口
					{
						index = coo2ind(band,i,j);
						window[k++] = input_data[index];//记入window数组
					}
				}

				index = coo2ind(band,row,col);
				std::nth_element(window.begin(), window.begin() + win_area / 2 + 1, window.end());//用STL找中值
				output_data[index] = window[win_area / 2 + 1];
				k = 0;
			}
		}
	}


	return 0L;
}

//多线程双边滤波
BOOL CImageProcessingEx::bilateral_filter(CImageDataset &imgIn, CImageDataset &imgOut,int N,double sigma_r,double sigma_s)
{
	/* 确保输入图像不为空 */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//记录输入图像的尺寸
	init(imgIn);

	/* 创建输出图像 */ 
	if(FALSE == imgIn.duplicate(imgOut))
	{ 
		return FALSE; 
	} 

	int num_of_threads = 8;
	int win_len = 2 * N + 1;

	//传入进程的参数结构体数组
	bilateral_filter_parameters* p = new bilateral_filter_parameters[num_of_threads];
	for (int i = 0; i < num_of_threads; ++i)
	{
		p[i].win_len = win_len;
		p[i].output = imgOut.m_data;
		p[i].input = imgIn.m_data;
		p[i].num_of_threads = num_of_threads;//线程数，也是处理的间隔
		p[i].N = N;
		p[i].sigma_r = sigma_r;
		p[i].sigma_s = sigma_s;

		p[i].start = i;//每个进程的开始处理的位置不一样
	}

	//////////////////////////////////多线程并行处理/////////////////////////

	HANDLE* Threads = new HANDLE[num_of_threads];//进程池
	for (int i = 0; i < num_of_threads; ++i)
	{
		Threads[i] = CreateThread(NULL, 0, bilateral_filter_thread, p + i, 0, NULL);
		SetThreadAffinityMask(Threads[i], (DWORD_PTR)pow(2.0, i));//附加到第i个CPU内核
	}

	DWORD state;
	do {//等待全部进程结束
		state = WaitForMultipleObjects(num_of_threads, Threads, TRUE, INFINITE);
	} while (state != WAIT_OBJECT_0);

	//关闭进程
	for (int i = 0; i < num_of_threads; ++i)
		CloseHandle(Threads[i]);

	//释放空间
	delete[]p;
	delete[]Threads;

	return TRUE;
}
DWORD WINAPI CImageProcessingEx::bilateral_filter_thread(LPVOID lpParamter)
{
	bilateral_filter_parameters* p = (bilateral_filter_parameters*)lpParamter;

	int index, row, col, i, j, band,k = 0;		//用于遍历
	int win_len = p->win_len;
	int win_area = win_len * win_len;   //窗口面积
	vector<double> window(win_area);//窗口
	int border = p->win_len / 2;		//边框宽度
	double* input_data = p->input;
	double* output_data = p->output;
	int start = p->start;
	int num_of_threads = p->num_of_threads;
	int N = p->N;
	double sigma_r = p->sigma_r;
	double sigma_s = p->sigma_s;

	double molecules;//分子
	double denominatores;//分母

	for (band = 0; band < n_band; band++) {
		for (row = N + start; row < n_row - N; row += num_of_threads) {//每隔num_of_threads行处理一行
			for (col = N; col < n_col - N; ++col) {//遍历全图

				//分子分母置零
				molecules = 0;
				denominatores = 0;

				for (i = row - N; i <= row + N; ++i) {
					for (j = col - N; j <= col + N; ++j)//遍历窗口
					{
						double w = W(band, row, col, i, j, sigma_r, sigma_s, input_data);

						index = coo2ind(band, i, j);

						//累加
						molecules += w * input_data[index];
						denominatores += w;
					}
				}

				index = coo2ind(band, row, col);
				output_data[index] = molecules / denominatores;
			}
		}
	}
	return 0L;
}

//拉普拉斯
BOOL CImageProcessingEx::Laplace(CImageDataset &imgIn, CImageDataset &imgOut)
{
	/* 确保输入图像不为空 */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//记录输入图像的尺寸
	init(imgIn);

	/* 创建输出图像 */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	double *output_data = imgOut.m_data; 
	const double *input_data=imgIn.m_data;

	int index,row,col,i,j,band,k=0;	//用于遍历,k用于在拉普拉斯算子中遍历

	int laplacian[9]={0,1,0,1,-4,1,0,1,0};

	double intermediate=0;//中间结果

	for(band=0;band<n_band;band++)
		for(row=1; row<n_row-1; ++row){
			for(col=1; col<n_col-1; ++col){//遍历全图


				for(i=row-1;i<=row+1;++i){
					for(j=col-1;j<=col+1;++j)//遍历窗口
					{	
						//跳过laplace算子中的0
						if(laplacian[k]==0) 
						{
							++k;
							continue;
						}
						index=coo2ind(band,i,j);
						intermediate+=input_data[index]*laplacian[k++];
					}
				}

			
				index=coo2ind(band,row,col);
				output_data[index]=input_data[index]-intermediate;

				//置0
				intermediate=0;
				k=0;
			}
	}


	return TRUE;
}

//傅里叶变换得到复数结果
BOOL CImageProcessingEx::DFT(CImageDataset &imgIn, complex<double> *complex_data,bool isFFT)
{
	/* 确保输入图像不为空 */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//记录输入图像的尺寸
	init(imgIn);

	const double *input_data=imgIn.m_data;

	int index,band,row,col;	//用于遍历

	//中心化
	for(band=0;band<n_band;band++)
		for(row=0; row<n_row; ++row){
			for(col=0; col<n_col; ++col){
				index=coo2ind(band,row,col);
				complex_data[index]=input_data[index]*(((row*col)&(0x01))?-1:1);//此处不用pow函数，可大大优化
			}
		}

	//选择是否采用FFT
	void (*dft_fun)(vector<complex<double>>&, bool);
	if (isFFT)
		dft_fun = FFT::fft;
	else
		dft_fun = MyDFT::dft;

	//工作数组
	vector<complex<double>> workArr(n_col);

	for (band = 0; band < n_band; band++)
	{
		//对每一行进行一维傅里叶变换
		for (row = 0; row < n_row; ++row)
		{
			//将一行从complex_data复制进workArr
			memcpy(workArr.data(), complex_data + coo2ind(band,row,0), n_col * sizeof(complex<double>));

			dft_fun(workArr, FALSE);//对这一行进行一维傅里叶变换

			//将一行从workArr复制回complex_data,作为其一行
			memcpy(complex_data + coo2ind(band, row, 0), workArr.data(), n_col * sizeof(complex<double>));
		}

		workArr.resize(n_row);//清空workArr,用列总数Resize

		//对每一列进行一维傅里叶变换
		for (col = 0; col < n_col; ++col)
		{
			//将一列从complex_data复制进workArr
			for (row = 0; row < n_row; ++row) {
				workArr[row] = complex_data[coo2ind(band, row, col)];
			}

			dft_fun(workArr, FALSE);//对这一行进行一维傅里叶变换

			//将一列从workArr复制回complex_data的一列
			for (row = 0; row < n_row; ++row) {
				complex_data[coo2ind(band, row, col)] = workArr[row];
			}
		}
	}


	return TRUE;
}

//得到可显示的傅里叶变换结果
BOOL CImageProcessingEx::DFT_show(CImageDataset &imgIn, CImageDataset &imgOut,int showWhat,bool visual_trans_option, bool isFFT)
{
	/* 确保输入图像不为空 */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//记录输入图像的尺寸
	init(imgIn);

	/* 创建输出图像 */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	//创建频谱
	complex<double> *complex_data=new complex<double>[n_pixel];

	//傅里叶变换
	if(FALSE==DFT(imgIn, complex_data,isFFT))
	{
		return FALSE; 
	}

	//对复数结果进行转换到0-255以便显示
	visualize(complex_data,imgOut.m_data,n_pixel,showWhat,visual_trans_option);

	delete []complex_data;
	complex_data=NULL;

	return TRUE;
}

//使傅里叶变换结果可视化
void CImageProcessingEx::visualize(complex<double>* complexImg, double* outputImg,int n_pixel,int doWhat,bool trans_option)
{
	double (*trans_fun)(const complex<double>& x);//定义函数指针
	enum showWhat{FREQUENCY=0,ANGLE=1,ENERGY=2};
	switch(doWhat)
	{
	case FREQUENCY://显示频谱
	default:
		trans_fun=amplitude;break;
	case ANGLE://显示相角
		trans_fun=angle;break;
	case ENERGY://显示能量
		trans_fun=energy;break;
	}

	//取模的同时找最大模和最小模
	for (int band = 0; band < n_band; ++band)
	{
		double max = 0;
		double min = DBL_MAX;
		for (int i = 0; i <n_area; ++i)
		{
			int index= band * n_area + i;
			outputImg[index] = trans_fun(complexImg[index]);//转换为幅度，相角，或者功率

			//记录当前最大值和最小值
			if (outputImg[index] > max)
				max = outputImg[index];
			if (outputImg[index] < min)
				min = outputImg[index];
		}

		if (trans_option)
		{
			//对数变换
			double c = 255 / log(max + 1);
			for (int i = 0; i < n_area; ++i)
			{
				outputImg[band * n_area + i] = c * log(outputImg[band * n_area + i] + 1);
			}
		}
		else
		{
			//线性拉伸
			double extr_diff = max - min;//极差
			for (int i = 0; i < n_area; ++i)
			{
				outputImg[band * n_area + i] = (outputImg[band * n_area + i] - min) / extr_diff * 255;
			}
		}
	}
}

//傅里叶反变换
BOOL CImageProcessingEx::IDFT(complex<double> *complex_data,CImageDataset &imgOut,bool isFFT)
{
	/* 确保输入图像不为空 */ 
	if(complex_data==NULL) 
	{ 
		return FALSE; 
	} 

	double *output_data=imgOut.m_data;

	int index,band,row,col;	//用于遍历

	//工作数组
	vector<complex<double>> workArr(n_col);

	//选择是否采用FFT
	void (*dft_fun)(vector<complex<double>>&, bool);
	if (isFFT)
		dft_fun = FFT::fft;
	else
		dft_fun = MyDFT::dft;

	for (band = 0; band < n_band; band++)
	{
		//对每一行进行一维傅里叶变换
		for (row = 0; row < n_row; ++row)
		{
			//将一行从complex_data复制进workArr
			memcpy(workArr.data(), complex_data + coo2ind(band, row, 0), n_col*sizeof(complex<double>));

			dft_fun(workArr, TRUE);//对这一行进行一维傅里叶变换

			//将一行从workArr复制回complex_data,作为其一行
			memcpy(complex_data + coo2ind(band, row, 0), workArr.data(), n_col*sizeof(complex<double>));
		}

		workArr.resize(n_row);//清空workArr,用行总数Resize

		//对每一列进行一维傅里叶变换
		for (col = 0; col < n_col; ++col)
		{
			//将一列从complex_data复制进workArr
			for (row = 0; row < n_row; ++row) {
				workArr[row] = complex_data[coo2ind(band,row, col)];
			}

			dft_fun(workArr, TRUE);//对这一行进行一维傅里叶变换

			//将一列从workArr复制回output的一列
			for (row = 0; row < n_row; ++row) {
				output_data[coo2ind(band,row, col)] = workArr[row].real();//取其实部
			}
		}
	}

	//中心化
	for (band = 0; band < n_band; band++){
		for (row = 0; row < n_row; ++row) {
			for (col = 0; col < n_col; ++col) {
				index = coo2ind(band, row, col);
				output_data[index] = output_data[index] * (((row * col) & (0x01)) ? -1 : 1);//此处不用pow函数，可大大优化
			}
		}
	}

	return TRUE;
}

//先正傅里叶变换再反变换计算差值
BOOL CImageProcessingEx::DFT_IDFT(CImageDataset &imgIn, CImageDataset &imgOut,bool isFFT)
{
	/* 确保输入图像不为空 */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//记录输入图像的尺寸
	init(imgIn);

	/* 创建输出图像 */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	//用于存放频谱
	complex<double> *complex_data=new complex<double>[n_pixel];

	//傅里叶变换
	if(FALSE==DFT(imgIn,complex_data,isFFT))
	{
		return FALSE;
	}

	//傅里叶反变换
	if(FALSE==IDFT(complex_data,imgOut,isFFT))
	{
		return FALSE;
	}

	for (int i = 0; i < n_pixel; ++i)
	{
		imgOut.m_data[i] -= imgIn.m_data[i];
	}

	return TRUE;
}

//高通滤波
BOOL CImageProcessingEx::HighpassFilter(CImageDataset &imgIn, CImageDataset &imgOut,int filterType,double cut_off)
{
	/* 确保输入图像不为空 */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//记录输入图像的尺寸
	init(imgIn);

	/* 创建输出图像 */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	//创建频谱
	complex<double> *complex_data=new complex<double>[n_pixel];
	
	//傅里叶变换得到频谱
	if(FALSE==DFT(imgIn, complex_data,TRUE))
	{
		return FALSE; 
	}

	//根据传进来的参数选择滤波器
	enum filter_type{IDEAL=0,BUTTERWORTH=1,GUASSIAN=2};
	double (*filter)(int row,int col,double cut);
	switch(filterType)
	{
	case IDEAL://理想高通滤波器
		filter=ideal_highpass;
		break;
	case BUTTERWORTH://butterworth
		filter=butterworth;
		break;
	case GUASSIAN://高斯
		filter=Guassian_highpass;
		break;
	}

	int band,row,col,i;	//用于遍历
	//做点乘
	for (band = 0; band < n_band; band++) {
		for (row = 0; row < n_row; ++row) {
			for (col = 0; col < n_col; ++col) {//遍历全图
				i = coo2ind(band,row, col);
				complex_data[i] = complex_data[i] * filter(row, col, cut_off);
			}
		}
	}

	//傅里叶反变换
	if(FALSE==IDFT(complex_data,imgOut,TRUE))
	{
		return FALSE;
	}

	delete []complex_data;
	complex_data=NULL;

	return TRUE;

}

//条带去除
BOOL CImageProcessingEx::StripRemove(CImageDataset& imgIn, CImageDataset& imgOut, double inclination, double angle)
{
	/* 确保输入图像不为空 */
	if (imgIn.empty())
	{
		return FALSE;
	}

	//记录输入图像的尺寸
	init(imgIn);

	/* 创建输出图像 */
	if (FALSE == imgOut.create(n_col, n_row, n_band))
	{
		return FALSE;
	}

	//创建频谱
	complex<double>* complex_data = new complex<double>[n_pixel];

	//傅里叶变换得到频谱
	if (FALSE == DFT(imgIn, complex_data,TRUE))
	{
		return FALSE;
	}


	int band,row, col, i;	//用于遍历
	//做点乘
	for (band = 0; band < n_band; band++) {
		for (row = 0; row < n_row; ++row) {
			for (col = 0; col < n_col; ++col) {//遍历全图
				i = coo2ind(band, row, col);
				complex_data[i] = complex_data[i] * WedgeFilter(row, col, inclination, angle);
			}
		}
	}

	//傅里叶反变换
	if (FALSE == IDFT(complex_data, imgOut,TRUE))
	{
		return FALSE;
	}

	delete[]complex_data;
	complex_data = NULL;

	return TRUE;
}

//PCA变换
BOOL CImageProcessingEx::PCA_transform(CImageDataset& imgIn, CImageDataset& imgOut,CImageDataset& imgPC,int P,bool recommend)
{
	/* 确保输入图像不为空 */
	if (imgIn.empty())
	{
		return FALSE;
	}

	//记录输入图像的尺寸
	init(imgIn);

	/* 创建反变换输出图像 */
	if (FALSE == imgOut.create(n_col, n_row, n_band))
	{
		return FALSE;
	}
	/* 创建输出主成分图像 */
	if (FALSE == imgPC.create(n_col, n_row, n_band))
	{
		return FALSE;
	}

	/********************STEP 1：构建影像数据矩阵 X*********************************/
	CMyMatrix X;
	BuiltDataMtx(imgIn, X);

	/********************STEP 2：根据原始图像数据矩阵 X ，计算协方差矩阵S***********/
	CMyMatrix S= X.cov();

	/******************STEP 3：计算协方差S矩阵的特征值和特征向量********************/
	float* eigvalues = new float[S.m_rows];
	float* eigvectors = new float[S.m_cols*S.m_rows];
	S.eig(eigvalues, eigvectors);

	/************************STEP 4：构建变换矩阵T************************/

	//特征向量归一化
	Normalize(eigvectors, S.m_rows, S.m_cols);

	//将特征值从大到小排序
	int* link = new int[S.m_rows];//索引表，存放下标
	for (int i = 0; i < S.m_rows; ++i)//初始化为-1
		{link[i] = -1;}
	int head = insertionSort(eigvalues, link, S.m_rows);//基于模拟链表的间接插入排序


	//按照对应特征值的顺序以各特征向量为列构成矩阵U
	CMyMatrix U(S.m_cols,S.m_rows);
	int k = 0;
	for (int i = head; i!=-1; i=link[i])
	{
		for (int j = 0; j < S.m_cols; ++j)
		{
			U.m_data[j* S.m_rows+k] = eigvectors[i * S.m_cols + j];
		}
		k++;
	}

	//计算矩阵U的转置矩阵，即为所求的 PCA 变换的变换矩阵UT
	CMyMatrix UT = U.transpose();

	/**************STEP 5：计算主分量**********************/
	CMyMatrix Y = (UT*X);

	//保存主成分影像
	for (int i = 0; i < n_pixel; ++i)
		imgPC.m_data[i] = Y.m_data[i];

	/**************STEP 6：利用前 p 个主分量重构遥感影像数据矩阵，亦即主成分反变换 *******/
	int p = P;//默认p是传进的参数P
	
	if (recommend)//如果让使用自动推荐p值就计算方差贡献率
	{
		double eig_sum = 0;
		for (int j = 0; j < S.m_cols; ++j)//计算特征值之和
		{
			eig_sum += eigvalues[j];
		}

		p = 0;
		double sum = 0;
		int i = head;

		do
		{
			sum += eigvalues[i]/eig_sum;
			i = link[i];//移到下一个最大值
			p++;//计数
		} while (sum < 0.85);
	}

	CMyMatrix ResultMtx(X.m_rows,Y.m_cols);//结果矩阵
	CMyMatrix Ui(X.m_rows,1);
	CMyMatrix Yi(1, Y.m_cols);

	k = head;
	for (int i = 0; i < p; ++i)
	{
		//第k个特征向量赋给列向量Ui
		for (int j = 0; j < X.m_rows; ++j)
		{
			Ui.m_data[j] = eigvectors[k * X.m_rows + j];
		}
		k = link[k];//k更新为下一个最大特征值的索引


		//将Y的第i行赋给Yi
		for (int j = 0; j < Y.m_cols; ++j)
		{
			Yi.m_data[j] = Y.m_data[i * Y.m_cols + j];
		}

		ResultMtx += (Ui*Yi);
	}

	/***************STEP 7：将上步中得到的遥感影像数据矩阵恢复为m 个波段的数字影像。 ***********/
	RebuiltDataMtx(imgOut, ResultMtx);

	delete[]link;
	delete[]eigvalues;
	delete[]eigvectors;

	return TRUE;
}

//创建影像矩阵
void CImageProcessingEx::BuiltDataMtx(CImageDataset& imgIn, CMyMatrix& mtx)
{
	int pixels = imgIn.m_xsize * imgIn.m_ysize* imgIn.m_rastercount;
	mtx.create(imgIn.m_rastercount, imgIn.m_xsize * imgIn.m_ysize);
	for (int i = 0; i < pixels; ++i)
	{
		mtx.m_data[i] = imgIn.m_data[i];
	}
}

//重建影像矩阵
void CImageProcessingEx::RebuiltDataMtx(CImageDataset& imgOut, CMyMatrix& mtx)
{
	int pixels = imgOut.m_xsize * imgOut.m_ysize*imgOut.m_rastercount;
	
	for (int i = 0; i < pixels; ++i)
	{
		imgOut.m_data[i]=mtx.m_data[i];
	}
}

//基于模拟链表数组的插入排序,用于排序特征值
int CImageProcessingEx::insertionSort(float* arr, int* link,int length)//这里是难点
{
	int head = 0;
	for (int i = 1; i < length; i++) //i在目标数组中顺序遍历
	{
		if (arr[i] > arr[head]) //如果当前位置的值小于head位置的值
		{
			link[i] = head;//将head连到当前位置的后面
			head = i;//将当前位置作为新的head
		}
		else //如果当前位置的值大于head位置的值
		{
			int j = head;//从head开始找
			while ((link[j] != -1) && (arr[link[j]] > arr[i]))
				j = link[j];//从head顺着link往后找，直到找到未连接的或者找到比当前值小的最大值为止

			if (link[j] == -1)//如果找到了未连接的
				link[j] = i;

			else //如果找到的是比当前值小的最大值
			{//则将当前值插到这个值后面

				link[i] = link[j];//相当于 curr->next = p->next;
				link[j] = i;	  //相当于    p->next = curr;
			}
		}
	}
	return head;
}

//特征值归一化
void CImageProcessingEx::Normalize(float* eigvectors, int n,int len)
{
	float average=0;
	float stDev = 0;
	for (int i = 0; i < n; ++i)
	{
		//计算平均数
		average = 0;
		for (int j = 0; j < len; ++j)
		{
			average += eigvectors[i * len + j]/len;
		}

		//计算标准差
		stDev = 0;
		for (int j = 0; j < len; ++j)
		{
			float tmp = average - eigvectors[i * len + j];
			stDev += tmp*tmp/len;
		}
		stDev = sqrt(stDev);

		//归一化
		for (int j = 0; j < len; ++j)
		{
			eigvectors[i * len + j]= (eigvectors[i * len + j]- average)/stDev;
		}
	}
}

//基于HIS的线性拉伸
BOOL CImageProcessingEx::stretch(CImageDataset& imgIn, CImageDataset& imgOut, double I_coe, double H_coe, double S_coe)
{
	/* 确保输入图像不为空 */
	if (imgIn.empty())
	{
		return FALSE;
	}

	//记录输入图像的尺寸
	init(imgIn);

	/* 将输入图像复制给输出图像 */
	if (FALSE == imgOut.create(n_col, n_row, n_band))
	{
		return FALSE;
	}

	double* in_data = imgIn.m_data;
	double* out_data= imgOut.m_data;

	double R, G, B, I, H, S,val;
	for (int row = 0; row < n_row; ++row)
	{
		for (int col = 0; col < n_col; ++col)
		{
			getRGB(in_data,row,col, R, G, B);//获得当前点的RGB
			RGB2IHS(R, G, B, I, H, S);//转换为IHS
			I = (val=I * I_coe) < 255 ? val : 255;
			H = (val=H * H_coe) < 360 ? val : 360;		//乘拉伸系数,同时防止超出范围
			S = (val=S * S_coe) < 1.0 ? val : 1.0;
			IHS2RGB(I, H, S,R, G, B);//转换为RGB
			setRGB(out_data, row, col, R, G, B);//设置当前点的RGB
		}
	}

	return TRUE;
}

