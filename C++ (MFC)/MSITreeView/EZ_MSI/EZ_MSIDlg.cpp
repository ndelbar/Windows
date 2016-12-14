
// EZ_MSIDlg.cpp : implementation file
//

// Todo: 

// 1.0 Todo
// - check for common problems (empty GUID, invalid shortcut)
// Digitally sign executable.

// 1.1 Todo
// Clean up code.
// - create multiple files for classes
// - Improve functions. and create functions for similar code.
// - Fix UI. Allow for Dark and light theme.

// 2.0 Todo.
// - Merge directories (Defer, since this could easily break shortcut linking)
// - double click to show file (Defer, since this is a complicated procedure.)

#include "stdafx.h"
#include "EZ_MSI.h"
#include "EZ_MSIDlg.h"
#include "afxdialogex.h"
#pragma comment(lib, "msi.lib")
#include <thread>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TYPE_STRING 's'
#define TYPE_INTEGER 'i'
#define TYPE_BINARY  'v'
#define ID_NAME 4006
#define ID_GUID 4007
#define ID_SHORTCUT_TARGET 4008
#define ID_REG_KEY_VALUE 4009

// CAboutDlg dialog used for App About

int g_nBackGroundGrey = 80;

int g_nFile = 8;
int g_nRegKey = 6;
int g_nReg = 4;
int g_nShortcut = 2;
int g_nDirectory = 0;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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




// CEZ_MSIDlg dialog



CEZ_MSIDlg::CEZ_MSIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEZ_MSIDlg::IDD, pParent)
	, m_strTest(_T(""))
	, m_bSafeToResize(false)
	, m_nExtraWindows(1)
	, m_nFindItemLocation(0)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEZ_MSIDlg* g_pMSIDlg;

void CEZ_MSIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_editMSITree);
	DDX_Text(pDX, IDC_EDIT1, m_strTest);
	DDX_Control(pDX, IDC_EDIT1, m_editTestCtrl);
	DDX_Control(pDX, IDC_TREE2, m_editMSITree2);
	DDX_Control(pDX, IDC_FIND, m_editFind);
	DDX_Control(pDX, IDC_WARNING, m_ctrlWarning);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE, m_editFileSize);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	ON_WM_VSCROLL()
//	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CMyTreeCtrl::OnTvnSelchanged)
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CEZ_MSIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_EXPANDALL, &CEZ_MSIDlg::OnViewExpandall)
	ON_COMMAND(ID_VIEW_COLLAPSEALL, &CEZ_MSIDlg::OnViewCollapseall)
	ON_COMMAND(ID_EXTRAWINDOWS_2, &CEZ_MSIDlg::OnExtraWindows2)
	ON_COMMAND(ID_EXTRAWINDOWS_1, &CEZ_MSIDlg::OnExtrawindows1)
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	/*ON_WM_NCPAINT()*/
	ON_COMMAND(ID_MODIFYTREE_REMOVEEMPTRYDIRECTORIES, &CEZ_MSIDlg::OnModifytreeRemoveemptrydirectories)
	ON_EN_CHANGE(IDC_EDIT1, &CEZ_MSIDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_FIND, &CEZ_MSIDlg::OnBnClickedFind)
	ON_COMMAND(ID_EXPORT_EXPORTALL, &CEZ_MSIDlg::OnExportExportall)
	ON_COMMAND(ID_HELP_ABOUTMSITREEVIEW, &CEZ_MSIDlg::OnHelpAboutmsitreeview)
	ON_BN_CLICKED(IDC_WARNING, &CEZ_MSIDlg::OnBnClickedWarning)
	ON_COMMAND(ID_MODIFYTREE_VIEWREGISTRYKEYS, &CEZ_MSIDlg::OnModifytreeViewregistrykeys)
	ON_COMMAND(ID_FILE_CLOSE32772, &CEZ_MSIDlg::OnFileClose)
	ON_COMMAND(ID_FILE_LOADMSI, &CEZ_MSIDlg::OnFileLoadmsi)
	ON_COMMAND(ID_EXPORT_GENERATECOMMANDLINECODE, &CEZ_MSIDlg::OnExportGeneratecommandlinecode)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_VIEW_RELVATIVEFILESIZES, &CEZ_MSIDlg::OnViewRelvativefilesizes)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_MODIFYTREE_EVALUATEPATH, &CEZ_MSIDlg::OnModifytreeEvaluatepath)
END_MESSAGE_MAP()

BOOL CEZ_MSIDlg::OnEraseBkgnd(CDC* pDC)
{
	CWindowDC dc(this);

	CRect rect;
	GetClientRect(&rect);
	CBrush myBrush(RGB(g_nBackGroundGrey - 20, g_nBackGroundGrey - 20, g_nBackGroundGrey - 20));    // dialog background color
	CBrush *pOld = pDC->SelectObject(&myBrush);
	BOOL bRes = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOld);    // restore old brush
	return bRes;                       // CDialog::OnEraseBkgnd(pDC);
}

// CEZ_MSIDlg message handlers

BOOL CEZ_MSIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	m_hIcon = AfxGetApp()->LoadIcon(IDI_T);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_editMSITree2.ShowWindow(SW_HIDE);

	// TODO: Add extra initialization here

	UpdateData(FALSE);

	m_ImageListTree.Create(16, 16, ILC_COLOR32, 0, 1);
	m_ImageListTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_CLOSE));
	m_ImageListTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_CLOSE));

	m_ImageListTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_SHORTCUT));
	m_ImageListTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_SHORTCUT));

	m_ImageListTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_REG));
	m_ImageListTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_REG));

	m_ImageListTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_REG_KEY));
	m_ImageListTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_REG_KEY));

	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_0));
	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_10));
	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_20));
	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_30));
	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_40));
	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_50));
	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_60));
	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_70));
	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_80));
	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_90));
	m_ImageListTree.Add(AfxGetApp()->LoadIconW(IDI_100));
	
	m_ImageListTree.SetBkColor(m_ImageListTree.GetBkColor());
	m_editMSITree.SetImageList(&m_ImageListTree, LVSIL_NORMAL);
	m_editMSITree2.SetImageList(&m_ImageListTree, LVSIL_NORMAL);
	m_editMSITree.SetBkColor(RGB(g_nBackGroundGrey, g_nBackGroundGrey, g_nBackGroundGrey));
	
	m_editMSITree.SetTextColor(RGB(255, 255, 255));
	m_editMSITree2.SetBkColor(RGB(g_nBackGroundGrey, g_nBackGroundGrey, g_nBackGroundGrey));
	m_editMSITree2.SetTextColor(RGB(255, 255, 255));

	CString CommandLine(GetCommandLine());

	InterpretCommandLine(CommandLine);

	m_editMSITree.SetScrollPos(SB_VERT, 0, TRUE);

	int nLastSlash = m_strTest.ReverseFind(_T('\\'));
	SetWindowText(_T("MSI Tree View - ") + m_strTest.Mid(nLastSlash + 1));

	if (m_bExportAndClose)
		CDialogEx::OnOK();

	///////////////////////////////////////////
	// Logic to display warning for empty GUID
	///////////////////////////////////////////

	//const int nGUIDCount = m_a_strGUIDInComponentTable.GetCount();

	//for (int i = 0; i < nGUIDCount; i++)
	//{
	//	if (m_a_strGUIDInComponentTable[i].IsEmpty())
	//	{
	//		m_ctrlWarning.ShowWindow(TRUE);
	//		break;
	//	}
	//}
	///////////////////////////////////////////

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEZ_MSIDlg::InterpretCommandLine(CString& CommandLine)
{		

	//CommandLine = _T("\"C:\\Users\\Nick\\Documents\\Visual Studio 2015\\Projects\\EZ_MSI\\Debug\\MsiTreeView.exe\" \"C:\\Users\\Nick\\Desktop\\NIDMM1510\\Products\\NI-DMM\\nidmmfulli.msi\" export:\"C:\\Users\\Nick\\Desktop\\Test18.txt\"");

	CString strAppName = AfxGetAppName();
	const int nAppNameLocation = CommandLine.Find(strAppName);

#ifndef _DEBUG
	if (nAppNameLocation != -1)
	{
		// AppName + .exe + space
		CommandLine = CommandLine.Mid(nAppNameLocation + strAppName.GetLength() + 5);
	}
#endif

	const int nExportLocation = CommandLine.Find(_T("export:"));
	const int nEndExportLocation = CommandLine.Find(_T("\""), nExportLocation + 9);

	m_bExportAndClose = nExportLocation != -1;

	if (m_bExportAndClose)
	{
		m_strExportLocation = CommandLine.Mid(nExportLocation + 8, nEndExportLocation - nExportLocation + 8);
		m_strExportLocation.Remove('\"');
		CommandLine = CommandLine.Left(nExportLocation - 1);
	}

	int nDelimLocation = CommandLine.Find(_T("\" \""));
	CommandLine = CommandLine.Mid(nDelimLocation);
	CommandLine.Remove(_T('\"'));
	CommandLine.Trim();

	// set m_strTest to commandline to view the command line on launch
	m_strTest = CommandLine;

	//GetMSIData(nDelimLocation == -1 ? _T("C:\\Users\\Nick\\Desktop\\MStudioRecipeProcessor.msi") : m_strTest);
	//GetMSIData(nDelimLocation == -1 ? _T("C:\\Users\\Nick\\Desktop\\NIDMM1510\\Products\\NI-DMM_Runtime\\nidmmrtei.msi") : m_strTest);
#ifdef _DEBUG
	GetMSIData(m_strTest);
	//m_editTestCtrl.SetWindowText(CommandLine);
#else
	GetMSIData(m_strTest);
#endif



	PopulateTable(m_editMSITree);

	if (m_bExportAndClose)
	{
		OnExportExportall();
	}

	m_bSafeToResize = true;
	g_pMSIDlg = this;
}

