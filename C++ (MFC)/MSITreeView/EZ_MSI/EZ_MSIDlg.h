
// EZ_MSIDlg.h : header file
//

#pragma once
#include <MsiQuery.h>
#include <msi.h>
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"

//#define LIGHT_COLOR		250
//#define DARK_COLOR		100

class CMyTreeCtrl : public CTreeCtrl
{
	//...
public:
	HTREEITEM GetNicksNextItem(HTREEITEM hITem);
	CString GetFullPath(const HTREEITEM hItem);
	
protected:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

// CEZ_MSIDlg dialog
class CEZ_MSIDlg : public CDialogEx
{
// Construction
public:
	CEZ_MSIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EZ_MSI_DIALOG };
	/*CRect m_rcClose;*/
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	BOOL SampleCode();
	BOOL SamplePopulateTable();
	BOOL PopulateTable(CMyTreeCtrl& MSITree);
	BOOL PopulateDirectoryTree(CMyTreeCtrl& MSITree);
	BOOL PopulateFiles(CMyTreeCtrl& MSITree);
	BOOL PopulateShortcuts(CMyTreeCtrl& MSITree);
	BOOL PopulateRegistry(CMyTreeCtrl& MSITree);

	BOOL GetMSIData(LPCTSTR szPath);
	BOOL GetColumnData(const MSIHANDLE &msiHandle, LPCTSTR szColumn, LPCTSTR szTable, CStringArray& a_strElements);

public:

	void DeleteAllArrays();

	CStringArray m_a_strDirectoryInComponentTable;
	CStringArray m_a_strComponentInComponentTable;
	CStringArray m_a_strGUIDInComponentTable;

	CStringArray m_a_strFileNameInFileTable;
	CStringArray m_a_strFileInFileTable;
	CStringArray m_a_strComponentInFileTable;
	CStringArray m_a_strFileSizeInFileTable;

	CStringArray m_a_strDirectoryInDirectory;
	CStringArray m_a_strDirectoryParentInDirectory;
	CStringArray m_a_strDefaultDirInDirectory;

	CStringArray m_a_strShortcutInShortcut;
	CStringArray m_a_strNameInShortcut;
	CStringArray m_a_strDirectoryInShortcut;
	CStringArray m_a_strTargetInShortcut;

	CStringArray m_a_strRootInRegistry;
	CStringArray m_a_strKeyInRegistry;
	CStringArray m_a_strNameInRegistry;
	CStringArray m_a_strValueInRegistry;
	CStringArray m_a_strComponent_InRegistry;

	CMyTreeCtrl m_editMSITree;
	CMyTreeCtrl m_editMSITree2;
	int m_nExtraWindows;

	void ExpandAll(CMyTreeCtrl& MSITree);
	void CollapseAll(CMyTreeCtrl& MSITree);
	int GetItemImage(HTREEITEM hTreeItem);
	void CopyToClipBoard(LPCTSTR szString);

private:
	CMap <CString, LPCTSTR, HTREEITEM, HTREEITEM&> m_RegistryHandle;
	CMap <CString, LPCTSTR, HTREEITEM, HTREEITEM&> m_DirectoryHandle;
	CMap <CString, LPCTSTR, HTREEITEM, HTREEITEM&> m_FileHandle;
	CMap <CString, LPCTSTR, HTREEITEM, HTREEITEM&> m_ShortcutHandle;

public:
	CString m_strTest;
	void InterpretCommandLine(CString& strCommandLine);

	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CEdit m_editTestCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	bool m_bSafeToResize;
	CArray<HTREEITEM> m_aFindItem;
	int m_nFindItemLocation;
	

	void ResizeTrees();
	
public:
	afx_msg void OnViewExpandall();
	afx_msg void OnViewCollapseall();
	afx_msg void OnExtraWindows2();

	CImageList m_ImageListTree;

	afx_msg void OnExtrawindows1();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnModifytreeRemoveemptrydirectories();


	afx_msg void OnEnChangeEdit1();
	CButton m_editFind;
	afx_msg void OnBnClickedFind();
	virtual void OnOK();
	afx_msg void OnExportExportall();
	afx_msg void OnHelpAboutmsitreeview();

private:
	bool m_bExportAndClose;
	CString m_strExportLocation;
	static UINT SearchTreeForString(LPVOID pParam);
public:
	CButton m_ctrlWarning;
	afx_msg void OnBnClickedWarning();
	afx_msg void OnModifytreeViewregistrykeys();
	afx_msg void OnFileClose();
	afx_msg void OnFileLoadmsi();
	afx_msg void OnExportGeneratecommandlinecode();

	BOOL GetApplicationDirectory(CString& _sApplicationDirectory);

	afx_msg void OnDropFiles(HDROP hDropInfo);

	bool LoadNewMSI(LPCTSTR szMSIPath);
	afx_msg void OnViewRelvativefilesizes();
	CStatic m_editFileSize;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnModifytreeEvaluatepath();

	CString m_strCurrentlySelectedFile;
	CString m_strCurrentlySelectedName;
	CString m_strMSIPath;
};


