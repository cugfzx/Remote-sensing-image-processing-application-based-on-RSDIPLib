#pragma once


// CMediumFilterDlg �Ի���

class CMediumFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMediumFilterDlg)

public:
	CMediumFilterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMediumFilterDlg();

// �Ի�������
	enum { IDD = IDD_MEDIUMFILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