void CEZ_MSIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

BOOL CEZ_MSIDlg::SamplePopulateTable()
{

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText = _T("United States");

	HTREEITEM hCountry = m_editMSITree.InsertItem(&tvInsert);

	// Insert subitems of that root. Pennsylvania is
	// a state in the United States, so its item will be a child
	// of the United States item. We won't set any image or states,
	// so we supply only the TVIF_TEXT mask flag. This
	// override provides nearly complete control over the
	// insertion operation without the tedium of initializing
	// a structure. If you're going to add lots of items
	// to a tree, you might prefer the structure override
	// as it affords you a performance win by allowing you
	// to initialize some fields of the structure only once,
	// outside of your insertion loop.

	HTREEITEM hPA = m_editMSITree.InsertItem(TVIF_TEXT,
		_T("Pennsylvania"), 0, 0, 0, 0, 0, hCountry, NULL);

	// Insert the "Washington" item and assure that it is
	// inserted after the "Pennsylvania" item. This override is 
	// more appropriate for conveniently inserting items with 
	// images.

	HTREEITEM hWA = m_editMSITree.InsertItem(_T("Washington"),
		0, 0, hCountry, hPA);

	// We'll add some cities under each of the states.
	// The override used here is most appropriate
	// for inserting text-only items.

	m_editMSITree.InsertItem(_T("Pittsburgh"), hPA, TVI_SORT);
	m_editMSITree.InsertItem(_T("Harrisburg"), hPA, TVI_SORT);
	m_editMSITree.InsertItem(_T("Altoona"), hPA, TVI_SORT);

	m_editMSITree.InsertItem(_T("Seattle"), hWA, TVI_SORT);
	m_editMSITree.InsertItem(_T("Kalaloch"), hWA, TVI_SORT);
	m_editMSITree.InsertItem(_T("Yakima"), hWA, TVI_SORT);

	return TRUE;
}



// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEZ_MSIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEZ_MSIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CEZ_MSIDlg::PopulateDirectoryTree(CMyTreeCtrl& MSITree)
{
	CMapStringToString DefaultDir;
	CMapStringToString DirectoryParent;

	for (int i = 0; i < m_a_strDirectoryParentInDirectory.GetCount(); i++)
	{
		bool bMatchingIDForParent = false;

		for (int j = 0; j < m_a_strDirectoryInDirectory.GetCount(); j++)
		{
			if (m_a_strDirectoryParentInDirectory[i] == m_a_strDirectoryInDirectory[j])
			{
				bMatchingIDForParent = true;
				break;
			}
		}

		if (!bMatchingIDForParent)
		{
			m_a_strDirectoryInDirectory.Add(m_a_strDirectoryParentInDirectory[i]);
			m_a_strDirectoryParentInDirectory.Add(_T(""));
			m_a_strDefaultDirInDirectory.Add(m_a_strDirectoryParentInDirectory[i]);
		}
	}

	for (int i = 0; i < m_a_strDirectoryInComponentTable.GetCount(); i++)
	{
		bool bMatchingIDForParent = false;

		for (int j = 0; j < m_a_strDirectoryInDirectory.GetCount(); j++)
		{
			if (m_a_strDirectoryInComponentTable[i] == m_a_strDirectoryInDirectory[j])
			{
				bMatchingIDForParent = true;
				break;
			}
		}

		if (!bMatchingIDForParent)
		{
			m_a_strDirectoryInDirectory.Add(m_a_strDirectoryInComponentTable[i]);
			m_a_strDirectoryParentInDirectory.Add(_T(""));
			m_a_strDefaultDirInDirectory.Add(m_a_strDirectoryInComponentTable[i]);
		}
	}

	for (int i = 0; i < m_a_strDirectoryInDirectory.GetCount(); i++)
	{
		CString strDirectoryInDirectory = m_a_strDirectoryInDirectory[i];
		CString strDefaultDirInDirectory = m_a_strDefaultDirInDirectory[i];
		CString strDirectoryParentInDirectory = m_a_strDirectoryParentInDirectory[i];

		DefaultDir.SetAt(strDirectoryInDirectory, strDefaultDirInDirectory);
		DirectoryParent.SetAt(strDirectoryInDirectory, strDirectoryParentInDirectory);
	}

	CStringArray aHTreeItems;
	int nCountToRemove = 0;
	// Place all root directories down first.
	for (int i = 0; i < m_a_strDirectoryInDirectory.GetCount(); i++)
	{
		if (DirectoryParent[m_a_strDirectoryInDirectory[i]].IsEmpty())
		{
			TVINSERTSTRUCT tvInsert;
			tvInsert.hParent = NULL;
			tvInsert.hInsertAfter = NULL;
			tvInsert.item.mask = TVIF_TEXT;

			int length = 1024;

			LPWSTR pwsz = m_a_strDefaultDirInDirectory[i].GetBuffer(length);

			// do something with the string that pwsz points to.

			m_a_strDefaultDirInDirectory[i].ReleaseBuffer();

			tvInsert.item.pszText = pwsz;

			HTREEITEM hTreeItem = MSITree.InsertItem(&tvInsert);
			MSITree.SetItemData(hTreeItem, i);
			m_DirectoryHandle.SetAt(m_a_strDirectoryInDirectory[i], hTreeItem);
			MSITree.Expand(hTreeItem, TVE_EXPAND);
			aHTreeItems.Add(m_a_strDirectoryInDirectory[i]);
			//MSITree.SetItemImage(hTreeItem, 0, 1);
		}
	}


	CStringArray aNextLevelHTreeItems;

	while (true)
	{
		for (int i = 0; i < m_a_strDirectoryInDirectory.GetCount(); i++)
		{
			bool bAddThisElement = false;

			for (int j = 0; j < aHTreeItems.GetCount(); j++)
			{
				CString LHS = aHTreeItems[j];
				CString RHS = m_a_strDirectoryParentInDirectory[i];

				if (LHS == RHS)
				{
					bAddThisElement = true;
					break;
				}
			}

			if (!DirectoryParent[m_a_strDirectoryInDirectory[i]].IsEmpty() && bAddThisElement)
			{
				TVINSERTSTRUCT tvInsert;
				tvInsert.hParent = m_DirectoryHandle[m_a_strDirectoryParentInDirectory[i]];
				tvInsert.hInsertAfter = NULL;
				tvInsert.item.mask = TVIF_TEXT;

				int length = 1024;

				LPWSTR pwsz = m_a_strDefaultDirInDirectory[i].GetBuffer(length);

				// do something with the string that pwsz points to.

				m_a_strDefaultDirInDirectory[i].ReleaseBuffer();

				tvInsert.item.pszText = pwsz;

				HTREEITEM hTreeItem = MSITree.InsertItem(&tvInsert);
				MSITree.SetItemData(hTreeItem, i);
				m_DirectoryHandle.SetAt(m_a_strDirectoryInDirectory[i], hTreeItem);
				aNextLevelHTreeItems.Add(m_a_strDirectoryInDirectory[i]);
				//MSITree.SetItemImage(hTreeItem, 0, 1);
			}
		}

		aHTreeItems.RemoveAll();
		aHTreeItems.Copy(aNextLevelHTreeItems);
		aNextLevelHTreeItems.RemoveAll();

		if (aHTreeItems.GetCount() == 0)
			break;
	}

	return TRUE;
}

