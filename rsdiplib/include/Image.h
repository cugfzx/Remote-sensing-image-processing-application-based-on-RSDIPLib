// ImageDataset.h: interface for the CImageDataset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEDATASET_H__277D4EAE_F0B2_4615_A6F7_8870945BD95D__INCLUDED_)
#define AFX_IMAGEDATASET_H__277D4EAE_F0B2_4615_A6F7_8870945BD95D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RSDIPLIB_API CImageDataset  
{
public:
	CImageDataset();
	virtual ~CImageDataset();

	void clear();
	BOOL empty();
	BOOL duplicate(CImageDataset &img);
	BOOL create(int, int, int, double value=0.0);
	
	int m_xsize;		//number of columns
	int m_ysize;		//number of rows
	int m_rastercount;	//number of bands
	double *m_data;		//image data stored as BSQ format
};

class RSDIPLIB_API CImageIO
{
public:
	static BOOL read(CImageDataset &img, const CString strFilename);
	static BOOL write(CImageDataset &img, const CString strFilename);
};

class RSDIPLIB_API CImageDisplay
{
public:
	static void show(CImageDataset &img, CWnd *pParent, CString strTitle, 
		int r=3, int g=2, int b=1,  int type=0);
};

class RSDIPLIB_API CImageProcessing
{
public:
	static BOOL minmax(CImageDataset &img, double *padfMinMax);
	static BOOL brightness(CImageDataset &imgIn, CImageDataset &imgOut, double percent);
	static BOOL histeq(CImageDataset &imgIn, CImageDataset &imgOut);
	static BOOL FourierTrans(CImageDataset &imgIn, CImageDataset &imgOut);
	static BOOL InverseFourierTrans(CImageDataset &imgIn, CImageDataset &imgOut);
	//More algorithms
};

#endif // !defined(AFX_IMAGEDATASET_H__277D4EAE_F0B2_4615_A6F7_8870945BD95D__INCLUDED_)
