// StripRemoveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "StripRemoveDlg.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "afxdialogex.h"


// CStripRemoveDlg 对话框

IMPLEMENT_DYNAMIC(CStripRemoveDlg, CDialogEx)

CStripRemoveDlg::CStripRemoveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStripRemoveDlg::IDD, pParent)
	, inclinationValue(0)
	, angleValue(0)
{

}

CStripRemoveDlg::~CStripRemoveDlg()
{
}

void CStripRemoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INCLINATION_SLIDER, inclination_slider);
	DDX_Control(pDX, IDC_ANGLE_SLIDER, angle_slider);
	DDX_Text(pDX, IDC_INCLINATION_EDIT, inclinationValue);
	DDX_Text(pDX, IDC_ANGLE_EDIT, angleValue);
}


BEGIN_MESSAGE_MAP(CStripRemoveDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_INCLINATION_EDIT, &CStripRemoveDlg::OnEnChangeInclinationEdit)
	ON_EN_CHANGE(IDC_ANGLE_EDIT, &CStripRemoveDlg::OnEnChangeAngleEdit)
	ON_BN_CLICKED(IDOK, &CStripRemoveDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStripRemoveDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CStripRemoveDlg 消息处理程序


BOOL CStripRemoveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	inclination_slider.SetRange(-90,90);
	inclination_slider.SetTicFreq(1);
	inclination_slider.SetPos(12);

	angle_slider.SetRange(0,5);
	angle_slider.SetTicFreq(1);
	angle_slider.SetPos(2);

	UpdateData(TRUE);

	inclinationValue=12;
	angleValue=2;

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CStripRemoveDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl* slider = (CSliderCtrl*)pScrollBar;
	if (slider = &inclination_slider)
	{
		UpdateData(TRUE);
		inclinationValue = inclination_slider.GetPos();
		UpdateData(FALSE);
	}
	if (slider = &angle_slider)
	{
		UpdateData(TRUE);
		angleValue = angle_slider.GetPos();
		UpdateData(FALSE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CStripRemoveDlg::OnEnChangeInclinationEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData(TRUE);
	inclination_slider.SetPos((int)inclinationValue);
}


void CStripRemoveDlg::OnEnChangeAngleEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData(TRUE);
	angle_slider.SetPos((int)angleValue);
}


void CStripRemoveDlg::OnBnClickedOk()
{
	//图像处理
	BOOL bSuccess = CImageProcessingEx::StripRemove(CimageProcessingDlg::imgIn, CimageProcessingDlg::imgOut, inclinationValue, angleValue);
	if (!bSuccess)
	{
		AfxMessageBox("处理图像失败！");
		return;
	}

	//显示处理结果
	CWnd* pMainWin = GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if (CimageProcessingDlg::imgOut.m_rastercount >= 3)
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "结果", 1, 2, 3, 0);
	else
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "结果", 1, 1, 1, 0);
	CDialogEx::OnOK();
}




void CStripRemoveDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