BOOL CEZ_MSIDlg::PopulateFiles(CMyTreeCtrl& MSITree)
{
	CMapStringToString FileName;
	CMapStringToString DirectoryFromComponentID;

	for (int i = 0; i < m_a_strFileInFileTable.GetCount(); i++)
	{
		FileName.SetAt(m_a_strFileInFileTable[i], m_a_strFileNameInFileTable[i]);
	}

	for (int i = 0; i < m_a_strComponentInComponentTable.GetCount(); i++)
	{
		CString str1 = m_a_strComponentInComponentTable[i];
		CString str2 = m_a_strDirectoryInComponentTable[i];
		DirectoryFromComponentID.SetAt(m_a_strComponentInComponentTable[i], m_a_strDirectoryInComponentTable[i]);
	}

	for (int i = 0; i < m_a_strFileInFileTable.GetCount(); i++)
	{
		TVINSERTSTRUCT tvInsert;

		CString str1 = m_a_strComponentInFileTable[i];
		CString str2 = DirectoryFromComponentID[m_a_strComponentInFileTable[i]];

		tvInsert.hParent = m_DirectoryHandle[DirectoryFromComponentID[m_a_strComponentInFileTable[i]]];
		tvInsert.hInsertAfter = TVI_FIRST;
		tvInsert.item.mask = TVIF_TEXT;

		int length = 1024;

		LPWSTR pwsz = m_a_strFileNameInFileTable[i].GetBuffer(length);

		// do something with the string that pwsz points to.

		m_a_strFileNameInFileTable[i].ReleaseBuffer();

		tvInsert.item.pszText = pwsz;

		HTREEITEM hTreeItem = MSITree.InsertItem(&tvInsert);
		m_FileHandle.SetAt(m_a_strFileInFileTable[i], hTreeItem);
		MSITree.SetItemState(hTreeItem, TVIS_BOLD, TVIS_BOLD);
		MSITree.SetItemImage(hTreeItem, g_nFile, g_nFile);
		MSITree.SetItemData(hTreeItem, i);
	}

	return TRUE;
}

BOOL CEZ_MSIDlg::PopulateShortcuts(CMyTreeCtrl& MSITree)
{
	CMapStringToString ShortcutName;
	CMapStringToString DirectoryFromShortcutID;

	for (int i = 0; i < m_a_strShortcutInShortcut.GetCount(); i++)
	{
		ShortcutName.SetAt(m_a_strShortcutInShortcut[i], m_a_strNameInShortcut[i]);
		DirectoryFromShortcutID.SetAt(m_a_strShortcutInShortcut[i], m_a_strDirectoryInShortcut[i]);
	}

	for (int i = 0; i < m_a_strShortcutInShortcut.GetCount(); i++)
	{
		TVINSERTSTRUCT tvInsert;

		tvInsert.hParent = m_DirectoryHandle[m_a_strDirectoryInShortcut[i]];
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;

		int length = 1024;

		LPWSTR pwsz = m_a_strNameInShortcut[i].GetBuffer(length);

		// do something with the string that pwsz points to.

		m_a_strNameInShortcut[i].ReleaseBuffer();

		tvInsert.item.pszText = pwsz;

		HTREEITEM hTreeItem = MSITree.InsertItem(&tvInsert);
		m_ShortcutHandle.SetAt(m_a_strNameInShortcut[i], hTreeItem);
		MSITree.SetItemState(hTreeItem, TVIS_BOLD, TVIS_BOLD);
		MSITree.SetItemImage(hTreeItem, g_nShortcut, g_nShortcut);
		MSITree.SetItemData(hTreeItem, i);
	}

	return TRUE;
}

BOOL CEZ_MSIDlg::PopulateRegistry(CMyTreeCtrl & MSITree)
{

	for (int i = 0; i < m_a_strRootInRegistry.GetCount(); i++)
	{

	}

	return 0;
}


BOOL CEZ_MSIDlg::PopulateTable(CMyTreeCtrl& MSITree)
{
	PopulateDirectoryTree(MSITree);
	PopulateFiles(MSITree);
	PopulateShortcuts(MSITree);

	ExpandAll(MSITree);

	return TRUE;
}

BOOL CEZ_MSIDlg::GetMSIData(LPCTSTR szPath)
{
	MSIHANDLE hDatabase;

//	TCHAR strValue[MAX_PATH]; // assuming UNICODE here; use std::string if _MBCS instead
	DWORD cchPath = MAX_PATH;

	if (MsiOpenDatabase(szPath, MSIDBOPEN_READONLY, &hDatabase) != ERROR_SUCCESS)
	{
		TRACE("fail");
	}

	m_strMSIPath = szPath;

	GetColumnData(hDatabase, _T("Component")		, _T("Component")	, m_a_strComponentInComponentTable);
	GetColumnData(hDatabase, _T("Directory_")		, _T("Component")	, m_a_strDirectoryInComponentTable);
	GetColumnData(hDatabase, _T("ComponentId")		, _T("Component")	, m_a_strGUIDInComponentTable);

	GetColumnData(hDatabase, _T("FileName")			, _T("File")		, m_a_strFileNameInFileTable);
	GetColumnData(hDatabase, _T("File")				, _T("File")		, m_a_strFileInFileTable);
	GetColumnData(hDatabase, _T("Component_")		, _T("File")		, m_a_strComponentInFileTable);
	GetColumnData(hDatabase, _T("FileSize")			, _T("File")		, m_a_strFileSizeInFileTable);

	GetColumnData(hDatabase, _T("Directory")		, _T("Directory")	, m_a_strDirectoryInDirectory);
	GetColumnData(hDatabase, _T("Directory_Parent")	, _T("Directory")	, m_a_strDirectoryParentInDirectory);
	GetColumnData(hDatabase, _T("DefaultDir")		, _T("Directory")	, m_a_strDefaultDirInDirectory);


	GetColumnData(hDatabase, _T("Shortcut")			, _T("Shortcut")	, m_a_strShortcutInShortcut);
	GetColumnData(hDatabase, _T("Directory_")		, _T("Shortcut")	, m_a_strDirectoryInShortcut);
	GetColumnData(hDatabase, _T("Name")				, _T("Shortcut")	, m_a_strNameInShortcut);
	GetColumnData(hDatabase, _T("Target")			, _T("Shortcut")		, m_a_strTargetInShortcut);

	GetColumnData(hDatabase, _T("Root")				, _T("Registry")	, m_a_strRootInRegistry);
	GetColumnData(hDatabase, _T("Key")				, _T("Registry")	, m_a_strKeyInRegistry);
	GetColumnData(hDatabase, _T("Name")				, _T("Registry")	, m_a_strNameInRegistry);
	GetColumnData(hDatabase, _T("Value")			, _T("Registry")	, m_a_strValueInRegistry);
	GetColumnData(hDatabase, _T("Component_")		, _T("Registry")	, m_a_strComponent_InRegistry);

	MsiCloseHandle(hDatabase);

	return TRUE;
}

// Populate an array with the column of the MSI
// #1: MSI Handle #2: Column #3: Table #4: array to populate
BOOL CEZ_MSIDlg::GetColumnData(const MSIHANDLE &msiHandle, LPCTSTR szColumn, LPCTSTR szTable, CStringArray& a_strElements)
{
	MSIHANDLE hView, hRecord;
	CString strPath, strCommand;
	TCHAR szTemp[256], szValue[32768];
	int uColumns = 0;
	CStringArray *saColumnNames = new CStringArray();
	CWordArray *waTypes = new CWordArray();
	CObList *olRecords = new CObList();
	DWORD dwLength;
	long lValue;

	strCommand.Format(_T("SELECT `%s` From %s"), szColumn, szTable);

	const UINT nStatus = MsiDatabaseOpenView(msiHandle, strCommand, &hView);

	if (nStatus != ERROR_SUCCESS)
	{
		delete saColumnNames;
		delete waTypes;
		delete olRecords;

		return FALSE;
	}
	// execute query - not a parameter query so second parameter is NULL.
	if (MsiViewExecute(hView, NULL) != ERROR_SUCCESS)
	{
		//TRACE("fail");
	}

	// assumes hInstall and strDirectory are defined and correct

	// read column names
	MsiViewGetColumnInfo(hView, MSICOLINFO_NAMES, &hRecord);

	uColumns = MsiRecordGetFieldCount(hRecord);

	//uColumns == 0;
	saColumnNames->SetSize(uColumns);
	waTypes->SetSize(uColumns);

	a_strElements.SetSize(0, uColumns);

	if (MsiViewGetColumnInfo(hView, MSICOLINFO_TYPES, &hRecord) != ERROR_SUCCESS)
	{
		//TRACE("fail");
	}

	for (int i = 0; i < uColumns; i++)
	{
		//long length;
		dwLength = 256;
		MsiRecordGetString(hRecord, i + 1, szTemp, &dwLength);
		//length = atol(&szTemp[1]);
		switch (tolower(szTemp[0]))
		{
		case('s') : // normal string type
		case('l') : // localizable string
			waTypes->SetAt(i, TYPE_STRING);
			break;
		case('i') :
			waTypes->SetAt(i, TYPE_INTEGER);
			break;
		case('v') :
			waTypes->SetAt(i, TYPE_BINARY);
			break;
		}
	}

	// read records until there are no more records
	while (MsiViewFetch(hView, &hRecord) == ERROR_SUCCESS)
	{
		for (int i = 0; i < uColumns; i++)
		{
			switch (waTypes->GetAt(i))
			{
			case(TYPE_INTEGER) :
			{
				lValue = MsiRecordGetInteger(hRecord, i + 1);
				CString strElement;
				strElement.Format(_T("%d"), lValue);
				a_strElements.Add(strElement);
				break;
			}
			case(TYPE_STRING) :
			{
				dwLength = 32768;
				//MsiRecordGetString(hRecord, i + 1, szValue, &dwLength);
				//(*psaRecord)[i] = szValue;
				MsiRecordGetString(hRecord, i + 1, szValue, &dwLength);
				CString strElement(szValue);
				const int separator = strElement.Find(_T("|"));
				if (separator >= 0) { strElement = strElement.Mid(separator + 1); }
				a_strElements.Add(strElement);
				break;
			}
			case(TYPE_BINARY) :
			{
				CStringArray *psaRecord = new CStringArray;
				psaRecord->SetSize(uColumns);
				olRecords->AddTail(psaRecord);
				(*psaRecord)[i] = "{Binary Data}";

				delete psaRecord;
				/*
				don't read binary data into string, if you want to read it into
				a BYTE buffer, the code looks like:
				DWORD dwLen = MsiRecordDataSize(hRecord,i + 1);
				char *pBinary = new char[dwLen];
				MsiRecordReadStream(hRecord,i + 1,pBinary,&dwLen);
				*/
				break;
			}
			default: break;
			}
		}
	}

	MsiCloseHandle(hRecord);
	MsiViewClose(hView);
	MsiCloseHandle(hView);

	delete saColumnNames;
	delete waTypes;
	delete olRecords;

	return TRUE;
}

