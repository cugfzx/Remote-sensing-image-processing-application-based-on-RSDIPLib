// StripRemoveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "StripRemoveDlg.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "afxdialogex.h"


// CStripRemoveDlg �Ի���

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


// CStripRemoveDlg ��Ϣ�������


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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CStripRemoveDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	UpdateData(TRUE);
	inclination_slider.SetPos((int)inclinationValue);
}


void CStripRemoveDlg::OnEnChangeAngleEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	UpdateData(TRUE);
	angle_slider.SetPos((int)angleValue);
}


void CStripRemoveDlg::OnBnClickedOk()
{
	//ͼ����
	BOOL bSuccess = CImageProcessingEx::StripRemove(CimageProcessingDlg::imgIn, CimageProcessingDlg::imgOut, inclinationValue, angleValue);
	if (!bSuccess)
	{
		AfxMessageBox("����ͼ��ʧ�ܣ�");
		return;
	}

	//��ʾ������
	CWnd* pMainWin = GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if (CimageProcessingDlg::imgOut.m_rastercount >= 3)
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "���", 1, 2, 3, 0);
	else
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "���", 1, 1, 1, 0);
	CDialogEx::OnOK();
}




void CStripRemoveDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
