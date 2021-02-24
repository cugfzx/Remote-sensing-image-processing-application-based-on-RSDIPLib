// HistMatchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "imageProcessing.h"
#include "imageProcessingDlg.h"
#include "ImageProcessingEx.h"
#include "HistMatchDlg.h"
#include "afxdialogex.h"


// CHistMatchDlg �Ի���

IMPLEMENT_DYNAMIC(CHistMatchDlg, CDialogEx)

CHistMatchDlg::CHistMatchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistMatchDlg::IDD, pParent)
	, RefImgPath(_T(""))
{

}

CHistMatchDlg::~CHistMatchDlg()
{
}

void CHistMatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, RefImgPath);
}


BEGIN_MESSAGE_MAP(CHistMatchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_HISTMATCH_CHOOSE, &CHistMatchDlg::OnBnClickedHistmatchChoose)
	ON_BN_CLICKED(IDOK, &CHistMatchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHistMatchDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SHOW_REF_IMG, &CHistMatchDlg::OnBnClickedShowRefImg)
END_MESSAGE_MAP()


// CHistMatchDlg ��Ϣ�������


void CHistMatchDlg::OnBnClickedHistmatchChoose()
{
	CFileDialog dlg(TRUE);//TRUE Ϊ OPEN �Ի���FALSE Ϊ SAVE AS �Ի���
	if(dlg.DoModal()==IDOK) 
	{ 
		RefImgPath = dlg.GetPathName();
		UpdateData(FALSE); //���¿ؼ�
	}
	
	//��ȡ�ο�ͼ�����imgRef
	if (FALSE == CImageIO::read(imgRef, RefImgPath)) 
	{
		AfxMessageBox("��ȡͼ��ʧ�ܣ�");
	}

}


void CHistMatchDlg::OnBnClickedOk()
{
	//ͼ����
	BOOL bSuccess = CImageProcessingEx::histmatch(CimageProcessingDlg::imgIn,imgRef,CimageProcessingDlg::imgOut); 
	if(!bSuccess) 
	{ 
		AfxMessageBox("����ͼ��ʧ�ܣ�"); 
		return; 
	} 
			
	//��ʾ������
	CWnd* pMainWin=GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if(CimageProcessingDlg::imgOut.m_rastercount>=3) 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "ֱ��ͼƥ����", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(CimageProcessingDlg::imgOut, pMainWin, "ֱ��ͼƥ����", 1, 1, 1, 0); 

	CDialogEx::OnOK();
}


void CHistMatchDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CHistMatchDlg::OnBnClickedShowRefImg()
{	
	//��ȡ�ο�ͼ��
	if(RefImgPath.IsEmpty())
	{
		AfxMessageBox("�ο�ͼ��·��Ϊ�գ�������ο�ͼ��·��"); 
		return;
	}
	else if(imgRef.empty())
	{
		if(FALSE == CImageIO::read(imgRef, RefImgPath)) //��ȡ�ο�ͼ�����imgRef
		{ 
			AfxMessageBox("��ȡͼ��ʧ�ܣ�"); 
			return;
		} 
	}
	
	//��ʾ�ο�ͼ��
	CWnd* pMainWin=GetDlgItem(IDD_IMAGEPROCESSING_DIALOG);
	if(imgRef.m_rastercount>=3) 
		CImageDisplay::show(imgRef, pMainWin, "�ο�ͼ��", 1, 2, 3, 0); 
	else 
		CImageDisplay::show(imgRef, pMainWin, "�ο�ͼ��", 1, 1, 1, 0);
	
}
