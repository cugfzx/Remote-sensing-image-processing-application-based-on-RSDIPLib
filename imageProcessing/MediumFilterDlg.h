#pragma once


// CMediumFilterDlg 对话框

class CMediumFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMediumFilterDlg)

public:
	CMediumFilterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMediumFilterDlg();

// 对话框数据
	enum { IDD = IDD_MEDIUMFILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
private:
	int filterSize;
public:
	virtual BOOL OnInitDialog();
};
