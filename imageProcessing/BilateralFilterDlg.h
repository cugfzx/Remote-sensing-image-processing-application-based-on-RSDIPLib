#pragma once


// CBilateralFilterDlg 对话框

class CBilateralFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBilateralFilterDlg)

public:
	CBilateralFilterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBilateralFilterDlg();

// 对话框数据
	enum { IDD = IDD_BILATERAL_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
private:
	CString N;
	CString sigma_s;
	CString sigma_r;
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
