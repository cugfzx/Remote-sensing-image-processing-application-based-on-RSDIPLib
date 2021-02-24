#pragma once


// CIHSStretchDlg 对话框

class CIHSStretchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIHSStretchDlg)

public:
	CIHSStretchDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CIHSStretchDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IHS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	double I_coe;
	double H_coe;
	double S_coe;
	CSliderCtrl I_Slider;
	CSliderCtrl H_Slider;
	CSliderCtrl S_Slider;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeIEdit();
	afx_msg void OnEnChangeHEdit();
	afx_msg void OnEnChangeSEdit();
};
