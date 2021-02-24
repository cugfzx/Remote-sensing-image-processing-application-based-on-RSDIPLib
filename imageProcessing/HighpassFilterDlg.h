#pragma once
#include "afxwin.h"


// CHighpassFilterDlg �Ի���

class CHighpassFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHighpassFilterDlg)

public:
	CHighpassFilterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHighpassFilterDlg();

// �Ի�������
	enum { IDD = IDD_HIGHPASS_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
