#include "StdAfx.h"
#include "ImageProcessingEx.h"
#include <iostream>
#include <iomanip>

//���徲̬��Ա����
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

//��ʼ��
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

//����ֱ��ͼ����
void CImageProcessingEx::getHist(const CImageDataset& img,int band,std::vector<int>& hist)
{
	int value;
	int N_row = img.m_ysize;
	int N_col = img.m_xsize;
	const double* imgData = img.m_data;
	for(int row=0; row<N_row; ++row)  //��ͼ���������н���ɨ�裬�������ֱ��ͼ
	{ 
		for(int col=0; col<N_col; ++col) 
		{ 
			int index = band * N_row * N_col + row * N_col + col;
			value=(UINT8)imgData[index];
			hist[value]++; 
		} 
	} 
}

//ֱ��ͼ����
BOOL CImageProcessingEx::histeq(CImageDataset &imgIn, CImageDataset &imgOut) 
{ 
	const int LEVEL = 256; //�Ҷ�ֵ����
	int k, row, col; //����ѭ��
	//double hist[LEVEL], accuHist[LEVEL]; //ֱ��ͼ���ۻ�ֱ��ͼ
	std::vector<int> hist(LEVEL);
	std::vector<double> accuHist(LEVEL);

	/* ȷ������ͼ��ǿ� */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/*��������ͼ��*/ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	//��������ͼ���������ָ��
	const double *input_data=imgIn.m_data;
	double *output_data = imgOut.m_data;
	int img_area = n_col * n_row;//��������������ε����ظ���

	for (int band = 0; band < n_band; band++)
	{/*  ���� 1����������ͼ���ֱ��ͼ  */
		getHist(imgIn, band,hist);

		/*  ���� 2�������ۻ�ֱ��ͼ  */
		accuHist[0] = hist[0] / img_area;
		for (k = 1; k < LEVEL - 1; k++) //��ֱ��ͼ��������ɨ�裬��������ۻ�ֱ��ͼ
		{
			accuHist[k] = accuHist[k - 1] + double(hist[k]) / img_area;
		}
		accuHist[255] = 1;

		/*  ���� 3���������ۻ�ֱ��ͼ�����Ҷȱ任���������������н������صĻҶȱ任 */
		int index, gray_value;
		for (row = 0; row < n_row; ++row)
		{
			for (col = 0; col < n_col; ++col)
			{
				//for(k=0;k<LEVEL;++k)//���һ��   ���һ��  ���һ��
				//{
				//	if(input_data[row*n_col+col] == k )
				//	{
				//		data[row*n_col+col] = int((LEVEL-1)*accuHist[k]+0.5);//�Ҷȱ任
				//		k=LEVEL;
				//	}
				//}
				//output_data[row*n_col+col] = int((LEVEL-1)*accuHist[gray_value]+0.5);//�Ҷȱ任

				index = coo2ind(band,row,col);
				gray_value = (int)input_data[index];

				output_data[index] = (LEVEL - 1) * accuHist[gray_value];//�Ҷȱ任
			}
		}
	}
	return TRUE;
}

