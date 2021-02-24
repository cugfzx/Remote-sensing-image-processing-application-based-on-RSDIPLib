// CIHSStretchDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "CIHSStretchDlg.h"
#include "afxdialogex.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"


// CIHSStretchDlg 对话框

IMPLEMENT_DYNAMIC(CIHSStretchDlg, CDialogEx)

CIHSStretchDlg::CIHSStretchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IHS_DIALOG, pParent)
	, I_coe(0)
	, H_coe(0)
	, S_coe(0)
{

}

CIHSStretchDlg::~CIHSStretchDlg()
{
}

void CIHSStretchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_I_EDIT, I_coe);
	DDX_Text(pDX, IDC_H_EDIT, H_coe);
	DDX_Text(pDX, IDC_S_EDIT, S_coe);
	DDX_Control(pDX, IDC_I_SLIDER, I_Slider);
	DDX_Control(pDX, IDC_H_SLIDER, H_Slider);
	DDX_Control(pDX, IDC_S_SLIDER, S_Slider);
}


BEGIN_MESSAGE_MAP(CIHSStretchDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK, &CIHSStretchDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_I_EDIT, &CIHSStretchDlg::OnEnChangeIEdit)
	ON_EN_CHANGE(IDC_H_EDIT, &CIHSStretchDlg::OnEnChangeHEdit)
	ON_EN_CHANGE(IDC_S_EDIT, &CIHSStretchDlg::OnEnChangeSEdit)
END_MESSAGE_MAP()


// CIHSStretchDlg 消息处理程序


BOOL CIHSStretchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	I_Slider.SetRange(50, 150);
	I_Slider.SetTicFreq(1);
	I_Slider.SetPos(100);

	H_Slider.SetRange(50, 150);
	H_Slider.SetTicFreq(1);
	H_Slider.SetPos(100);

	S_Slider.SetRange(50, 150);
	S_Slider.SetTicFreq(1);
	S_Slider.SetPos(100);

	UpdateData(TRUE);

	I_coe = 100;
	H_coe = 100;
	S_coe = 100;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CIHSStretchDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl* slider = (CSliderCtrl*)pScrollBar;
	if (slider = &I_Slider)
	{
		UpdateData(TRUE);
		I_coe = I_Slider.GetPos();
		UpdateData(FALSE);
	}
	if (slider = &H_Slider)
	{
		UpdateData(TRUE);
		H_coe = H_Slider.GetPos();
		UpdateData(FALSE);
	}
	if (slider = &S_Slider)
	{
		UpdateData(TRUE);
		S_coe = S_Slider.GetPos();
		UpdateData(FALSE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CIHSStretchDlg::OnBnClickedOk()
{
	
	if (CimageProcessingDlg::imgIn.m_rastercount != 3)
	{
		AfxMessageBox("输入图像不是RGB图像");
		return;
	}

	//图像处理
	BOOL bSuccess = CImageProcessingEx::stretch(CimageProcessingDlg::imgIn, CimageProcessingDlg::imgOut, I_coe/100, H_coe/100,S_coe/100);
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
	CDialogEx::OnOK();
}


void CIHSStretchDlg::OnEnChangeIEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData(TRUE);
	I_Slider.SetPos(I_coe);
}


void CIHSStretchDlg::OnEnChangeHEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData(TRUE);
	H_Slider.SetPos(H_coe);
}


void CIHSStretchDlg::OnEnChangeSEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData(TRUE);
	S_Slider.SetPos(S_coe);
}
