#pragma once

// CHistMatchDlg 对话框

class CHistMatchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistMatchDlg)

public:
	CHistMatchDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistMatchDlg();

// 对话框数据
	enum { IDD = IDD_HISTMATCHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString RefImgPath;
	CImageDataset imgRef;
public:
	afx_msg void OnBnClickedHistmatchChoose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedShowRefImg();
};
