// LibraryFrame.cpp : implementation file
//

#include "stdafx.h"
#include "PCDraft.h"
#include "LibraryFrame.h"
BOOL AFXAPI AfxIsDescendant(HWND hWndParent,HWND hWndChild);
HWND AFXAPI AfxGetParentOwner(HWND hWnd);

bool has_subdir(CString str)
{

   CFileFind find;

   if(find.FindFile(str + "\\*.*"))
   {

      while(find.FindNextFileA())
      {

         if(find.IsDots())
         {

            

         }
         else if(find.IsDirectory())
         {

            return true;

         }


      } 

   }


   return false;

}

CString get_utf8(LPCWSTR lpcwsz)
{

   CString str;

   int count = ::WideCharToMultiByte(CP_UTF8,0,lpcwsz,-1,NULL,0,NULL,0);

   if(count > 0)
   {
      char * lpDst = str.GetBufferSetLength(count + 1);
      memset(lpDst,0,count + 1);

      ::WideCharToMultiByte(CP_UTF8,0,lpcwsz,-1,lpDst,count,NULL,0);
      str.ReleaseBuffer();
   }
   return str;


}

// CLibraryFrame

IMPLEMENT_DYNCREATE(CLibraryFrame,CMiniFrameWnd)

CLibraryFrame::CLibraryFrame()
{
   m_bTracking = false;
   m_ulRef = 1;
}

CLibraryFrame::~CLibraryFrame()
{
}


BEGIN_MESSAGE_MAP(CLibraryFrame,CMiniFrameWnd)
   ON_WM_CREATE()
   ON_WM_NCCREATE()
   ON_WM_ACTIVATE()
   ON_WM_NCACTIVATE()
   ON_WM_NCLBUTTONDBLCLK()
   ON_WM_NCLBUTTONDOWN()
   ON_WM_NCLBUTTONUP()
   ON_WM_GETMINMAXINFO()
   ON_COMMAND(ID_FILE_NEWLIBRARY,&CLibraryFrame::OnFileNewlibrary)
   ON_COMMAND(ID_FILE_OPENLIBRARY,&CLibraryFrame::OnFileOpenlibrary)
END_MESSAGE_MAP()

//void CLibraryFrame::RecalcLayout(BOOL bNotify)
//{
//   CMiniFrameWnd::RecalcLayout(bNotify);
//}
// CLibraryFrame message handlers


BOOL CLibraryFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Add your specialized code here and/or call the base class
   if(!CMiniFrameWnd::PreCreateWindow(cs))
      return FALSE;
   cs.dwExStyle &= ~WS_EX_TOOLWINDOW;
   cs.style |= MFS_SYNCACTIVE;
   cs.hwndParent = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
   return TRUE;

}
Gdiplus::Bitmap * LoadPNG(LPCTSTR pName,LPCTSTR pType,HMODULE hInst);
//BOOL CLibraryFrame::CreateEx(DWORD dwExStyle,LPCTSTR lpClassName,
//   LPCTSTR lpszWindowName,DWORD dwStyle,const RECT& rect,
//   CWnd* pParentWnd,UINT nID)
//{
//   // set m_bInRecalcLayout to avoid flashing during creation
//   // RecalcLayout will be called once something is docked
//   m_bInRecalcLayout = TRUE;
//
//   dwStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU | MFS_MOVEFRAME |
//      MFS_4THICKFRAME | MFS_SYNCACTIVE | MFS_BLOCKSYSMENU |
//
//      FWS_SNAPTOBARS;
//
//   DWORD dwBarStyle = 0;
//
//   if(dwBarStyle & CBRS_SIZE_DYNAMIC)
//      dwStyle &= ~MFS_MOVEFRAME;
//
////   DWORD dwExStyle = 0;
//   if(!CMiniFrameWnd::CreateEx(dwExStyle,lpClassName ? lpClassName :
//      AfxRegisterWndClass(CS_DBLCLKS,::LoadCursor(NULL,IDC_ARROW)),
//      lpszWindowName,dwStyle,rect,pParentWnd,(UINT_PTR)nID))
//   {
//      return false;
//   }
//   dwStyle = dwBarStyle & (CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT) ?
//   CBRS_ALIGN_LEFT : CBRS_ALIGN_TOP;
//   dwStyle |= dwBarStyle & CBRS_FLOAT_MULTI;
//
//   CMenu* pSysMenu = GetSystemMenu(FALSE);
//   if(pSysMenu)
//   {
//      pSysMenu->DeleteMenu(SC_SIZE,MF_BYCOMMAND);
//      pSysMenu->DeleteMenu(SC_MINIMIZE,MF_BYCOMMAND);
//      pSysMenu->DeleteMenu(SC_MAXIMIZE,MF_BYCOMMAND);
//      pSysMenu->DeleteMenu(SC_RESTORE,MF_BYCOMMAND);
//
//      CString strHide;
//      if(strHide.LoadString(AFX_IDS_HIDE))
//      {
//         pSysMenu->DeleteMenu(SC_CLOSE,MF_BYCOMMAND);
//         pSysMenu->AppendMenu(MF_STRING | MF_ENABLED,SC_CLOSE,strHide);
//      }
//   }
//
//   // must initially create with parent frame as parent
//   //if(!m_wndDockBar.Create(pParent,WS_CHILD | WS_VISIBLE | dwStyle,
//   if(!m_wndDockBar.Create(pParentWnd,WS_CHILD  | dwStyle,
//      AFX_IDW_DOCKBAR_FLOAT))
//   {
//      m_bInRecalcLayout = FALSE;
//      return FALSE;
//   }
//
//   // set parent to CMiniDockFrameWnd
//   m_wndDockBar.SetParent(this);
//   m_bInRecalcLayout = FALSE;
//
//   return TRUE;
//
//}

int CLibraryFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if(CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
      return -1;
   ModifyStyle(0,MFS_SYNCACTIVE | WS_THICKFRAME);
   
   EnableDocking(CBRS_ALIGN_TOP);
   if(m_toolbar.CreateEx(this,TBSTYLE_FLAT | TBSTYLE_LIST,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
   {
      Gdiplus::Bitmap * p = LoadPNG(MAKEINTRESOURCEA(IDR_LIBRARY),"PNG",NULL);
      if(p != NULL)
      {
         HBITMAP h = NULL;
         p->GetHBITMAP(Gdiplus::Color(0,0,0,0),& h);
         if(m_toolbar.SetBitmap(h))
         {

            UINT ui[]=
            {
               ID_VIEW_LIBRARY,
               ID_VIEW_PHOTO_LIBRARY
            };

            m_toolbar.SetSizes(CSize(39,38),CSize(32,32));

            if(m_toolbar.SetButtons(ui,sizeof(ui) / sizeof(ui[0])))
            {
               m_toolbar.SetButtonText(0,"Microspot Data");
               m_toolbar.SetButtonText(1,"Photos");

               m_toolbar.EnableDocking(CBRS_ALIGN_TOP);
               ShowControlBar(&m_toolbar,true,true);

            }

         }

         delete p;

      }

   }


   return 0;
}


void CLibraryFrame::OnActivate(UINT nState,CWnd* pWndOther,BOOL bMinimized)
{
   //CMiniFrameWnd::OnActivate(nState,pWndOther,bMinimized);
   CWnd::OnActivate(nState,pWndOther,bMinimized);

   // hide the menu bar
   if(nState == WA_INACTIVE && (m_dwMenuBarVisibility & AFX_MBV_KEEPVISIBLE) == 0)
   {
      SetMenuBarState(AFX_MBS_HIDDEN);
   }

   // get top level frame unless this is a child window
   // determine if window should be active or not
   CFrameWnd* pTopLevel = (GetStyle() & WS_CHILD) ? this : m_pMainFrame;
   ENSURE_VALID(pTopLevel);
   CWnd* pActive = (nState == WA_INACTIVE ? pWndOther : this);
   BOOL bStayActive = (pActive != NULL) ?
      (pTopLevel == pActive ||
      (pTopLevel == m_pMainFrame &&
      (pActive == pTopLevel ||
      pActive->SendMessage(WM_FLOATSTATUS,FS_SYNCACTIVE) != 0)))
      : FALSE;
   pTopLevel->m_nFlags &= ~WF_STAYACTIVE;
   if(bStayActive)
      pTopLevel->m_nFlags |= WF_STAYACTIVE;

   // sync floating windows to the new state
   NotifyFloatingWindows(bStayActive ? FS_ACTIVATE : FS_DEACTIVATE);

   // get active view (use active frame if no active view)
   //CView* pActiveView = GetActiveView();
   //if(pActiveView == NULL)
   //   pActiveView = GetActiveFrame()->GetActiveView();

   //// when frame gets activated, re-activate current view
   //if(pActiveView != NULL)
   //{
   //   if(nState != WA_INACTIVE && !bMinimized)
   //      pActiveView->OnActivateView(TRUE,pActiveView,pActiveView);

   //   // always notify the view of frame activations
   //   pActiveView->OnActivateFrame(nState,this);
   //}

}


BOOL CLibraryFrame::OnNcActivate(BOOL bActive)
{
   // TODO: Add your message handler code here and/or call default
   //if(m_bTracking)
   //{
     // CFrameWnd::OnNcActivate(bActive);
//   }
   //else
   {
      return CMiniFrameWnd::OnNcActivate(bActive);

   }
}
BOOL CLibraryFrame::OnNcCreate(LPCREATESTRUCT lpcs)
{
   if(!CFrameWnd::OnNcCreate(lpcs))
      return FALSE;

   if(GetStyle() & MFS_SYNCACTIVE)
   {
      // syncronize activation state with top level parent
      CWnd* pParentWnd = AfxGetApp()->m_pMainWnd;
      CWnd* pActiveWnd = GetForegroundWindow();
      BOOL bActive = (pParentWnd == pActiveWnd) ||
         (pParentWnd->GetLastActivePopup() == pActiveWnd &&
         pActiveWnd->SendMessage(WM_FLOATSTATUS,FS_SYNCACTIVE) != 0);

      // the WM_FLOATSTATUS does the actual work
      SendMessage(WM_FLOATSTATUS,bActive ? FS_ACTIVATE : FS_DEACTIVATE);
   }

   return TRUE;
}


void CLibraryFrame::NotifyFloatingWindows(DWORD dwFlags)
{
   ASSERT_VALID(this);
   ASSERT(m_hWnd != NULL);

   // get top level parent frame window first unless this is a child window
   CFrameWnd* pParent = (GetStyle() & WS_CHILD) ? this : m_pMainFrame;
   ENSURE(pParent != NULL);
   if(dwFlags & (FS_DEACTIVATE | FS_ACTIVATE))
   {
      // update parent window activation state
      BOOL bActivate = !(dwFlags & FS_DEACTIVATE);
      BOOL bEnabled = pParent->IsWindowEnabled();

      if(bActivate && bEnabled && pParent != this)
      {
         // Excel will try to Activate itself when it receives a
         // WM_NCACTIVATE so we need to keep it from doing that here.
         m_nFlags |= WF_KEEPMINIACTIVE;
         pParent->SendMessage(WM_NCACTIVATE,TRUE);
         m_nFlags &= ~WF_KEEPMINIACTIVE;
      }
      else
      {
         pParent->SendMessage(WM_NCACTIVATE,FALSE);
      }
   }

   // then update the state of all floating windows owned by the parent
   HWND hWnd = ::GetWindow(::GetDesktopWindow(),GW_CHILD);
   while(hWnd != NULL)
   {
      if(AfxIsDescendant(pParent->m_hWnd,hWnd))
         ::SendMessage(hWnd,WM_FLOATSTATUS,dwFlags,0);
      hWnd = ::GetWindow(hWnd,GW_HWNDNEXT);
   }
}


void CLibraryFrame::OnNcLButtonDown(UINT nHitTest,CPoint point)
{
   //if(nHitTest == HTCAPTION)
   //{
   //   // special activation for floating toolbars
   //   ActivateTopParent();

   //   // initiate toolbar drag for non-CBRS_FLOAT_MULTI toolbars
   //   //if((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
   //   //{
   //   //   int nPos = 1;
   //   //   CControlBar* pBar = NULL;
   //   //   while(pBar == NULL && nPos < m_wndDockBar.m_arrBars.GetSize())
   //   //      pBar = m_wndDockBar.GetDockedControlBar(nPos++);

   //   //   ENSURE_VALID(pBar);
   //   //   ASSERT_KINDOF(CControlBar,pBar);
   //   //   ENSURE(pBar->m_pDockContext != NULL);
   //   //   pBar->m_pDockContext->StartDrag(point);
   //   //   return;
   //   //}
   //}
   //else if(nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST)
   //{
   //   // special activation for floating toolbars
   //   ActivateTopParent();

   //   //int nPos = 1;
   //   //CControlBar* pBar = NULL;
   //   //while(pBar == NULL && nPos < m_wndDockBar.m_arrBars.GetSize())
   //   //   pBar = m_wndDockBar.GetDockedControlBar(nPos++);

   //   //ENSURE_VALID(pBar);
   //   //ASSERT_KINDOF(CControlBar,pBar);
   //   //ENSURE(pBar->m_pDockContext != NULL);

   //   //// CBRS_SIZE_DYNAMIC toolbars cannot have the CBRS_FLOAT_MULTI style
   //   //ASSERT((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0);
   //   //pBar->m_pDockContext->StartResize(nHitTest,point);
   //   return;
   //}
   m_bTracking = true;
   CMiniFrameWnd::OnNcLButtonDown(nHitTest,point);

   //Default();
}

void CLibraryFrame::OnNcLButtonDblClk(UINT nHitTest,CPoint point)
{
   if(nHitTest == HTCAPTION)
   {
      // special activation for floating toolbars
      ActivateTopParent();

      // initiate toolbar toggle for non-CBRS_FLOAT_MULTI toolbars
      //if((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
      //{
      //   int nPos = 1;
      //   CControlBar* pBar = NULL;
      //   while(pBar == NULL && nPos < m_wndDockBar.m_arrBars.GetSize())
      //      pBar = m_wndDockBar.GetDockedControlBar(nPos++);

      //   ENSURE_VALID(pBar);
      //   ASSERT_KINDOF(CControlBar,pBar);
      //   ENSURE(pBar->m_pDockContext != NULL);
      //   pBar->m_pDockContext->ToggleDocking();
      //   return;
      //}
   }
   CMiniFrameWnd::OnNcLButtonDblClk(nHitTest,point);
}

BOOL CLibraryFrame::Create(CWnd* pParent,DWORD dwBarStyle, PVOID p)
{
   // set m_bInRecalcLayout to avoid flashing during creation
   // RecalcLayout will be called once something is docked
   m_bInRecalcLayout = TRUE;

   //DWORD dwStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU | MFS_MOVEFRAME |
   //   MFS_4THICKFRAME | MFS_SYNCACTIVE | MFS_BLOCKSYSMENU |
   //   FWS_SNAPTOBARS;
   DWORD dwStyle = WS_POPUP |  WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU |  MFS_SYNCACTIVE |
      WS_THICKFRAME;

   if(dwBarStyle & CBRS_SIZE_DYNAMIC)
      dwStyle &= ~MFS_MOVEFRAME;

   DWORD dwExStyle = 0;
   m_strCaption = "Library";

   CRect rect = rectDefault;
   if(!CWnd::CreateEx(dwExStyle,AfxRegisterWndClass(CS_DBLCLKS,::LoadCursor(NULL,IDC_ARROW)),
      m_strCaption,dwStyle,rect.left,rect.top,rect.right - rect.left,
      rect.bottom - rect.top,pParent->GetSafeHwnd(),(HMENU)(UINT_PTR)0,p))
   {
      return FALSE;
   }

   dwStyle = dwBarStyle & (CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT) ?
   CBRS_ALIGN_LEFT : CBRS_ALIGN_TOP;
   dwStyle |= dwBarStyle & CBRS_FLOAT_MULTI;

   //CMenu* pSysMenu = GetSystemMenu(FALSE);
   //if(pSysMenu)
   //{
   //   pSysMenu->DeleteMenu(SC_SIZE,MF_BYCOMMAND);
   //   pSysMenu->DeleteMenu(SC_MINIMIZE,MF_BYCOMMAND);
   //   pSysMenu->DeleteMenu(SC_MAXIMIZE,MF_BYCOMMAND);
   //   pSysMenu->DeleteMenu(SC_RESTORE,MF_BYCOMMAND);

   //   CString strHide;
   //   if(strHide.LoadString(AFX_IDS_HIDE))
   //   {
   //      pSysMenu->DeleteMenu(SC_CLOSE,MF_BYCOMMAND);
   //      pSysMenu->AppendMenu(MF_STRING | MF_ENABLED,SC_CLOSE,strHide);
   //   }
   //}

   //// must initially create with parent frame as parent
   //if(!m_wndDockBar.Create(pParent,WS_CHILD | WS_VISIBLE | dwStyle,
   //   AFX_IDW_DOCKBAR_FLOAT))
   //{
   //   m_bInRecalcLayout = FALSE;
   //   return FALSE;
   //}

   //// set parent to CMiniDockFrameWnd
   //m_wndDockBar.SetParent(this);
   m_bInRecalcLayout = FALSE;

   return TRUE;
}

BOOL CLibraryFrame::LoadFrame(UINT nIDResource,DWORD dwDefaultStyle,
   CWnd* pParentWnd,CCreateContext* pContext)
{
   // only do this once
   ASSERT_VALID_IDR(nIDResource);
   ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

   m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

   CString strFullString;
   if(strFullString.LoadString(nIDResource))
      AfxExtractSubString(m_strTitle,strFullString,0);    // first sub-string

//   VERIFY(AfxDeferRegisterClass(AFX_WNDFRAMEORVIEW_REG));

   // attempt to create the window
   LPCTSTR lpszClass = GetIconWndClass(dwDefaultStyle,nIDResource);
   CString strTitle = m_strTitle;
   if(!Create(AfxGetApp()->m_pMainWnd, 0, pContext))
   {
      return FALSE;   // will self destruct on failure normally
   }

   // save the default menu handle
   ASSERT(m_hWnd != NULL);
   m_hMenuDefault = m_dwMenuBarState == AFX_MBS_VISIBLE ? ::GetMenu(m_hWnd) : m_hMenu;

   // load accelerator resource
   LoadAccelTable(ATL_MAKEINTRESOURCE(nIDResource));

   if(pContext == NULL)   // send initial update
      SendMessageToDescendants(WM_INITIALUPDATE,0,0,TRUE,TRUE);

   return TRUE;
}


void CLibraryFrame::OnNcLButtonUp(UINT nHitTest,CPoint point)
{
   CMiniFrameWnd::OnNcLButtonUp(nHitTest,point);

   m_bTracking = false;
}


void CLibraryFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
   // TODO: Add your message handler code here and/or call default

   CMiniFrameWnd::OnGetMinMaxInfo(lpMMI);

   lpMMI->ptMinTrackSize.x = 320;
   lpMMI->ptMinTrackSize.y = 450;
}


int CALLBACK LibraryFrameBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
   CLibraryFrame * pframe = (CLibraryFrame *)lpData;

   if(uMsg == BFFM_IUNKNOWN)
   {
      IUnknown * pUk = (IUnknown *) lParam;
      if(pUk != NULL)
      {
         CComPtr<IFolderFilterSite> pfilter;

         pUk->QueryInterface(&pfilter);
         if(pfilter != NULL)
         {
            IUnknown * pThis = (IUnknown *)pframe;
            pfilter->SetFilter(pThis);


         }

      }

   }
   else if(uMsg == BFFM_SELCHANGED)
   {
      LPITEMIDLIST pidl = (LPITEMIDLIST) lParam;
      // get the name of the folder
      TCHAR path[MAX_PATH];
      SHGetPathFromIDList(pidl,path);
      pframe->m_strSelPath =  path;

   }
   else if(uMsg == BFFM_VALIDATEFAILED)
   {
      LPCTSTR lpcwszNewName = (LPCTSTR)lParam;

      CString str;

      str = lpcwszNewName;

      CString strCandidateNewPath;

      if(!pframe->ValidateNewName(str, strCandidateNewPath))
      {
         AfxMessageBox("A library with this name already exists at the specified location (" + strCandidateNewPath + "). Please choose another location or another name.",MB_ICONINFORMATION);
         return 1;
      }


   }
   return 0;

}

