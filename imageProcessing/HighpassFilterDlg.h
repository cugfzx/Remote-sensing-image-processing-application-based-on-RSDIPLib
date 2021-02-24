#pragma once
#include "afxwin.h"


// CHighpassFilterDlg 对话框

class CHighpassFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHighpassFilterDlg)

public:
	CHighpassFilterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHighpassFilterDlg();

// 对话框数据
	enum { IDD = IDD_HIGHPASS_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox filter_combox;
public:
	virtual BOOL OnInitDialog();
private:
	double cut_off;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
