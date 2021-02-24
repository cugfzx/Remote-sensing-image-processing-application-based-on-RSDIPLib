#pragma once


// CPCADlg 对话框

class CPCADlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPCADlg)

public:
	CPCADlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPCADlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PCA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// //前p个主成分
	int p;

	// //是否采用推荐P值
	BOOL recommend;

	CImageDataset imgPC;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedSavepcButton();
	afx_msg void OnBnClickedSaveInverseButton();
};