bool CLibraryFrame::ValidateNewName(const char * pszNewName,CString & strCandidateNewPath)
{
   CString & strNewPath = strCandidateNewPath;

   strNewPath = m_strSelPath;

   strNewPath.TrimRight("/");
   strNewPath.TrimRight("\\");

   strNewPath += "\\" + CString(pszNewName);
   CString strExt = ".spotlibrary";
   if(strNewPath.Right(strExt.GetLength()).CompareNoCase(strExt) != 0)
   {

      strNewPath += ".spotlibrary";

   }

   if(PathIsDirectory(strNewPath))
   {
      return false;
   }
   if(PathFileExists(strNewPath))
   {
      return false;
   }
   m_strNewPath = strNewPath;

   return true;


}


HRESULT STDMETHODCALLTYPE CLibraryFrame::QueryInterface(const IID & iid,void ** p)
{

   if(iid == IID_IUnknown)
   {
      
      *p = (IUnknown *) this;
      return S_OK;
   }
   else if(iid == IID_IFolderFilter)
   {

      *p = (IFolderFilter *) this;
      return S_OK;
   }
   return E_FAIL;
}


ULONG STDMETHODCALLTYPE CLibraryFrame::AddRef()
{

   return ++m_ulRef;

}


ULONG STDMETHODCALLTYPE CLibraryFrame::Release()
{
   
   return --m_ulRef;

}


