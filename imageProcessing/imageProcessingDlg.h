
// imageProcessingDlg.h : 头文件
//

#pragma once
#include "RSDIPLib.h"

// CimageProcessingDlg 对话框
class CimageProcessingDlg : public CDialogEx
{
// 构造
public:
	CimageProcessingDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IMAGEPROCESSING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CMenu m_Menu;
	CString m_strImgInput;   //读取图片的路径
	CString m_strImgOutput;  //保存图片的路径

public:
	static CImageDataset imgIn;     //读取后的遥感图像
	static CImageDataset imgOut;    //待保存的遥感图像
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
