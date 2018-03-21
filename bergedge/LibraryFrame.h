#pragma once

#include "CustomMiniDockFrameWnd.h"

// CLibraryFrame frame

class CLibraryFrame :
   public CMiniFrameWnd,
   public IFolderFilter
{
public:
   ULONG    m_ulRef;
   bool        m_bOpenLibrary;
   CString        m_strNewPath;
   CString        m_strSelPath;
   DECLARE_DYNCREATE(CLibraryFrame)
	CLibraryFrame();           // protected constructor used by dynamic creation
	virtual ~CLibraryFrame();
   CToolBar    m_toolbar;
   CFrameWnd *    m_pMainFrame;
   bool m_bTracking;
   virtual void NotifyFloatingWindows(DWORD dwFlags);
   virtual BOOL LoadFrame(UINT nIDResource,DWORD dwDefaultStyle, CWnd* pParentWnd,CCreateContext* pContext);
   BOOL Create(CWnd* pParent,DWORD dwBarStyle, PVOID p);
protected:
	DECLARE_MESSAGE_MAP()
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
   afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpcs);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnActivate(UINT nState,CWnd* pWndOther,BOOL bMinimized);
   afx_msg BOOL OnNcActivate(BOOL bActive);
   afx_msg void OnNcLButtonDblClk(UINT nHitTest,CPoint point);
   afx_msg void OnNcLButtonDown(UINT nHitTest,CPoint point);
   afx_msg void OnNcLButtonUp(UINT nHitTest,CPoint point);
   afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
   afx_msg void OnFileNewlibrary();
   afx_msg void OnFileOpenlibrary();
   bool ValidateNewName(const char * psz, CString & strCandidateNewPath);
   virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID &,void **);
   virtual ULONG STDMETHODCALLTYPE AddRef();
   virtual ULONG STDMETHODCALLTYPE Release();
   virtual HRESULT STDMETHODCALLTYPE GetEnumFlags(IShellFolder * psf,PCIDLIST_ABSOLUTE pidlFolder,HWND * phwnd,DWORD * pgrfFlags);
   virtual HRESULT STDMETHODCALLTYPE ShouldShow(IShellFolder * psf,PCIDLIST_ABSOLUTE pidlFolder,PCUITEMID_CHILD pidlItem);
};


