// CBilateralFilterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "BilateralFilterDlg.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "afxdialogex.h"


// CBilateralFilterDlg �Ի���

IMPLEMENT_DYNAMIC(CBilateralFilterDlg, CDialogEx)

CBilateralFilterDlg::CBilateralFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBilateralFilterDlg::IDD, pParent)
	, N(_T(""))
	, sigma_s(_T(""))
	, sigma_r(_T(""))
{

}

CBilateralFilterDlg::~CBilateralFilterDlg()
{
}

void CBilateralFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILTERSIZE, N);
	DDX_Text(pDX, IDC_SIGMA_S, sigma_s);
	DDX_Text(pDX, IDC_SIGMA_R, sigma_r);
}


BEGIN_MESSAGE_MAP(CBilateralFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBilateralFilterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBilateralFilterDlg ��Ϣ�������


void CBilateralFilterDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	int i_N=(int)_tstof(N);
	double sigma_R=_tstof(sigma_r);
	double sigma_S=_tstof(sigma_s);
	//ͼ����
	BOOL bSuccess = CImageProcessingEx::bilateral_filter(CimageProcessingDlg::imgIn,CimageProcessingDlg::imgOut,i_N,sigma_R,sigma_S); 
	if(!bSuccess) 
	{ 
		AfxMessageBox("����ͼ��ʧ�ܣ�"); 
		return; 
	} 
			
	//��ʾ������
	CWnd* pMainWin=GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if(CimageProcessingDlg::imgOut.m_rastercount>=3) 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "���", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "���", 1, 1, 1, 0); 

	CDialogEx::OnOK();
	
}


BOOL CBilateralFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_FILTERSIZE)->SetWindowText(_T("5"));
	GetDlgItem(IDC_SIGMA_S)->SetWindowText(_T("3"));
	GetDlgItem(IDC_SIGMA_R)->SetWindowText(_T("0.1"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
