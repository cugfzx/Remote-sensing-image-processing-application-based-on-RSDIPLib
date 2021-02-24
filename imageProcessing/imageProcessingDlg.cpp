
// imageProcessingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "HistMatchDlg.h"
#include "MediumFilterDlg.h"
#include "BilateralFilterDlg.h"
#include "HighpassFilterDlg.h"
#include "StripRemoveDlg.h"
#include "FFTDlg.h"
#include "PCADlg.h"
#include "CIHSStretchDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CImageDataset CimageProcessingDlg::imgIn;     //读取后的遥感图像
CImageDataset CimageProcessingDlg::imgOut;    //待保存的遥感图像

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CimageProcessingDlg 对话框




CimageProcessingDlg::CimageProcessingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CimageProcessingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CimageProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CimageProcessingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_OPENFILE, &CimageProcessingDlg::OnOpenfile)
	ON_COMMAND(ID_SAVEFILE, &CimageProcessingDlg::OnSavefile)
	ON_COMMAND(ID_HISTEQUAL, &CimageProcessingDlg::OnHistequal)
	ON_COMMAND(ID_HISTMATCH, &CimageProcessingDlg::OnHistmatch)
	ON_COMMAND(ID_MEDIUM_FILTER, &CimageProcessingDlg::OnMediumFilter)
	ON_COMMAND(ID_BILATERAL_FILTER, &CimageProcessingDlg::OnBilateralFilter)
	ON_COMMAND(ID_ABOUT, &CimageProcessingDlg::OnAbout)
	ON_COMMAND(ID_LAPLACE, &CimageProcessingDlg::OnLaplace)
	ON_COMMAND(ID_FOURIER_TRANS, &CimageProcessingDlg::OnFourierTrans)
	ON_COMMAND(ID_HIGHPASS_FILTER, &CimageProcessingDlg::OnHighpassFilter)
	ON_COMMAND(ID_STRIPE_REMOVE, &CimageProcessingDlg::OnStripeRemove)
	ON_COMMAND(ID_PCA, &CimageProcessingDlg::OnPca)
	ON_COMMAND(ID_IHS, &CimageProcessingDlg::OnIhs)
END_MESSAGE_MAP()


// CimageProcessingDlg 消息处理程序

BOOL CimageProcessingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将菜单项添加到系统菜单中。
	m_Menu.LoadMenu(IDR_MENU1);  //  IDR_MENU1

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetMenu(&m_Menu);
	SetWindowText("ENVI低配版 V1.1 By 付祉祥");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CimageProcessingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CimageProcessingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CimageProcessingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//打开文件按钮
void CimageProcessingDlg::OnOpenfile()
{
	CFileDialog dlg(TRUE);///TRUE 为 OPEN 对话框，FALSE 为 SAVE AS 对话框
	if(dlg.DoModal()==IDOK) 
	{ 
		if(FALSE == CImageIO::read(imgIn, dlg.GetPathName())) 
		{ 
			AfxMessageBox("读取图像失败！"); 
		} 
		else 
		{ 
			m_strImgInput = dlg.GetPathName(); 
			UpdateData(FALSE); 
			if(imgIn.m_rastercount>=3) 
				CImageDisplay::show(imgIn, this, dlg.GetFileName(), 1, 2, 3, 0); 
			else 
				CImageDisplay::show(imgIn, this, dlg.GetFileName(), 1, 1, 1, 0); 
		} 
	} 
}

//保存文件按钮
void CimageProcessingDlg::OnSavefile()
{
	CFileDialog dlg(FALSE);///TRUE 为 OPEN 对话框，FALSE 为 SAVE AS 对话框
	CImageDataset& img_P= imgOut;
	if(dlg.DoModal()==IDOK)
	{
		if (imgOut.empty())
		{
			img_P = imgIn;
		}
		

		if (FALSE == CImageIO::write(img_P, dlg.GetPathName()))
		{
			AfxMessageBox("保存图像失败！");
		}
		else
		{
			if (imgOut.m_rastercount >= 3)
				CImageDisplay::show(img_P, this, dlg.GetFileName(), 1, 2, 3, 0);
			else
				CImageDisplay::show(img_P, this, dlg.GetFileName(), 1, 1, 1, 0);
		}
	}
}


void CimageProcessingDlg::OnHistequal()
{
	int bSuccess = CImageProcessingEx::histeq(imgIn, imgOut); 
	if(!bSuccess) 
	{ 
		AfxMessageBox("处理图像失败！"); 
		return; 
	} 
	if(imgOut.m_rastercount>=3) 
		CImageDisplay::show(imgOut, this, "直方图均衡化", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(imgOut, this, "直方图均衡化", 1, 1, 1, 0); 
}


void CimageProcessingDlg::OnHistmatch()
{
	CHistMatchDlg dlg;
	dlg.DoModal();

}


void CimageProcessingDlg::OnMediumFilter()
{
	CMediumFilterDlg dlg;
	dlg.DoModal();
}


void CimageProcessingDlg::OnBilateralFilter()
{
	CBilateralFilterDlg dlg;
	dlg.DoModal();
}


void CimageProcessingDlg::OnAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void CimageProcessingDlg::OnLaplace()
{
	//图像处理
	BOOL bSuccess = CImageProcessingEx::Laplace(CimageProcessingDlg::imgIn,CimageProcessingDlg::imgOut);
	if(!bSuccess) 
	{ 
		AfxMessageBox("处理图像失败！"); 
		return; 
	} 
			
	//显示处理结果
	CWnd* pMainWin=GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if(CimageProcessingDlg::imgOut.m_rastercount>=3) 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "结果", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "结果", 1, 1, 1, 0); 
}

//傅里叶变换
void CimageProcessingDlg::OnFourierTrans()
{
	CFFTDlg dlg;
	dlg.DoModal();
}


void CimageProcessingDlg::OnHighpassFilter()
{
	CHighpassFilterDlg dlg;
	dlg.DoModal();
}


void CimageProcessingDlg::OnStripeRemove()
{
	CStripRemoveDlg dlg;
	dlg.DoModal();
}


void CimageProcessingDlg::OnPca()
{
	CPCADlg dlg;
	dlg.DoModal();
}


void CimageProcessingDlg::OnIhs()
{
	CIHSStretchDlg dlg;
	dlg.DoModal();
}
