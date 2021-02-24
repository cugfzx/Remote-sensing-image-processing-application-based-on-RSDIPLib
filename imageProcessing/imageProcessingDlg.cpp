
// imageProcessingDlg.cpp : ʵ���ļ�
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


CImageDataset CimageProcessingDlg::imgIn;     //��ȡ���ң��ͼ��
CImageDataset CimageProcessingDlg::imgOut;    //�������ң��ͼ��

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CimageProcessingDlg �Ի���




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


// CimageProcessingDlg ��Ϣ�������

BOOL CimageProcessingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���˵�����ӵ�ϵͳ�˵��С�
	m_Menu.LoadMenu(IDR_MENU1);  //  IDR_MENU1

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetMenu(&m_Menu);
	SetWindowText("ENVI����� V1.1 By ������");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CimageProcessingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CimageProcessingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//���ļ���ť
void CimageProcessingDlg::OnOpenfile()
{
	CFileDialog dlg(TRUE);///TRUE Ϊ OPEN �Ի���FALSE Ϊ SAVE AS �Ի���
	if(dlg.DoModal()==IDOK) 
	{ 
		if(FALSE == CImageIO::read(imgIn, dlg.GetPathName())) 
		{ 
			AfxMessageBox("��ȡͼ��ʧ�ܣ�"); 
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

//�����ļ���ť
void CimageProcessingDlg::OnSavefile()
{
	CFileDialog dlg(FALSE);///TRUE Ϊ OPEN �Ի���FALSE Ϊ SAVE AS �Ի���
	CImageDataset& img_P= imgOut;
	if(dlg.DoModal()==IDOK)
	{
		if (imgOut.empty())
		{
			img_P = imgIn;
		}
		

		if (FALSE == CImageIO::write(img_P, dlg.GetPathName()))
		{
			AfxMessageBox("����ͼ��ʧ�ܣ�");
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
		AfxMessageBox("����ͼ��ʧ�ܣ�"); 
		return; 
	} 
	if(imgOut.m_rastercount>=3) 
		CImageDisplay::show(imgOut, this, "ֱ��ͼ���⻯", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(imgOut, this, "ֱ��ͼ���⻯", 1, 1, 1, 0); 
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
	//ͼ����
	BOOL bSuccess = CImageProcessingEx::Laplace(CimageProcessingDlg::imgIn,CimageProcessingDlg::imgOut);
	if(!bSuccess) 
	{ 
		AfxMessageBox("����ͼ��ʧ�ܣ�"); 
		return; 
	} 
			
	//��ʾ������
	CWnd* pMainWin=GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if(CimageProcessingDlg::imgOut.m_rastercount>=3) 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "���", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "���", 1, 1, 1, 0); 
}

//����Ҷ�任
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
