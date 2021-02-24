// HighpassFilterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "HighpassFilterDlg.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "afxdialogex.h"


// CHighpassFilterDlg 对话框

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


// CHighpassFilterDlg 消息处理程序


BOOL CHighpassFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	filter_combox.InsertString(0,TEXT("理想高通滤波"));
	filter_combox.InsertString(1,TEXT("Butterworth滤波"));
	filter_combox.InsertString(2,TEXT("高斯高通滤波"));

	//默认选择Butterworth滤波
	filter_combox.SetCurSel(1);
	cut_off=-1;//默认-1

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CHighpassFilterDlg::OnBnClickedOk()
{
	//获取选择的滤波器
	int filterType=filter_combox.GetCurSel();
	UpdateData(TRUE);

	if(cut_off==-1) 
	{ 
		AfxMessageBox("请输入截止频率"); 
		return; 
	} 

	//图像处理
	BOOL bSuccess = CImageProcessingEx::HighpassFilter(CimageProcessingDlg::imgIn,CimageProcessingDlg::imgOut,filterType,cut_off);
	if(!bSuccess) 
	{ 
		AfxMessageBox("处理图像失败！"); 
		return; 
	} 
			
	//显示处理结果
	CWnd* pMainWin=GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if(CimageProcessingDlg::imgOut.m_rastercount>=3) 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "结果", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "结果", 1, 1, 1, 0); 

	CDialogEx::OnOK();
}


void CHighpassFilterDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
