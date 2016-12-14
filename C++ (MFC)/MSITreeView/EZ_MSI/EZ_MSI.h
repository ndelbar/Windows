
// EZ_MSI.h : main header file for the PROJECT_NAME application
//

#pragma once
#include "EZ_MSIDlg.h"
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CEZ_MSIApp:
// See EZ_MSI.cpp for the implementation of this class
//

class CEZ_MSIApp : public CWinApp
{
public:
	CEZ_MSIApp();
	CEZ_MSIDlg m_Dlg;
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEZ_MSIApp theApp;