void CEZ_MSIDlg::DeleteAllArrays()
{
	m_a_strDirectoryInComponentTable.SetSize(0, 100);
	m_a_strComponentInComponentTable.SetSize(0,100);
	m_a_strGUIDInComponentTable.SetSize(0,100);

	m_a_strFileNameInFileTable.SetSize(0,100);
	m_a_strFileInFileTable.SetSize(0,100);
	m_a_strComponentInFileTable.SetSize(0,100);
	m_a_strFileSizeInFileTable.SetSize(0,100);

	m_a_strDirectoryInDirectory.SetSize(0,100);
	m_a_strDirectoryParentInDirectory.SetSize(0,100);
	m_a_strDefaultDirInDirectory.SetSize(0,100);

	m_a_strShortcutInShortcut.SetSize(0,100);
	m_a_strNameInShortcut.SetSize(0,100);
	m_a_strDirectoryInShortcut.SetSize(0,100);
	m_a_strTargetInShortcut.SetSize(0,100);

	m_a_strRootInRegistry.SetSize(0,100);
	m_a_strKeyInRegistry.SetSize(0,100);
	m_a_strNameInRegistry.SetSize(0,100);
	m_a_strValueInRegistry.SetSize(0,100);
	m_a_strComponent_InRegistry.SetSize(0,100);

	m_RegistryHandle.RemoveAll();
	m_DirectoryHandle.RemoveAll();
	m_FileHandle.RemoveAll();
	m_ShortcutHandle.RemoveAll();
}

void CEZ_MSIDlg::ExpandAll(CMyTreeCtrl& MSITree)
{
	HTREEITEM hItem;

	hItem = MSITree.GetRootItem();
	while (hItem != NULL)
	{
		MSITree.Expand(hItem, TVE_EXPAND);
		hItem = MSITree.GetNicksNextItem(hItem);
	}
}

void CEZ_MSIDlg::CollapseAll(CMyTreeCtrl& MSITree)
{
	HTREEITEM hItem;

	hItem = MSITree.GetRootItem();
	while (hItem != NULL)
	{
		CString strText = m_editMSITree.GetItemText(hItem);

		MSITree.Expand(hItem, TVE_COLLAPSE);
		hItem = MSITree.GetNicksNextItem(hItem);
	}
}

BOOL CEZ_MSIDlg::SampleCode()
{
	MSIHANDLE hDatabase, hView, hRecord;
	CString strPath, strCommand;
	TCHAR szTemp[256], szValue[32768];
	int uColumns;
	CStringArray *saColumnNames = new CStringArray();
	CWordArray *waTypes = new CWordArray();
	CObList *olRecords = new CObList();
	DWORD dwLength;
	long lValue;

	strPath = _T("C:\\Users\\Nick\\Desktop\\DotNet40.msi");
	if (MsiOpenDatabase(strPath, MSIDBOPEN_READONLY, &hDatabase) != ERROR_SUCCESS)
	{
		TRACE("fail");
	}

	strCommand = "SELECT FileName From File";

	if (MsiDatabaseOpenView(hDatabase, strCommand, &hView) != ERROR_SUCCESS)
	{
		TRACE("fail");
	}
	// execute query - not a parameter query so second parameter is NULL.
	if (MsiViewExecute(hView, NULL) != ERROR_SUCCESS)
	{
		TRACE("fail");
	}

	// read column names
	MsiViewGetColumnInfo(hView, MSICOLINFO_NAMES, &hRecord);
	// get total number of columns in table.
	uColumns = MsiRecordGetFieldCount(hRecord);
	saColumnNames->SetSize(uColumns);
	waTypes->SetSize(uColumns);

	for (int i = 0; i < uColumns; ++i)
	{
		dwLength = 256;
		if (MsiRecordGetString(hRecord, i + 1, szTemp, &dwLength) != ERROR_SUCCESS)
		{
			return FALSE;
		}
		saColumnNames->SetAt(i, szTemp);
	}

	MsiCloseHandle(hRecord);

	if (MsiViewGetColumnInfo(hView, MSICOLINFO_TYPES, &hRecord) != ERROR_SUCCESS)
	{
		TRACE("fail");
	}

	for (int i = 0; i < uColumns; i++)
	{
		//long length;
		dwLength = 256;
		MsiRecordGetString(hRecord, i + 1, szTemp, &dwLength);
		//length = atol(&szTemp[1]);
		switch (tolower(szTemp[0]))
		{
		case('s') : // normal string type
		case('l') : // localizable string
					waTypes->SetAt(i, TYPE_STRING);
			break;
		case('i') :
			waTypes->SetAt(i, TYPE_INTEGER);
			break;
		case('v') :
			waTypes->SetAt(i, TYPE_BINARY);
			break;
		}
	}
	MsiCloseHandle(hRecord);

	// read records until there are no more records
	while (MsiViewFetch(hView, &hRecord) == ERROR_SUCCESS)
	{
		CStringArray *psaRecord = new CStringArray;
		psaRecord->SetSize(uColumns);
		olRecords->AddTail(psaRecord);
		for (int i = 0; i < uColumns; i++)
		{
			switch (waTypes->GetAt(i))
			{
			case(TYPE_INTEGER) :
				lValue = MsiRecordGetInteger(hRecord, i + 1);
				break;
			case(TYPE_STRING) :
				dwLength = 32768;
				MsiRecordGetString(hRecord, i + 1, szValue, &dwLength);
				(*psaRecord)[i] = szValue;
				break;
			case(TYPE_BINARY) :
				(*psaRecord)[i] = "{Binary Data}";
				/*
				don't read binary data into string, if you want to read it into
				a BYTE buffer, the code looks like:
				DWORD dwLen = MsiRecordDataSize(hRecord,i + 1);
				char *pBinary = new char[dwLen];
				MsiRecordReadStream(hRecord,i + 1,pBinary,&dwLen);
				*/
				break;
			default: break;
			}


		}
	}
	MsiCloseHandle(hRecord);
	MsiViewClose(hView);
	MsiCloseHandle(hView);
	MsiCloseHandle(hDatabase);

	return TRUE;
}


void CEZ_MSIDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CDialogEx::OnSizing(fwSide, pRect);

	CString pos;

	m_editFind.SetWindowPos(NULL, (pRect->right - pRect->left) / 2 - 50, pRect->bottom - pRect->top - 96, 40, 24, SWP_NOZORDER);
	m_editTestCtrl.SetWindowPos(NULL, (pRect->right - pRect->left) / 2, pRect->bottom - pRect->top - 96, (pRect->right - pRect->left - 48) / 2, 26, SWP_NOZORDER);
	m_editMSITree.SetWindowPos(NULL, 11, 11, (pRect->right - pRect->left) / m_nExtraWindows - 37, pRect->bottom - pRect->top - 121, SWP_NOZORDER);
	m_editMSITree2.SetWindowPos(NULL, (pRect->right - pRect->left - 24) / m_nExtraWindows, 11, (pRect->right - pRect->left - 24) / m_nExtraWindows, pRect->bottom - pRect->top - 121, SWP_NOZORDER);

}


void CEZ_MSIDlg::OnSize(UINT nType, int cx, int cy)
{
	if (m_bSafeToResize)
	{
		ResizeTrees();
	}
}


void CEZ_MSIDlg::OnViewExpandall()
{
	ExpandAll(m_editMSITree);
	ExpandAll(m_editMSITree2);

}


void CEZ_MSIDlg::OnViewCollapseall()
{
	CollapseAll(m_editMSITree);
	CollapseAll(m_editMSITree2);
}

