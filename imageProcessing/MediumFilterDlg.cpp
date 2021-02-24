// MediumFilterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "MediumFilterDlg.h"
#include "afxdialogex.h"


// CMediumFilterDlg �Ի���

IMPLEMENT_DYNAMIC(CMediumFilterDlg, CDialogEx)

CMediumFilterDlg::CMediumFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMediumFilterDlg::IDD, pParent)
{

}

CMediumFilterDlg::~CMediumFilterDlg()
{
}

void CMediumFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMediumFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMediumFilterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMediumFilterDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO1, &CMediumFilterDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMediumFilterDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CMediumFilterDlg::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CMediumFilterDlg ��Ϣ�������


void CMediumFilterDlg::OnBnClickedOk()
{
	//ͼ����
		BOOL bSuccess = CImageProcessingEx::medium_filter(CimageProcessingDlg::imgIn,CimageProcessingDlg::imgOut,filterSize); 
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


void CMediumFilterDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

//3*3
void CMediumFilterDlg::OnBnClickedRadio1()
{
	filterSize=3;
}

//5*5
void CMediumFilterDlg::OnBnClickedRadio2()
{
	filterSize=5;
}

//7*7
void CMediumFilterDlg::OnBnClickedRadio3()
{
	filterSize=7;
}


BOOL CMediumFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	filterSize=3;
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);//ѡ��3*3

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
