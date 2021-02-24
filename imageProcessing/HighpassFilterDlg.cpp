// HighpassFilterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "HighpassFilterDlg.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "afxdialogex.h"


// CHighpassFilterDlg �Ի���

IMPLEMENT_DYNAMIC(CHighpassFilterDlg, CDialogEx)

CHighpassFilterDlg::CHighpassFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHighpassFilterDlg::IDD, pParent)
	, cut_off(0)
{

}

CHighpassFilterDlg::~CHighpassFilterDlg()
{
}

void CHighpassFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILTER_COMBO, filter_combox);
	DDX_Text(pDX, IDC_CUTOFF_EDIT, cut_off);
}


BEGIN_MESSAGE_MAP(CHighpassFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHighpassFilterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHighpassFilterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CHighpassFilterDlg ��Ϣ�������


BOOL CHighpassFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	filter_combox.InsertString(0,TEXT("�����ͨ�˲�"));
	filter_combox.InsertString(1,TEXT("Butterworth�˲�"));
	filter_combox.InsertString(2,TEXT("��˹��ͨ�˲�"));

	//Ĭ��ѡ��Butterworth�˲�
	filter_combox.SetCurSel(1);
	cut_off=-1;//Ĭ��-1

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CHighpassFilterDlg::OnBnClickedOk()
{
	//��ȡѡ����˲���
	int filterType=filter_combox.GetCurSel();
	UpdateData(TRUE);

	if(cut_off==-1) 
	{ 
		AfxMessageBox("�������ֹƵ��"); 
		return; 
	} 

	//ͼ����
	BOOL bSuccess = CImageProcessingEx::HighpassFilter(CimageProcessingDlg::imgIn,CimageProcessingDlg::imgOut,filterType,cut_off);
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


void CHighpassFilterDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
