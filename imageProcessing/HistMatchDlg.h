#pragma once

// CHistMatchDlg �Ի���

class CHistMatchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistMatchDlg)

public:
	CHistMatchDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistMatchDlg();

// �Ի�������
	enum { IDD = IDD_HISTMATCHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
