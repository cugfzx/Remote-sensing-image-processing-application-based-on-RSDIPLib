#pragma once


// CBilateralFilterDlg �Ի���

class CBilateralFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBilateralFilterDlg)

public:
	CBilateralFilterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBilateralFilterDlg();

// �Ի�������
	enum { IDD = IDD_BILATERAL_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	
private:
	CString N;
	CString sigma_s;
	CString sigma_r;
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