void CEZ_MSIDlg::OnExtraWindows2()
{
	m_nExtraWindows = 2;
	m_editMSITree2.ShowWindow(SW_SHOW);
	m_editMSITree2.ShowScrollBar(SB_VERT, FALSE);
	PopulateTable(m_editMSITree2);
	ResizeTrees();
}


void CEZ_MSIDlg::OnExtrawindows1()
{
	m_nExtraWindows = 1;
	m_editMSITree2.ShowWindow(SW_HIDE);
	ResizeTrees();
}

void CEZ_MSIDlg::ResizeTrees()
{
	RECT Rect;
	AfxGetMainWnd()->GetWindowRect(&Rect);

	m_editFileSize.SetWindowPos(NULL, 11, Rect.bottom - Rect.top - 96, 100, 24, SWP_NOZORDER);
	m_editFind.SetWindowPos(NULL, (Rect.right - Rect.left) / 2 - 50, Rect.bottom - Rect.top - 96, 40, 24, SWP_NOZORDER);
	m_editTestCtrl.SetWindowPos(NULL, (Rect.right - Rect.left) / 2, Rect.bottom - Rect.top - 96, (Rect.right - Rect.left - 48) / 2, 26, SWP_NOZORDER);
	m_editMSITree.SetWindowPos(NULL, 11, 11, (Rect.right - Rect.left) / m_nExtraWindows - 37, Rect.bottom - Rect.top - 121, SWP_NOZORDER);
	m_editMSITree2.SetWindowPos(NULL, (Rect.right - Rect.left - 24) / m_nExtraWindows, 11, (Rect.right - Rect.left - 24) / m_nExtraWindows, Rect.bottom - Rect.top - 121, SWP_NOZORDER);
}



void CEZ_MSIDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMyTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	//int nScroll = g_pMSIDlg->m_editMSITree.GetScrollPos(SB_VERT);

	//if (g_pMSIDlg->m_nExtraWindows == 2)
	//{
	//	RECT Rect;
	//	AfxGetMainWnd()->GetWindowRect(&Rect);
	//	int nHeigth = (Rect.bottom - Rect.top) * .04;
	//	g_pMSIDlg->m_editMSITree2.SetScrollPos(SB_VERT, nScroll + nHeigth);
	//	g_pMSIDlg->m_editMSITree2.SetRedraw();
	//	TRACE("\n%d", nHeigth);
	//}
	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);


}

//void CEZ_MSIDlg::OnNcPaint()
//{
//	// If you only change the menu bar, let the framework draw 
//	// the window first
//	Default();
//
//	// To accesses the entire screen area of a CWnd 
//	// (both client and nonclient areas).
//	CWindowDC dc(this);
//
//	CRect rc;
//	GetWindowRect(rc);
//
//	// Size of menu bar (non-client area) is smaller
//	rc.bottom = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
//
//	// Compute color increment
//	double decrement = (double)(LIGHT_COLOR - DARK_COLOR) / rc.Width();
//
//	// Draw menu bar
//	for (int i = 2; i < rc.Width() - 2; i++)
//	{
//		double color;
//		CPen* oldPen;
//		color = LIGHT_COLOR - decrement * (double)i;
//		CPen pen(PS_SOLID, 1, RGB(0, 0, (int)color));
//		dc.MoveTo(i, 1);
//		oldPen = dc.SelectObject(&pen);
//		dc.LineTo(i, rc.bottom - 1);
//		dc.SelectObject(oldPen);
//	}
//
//	// Area for buttons
//	CRect closeRect;
//	closeRect.left = rc.right - rc.left - 40;
//	closeRect.top = GetSystemMetrics(SM_CYFRAME);
//	closeRect.right = rc.right - rc.left - 5;
//	closeRect.bottom = GetSystemMetrics(SM_CYSIZE );
//
//	// Put the close button on the caption
//	dc.DrawFrameControl(closeRect,
//		DFC_CAPTION,
//		DFCS_CAPTIONCLOSE);
//
//	// Save button position
//	m_rcClose = closeRect;
//}


void CEZ_MSIDlg::OnModifytreeRemoveemptrydirectories()
{
	HTREEITEM hItem;

	int nItemImage;
	int nEmptyFoldersDetected = 1;
	
	while (nEmptyFoldersDetected)
	{
		nEmptyFoldersDetected = 0;
		hItem = m_editMSITree.GetRootItem();

		while (hItem != NULL)
		{
			m_editMSITree.GetItemImage(hItem, nItemImage, nItemImage);
			CString strText = m_editMSITree.GetItemText(hItem);

			HTREEITEM ChildItem = m_editMSITree.GetChildItem(hItem);

			if (!ChildItem && !nItemImage)
			{
				m_editMSITree.DeleteItem(hItem);
				nEmptyFoldersDetected++;
			}

			hItem = m_editMSITree.GetNicksNextItem(hItem);
		}
	}

	EnableMenuItem(GetMenu()->m_hMenu, ID_MODIFYTREE_REMOVEEMPTRYDIRECTORIES, MF_DISABLED);
}

HTREEITEM CMyTreeCtrl::GetNicksNextItem(HTREEITEM hItem)
{
	HTREEITEM ItemWereEvaluating = hItem;

	hItem = GetChildItem(hItem);

	HTREEITEM hParent = NULL;
	do 
	{
		if (hParent)
		{
			ItemWereEvaluating = hParent;
			hParent = NULL;
		}

		if (!hItem)
			hItem = GetNextItem(ItemWereEvaluating, TVGN_NEXT);

		hParent = GetParentItem(ItemWereEvaluating);

		if (!hItem)
			hItem = GetNextItem(hParent, TVGN_NEXT);


	} while (hParent && !hItem);

	return hItem;
}

bool g_bSearching = false;
bool g_bInitiateNewSearch = false;

UINT CEZ_MSIDlg::SearchTreeForString(LPVOID pParam)
{
	CString strRHS = theApp.m_Dlg.m_strTest;
	CString strLHS;
	strRHS.MakeUpper();

	HTREEITEM hItem;
	bool bLeftWildCardDetected = false;
	bool bRightWildCardDetected = false;

	hItem = theApp.m_Dlg.m_editMSITree.GetRootItem();

	for (int i = 0; i < theApp.m_Dlg.m_aFindItem.GetCount(); i++)
	{
		theApp.m_Dlg.m_editMSITree.SetItemState(theApp.m_Dlg.m_aFindItem[i], NULL, TVIF_STATE | TVIS_SELECTED);
	}

	theApp.m_Dlg.m_aFindItem.SetSize(0);
	theApp.m_Dlg.m_nFindItemLocation = 0;

	theApp.m_Dlg.m_editFind.EnableWindow(FALSE);

	g_bSearching = true;

	while (hItem != NULL)
	{
		strLHS = theApp.m_Dlg.m_editMSITree.GetItemText(hItem);
		strLHS.MakeUpper();

		if (strRHS.Find(_T("*.")) > -1)
		{
			strRHS = strRHS.Mid(2);
			bLeftWildCardDetected = true;
		}

		if (strRHS.Find(_T(".*")) > -1)
		{
			strRHS = strRHS.Left(strRHS.Find(_T(".")));
			bRightWildCardDetected = true;
		}

		if (bLeftWildCardDetected)
		{
			int nLocation = strLHS.ReverseFind(_T('.'));
			strLHS = strLHS.Mid(nLocation + 1);
		}

		if (bRightWildCardDetected)
		{
			int nLocation = strLHS.ReverseFind(_T('.'));
			strLHS = strLHS.Left(nLocation);
		}

		if (strLHS == strRHS)
		{
			theApp.m_Dlg.m_editFind.EnableWindow(TRUE);
			theApp.m_Dlg.m_editMSITree.SetItemStateEx(hItem, TVIS_SELECTED);
			theApp.m_Dlg.m_aFindItem.Add(hItem);
		}
		else
		{
			theApp.m_Dlg.m_editMSITree.SetItemStateEx(hItem, NULL);
		}


		hItem = theApp.m_Dlg.m_editMSITree.GetNicksNextItem(hItem);

		if (g_bInitiateNewSearch)
		{
			hItem = theApp.m_Dlg.m_editMSITree.GetRootItem();
			g_bInitiateNewSearch = false;
			strRHS = theApp.m_Dlg.m_strTest;
			strRHS.MakeUpper();
			theApp.m_Dlg.m_aFindItem.SetSize(0);
			theApp.m_Dlg.m_nFindItemLocation = 0;
			theApp.m_Dlg.m_editFind.EnableWindow(FALSE);
			bLeftWildCardDetected = false;
			bRightWildCardDetected = false;
		}
	}

	g_bSearching = false;

	return TRUE;
}

void CEZ_MSIDlg::OnEnChangeEdit1()
{
	UpdateData();

	if (!g_bSearching)
	{
		AfxBeginThread(SearchTreeForString, NULL);
	}
	else
	{
		g_bInitiateNewSearch = true;
	}
}


