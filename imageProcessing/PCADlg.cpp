// PCADlg.cpp: 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "imageProcessing.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "PCADlg.h"
// CPCADlg 对话框

IMPLEMENT_DYNAMIC(CPCADlg, CDialogEx)

CPCADlg::CPCADlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PCA_DIALOG, pParent)
	, p(0)
	, recommend(FALSE)
{

}

CPCADlg::~CPCADlg()
{
}

void CPCADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_P, p);
	DDX_Check(pDX, IDC_CHECK1, recommend);
}


BEGIN_MESSAGE_MAP(CPCADlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPCADlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPCADlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &CPCADlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_SAVEPC_BUTTON, &CPCADlg::OnBnClickedSavepcButton)
	ON_BN_CLICKED(IDC_SAVE_INVERSE_BUTTON, &CPCADlg::OnBnClickedSaveInverseButton)
END_MESSAGE_MAP()


// CPCADlg 消息处理程序


void CPCADlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (recommend == FALSE)//如果没选使用推荐P值
	{
		if (p > CimageProcessingDlg::imgIn.m_rastercount || p <= 0)
		{
			AfxMessageBox("p值大于图像波段数");
			return;
		}
	}
	//图像处理
	BOOL bSuccess = CImageProcessingEx::PCA_transform(CimageProcessingDlg::imgIn, CimageProcessingDlg::imgOut, imgPC,p,recommend);
	if (!bSuccess)
	{
		AfxMessageBox("处理图像失败！");
		return;
	}


	//显示处理结果
	if (CimageProcessingDlg::imgOut.m_rastercount >= 3)
	CImageDisplay::show(imgPC, this, "主成分影像", 1, 2, 3, 0);
	else
	CImageDisplay::show(imgPC, this, "主成分影像", 1, 1, 1, 0);

	if (imgPC.m_rastercount >= 3)
		CImageDisplay::show(CimageProcessingDlg::imgOut, this, "主成分反变换结果", 1, 2, 3, 0);
	else
		CImageDisplay::show(CimageProcessingDlg::imgOut, this, "主成分反变换结果", 1, 1, 1, 0);

	//CDialogEx::OnOK();
}


void CPCADlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CPCADlg::OnBnClickedCheck1()
{
	int state = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	if(state==1)//选中
		((CButton*)GetDlgItem(IDC_EDIT_P))->EnableWindow(FALSE);
	else if (state == 0)//cancel选中
		((CButton*)GetDlgItem(IDC_EDIT_P))->EnableWindow(TRUE);
}


void CPCADlg::OnBnClickedSavepcButton()
{
	CFileDialog dlg(FALSE);///TRUE 为 OPEN 对话框，FALSE 为 SAVE AS 对话框
	if (dlg.DoModal() == IDOK)
	{

		if (FALSE == CImageIO::write(imgPC, dlg.GetPathName()))
		{
			AfxMessageBox("保存图像失败！");
		}
		else
		{
			AfxMessageBox("保存图像成功！");
		}
	}
}


void CPCADlg::OnBnClickedSaveInverseButton()
{
	CFileDialog dlg(FALSE);///TRUE 为 OPEN 对话框，FALSE 为 SAVE AS 对话框
	if (dlg.DoModal() == IDOK)
	{

		if (FALSE == CImageIO::write(CimageProcessingDlg::imgOut, dlg.GetPathName()))
		{
			AfxMessageBox("保存图像失败！");
		}
		else
		{
			AfxMessageBox("保存图像成功！");
		}
	}
}
