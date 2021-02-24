// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RSDIPLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RSDIPLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef RSDIPLIB_EXPORTS
#define RSDIPLIB_API __declspec(dllexport)
#else
#define RSDIPLIB_API __declspec(dllimport)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"
#include "Matrix.h"

// This class is exported from the RSDIPLib.dll
class RSDIPLIB_API CRSDIPLib {
public:
	CRSDIPLib(void);
	// TODO: add your methods here.
};

extern RSDIPLIB_API int nRSDIPLib;

RSDIPLIB_API int fnRSDIPLib(void);