void CEZ_MSIDlg::OnBnClickedFind()
{
	if (m_nFindItemLocation < m_aFindItem.GetCount())
	{
		m_editMSITree.SelectItem(m_aFindItem[m_nFindItemLocation]);
		m_editMSITree.SetItemState(m_aFindItem[m_nFindItemLocation], TVIF_STATE | TVIS_SELECTED, TVIF_STATE | TVIS_SELECTED);

		if (m_nFindItemLocation > 0)
			m_editMSITree.SetItemState(m_aFindItem[m_nFindItemLocation - 1], NULL, TVIF_STATE | TVIS_SELECTED);
		else
			m_editMSITree.SetItemState(m_aFindItem[m_aFindItem.GetCount() - 1], NULL, TVIF_STATE | TVIS_SELECTED);

		m_nFindItemLocation++;

		if (m_nFindItemLocation >= m_aFindItem.GetCount())
			m_nFindItemLocation = 0;
	}
}

int CEZ_MSIDlg::GetItemImage(HTREEITEM hItem)
{
	int nImage = 0;
	m_editMSITree.GetItemImage(hItem, nImage, nImage);
	return nImage;	
}


void CEZ_MSIDlg::OnOK()
{
	return;
}

void CEZ_MSIDlg::CopyToClipBoard(LPCTSTR szString)
{
	CString strString(szString);

	if (OpenClipboard())
	{
		EmptyClipboard();
		size_t size = (strString.GetLength() + 1) * sizeof(TCHAR);
		HGLOBAL hClipboardData = GlobalAlloc(NULL, size);
		if (hClipboardData)
		{
			TCHAR* pchData = (TCHAR*)GlobalLock(hClipboardData);
			if (pchData)
			{
				memcpy(pchData, (TCHAR*)strString.GetString(), size);
				GlobalUnlock(hClipboardData);
				SetClipboardData(CF_UNICODETEXT, hClipboardData);
			}
		}
		CloseClipboard();
	}
}

void CMyTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	POINT p;
	if (GetCursorPos(&p))
	{
		//cursor position now in p.x and p.y
	}

	CPoint ptMousePos;
	UINT uFlags;
	HTREEITEM htItem;

	ptMousePos = (CPoint)GetMessagePos();  ScreenToClient(&ptMousePos);

	htItem = HitTest(ptMousePos, &uFlags);

	if (htItem == NULL && !(TVHT_ONITEM & uFlags))
	{
		return;
	}

	SelectItem(htItem);
	
	DWORD dwFile = GetItemData(htItem);

	int nImage = theApp.m_Dlg.GetItemImage(htItem);


	CMenu SubMenu, MainMenu;
	
	//SubMenu.CreatePopupMenu();
	//SubMenu.AppendMenu(MF_STRING, 4001, _T("1"));
	//SubMenu.AppendMenu(MF_STRING, 4002, _T("2"));
	//SubMenu.AppendMenu(MF_STRING, 4003, _T("4"));
	//SubMenu.AppendMenu(MF_STRING, 4004, _T("8"));
	//Sample 05:Create the Main Menu
	MainMenu.CreatePopupMenu();
	MainMenu.AppendMenu(MF_STRING, ID_NAME, GetItemText(GetSelectedItem()));
	int iComponent = 0;
	int iTarget = 0;
	bool bDirectory = false;
	HTREEITEM hTargetItem = theApp.m_Dlg.m_editMSITree.GetRootItem();

	if (nImage >= g_nFile)
	{
		for (iComponent = 0; iComponent < theApp.m_Dlg.m_a_strGUIDInComponentTable.GetCount(); iComponent++)
		{
			if (theApp.m_Dlg.m_a_strComponentInComponentTable[iComponent] == theApp.m_Dlg.m_a_strComponentInFileTable[dwFile])
				break;
		}
		MainMenu.AppendMenu(MF_STRING, ID_GUID, theApp.m_Dlg.m_a_strGUIDInComponentTable[iComponent]);
	}
	else if (nImage == g_nRegKey)
	{
		MainMenu.AppendMenu(MF_STRING, ID_REG_KEY_VALUE, _T("Value = ") + theApp.m_Dlg.m_a_strValueInRegistry[dwFile]);
	}
	else if (nImage == g_nShortcut)
	{
		bDirectory = theApp.m_Dlg.m_a_strTargetInShortcut[dwFile].Find(_T('#')) == -1;

		const int nCount = bDirectory ? theApp.m_Dlg.m_a_strDirectoryInDirectory.GetCount() : theApp.m_Dlg.m_a_strFileInFileTable.GetCount();

		CString strFileWithoutBrackets = theApp.m_Dlg.m_a_strTargetInShortcut[dwFile];
		strFileWithoutBrackets.Remove(_T('['));
		strFileWithoutBrackets.Remove(_T(']'));
		strFileWithoutBrackets.Remove(_T('#'));

		for (iTarget = 0; iTarget < nCount; iTarget++)
		{
			if (bDirectory)
			{
				if (theApp.m_Dlg.m_a_strDirectoryInDirectory[iTarget] == strFileWithoutBrackets)
				{
					MainMenu.AppendMenu(MF_STRING, ID_SHORTCUT_TARGET, _T("Target: ") + theApp.m_Dlg.m_a_strDirectoryInDirectory[iTarget]);

					while (hTargetItem != NULL)
					{
						int nImage = -1;
						theApp.m_Dlg.m_editMSITree.GetItemImage(hTargetItem, nImage, nImage);
						
						if (nImage == g_nDirectory)
							if (theApp.m_Dlg.m_a_strDirectoryInDirectory[theApp.m_Dlg.m_editMSITree.GetItemData(hTargetItem)] == theApp.m_Dlg.m_a_strDirectoryInDirectory[iTarget])
								break;

						hTargetItem = theApp.m_Dlg.m_editMSITree.GetNicksNextItem(hTargetItem);
					}

					break;
				}
			}
			else
			{
				if (theApp.m_Dlg.m_a_strFileInFileTable[iTarget] == strFileWithoutBrackets)
				{
					MainMenu.AppendMenu(MF_STRING, ID_SHORTCUT_TARGET, _T("Target: ") + theApp.m_Dlg.m_a_strFileInFileTable[iTarget]);

					while (hTargetItem != NULL)
					{
						int nImage = -1;
						theApp.m_Dlg.m_editMSITree.GetItemImage(hTargetItem, nImage, nImage);

						if (nImage == g_nFile)
							if (theApp.m_Dlg.m_a_strFileInFileTable[theApp.m_Dlg.m_editMSITree.GetItemData(hTargetItem)] == theApp.m_Dlg.m_a_strFileInFileTable[iTarget])
								break;

						hTargetItem = theApp.m_Dlg.m_editMSITree.GetNicksNextItem(hTargetItem);
					}

					break;
				}
			}

		}


	}

	//MainMenu.AppendMenu(MF_POPUP, (UINT)SubMenu.GetSafeHmenu(), _T("Line Thickness"));

	UINT nResult = MainMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, p.x, p.y, this);

	if (nResult == ID_NAME)
		theApp.m_Dlg.CopyToClipBoard(GetItemText(GetSelectedItem()));
	else if (nResult == ID_GUID)
		theApp.m_Dlg.CopyToClipBoard(theApp.m_Dlg.m_a_strGUIDInComponentTable[iComponent]);
	else if (nResult == ID_SHORTCUT_TARGET)
	{
		theApp.m_Dlg.m_editMSITree.SelectItem(hTargetItem);
		theApp.m_Dlg.m_editMSITree.SetItemState(hTargetItem, TVIF_STATE | TVIS_SELECTED, TVIF_STATE | TVIS_SELECTED);
	}

//	else if (nResult == ID_SHORTCUT_TARGET)


}

CString CMyTreeCtrl::GetFullPath(const HTREEITEM hItem)
{
	HTREEITEM local_hItem = hItem;
	CString strPathName;

	while (local_hItem)
	{
		strPathName = _T("\\") + GetItemText(local_hItem) + strPathName;
		local_hItem = GetParentItem(local_hItem);
	}

	strPathName.Replace(_T("\\.\\"), _T("\\"));

	return strPathName;
}

void CEZ_MSIDlg::OnExportExportall()
{
	HTREEITEM hItem;

	CStdioFile strExport;

	OPENFILENAME opn;

	TCHAR szFile[300];

	if (!m_bExportAndClose)
	{
		ZeroMemory(&opn, sizeof(opn));
		opn.lStructSize = sizeof(opn);
		opn.hwndOwner = GetSafeHwnd();
		opn.lpstrFile = szFile;
		opn.lpstrFile[0] = '\0';
		opn.nMaxFile = sizeof(szFile);
		opn.Flags = OFN_EXPLORER;
		opn.lpstrFilter = NULL;
		opn.lpstrCustomFilter = NULL;
		opn.nFilterIndex = 0;
		opn.lpstrFileTitle = _T("Export file");
		opn.lpstrInitialDir = NULL;
		opn.lpstrTitle = _T("Export file");
		opn.lpstrFilter = L"Export";
		opn.nFilterIndex = 1;
	}

	if (m_bExportAndClose || GetOpenFileName(&opn))
	{
		if (!m_bExportAndClose)
			CString test(opn.lpstrFile);

		BOOL bSuccess = strExport.Open(m_bExportAndClose ? m_strExportLocation : opn.lpstrFile, CFile::modeWrite | CFile::modeCreate | CFile::typeUnicode);

		hItem = m_editMSITree.GetRootItem();
		while (hItem != NULL)
		{
			int nImage = 0;
			m_editMSITree.GetItemImage(hItem, nImage, nImage);
			if (nImage)
			{
				strExport.WriteString(m_editMSITree.GetFullPath(hItem) + _T("\r\n"));
			}
			hItem = m_editMSITree.GetNicksNextItem(hItem);
		}
		strExport.Close();
	}
}