HRESULT CLibraryFrame::GetEnumFlags(IShellFolder * psf, PCIDLIST_ABSOLUTE pidlFolder, HWND * phwnd, DWORD * pgrfFlags)
{
   *pgrfFlags = SHCONTF_FOLDERS;
   
   return S_OK;
}


HRESULT CLibraryFrame::ShouldShow(IShellFolder * psf,PCIDLIST_ABSOLUTE pidlFolder,PCUITEMID_CHILD pidlItem)
{
   IShellItem *psi;

   HRESULT hr = SHCreateItemWithParent(NULL,psf,pidlItem,IID_PPV_ARGS(&psi));
   if(SUCCEEDED(hr))
   {
      // Determine here whether the item should be shown. This determination
      // is application-dependent.
      LPWSTR lpwstr = NULL;
      psi->GetDisplayName(SIGDN_FILESYSPATH,&lpwstr);
      CString str = get_utf8(lpwstr);
      CoTaskMemFree(lpwstr);

      if(m_bOpenLibrary)
      {

         CString strLo(str);

         strLo.MakeLower();

         const char * pszFind = ".spotlibrary";

         int iLenFind = strlen(pszFind);

         int iFind = strLo.Find(pszFind);

         if(iFind >= 0 && strLo.GetLength() > iFind + iLenFind + 1)
         {
            return S_FALSE;

         }

         if(!has_subdir(str))
         {

            return S_FALSE;

         }

      }
      else
      {

      }

      
      psi->Release();
   }

   return S_OK;
}


