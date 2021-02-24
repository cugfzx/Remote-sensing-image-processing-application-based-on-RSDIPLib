#pragma once

// CFFTDlg 对话框

class CFFTDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFFTDlg)

public:
	CFFTDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFFTDlg();

// 对话框数据
	enum { IDD = IDD_FFT_DIALOG };
	enum option{LINEAR=0,LOG=1};
	enum doWhat{FORWARD_ONLY=1,DO_BOTH=0};
	enum showWhat{FREQUENCY=0,ANGLE=1,ENERGY=2};
	enum isFFT{FFT=1,DFT=0};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedLinearRadio();
	afx_msg void OnBnClickedLogRadio();
	virtual BOOL OnInitDialog();
private:
	BOOL option;//拉伸显示方式
	BOOL doWhat;//正变换还是正反变换
	int showWhat;//展示频谱还是角谱还是能量谱
	bool isFFT;//是否采用FFT
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
