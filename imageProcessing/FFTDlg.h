#pragma once

// CFFTDlg �Ի���

class CFFTDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFFTDlg)

public:
	CFFTDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFFTDlg();

// �Ի�������
	enum { IDD = IDD_FFT_DIALOG };
	enum option{LINEAR=0,LOG=1};
	enum doWhat{FORWARD_ONLY=1,DO_BOTH=0};
	enum showWhat{FREQUENCY=0,ANGLE=1,ENERGY=2};
	enum isFFT{FFT=1,DFT=0};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedLinearRadio();
	afx_msg void OnBnClickedLogRadio();
	virtual BOOL OnInitDialog();
private:
	BOOL option;//������ʾ��ʽ
	BOOL doWhat;//���任���������任
	int showWhat;//չʾƵ�׻��ǽ��׻���������
	bool isFFT;//�Ƿ����FFT
public:
	afx_msg void OnBnClickedInverseRadio();
	afx_msg void OnBnClickedForwardOnlyRadio();
	afx_msg void OnBnClickedDoBothRadio();
	afx_msg void OnBnClickedFrequencyRadio();
	afx_msg void OnBnClickedAngleRadio();
	afx_msg void OnBnClickedEnergyRadio();
	afx_msg void OnBnClickedUseDftRadio();
	afx_msg void OnBnClickedUseFftRadio();
};
