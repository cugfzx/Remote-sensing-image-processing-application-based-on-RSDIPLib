// HistMatchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "HistMatchDlg.h"
#include "afxdialogex.h"


// CHistMatchDlg 对话框

IMPLEMENT_DYNAMIC(CHistMatchDlg, CDialogEx)

CHistMatchDlg::CHistMatchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistMatchDlg::IDD, pParent)
	, RefImgPath(_T(""))
{

}

CHistMatchDlg::~CHistMatchDlg()
{
}

void CHistMatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, RefImgPath);
}


BEGIN_MESSAGE_MAP(CHistMatchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_HISTMATCH_CHOOSE, &CHistMatchDlg::OnBnClickedHistmatchChoose)
	ON_BN_CLICKED(IDOK, &CHistMatchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHistMatchDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SHOW_REF_IMG, &CHistMatchDlg::OnBnClickedShowRefImg)
END_MESSAGE_MAP()


// CHistMatchDlg 消息处理程序


void CHistMatchDlg::OnBnClickedHistmatchChoose()
{
	CFileDialog dlg(TRUE);//TRUE 为 OPEN 对话框，FALSE 为 SAVE AS 对话框
	if(dlg.DoModal()==IDOK) 
	{ 
		RefImgPath = dlg.GetPathName();
		UpdateData(FALSE); //更新控件
	}
	
	//读取参考图像存入imgRef
	if (FALSE == CImageIO::read(imgRef, RefImgPath)) 
	{
		AfxMessageBox("读取图像失败！");
	}

}


void CHistMatchDlg::OnBnClickedOk()
{
	//图像处理
	BOOL bSuccess = CImageProcessingEx::histmatch(CimageProcessingDlg::imgIn,imgRef,CimageProcessingDlg::imgOut); 
	if(!bSuccess) 
	{ 
		AfxMessageBox("处理图像失败！"); 
		return; 
	} 
			
	//显示处理结果
	CWnd* pMainWin=GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if(CimageProcessingDlg::imgOut.m_rastercount>=3) 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "直方图匹配结果", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "直方图匹配结果", 1, 1, 1, 0); 

	CDialogEx::OnOK();
}


void CHistMatchDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CHistMatchDlg::OnBnClickedShowRefImg()
{	
	//读取参考图像
	if(RefImgPath.IsEmpty())
	{
		AfxMessageBox("参考图像路径为空，请输入参考图像路径"); 
		return;
	}
	else if(imgRef.empty())
	{
		if(FALSE == CImageIO::read(imgRef, RefImgPath)) //读取参考图像存入imgRef
		{ 
			AfxMessageBox("读取图像失败！"); 
			return;
		} 
	}
	
	//显示参考图像
	CWnd* pMainWin=GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if(imgRef.m_rastercount>=3) 
		CImageDisplay::show(imgRef, pMainWin, "参考图像", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(imgRef, pMainWin, "参考图像", 1, 1, 1, 0);
	
}