void CEZ_MSIDlg::OnHelpAboutmsitreeview()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CEZ_MSIDlg::OnBnClickedWarning()
{
	// TODO: Add your control notification handler code here
}


void CEZ_MSIDlg::OnModifytreeViewregistrykeys()
{
	////////////////////////////////////////////////////
	// Allow 2 windows to show separate information.
	////////////////////////////////////////////////////

	m_nExtraWindows = 2;
	m_editMSITree2.ShowWindow(SW_SHOW);
	m_editMSITree2.ShowScrollBar(SB_VERT, FALSE);
	
	CMyTreeCtrl& myTreeCtrl = m_editMSITree2;

	ResizeTrees();
	
	////////////////////////////////////////////////////

	CStringArray strAddedRoots;
	CStringArray aHTreeItems;

	for (int i = 0; i < m_a_strRootInRegistry.GetCount(); i++)
	{
		bool bSkip = false;

		for (int j = 0; j < strAddedRoots.GetCount(); j++)
		{
			if (strAddedRoots[j] == m_a_strRootInRegistry[i])
			{
				bSkip = true;
				break;
			}
		}

		if (bSkip)
			continue;
		else
			strAddedRoots.Add(m_a_strRootInRegistry[i]);

		TVINSERTSTRUCT tvInsert;
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;

		int length = 1024;

		
		CString strRootName;

		if (m_a_strRootInRegistry[i] == _T("-1"))
			strRootName = _T("HKEY_CURRENT_USER");
		else if (m_a_strRootInRegistry[i] == _T("0"))
			strRootName = _T("HKEY_CLASSES_ROOT");
		else if (m_a_strRootInRegistry[i] == _T("1"))
			strRootName = _T("HKEY_CURRENT_USER");
		else if (m_a_strRootInRegistry[i] == _T("2"))
				strRootName = _T("HKEY_LOCAL_MACHINE");
		else if (m_a_strRootInRegistry[i] == _T("3"))
			strRootName = _T("HKEY_USERS");

		LPWSTR pwsz = strRootName.GetBuffer(length);

		// do something with the string that pwsz points to.

		strRootName.ReleaseBuffer();

		tvInsert.item.pszText = pwsz;

		HTREEITEM hTreeItem = myTreeCtrl.InsertItem(&tvInsert);
		myTreeCtrl.SetItemData(hTreeItem, i);
		m_RegistryHandle.SetAt(m_a_strRootInRegistry[i], hTreeItem);
		myTreeCtrl.Expand(hTreeItem, TVE_EXPAND);
		aHTreeItems.Add(m_a_strRootInRegistry[i]);
		myTreeCtrl.SetItemImage(hTreeItem, g_nReg, g_nReg);
		//MSITree.SetItemImage(hTreeItem, 0, 1);
	}

	for (int i = 0; i < m_a_strKeyInRegistry.GetCount(); i++)
	{
		CStringArray strKeyElements;
		int curPos = 0;

		strKeyElements.Add(_T(""));

		while (curPos < m_a_strKeyInRegistry[i].GetLength() && curPos != -1)
			strKeyElements.Add(m_a_strKeyInRegistry[i].Tokenize(_T("\\"), curPos));

		HTREEITEM hTreeItem = NULL;
		HTREEITEM hParentTreeItem = NULL;

		CString strElements;
		
		for (int j = 0; j < strKeyElements.GetCount(); j++)
		{
			strElements = strElements + strKeyElements[j];
			hParentTreeItem = hTreeItem;
			hTreeItem = m_RegistryHandle[m_a_strRootInRegistry[i] + strElements];
			
			if (hTreeItem)
				continue;

			TVINSERTSTRUCT tvInsert;
			tvInsert.hParent = hParentTreeItem;
			tvInsert.hInsertAfter = NULL;
			tvInsert.item.mask = TVIF_TEXT;

			int length = 1024;

			LPWSTR pwsz = strKeyElements[j].GetBuffer(length);

			// do something with the string that pwsz points to.

			strKeyElements[j].ReleaseBuffer();

			tvInsert.item.pszText = pwsz;

			hTreeItem = myTreeCtrl.InsertItem(&tvInsert);
			m_RegistryHandle.SetAt(m_a_strRootInRegistry[i] + strElements, hTreeItem);
			myTreeCtrl.SetItemImage(hTreeItem, g_nReg, g_nReg);
		}
	}

	for (int i = 0; i < m_a_strNameInRegistry.GetCount(); i++)
	{
		CStringArray strKeyElements;
		int curPos = 0;

		CString strElements; 

		while (curPos < m_a_strKeyInRegistry[i].GetLength() && curPos != -1)
		{
			strElements = strElements + m_a_strKeyInRegistry[i].Tokenize(_T("\\"), curPos);
		}

		TVINSERTSTRUCT tvInsert;
		tvInsert.hParent = m_RegistryHandle[m_a_strRootInRegistry[i] + strElements];
		tvInsert.hInsertAfter = TVI_FIRST;
		tvInsert.item.mask = TVIF_TEXT;

		int length = 1024;

		LPWSTR pwsz = m_a_strNameInRegistry[i].GetBuffer(length);

		// do something with the string that pwsz points to.

		m_a_strNameInRegistry[i].ReleaseBuffer();

		tvInsert.item.pszText = pwsz;

		HTREEITEM hTreeItem = myTreeCtrl.InsertItem(&tvInsert);
		m_RegistryHandle.SetAt(m_a_strRootInRegistry[i] + strElements + m_a_strNameInRegistry[i], hTreeItem);
		myTreeCtrl.SetItemData(hTreeItem, i);
		//myTreeCtrl.SetItemState(hTreeItem, TVIS_BOLD, TVIS_BOLD);
		myTreeCtrl.SetItemImage(hTreeItem, g_nRegKey, g_nRegKey);
	}

	EnableMenuItem(GetMenu()->m_hMenu, ID_MODIFYTREE_VIEWREGISTRYKEYS, MF_DISABLED);

	ExpandAll(myTreeCtrl);

	myTreeCtrl.SetScrollPos(SB_VERT, 0, TRUE);
}


void CEZ_MSIDlg::OnFileClose()
{
	CDialogEx::OnOK();
}


void CEZ_MSIDlg::OnFileLoadmsi()
{
	OPENFILENAME opn;

	TCHAR szFile[300];

	ZeroMemory(&opn, sizeof(opn));
	opn.lStructSize = sizeof(opn);
	opn.hwndOwner = GetSafeHwnd();
	opn.lpstrFile = szFile;
	opn.lpstrFile[0] = '\0';
	opn.nMaxFile = sizeof(szFile);
	opn.Flags = OFN_EXPLORER;
	opn.lpstrFilter = NULL;
	opn.lpstrCustomFilter = NULL;
	opn.nFilterIndex = 0;
	opn.lpstrFileTitle = _T("Export file");
	opn.lpstrInitialDir = NULL;
	opn.lpstrTitle = _T("Export file");
	opn.lpstrFilter = L"Export";
	opn.nFilterIndex = 1;


	if (GetOpenFileName(&opn))
	{
		LoadNewMSI(opn.lpstrFile);
	}
}


void CEZ_MSIDlg::OnExportGeneratecommandlinecode()
{
	//TCHAR szDirectory[MAX_PATH] = _T("");
	//::GetCurrentDirectory(sizeof(szDirectory) - 1, szDirectory);

	CString strFullPath;
	GetApplicationDirectory(strFullPath);
	CString strAppName(AfxGetAppName());

	m_editTestCtrl.SetWindowText(_T("\"") + strFullPath + strAppName + _T(".exe") + _T("\" \"") + m_strTest + _T("\" export:\"<Export Location>\""));
}

BOOL CEZ_MSIDlg::GetApplicationDirectory(CString& _sApplicationDirectory)
{
	TCHAR szPathName[_MAX_PATH];
	if (FAILED(::GetModuleFileName(NULL, szPathName, _MAX_PATH)))
	{
		return FALSE;
	}

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDirectory[_MAX_DIR];
	TCHAR szFileName[_MAX_FNAME];
	TCHAR szExtension[_MAX_EXT];

	// Split path
	_tsplitpath_s(szPathName, szDrive, _MAX_DRIVE, szDirectory, _MAX_DIR,
		szFileName, _MAX_FNAME, szExtension, _MAX_EXT);

	// Make safe path
	_tmakepath_s(szPathName, _MAX_PATH, szDrive, szDirectory, _T(""), _T(""));

	_sApplicationDirectory = szPathName;

	return TRUE;
}

