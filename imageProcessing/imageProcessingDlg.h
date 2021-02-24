
// imageProcessingDlg.h : ͷ�ļ�
//

#pragma once
#include "RSDIPLib.h"

// CimageProcessingDlg �Ի���
class CimageProcessingDlg : public CDialogEx
{
// ����
public:
	CimageProcessingDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IMAGEPROCESSING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CMenu m_Menu;
	CString m_strImgInput;   //��ȡͼƬ��·��
	CString m_strImgOutput;  //����ͼƬ��·��

public:
	static CImageDataset imgIn;     //��ȡ���ң��ͼ��
	static CImageDataset imgOut;    //�������ң��ͼ��
	afx_msg void OnOpenfile();
	afx_msg void OnSavefile();
	afx_msg void OnHistequal();
	afx_msg void OnHistmatch();
	afx_msg void OnMediumFilter();
	afx_msg void OnBilateralFilter();
	afx_msg void OnAbout();
	afx_msg void OnLaplace();
	afx_msg void OnFourierTrans();
	afx_msg void OnHighpassFilter();
	afx_msg void OnStripeRemove();
	afx_msg void OnPca();
	afx_msg void OnIhs();
};
