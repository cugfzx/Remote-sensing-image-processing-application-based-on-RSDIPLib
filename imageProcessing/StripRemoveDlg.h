#pragma once
#include "afxcmn.h"


// CStripRemoveDlg �Ի���

class CStripRemoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStripRemoveDlg)

public:
	CStripRemoveDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStripRemoveDlg();

// �Ի�������
	enum { IDD = IDD_STRIP_REMOVE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	// ������ǻ���
	CSliderCtrl inclination_slider;

	// Ш���˲����Žǻ���
	CSliderCtrl angle_slider;

	// //�������ֵ
	double inclinationValue;

	// Ш���Ž�ֵ
	double angleValue;
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeInclinationEdit();
	afx_msg void OnEnChangeAngleEdit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
