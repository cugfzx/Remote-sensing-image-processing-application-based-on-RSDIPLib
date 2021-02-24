#pragma once
#include "afxcmn.h"


// CStripRemoveDlg 对话框

class CStripRemoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStripRemoveDlg)

public:
	CStripRemoveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStripRemoveDlg();

// 对话框数据
	enum { IDD = IDD_STRIP_REMOVE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	// 条带倾角滑块
	CSliderCtrl inclination_slider;

	// 楔形滤波器张角滑块
	CSliderCtrl angle_slider;

	// //条带倾角值
	double inclinationValue;

	// 楔形张角值
	double angleValue;
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeInclinationEdit();
	afx_msg void OnEnChangeAngleEdit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
