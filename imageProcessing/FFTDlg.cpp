// FFTDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "FFTDlg.h"
#include "afxdialogex.h"
#include "Image.h"

// CFFTDlg �Ի���

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


// CFFTDlg ��Ϣ�������


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

		//ͼ����
		BOOL bSuccess = CImageProcessingEx::DFT_show(CimageProcessingDlg::imgIn,CimageProcessingDlg::imgOut,showWhat,option,isFFT);
		if(!bSuccess) 
		{ 
			AfxMessageBox("����ͼ��ʧ�ܣ�"); 
			return; 
		} 
	}
	else if(doWhat==DO_BOTH)
	{
		//ͼ����
		BOOL bSuccess = CImageProcessingEx::DFT_IDFT(CimageProcessingDlg::imgIn,CimageProcessingDlg::imgOut,isFFT);
		if(!bSuccess) 
		{ 
			AfxMessageBox("����ͼ��ʧ�ܣ�"); 
			return; 
		} 
	}
			
	//��ʾ������
	CWnd* pMainWin=GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if(CimageProcessingDlg::imgOut.m_rastercount>=3) 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "���", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "���", 1, 1, 1, 0); 

	CDialogEx::OnOK();
}


void CFFTDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	option=LOG;
	((CButton *)GetDlgItem(IDC_LOG_RADIO))->SetCheck(TRUE);//ѡ�϶����任

	doWhat=FORWARD_ONLY;
	((CButton *)GetDlgItem(IDC_FORWARD_ONLY_RADIO))->SetCheck(TRUE);//ѡ�����任

	showWhat=ENERGY;
	((CButton *)GetDlgItem(IDC_ENERGY_RADIO))->SetCheck(TRUE);//ѡ��������

	isFFT = FFT;
	((CButton*)GetDlgItem(IDC_USE_FFT_RADIO))->SetCheck(TRUE);//ѡ��������

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