//ֱ��ͼƥ��
BOOL CImageProcessingEx::histmatch(CImageDataset &imgIn, CImageDataset &imgRef, CImageDataset &imgOut)
{
	const int LEVEL = 256; //�Ҷ�ֵ����

	/* ȷ������Ͳο�ͼ��Ϊ�ջ�������ͼ�񲨶������� */ 
	if(imgIn.empty()||imgRef.empty()||imgIn.m_rastercount!=imgRef.m_rastercount) 
	{ 
		return FALSE; 
	} 

	int band,row,col,i,j;//���ڱ���

	/*ͳ�Ʊ�*/
	std::vector<int> input_hist(LEVEL), ref_hist(LEVEL);//����ͼֱ��ͼ�Ͳο�ͼֱ��ͼ
	std::vector<double> input_accuHist(LEVEL),ref_accuHist(LEVEL); //����ͼ�ۻ�ֱ��ͼ�Ͳο��ۻ�ֱ��ͼ
	
	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/* �������ͼ�� */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 
	double *output_data = imgOut.m_data; 
	const double *input_data=imgIn.m_data;
	const double *ref_data=imgRef.m_data;
	

	for (band = 0; band < n_band; band++)
	{
		/*  ���� 1����������ͼ��Ͳο�ͼ���ֱ��ͼ  */
		getHist(imgIn, band,input_hist);
		getHist(imgRef, band,ref_hist);

		/*  ���� 2����������ͼ��Ͳο�ͼ����ۻ�ֱ��ͼ  */
		input_accuHist[0] = input_hist[0] / n_area;
		ref_accuHist[0] = ref_hist[0] / n_area;
		for (i = 1; i < LEVEL - 1; ++i) //��������ۻ�ֱ��ͼ
		{
			input_accuHist[i] = input_accuHist[i - 1] + double(input_hist[i]) / n_area;
			ref_accuHist[i] = ref_accuHist[i - 1] + double(ref_hist[i]) / n_area;
		}
		input_accuHist[255] = 1;
		ref_accuHist[255] = 1;

		/*  ���� 3���������ۻ�ֱ��ͼ�����Ҷȱ任���������������н������صĻҶȱ任 */

		//����ӳ��
		vector<double> deviation(LEVEL);//�м���ʱ����
		int mapping_table[LEVEL];//��¼ӳ��ı�
		vector<double>::iterator minElement;

		for (i = 0; i < LEVEL; ++i)//������ͼ���ۻ�ֱ��ͼ�б���
		{
			for (j = 0; j < LEVEL; ++j)//�ڲο�ͼ���ۻ�ֱ��ͼ�б���
			{
				deviation[j] = abs(input_accuHist[i] - ref_accuHist[j]);//���������ۻ�ֱ��ͼ�е�ÿ��ֵ��ο�ͼ�ۻ�ֱ��ͼ�е�����ֵ����
			}
			minElement = min_element(deviation.begin(), deviation.end());//�ҵ���С�Ĳ�
			mapping_table[i] = distance(deviation.begin(), minElement);//��С����±����ӳ���

		}

		//�Ҷȱ任
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

//���߳���ֵ�˲�                                                                            
BOOL CImageProcessingEx::medium_filter(CImageDataset &imgIn, CImageDataset &imgOut,int win_len)
{
	/* ȷ������ͼ��Ϊ�� */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/* �������ͼ�� */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	int win_area=win_len*win_len;   //�������
	int border=win_len/2;			//�߿���
	int num_of_threads = 8;

	//��������̵Ĳ����ṹ�����鸳ֵ
	parameters* p = new parameters[num_of_threads];
	for (int i = 0; i < num_of_threads; ++i)
	{
		p[i].win_len = win_len;
		p[i].output = imgOut.m_data;
		p[i].input = imgIn.m_data;
		p[i].num_of_threads = num_of_threads;//�߳�����Ҳ�Ǵ���ļ��
		//�����ÿ�����̶�һ��

		p[i].start = i;//ÿ�����̵Ŀ�ʼ�����λ�ò�һ��
	}

	//////////////////////////////////���̲߳���///////////////////////

	HANDLE* Threads=new HANDLE[num_of_threads];//�̳߳�
	for(int i=0;i< num_of_threads;++i)
	{
		Threads[i] = CreateThread(NULL, 0, medium_filter_thread, p+i, 0, NULL);
		SetThreadAffinityMask(Threads[i], (DWORD_PTR)pow(2.0,i));//���ӵ���i��CPU�ں�
	}

	DWORD state;
	do{//�ȴ�ȫ�����̽���
		state=WaitForMultipleObjects(num_of_threads, Threads,TRUE,INFINITE);
	}while(state!=WAIT_OBJECT_0);

	//�رս���
	for(int i=0;i< num_of_threads;++i)
		CloseHandle(Threads[i]);

	//�ͷſռ�
	delete[]p;
	delete[]Threads;

	return TRUE;
	
}
DWORD WINAPI CImageProcessingEx::medium_filter_thread(LPVOID lpParamter)
{
	parameters*p = (parameters*)lpParamter;

	int index,band,row,col,i,j,k=0;		//���ڱ���

	int win_len=p->win_len;
	int win_area=win_len*win_len;   //�������
	int border=p->win_len/2;		//�߿���

	double* input_data=p->input;
	double* output_data=p->output;

	int start=p->start;
	int num_of_threads = p->num_of_threads;

	vector<double> window(win_area);//����

	for (band = 0; band < n_band; band++) {
		for (row = border + start; row < n_row - border; row += num_of_threads) {//ÿ��num_of_threads�д���һ�У�����ȫͼ
			for (col = border; col < n_col - border; ++col) {

				for (i = row - win_len / 2; i <= row + win_len / 2; ++i) {
					for (j = col - win_len / 2; j <= col + win_len / 2; ++j)//��������
					{
						index = coo2ind(band,i,j);
						window[k++] = input_data[index];//����window����
					}
				}

				index = coo2ind(band,row,col);
				std::nth_element(window.begin(), window.begin() + win_area / 2 + 1, window.end());//��STL����ֵ
				output_data[index] = window[win_area / 2 + 1];
				k = 0;
			}
		}
	}


	return 0L;
}

//���߳�˫���˲�
BOOL CImageProcessingEx::bilateral_filter(CImageDataset &imgIn, CImageDataset &imgOut,int N,double sigma_r,double sigma_s)
{
	/* ȷ������ͼ��Ϊ�� */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/* �������ͼ�� */ 
	if(FALSE == imgIn.duplicate(imgOut))
	{ 
		return FALSE; 
	} 

	int num_of_threads = 8;
	int win_len = 2 * N + 1;

	//������̵Ĳ����ṹ������
	bilateral_filter_parameters* p = new bilateral_filter_parameters[num_of_threads];
	for (int i = 0; i < num_of_threads; ++i)
	{
		p[i].win_len = win_len;
		p[i].output = imgOut.m_data;
		p[i].input = imgIn.m_data;
		p[i].num_of_threads = num_of_threads;//�߳�����Ҳ�Ǵ���ļ��
		p[i].N = N;
		p[i].sigma_r = sigma_r;
		p[i].sigma_s = sigma_s;

		p[i].start = i;//ÿ�����̵Ŀ�ʼ�����λ�ò�һ��
	}

	//////////////////////////////////���̲߳��д���/////////////////////////

	HANDLE* Threads = new HANDLE[num_of_threads];//���̳�
	for (int i = 0; i < num_of_threads; ++i)
	{
		Threads[i] = CreateThread(NULL, 0, bilateral_filter_thread, p + i, 0, NULL);
		SetThreadAffinityMask(Threads[i], (DWORD_PTR)pow(2.0, i));//���ӵ���i��CPU�ں�
	}

	DWORD state;
	do {//�ȴ�ȫ�����̽���
		state = WaitForMultipleObjects(num_of_threads, Threads, TRUE, INFINITE);
	} while (state != WAIT_OBJECT_0);

	//�رս���
	for (int i = 0; i < num_of_threads; ++i)
		CloseHandle(Threads[i]);

	//�ͷſռ�
	delete[]p;
	delete[]Threads;

	return TRUE;
}
DWORD WINAPI CImageProcessingEx::bilateral_filter_thread(LPVOID lpParamter)
{
	bilateral_filter_parameters* p = (bilateral_filter_parameters*)lpParamter;

	int index, row, col, i, j, band,k = 0;		//���ڱ���
	int win_len = p->win_len;
	int win_area = win_len * win_len;   //�������
	vector<double> window(win_area);//����
	int border = p->win_len / 2;		//�߿���
	double* input_data = p->input;
	double* output_data = p->output;
	int start = p->start;
	int num_of_threads = p->num_of_threads;
	int N = p->N;
	double sigma_r = p->sigma_r;
	double sigma_s = p->sigma_s;

	double molecules;//����
	double denominatores;//��ĸ

	for (band = 0; band < n_band; band++) {
		for (row = N + start; row < n_row - N; row += num_of_threads) {//ÿ��num_of_threads�д���һ��
			for (col = N; col < n_col - N; ++col) {//����ȫͼ

				//���ӷ�ĸ����
				molecules = 0;
				denominatores = 0;

				for (i = row - N; i <= row + N; ++i) {
					for (j = col - N; j <= col + N; ++j)//��������
					{
						double w = W(band, row, col, i, j, sigma_r, sigma_s, input_data);

						index = coo2ind(band, i, j);

						//�ۼ�
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

//������˹
BOOL CImageProcessingEx::Laplace(CImageDataset &imgIn, CImageDataset &imgOut)
{
	/* ȷ������ͼ��Ϊ�� */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/* �������ͼ�� */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	double *output_data = imgOut.m_data; 
	const double *input_data=imgIn.m_data;

	int index,row,col,i,j,band,k=0;	//���ڱ���,k������������˹�����б���

	int laplacian[9]={0,1,0,1,-4,1,0,1,0};

	double intermediate=0;//�м���

	for(band=0;band<n_band;band++)
		for(row=1; row<n_row-1; ++row){
			for(col=1; col<n_col-1; ++col){//����ȫͼ


				for(i=row-1;i<=row+1;++i){
					for(j=col-1;j<=col+1;++j)//��������
					{	
						//����laplace�����е�0
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

				//��0
				intermediate=0;
				k=0;
			}
	}


	return TRUE;
}

//����Ҷ�任�õ��������
BOOL CImageProcessingEx::DFT(CImageDataset &imgIn, complex<double> *complex_data,bool isFFT)
{
	/* ȷ������ͼ��Ϊ�� */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	const double *input_data=imgIn.m_data;

	int index,band,row,col;	//���ڱ���

	//���Ļ�
	for(band=0;band<n_band;band++)
		for(row=0; row<n_row; ++row){
			for(col=0; col<n_col; ++col){
				index=coo2ind(band,row,col);
				complex_data[index]=input_data[index]*(((row*col)&(0x01))?-1:1);//�˴�����pow�������ɴ���Ż�
			}
		}

	//ѡ���Ƿ����FFT
	void (*dft_fun)(vector<complex<double>>&, bool);
	if (isFFT)
		dft_fun = FFT::fft;
	else
		dft_fun = MyDFT::dft;

	//��������
	vector<complex<double>> workArr(n_col);

	for (band = 0; band < n_band; band++)
	{
		//��ÿһ�н���һά����Ҷ�任
		for (row = 0; row < n_row; ++row)
		{
			//��һ�д�complex_data���ƽ�workArr
			memcpy(workArr.data(), complex_data + coo2ind(band,row,0), n_col * sizeof(complex<double>));

			dft_fun(workArr, FALSE);//����һ�н���һά����Ҷ�任

			//��һ�д�workArr���ƻ�complex_data,��Ϊ��һ��
			memcpy(complex_data + coo2ind(band, row, 0), workArr.data(), n_col * sizeof(complex<double>));
		}

		workArr.resize(n_row);//���workArr,��������Resize

		//��ÿһ�н���һά����Ҷ�任
		for (col = 0; col < n_col; ++col)
		{
			//��һ�д�complex_data���ƽ�workArr
			for (row = 0; row < n_row; ++row) {
				workArr[row] = complex_data[coo2ind(band, row, col)];
			}

			dft_fun(workArr, FALSE);//����һ�н���һά����Ҷ�任

			//��һ�д�workArr���ƻ�complex_data��һ��
			for (row = 0; row < n_row; ++row) {
				complex_data[coo2ind(band, row, col)] = workArr[row];
			}
		}
	}


	return TRUE;
}

//�õ�����ʾ�ĸ���Ҷ�任���
BOOL CImageProcessingEx::DFT_show(CImageDataset &imgIn, CImageDataset &imgOut,int showWhat,bool visual_trans_option, bool isFFT)
{
	/* ȷ������ͼ��Ϊ�� */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/* �������ͼ�� */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	//����Ƶ��
	complex<double> *complex_data=new complex<double>[n_pixel];

	//����Ҷ�任
	if(FALSE==DFT(imgIn, complex_data,isFFT))
	{
		return FALSE; 
	}

	//�Ը����������ת����0-255�Ա���ʾ
	visualize(complex_data,imgOut.m_data,n_pixel,showWhat,visual_trans_option);

	delete []complex_data;
	complex_data=NULL;

	return TRUE;
}

//ʹ����Ҷ�任������ӻ�
void CImageProcessingEx::visualize(complex<double>* complexImg, double* outputImg,int n_pixel,int doWhat,bool trans_option)
{
	double (*trans_fun)(const complex<double>& x);//���庯��ָ��
	enum showWhat{FREQUENCY=0,ANGLE=1,ENERGY=2};
	switch(doWhat)
	{
	case FREQUENCY://��ʾƵ��
	default:
		trans_fun=amplitude;break;
	case ANGLE://��ʾ���
		trans_fun=angle;break;
	case ENERGY://��ʾ����
		trans_fun=energy;break;
	}

	//ȡģ��ͬʱ�����ģ����Сģ
	for (int band = 0; band < n_band; ++band)
	{
		double max = 0;
		double min = DBL_MAX;
		for (int i = 0; i <n_area; ++i)
		{
			int index= band * n_area + i;
			outputImg[index] = trans_fun(complexImg[index]);//ת��Ϊ���ȣ���ǣ����߹���

			//��¼��ǰ���ֵ����Сֵ
			if (outputImg[index] > max)
				max = outputImg[index];
			if (outputImg[index] < min)
				min = outputImg[index];
		}

		if (trans_option)
		{
			//�����任
			double c = 255 / log(max + 1);
			for (int i = 0; i < n_area; ++i)
			{
				outputImg[band * n_area + i] = c * log(outputImg[band * n_area + i] + 1);
			}
		}
		else
		{
			//��������
			double extr_diff = max - min;//����
			for (int i = 0; i < n_area; ++i)
			{
				outputImg[band * n_area + i] = (outputImg[band * n_area + i] - min) / extr_diff * 255;
			}
		}
	}
}

//����Ҷ���任
BOOL CImageProcessingEx::IDFT(complex<double> *complex_data,CImageDataset &imgOut,bool isFFT)
{
	/* ȷ������ͼ��Ϊ�� */ 
	if(complex_data==NULL) 
	{ 
		return FALSE; 
	} 

	double *output_data=imgOut.m_data;

	int index,band,row,col;	//���ڱ���

	//��������
	vector<complex<double>> workArr(n_col);

	//ѡ���Ƿ����FFT
	void (*dft_fun)(vector<complex<double>>&, bool);
	if (isFFT)
		dft_fun = FFT::fft;
	else
		dft_fun = MyDFT::dft;

	for (band = 0; band < n_band; band++)
	{
		//��ÿһ�н���һά����Ҷ�任
		for (row = 0; row < n_row; ++row)
		{
			//��һ�д�complex_data���ƽ�workArr
			memcpy(workArr.data(), complex_data + coo2ind(band, row, 0), n_col*sizeof(complex<double>));

			dft_fun(workArr, TRUE);//����һ�н���һά����Ҷ�任

			//��һ�д�workArr���ƻ�complex_data,��Ϊ��һ��
			memcpy(complex_data + coo2ind(band, row, 0), workArr.data(), n_col*sizeof(complex<double>));
		}

		workArr.resize(n_row);//���workArr,��������Resize

		//��ÿһ�н���һά����Ҷ�任
		for (col = 0; col < n_col; ++col)
		{
			//��һ�д�complex_data���ƽ�workArr
			for (row = 0; row < n_row; ++row) {
				workArr[row] = complex_data[coo2ind(band,row, col)];
			}

			dft_fun(workArr, TRUE);//����һ�н���һά����Ҷ�任

			//��һ�д�workArr���ƻ�output��һ��
			for (row = 0; row < n_row; ++row) {
				output_data[coo2ind(band,row, col)] = workArr[row].real();//ȡ��ʵ��
			}
		}
	}

	//���Ļ�
	for (band = 0; band < n_band; band++){
		for (row = 0; row < n_row; ++row) {
			for (col = 0; col < n_col; ++col) {
				index = coo2ind(band, row, col);
				output_data[index] = output_data[index] * (((row * col) & (0x01)) ? -1 : 1);//�˴�����pow�������ɴ���Ż�
			}
		}
	}

	return TRUE;
}

//��������Ҷ�任�ٷ��任�����ֵ
BOOL CImageProcessingEx::DFT_IDFT(CImageDataset &imgIn, CImageDataset &imgOut,bool isFFT)
{
	/* ȷ������ͼ��Ϊ�� */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/* �������ͼ�� */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	//���ڴ��Ƶ��
	complex<double> *complex_data=new complex<double>[n_pixel];

	//����Ҷ�任
	if(FALSE==DFT(imgIn,complex_data,isFFT))
	{
		return FALSE;
	}

	//����Ҷ���任
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

//��ͨ�˲�
BOOL CImageProcessingEx::HighpassFilter(CImageDataset &imgIn, CImageDataset &imgOut,int filterType,double cut_off)
{
	/* ȷ������ͼ��Ϊ�� */ 
	if(imgIn.empty()) 
	{ 
		return FALSE; 
	} 

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/* �������ͼ�� */ 
	if(FALSE == imgOut.create(n_col,n_row,n_band))
	{ 
		return FALSE; 
	} 

	//����Ƶ��
	complex<double> *complex_data=new complex<double>[n_pixel];
	
	//����Ҷ�任�õ�Ƶ��
	if(FALSE==DFT(imgIn, complex_data,TRUE))
	{
		return FALSE; 
	}

	//���ݴ������Ĳ���ѡ���˲���
	enum filter_type{IDEAL=0,BUTTERWORTH=1,GUASSIAN=2};
	double (*filter)(int row,int col,double cut);
	switch(filterType)
	{
	case IDEAL://�����ͨ�˲���
		filter=ideal_highpass;
		break;
	case BUTTERWORTH://butterworth
		filter=butterworth;
		break;
	case GUASSIAN://��˹
		filter=Guassian_highpass;
		break;
	}

	int band,row,col,i;	//���ڱ���
	//�����
	for (band = 0; band < n_band; band++) {
		for (row = 0; row < n_row; ++row) {
			for (col = 0; col < n_col; ++col) {//����ȫͼ
				i = coo2ind(band,row, col);
				complex_data[i] = complex_data[i] * filter(row, col, cut_off);
			}
		}
	}

	//����Ҷ���任
	if(FALSE==IDFT(complex_data,imgOut,TRUE))
	{
		return FALSE;
	}

	delete []complex_data;
	complex_data=NULL;

	return TRUE;

}

//����ȥ��
BOOL CImageProcessingEx::StripRemove(CImageDataset& imgIn, CImageDataset& imgOut, double inclination, double angle)
{
	/* ȷ������ͼ��Ϊ�� */
	if (imgIn.empty())
	{
		return FALSE;
	}

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/* �������ͼ�� */
	if (FALSE == imgOut.create(n_col, n_row, n_band))
	{
		return FALSE;
	}

	//����Ƶ��
	complex<double>* complex_data = new complex<double>[n_pixel];

	//����Ҷ�任�õ�Ƶ��
	if (FALSE == DFT(imgIn, complex_data,TRUE))
	{
		return FALSE;
	}


	int band,row, col, i;	//���ڱ���
	//�����
	for (band = 0; band < n_band; band++) {
		for (row = 0; row < n_row; ++row) {
			for (col = 0; col < n_col; ++col) {//����ȫͼ
				i = coo2ind(band, row, col);
				complex_data[i] = complex_data[i] * WedgeFilter(row, col, inclination, angle);
			}
		}
	}

	//����Ҷ���任
	if (FALSE == IDFT(complex_data, imgOut,TRUE))
	{
		return FALSE;
	}

	delete[]complex_data;
	complex_data = NULL;

	return TRUE;
}

//PCA�任
BOOL CImageProcessingEx::PCA_transform(CImageDataset& imgIn, CImageDataset& imgOut,CImageDataset& imgPC,int P,bool recommend)
{
	/* ȷ������ͼ��Ϊ�� */
	if (imgIn.empty())
	{
		return FALSE;
	}

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/* �������任���ͼ�� */
	if (FALSE == imgOut.create(n_col, n_row, n_band))
	{
		return FALSE;
	}
	/* ����������ɷ�ͼ�� */
	if (FALSE == imgPC.create(n_col, n_row, n_band))
	{
		return FALSE;
	}

	/********************STEP 1������Ӱ�����ݾ��� X*********************************/
	CMyMatrix X;
	BuiltDataMtx(imgIn, X);

	/********************STEP 2������ԭʼͼ�����ݾ��� X ������Э�������S***********/
	CMyMatrix S= X.cov();

	/******************STEP 3������Э����S���������ֵ����������********************/
	float* eigvalues = new float[S.m_rows];
	float* eigvectors = new float[S.m_cols*S.m_rows];
	S.eig(eigvalues, eigvectors);

	/************************STEP 4�������任����T************************/

	//����������һ��
	Normalize(eigvectors, S.m_rows, S.m_cols);

	//������ֵ�Ӵ�С����
	int* link = new int[S.m_rows];//����������±�
	for (int i = 0; i < S.m_rows; ++i)//��ʼ��Ϊ-1
		{link[i] = -1;}
	int head = insertionSort(eigvalues, link, S.m_rows);//����ģ������ļ�Ӳ�������


	//���ն�Ӧ����ֵ��˳���Ը���������Ϊ�й��ɾ���U
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

	//�������U��ת�þ��󣬼�Ϊ����� PCA �任�ı任����UT
	CMyMatrix UT = U.transpose();

	/**************STEP 5������������**********************/
	CMyMatrix Y = (UT*X);

	//�������ɷ�Ӱ��
	for (int i = 0; i < n_pixel; ++i)
		imgPC.m_data[i] = Y.m_data[i];

	/**************STEP 6������ǰ p ���������ع�ң��Ӱ�����ݾ����༴���ɷַ��任 *******/
	int p = P;//Ĭ��p�Ǵ����Ĳ���P
	
	if (recommend)//�����ʹ���Զ��Ƽ�pֵ�ͼ��㷽�����
	{
		double eig_sum = 0;
		for (int j = 0; j < S.m_cols; ++j)//��������ֵ֮��
		{
			eig_sum += eigvalues[j];
		}

		p = 0;
		double sum = 0;
		int i = head;

		do
		{
			sum += eigvalues[i]/eig_sum;
			i = link[i];//�Ƶ���һ�����ֵ
			p++;//����
		} while (sum < 0.85);
	}

	CMyMatrix ResultMtx(X.m_rows,Y.m_cols);//�������
	CMyMatrix Ui(X.m_rows,1);
	CMyMatrix Yi(1, Y.m_cols);

	k = head;
	for (int i = 0; i < p; ++i)
	{
		//��k��������������������Ui
		for (int j = 0; j < X.m_rows; ++j)
		{
			Ui.m_data[j] = eigvectors[k * X.m_rows + j];
		}
		k = link[k];//k����Ϊ��һ���������ֵ������


		//��Y�ĵ�i�и���Yi
		for (int j = 0; j < Y.m_cols; ++j)
		{
			Yi.m_data[j] = Y.m_data[i * Y.m_cols + j];
		}

		ResultMtx += (Ui*Yi);
	}

	/***************STEP 7�����ϲ��еõ���ң��Ӱ�����ݾ���ָ�Ϊm �����ε�����Ӱ�� ***********/
	RebuiltDataMtx(imgOut, ResultMtx);

	delete[]link;
	delete[]eigvalues;
	delete[]eigvectors;

	return TRUE;
}

//����Ӱ�����
void CImageProcessingEx::BuiltDataMtx(CImageDataset& imgIn, CMyMatrix& mtx)
{
	int pixels = imgIn.m_xsize * imgIn.m_ysize* imgIn.m_rastercount;
	mtx.create(imgIn.m_rastercount, imgIn.m_xsize * imgIn.m_ysize);
	for (int i = 0; i < pixels; ++i)
	{
		mtx.m_data[i] = imgIn.m_data[i];
	}
}

//�ؽ�Ӱ�����
void CImageProcessingEx::RebuiltDataMtx(CImageDataset& imgOut, CMyMatrix& mtx)
{
	int pixels = imgOut.m_xsize * imgOut.m_ysize*imgOut.m_rastercount;
	
	for (int i = 0; i < pixels; ++i)
	{
		imgOut.m_data[i]=mtx.m_data[i];
	}
}

//����ģ����������Ĳ�������,������������ֵ
int CImageProcessingEx::insertionSort(float* arr, int* link,int length)//�������ѵ�
{
	int head = 0;
	for (int i = 1; i < length; i++) //i��Ŀ��������˳�����
	{
		if (arr[i] > arr[head]) //�����ǰλ�õ�ֵС��headλ�õ�ֵ
		{
			link[i] = head;//��head������ǰλ�õĺ���
			head = i;//����ǰλ����Ϊ�µ�head
		}
		else //�����ǰλ�õ�ֵ����headλ�õ�ֵ
		{
			int j = head;//��head��ʼ��
			while ((link[j] != -1) && (arr[link[j]] > arr[i]))
				j = link[j];//��head˳��link�����ң�ֱ���ҵ�δ���ӵĻ����ҵ��ȵ�ǰֵС�����ֵΪֹ

			if (link[j] == -1)//����ҵ���δ���ӵ�
				link[j] = i;

			else //����ҵ����Ǳȵ�ǰֵС�����ֵ
			{//�򽫵�ǰֵ�嵽���ֵ����

				link[i] = link[j];//�൱�� curr->next = p->next;
				link[j] = i;	  //�൱��    p->next = curr;
			}
		}
	}
	return head;
}

//����ֵ��һ��
void CImageProcessingEx::Normalize(float* eigvectors, int n,int len)
{
	float average=0;
	float stDev = 0;
	for (int i = 0; i < n; ++i)
	{
		//����ƽ����
		average = 0;
		for (int j = 0; j < len; ++j)
		{
			average += eigvectors[i * len + j]/len;
		}

		//�����׼��
		stDev = 0;
		for (int j = 0; j < len; ++j)
		{
			float tmp = average - eigvectors[i * len + j];
			stDev += tmp*tmp/len;
		}
		stDev = sqrt(stDev);

		//��һ��
		for (int j = 0; j < len; ++j)
		{
			eigvectors[i * len + j]= (eigvectors[i * len + j]- average)/stDev;
		}
	}
}

//����HIS����������
BOOL CImageProcessingEx::stretch(CImageDataset& imgIn, CImageDataset& imgOut, double I_coe, double H_coe, double S_coe)
{
	/* ȷ������ͼ��Ϊ�� */
	if (imgIn.empty())
	{
		return FALSE;
	}

	//��¼����ͼ��ĳߴ�
	init(imgIn);

	/* ������ͼ���Ƹ����ͼ�� */
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
			getRGB(in_data,row,col, R, G, B);//��õ�ǰ���RGB
			RGB2IHS(R, G, B, I, H, S);//ת��ΪIHS
			I = (val=I * I_coe) < 255 ? val : 255;
			H = (val=H * H_coe) < 360 ? val : 360;		//������ϵ��,ͬʱ��ֹ������Χ
			S = (val=S * S_coe) < 1.0 ? val : 1.0;
			IHS2RGB(I, H, S,R, G, B);//ת��ΪRGB
			setRGB(out_data, row, col, R, G, B);//���õ�ǰ���RGB
		}
	}

	return TRUE;
}