void CLibraryFrame::OnFileNewlibrary()
{

   //CString strFilter;

   //strFilter.LoadString(IDS_LIBRARYSAVEFILTERS);

   //CFileDialog dlg(false,"spotlibrary","untitled library.spotlibrary",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "");

   //dlg.DoModal();
   m_bOpenLibrary = false;

   BROWSEINFO bi ={0};
   bi.hwndOwner = GetSafeHwnd();
   bi.lParam = (LPARAM) this;
   bi.ulFlags = BIF_USENEWUI | BIF_NONEWFOLDERBUTTON | BIF_VALIDATE;
   bi.lpfn = &LibraryFrameBrowseCallbackProc;
   LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
   if(pidl != 0)
   {
      // get the name of the folder
      TCHAR path[MAX_PATH];
      SHGetPathFromIDList(pidl,path);

      // free memory used
      IMalloc * imalloc = 0;
      if(SUCCEEDED(SHGetMalloc(&imalloc)))
      {
         imalloc->Free(pidl);
         imalloc->Release();
      }
   }
}


void CLibraryFrame::OnFileOpenlibrary()
{
   m_bOpenLibrary = true;
   BROWSEINFO bi ={0};
   bi.lParam = (LPARAM) this;
   bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON;
   bi.lpfn = &LibraryFrameBrowseCallbackProc;
   bi.lpszTitle = _T("Select Microspot Library");
   bi.hwndOwner = GetSafeHwnd();
   LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
   if(pidl != 0)
   {
      // get the name of the folder
      TCHAR path[MAX_PATH];
      SHGetPathFromIDList(pidl,path);

      // free memory used
      IMalloc * imalloc = 0;
      if(SUCCEEDED(SHGetMalloc(&imalloc)))
      {
         imalloc->Free(pidl);
         imalloc->Release();
      }
   }
}