void CEZ_MSIDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnDropFiles(hDropInfo);

	CString sFile;
	DWORD   nBuffer = 0;

	// Get the number of files dropped 
	int nFilesDropped = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (int i = 0; i<nFilesDropped; i++)
	{
		// Get the buffer size of the file. 
		nBuffer = DragQueryFile(hDropInfo, i, NULL, 0);

		// Get path and name of the file 
		DragQueryFile(hDropInfo, i, sFile.GetBuffer(nBuffer + 1), nBuffer + 1);
		sFile.ReleaseBuffer();

		LoadNewMSI(sFile);

		//m_editTestCtrl.SetWindowText(sFile);

		//list.AddString(PathFindFileName(sFile));

	}

	// Free the memory block containing the dropped-file information 
	DragFinish(hDropInfo);
}

bool CEZ_MSIDlg::LoadNewMSI(LPCTSTR szMSIPath)
{
	m_editMSITree.DeleteAllItems();
	m_editMSITree2.DeleteAllItems();

	DeleteAllArrays();

	m_nExtraWindows = 1;
	m_editMSITree2.ShowWindow(SW_HIDE);
	EnableMenuItem(GetMenu()->m_hMenu, ID_MODIFYTREE_VIEWREGISTRYKEYS, MF_ENABLED);
	EnableMenuItem(GetMenu()->m_hMenu, ID_MODIFYTREE_REMOVEEMPTRYDIRECTORIES, MF_ENABLED);
	ResizeTrees();

	CString strNewFile(szMSIPath);

	CString strCommandLine = CString(_T("_\"")) + AfxGetAppName() + _T("\"");

	strCommandLine += _T(" \"") + strNewFile + _T("\"");

	InterpretCommandLine(strCommandLine);

	m_editMSITree.SetScrollPos(SB_VERT, 0, TRUE);

	int nLastSlash = m_strTest.ReverseFind(_T('\\'));
	SetWindowText(_T("MSI Tree View - ") + m_strTest.Mid(nLastSlash + 1));

	return true;
}


void CMyTreeCtrl::OnPaint()
{
	CTreeCtrl::OnPaint();
	//CPaintDC dc(this); // device context for painting
	////				   // TODO: Add your message handler code here
	////				   // Do not call CTreeCtrl::OnPaint() for painting messages
	////CPaintDC dc(this);
	//CPen pen;
	//CBrush brush;
	//// Magenta pen
	//pen.CreatePen(PS_SOLID, 1, 0x00FF00FF);
	//// "transparent" brush
	//brush.CreateStockObject(HOLLOW_BRUSH);

	//// select brush and pen
	//dc.SelectObject(&pen);
	//dc.SelectObject(&brush);
	//// rectangle with magenta frame and "transparent" background
	//dc.Rectangle(0, 0, 20, 20);


}


void CEZ_MSIDlg::OnViewRelvativefilesizes()
{
	const int nFileCount = m_a_strFileSizeInFileTable.GetCount();
	CArray<int> a_nSize;
	a_nSize.SetSize(0);
	int nMaxSize = 0;
	int nMinSize = INT_MAX;
	for (int i = 0; i < nFileCount; i++)
	{
		int nSize = _ttoi(m_a_strFileSizeInFileTable[i]);

		if (nSize > nMaxSize)
			nMaxSize = nSize;

		if (nSize < nMinSize)
			nMinSize = nSize;

		a_nSize.Add(nSize);
	}

	HTREEITEM hItem;

	hItem = m_editMSITree.GetRootItem();

	while (hItem != NULL)
	{
		int nCurrentImage = 0;
		m_editMSITree.GetItemImage(hItem, nCurrentImage, nCurrentImage);
		bool bFile = nCurrentImage >= g_nFile;
		if (bFile)
		{

			int nElement = m_editMSITree.GetItemData(hItem);
			int nImage = (a_nSize[nElement] / nMaxSize * 5) + g_nFile + 1;
			m_editMSITree.SetItemImage(hItem, nImage, nImage);
		}
		hItem = m_editMSITree.GetNicksNextItem(hItem);
	}
}

HBRUSH CEZ_MSIDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(g_nBackGroundGrey - 20, g_nBackGroundGrey - 20, g_nBackGroundGrey - 20));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}

	return hbr;
}


void CMyTreeCtrl::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	
	int iComponent = 0;
	int iTarget = 0;
	bool bDirectory = false;
	HTREEITEM hTargetItem = GetSelectedItem();

	DWORD dItem = GetItemData(hTargetItem);
	int nImage = 0;
	GetItemImage(hTargetItem, nImage, nImage);

	if (nImage >= g_nFile)
	{
		CString strUpdatedFileSize;
		double dSize = _ttof(theApp.m_Dlg.m_a_strFileSizeInFileTable[dItem]);
		if (dSize / (1024 * 1024) > 1)
			strUpdatedFileSize.Format(_T("%.2f MB"), dSize / (1024 * 1024));
		else if (dSize / 1024 > 1)
			strUpdatedFileSize.Format(_T("%.2f KB"), dSize / (1024));
		else
			strUpdatedFileSize.Format(_T("%.2f Bytes"), dSize);

		theApp.m_Dlg.m_strCurrentlySelectedName = theApp.m_Dlg.m_a_strFileNameInFileTable[dItem];
		theApp.m_Dlg.m_strCurrentlySelectedFile = theApp.m_Dlg.m_a_strFileInFileTable[dItem];
		theApp.m_Dlg.m_editFileSize.SetWindowText(strUpdatedFileSize + _T("                "));
	}
	else
	{
		theApp.m_Dlg.m_editFileSize.SetWindowText(_T("                                "));
	}

	*pResult = 0;
}


void CEZ_MSIDlg::OnModifytreeEvaluatepath()
{
	MSIHANDLE hInstall;
	TCHAR *szBuf;
	DWORD cch = 0;

	if (MsiOpenPackage(m_strMSIPath, &hInstall) == ERROR_SUCCESS)
	{
		if (MsiDoAction(hInstall, _T("CostInitialize")) == ERROR_SUCCESS
			&& MsiDoAction(hInstall, _T("FileCost")) == ERROR_SUCCESS
			&& MsiDoAction(hInstall, _T("CostFinalize")) == ERROR_SUCCESS)
		{
			if (MsiGetSourcePath(hInstall, m_strTest, _T(""), &cch) == ERROR_MORE_DATA)
			{
				cch++; // add 1 to include null terminator since MsiGetTargetPath does not include it on return 
				szBuf = (TCHAR *)malloc(cch * sizeof(TCHAR));
				if (szBuf)
				{
					int nResult = MsiGetSourcePath(hInstall, m_strMSIPath, szBuf, &cch);
					if (MsiGetSourcePath(hInstall, m_strMSIPath, szBuf, &cch) == ERROR_SUCCESS)
					{
						m_strTest = szBuf;
					}
					free(szBuf);
				}
			}
		}
		MsiCloseHandle(hInstall);
	}

}


void CMyTreeCtrl::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnNcLButtonDblClk(nHitTest, point);
}


void CMyTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//POINT p;
	//if (GetCursorPos(&p))
	//{
	//	//cursor position now in p.x and p.y
	//}

	//CPoint ptMousePos;
	//UINT uFlags;
	//HTREEITEM htItem;

	//ptMousePos = (CPoint)GetMessagePos();  ScreenToClient(&ptMousePos);

	//htItem = HitTest(ptMousePos, &uFlags);

	//if (htItem == NULL && !(TVHT_ONITEM & uFlags))
	//{
	//	return;
	//}

	//SelectItem(htItem);

	//DWORD dwFile = GetItemData(htItem);

	//int nImage = theApp.m_Dlg.GetItemImage(htItem);

	//CStringA strExtractCode = "\"\"C:\\Program Files\\7-zip\\7z.exe\" e \"" + CStringA(theApp.m_Dlg.m_strTest) + "\" -o\"%temp%\\MSI Tree View\\\" \"" + CStringA(theApp.m_Dlg.m_strCurrentlySelectedFile);
	//
	//system(strExtractCode);

	//strExtractCode = "DEL /q /f \"%temp%\\MSI Tree View\\" + CStringA(theApp.m_Dlg.m_strCurrentlySelectedName) + "\"";

	//system(strExtractCode);

	//strExtractCode = "MOVE /y \"%temp%\\MSI Tree View\\" + CStringA(theApp.m_Dlg.m_strCurrentlySelectedFile) + "\" \"%temp%\\MSI Tree View\\" + CStringA(theApp.m_Dlg.m_strCurrentlySelectedName) + "\"";

	//system(strExtractCode);

	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}
