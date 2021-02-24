// FFTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "FFTDlg.h"
#include "afxdialogex.h"
#include "Image.h"

// CFFTDlg 对话框

IMPLEMENT_DYNAMIC(CFFTDlg, CDialogEx)

CFFTDlg::CFFTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFFTDlg::IDD, pParent)
{

}

CFFTDlg::~CFFTDlg()
{
}

void CFFTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFFTDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFFTDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFFTDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_LINEAR_RADIO, &CFFTDlg::OnBnClickedLinearRadio)
	ON_BN_CLICKED(IDC_LOG_RADIO, &CFFTDlg::OnBnClickedLogRadio)
	ON_BN_CLICKED(IDC_FORWARD_ONLY_RADIO, &CFFTDlg::OnBnClickedForwardOnlyRadio)
	ON_BN_CLICKED(IDC_DO_BOTH_RADIO, &CFFTDlg::OnBnClickedDoBothRadio)
	ON_BN_CLICKED(IDC_FREQUENCY_RADIO, &CFFTDlg::OnBnClickedFrequencyRadio)
	ON_BN_CLICKED(IDC_ANGLE_RADIO, &CFFTDlg::OnBnClickedAngleRadio)
	ON_BN_CLICKED(IDC_ENERGY_RADIO, &CFFTDlg::OnBnClickedEnergyRadio)
	ON_BN_CLICKED(IDC_USE_DFT_RADIO, &CFFTDlg::OnBnClickedUseDftRadio)
	ON_BN_CLICKED(IDC_USE_FFT_RADIO, &CFFTDlg::OnBnClickedUseFftRadio)
END_MESSAGE_MAP()


// CFFTDlg 消息处理程序


void CFFTDlg::OnBnClickedOk()
{
	if(doWhat==FORWARD_ONLY)
	{
		////////////////DEBUG////////////////
		/*double test[]={7,8,5,8,4,6,2,2,0};
		CimageProcessingDlg::imgIn.m_data=test;
		CimageProcessingDlg::imgIn.m_xsize=3;
		CimageProcessingDlg::imgIn.m_ysize=3;
		CimageProcessingDlg::imgIn.m_rastercount=1;*/
		//////////////////////////////////////

		//图像处理
		BOOL bSuccess = CImageProcessingEx::DFT_show(CimageProcessingDlg::imgIn,CimageProcessingDlg::imgOut,showWhat,option,isFFT);
		if(!bSuccess) 
		{ 
			AfxMessageBox("处理图像失败！"); 
			return; 
		} 
	}
	else if(doWhat==DO_BOTH)
	{
		//图像处理
		BOOL bSuccess = CImageProcessingEx::DFT_IDFT(CimageProcessingDlg::imgIn,CimageProcessingDlg::imgOut,isFFT);
		if(!bSuccess) 
		{ 
			AfxMessageBox("处理图像失败！"); 
			return; 
		} 
	}
			
	//显示处理结果
	CWnd* pMainWin=GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if(CimageProcessingDlg::imgOut.m_rastercount>=3) 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "结果", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "结果", 1, 1, 1, 0); 

	CDialogEx::OnOK();
}


void CFFTDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CFFTDlg::OnBnClickedLinearRadio()
{
	option=LINEAR;
}


void CFFTDlg::OnBnClickedLogRadio()
{
	option=LOG;
}


BOOL CFFTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	option=LOG;
	((CButton *)GetDlgItem(IDC_LOG_RADIO))->SetCheck(TRUE);//选上对数变换

	doWhat=FORWARD_ONLY;
	((CButton *)GetDlgItem(IDC_FORWARD_ONLY_RADIO))->SetCheck(TRUE);//选上正变换

	showWhat=ENERGY;
	((CButton *)GetDlgItem(IDC_ENERGY_RADIO))->SetCheck(TRUE);//选上能量谱

	isFFT = FFT;
	((CButton*)GetDlgItem(IDC_USE_FFT_RADIO))->SetCheck(TRUE);//选上能量谱

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CFFTDlg::OnBnClickedForwardOnlyRadio()
{
	doWhat=FORWARD_ONLY;
	((CButton *)GetDlgItem(IDC_LINEAR_RADIO))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_LOG_RADIO))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_ANGLE_RADIO))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_ENERGY_RADIO))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_FREQUENCY_RADIO))->EnableWindow(TRUE);
}


void CFFTDlg::OnBnClickedDoBothRadio()
{
	doWhat=DO_BOTH;
	((CButton *)GetDlgItem(IDC_LINEAR_RADIO))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_LOG_RADIO))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_ANGLE_RADIO))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_ENERGY_RADIO))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_FREQUENCY_RADIO))->EnableWindow(FALSE);
}


void CFFTDlg::OnBnClickedFrequencyRadio()
{
	showWhat=FREQUENCY;
}


void CFFTDlg::OnBnClickedAngleRadio()
{
	showWhat=ANGLE;
}


void CFFTDlg::OnBnClickedEnergyRadio()
{
	showWhat=ENERGY;
}


void CFFTDlg::OnBnClickedUseDftRadio()
{
	isFFT = DFT;
}


void CFFTDlg::OnBnClickedUseFftRadio()
{
	isFFT = FFT;
}
