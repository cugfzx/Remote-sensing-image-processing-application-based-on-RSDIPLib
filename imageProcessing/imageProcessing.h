
// imageProcessing.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CimageProcessingApp:
// �йش����ʵ�֣������ imageProcessing.cpp
//

class CimageProcessingApp : public CWinApp
{
public:
	CimageProcessingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CimageProcessingApp theApp;