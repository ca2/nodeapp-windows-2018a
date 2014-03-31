#pragma once


namespace win
{


   CLASS_DECL_win LRESULT CALLBACK __send_message_hook(int32_t, WPARAM, LPARAM);
   //CLASS_DECL_win void _gen::StandardSubclass;
   CLASS_DECL_win LRESULT CALLBACK __cbt_filter_hook(int32_t, WPARAM, LPARAM);
   CLASS_DECL_win LRESULT __call_window_procedure(sp(::user::interaction) pwindow, oswindow oswindow, UINT nMsg, WPARAM wParam, LPARAM lParam);


   class CLASS_DECL_win window : 
      virtual public ::window,
      virtual public oswindow_handle
   {
   public:


      spa(::user::interaction)         m_guieptraMouseHover;
      ::message_queue_listener *         m_pcallback;
      string                           m_strWindowText;
      bool                             m_bUpdateGraphics;
      //visual::dib_sp                 m_spdib;
      //visual::dib_sp                 m_spdibMultAlphaWork;

      mutex *                          m_pmutexGraphics;
      ::draw2d::dib_sp                 m_spdib;
      //HBITMAP                       m_hbitmap;
      //Gdiplus::Bitmap *             m_pbitmap;
      //::draw2d::graphics_sp             m_spg;
      //::draw2d::bitmap_sp               m_spb;
      //BITMAPINFO m_bitmapinfo;
      //COLORREF * m_pcolorref;
      size m_size;
      point m_pt;
      



      window();
      window(sp(base_application) papp);
      virtual ~window();


      virtual void construct(oswindow oswindow);


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      virtual void mouse_hover_add(sp(::user::interaction) pinterface);
      virtual void mouse_hover_remove(sp(::user::interaction) pinterface);

      bool create_message_queue(const char * pszName, ::message_queue_listener * pcallback = NULL);

      //static const MSG* GetCurrentMessage();

      virtual void install_message_handling(::message::dispatch * pinterface);

      bool operator==(const window& wnd) const;
      bool operator!=(const window& wnd) const;

      uint32_t GetStyle();
      uint32_t GetExStyle();
      bool ModifyStyle(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags = 0);
      bool ModifyStyleEx(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags = 0);

      //virtual sp(::user::interaction) get_owner();
      virtual void set_owner(sp(::user::interaction) pOwnerWnd);

      virtual oswindow get_handle();

      virtual bool _001OnCmdMsg(base_cmd_msg * pcmdmsg);   

      virtual bool BaseOnControlEvent(::user::control_event * pevent);

      void _002OnDraw(::draw2d::graphics * pdc);

      DECL_GEN_SIGNAL(_001OnEraseBkgnd)
      DECL_GEN_SIGNAL(_001OnMove)
      DECL_GEN_SIGNAL(_001OnSize)
      DECL_GEN_SIGNAL(_001OnShowWindow)
      DECL_GEN_SIGNAL(_001OnProdevianSynch)

      ::user::window_interface *      m_pbasewnd;
      sp(::user::interaction)        m_puiCapture;

      virtual void win_update_graphics();

   #if(WINVER >= 0x0500)

      bool GetWindowInfo(PWINDOWINFO pwi) const;
      bool GetTitleBarInfo(PTITLEBARINFO pti) const;

   #endif   // WINVER >= 0x0500

      virtual ::window_sp from_os_data(void * pdata);
      virtual void * get_os_data() const;

      static ::window_sp from_handle(oswindow oswindow);
      static ::window_sp FromHandlePermanent(oswindow oswindow);
      static void DeleteTempMap();
      bool attach(oswindow oswindow_New);
      oswindow detach();

      // subclassing/unsubclassing functions
      virtual void pre_subclass_window();
      bool subclass_window(oswindow oswindow);
      bool SubclassDlgItem(UINT nID, ::window_sp pParent);
      oswindow unsubclass_window();

      // handling of RT_DLGINIT resource (extension to RT_DIALOG)
      bool ExecuteDlgInit(const char * lpszResourceName);
      bool ExecuteDlgInit(LPVOID lpResource);

      using ::user::interaction::create;
      // for child windows, views, panes etc
      virtual bool create(const char * lpszClassName,
         const char * lpszWindowName, uint32_t dwStyle,
         const RECT& rect,
         sp(::user::interaction) pParentWnd, id id,
         sp(::create_context) pContext = NULL);

      // advanced creation (allows access to extended styles)
      virtual bool CreateEx(uint32_t dwExStyle, const char * lpszClassName,
         const char * lpszWindowName, uint32_t dwStyle,
         int32_t x, int32_t y, int32_t nWidth, int32_t nHeight,
         oswindow oswindow_Parent, id id, LPVOID lpParam = NULL);

      virtual bool CreateEx(uint32_t dwExStyle, const char * lpszClassName,
         const char * lpszWindowName, uint32_t dwStyle,
         const RECT& rect,
         sp(::user::interaction) pParentWnd, id id,
         LPVOID lpParam = NULL);

      virtual bool DestroyWindow();

      // special pre-creation and window rect adjustment hooks
      virtual bool pre_create_window(CREATESTRUCT& cs);

      // Advanced: virtual AdjustWindowRect
      enum AdjustType { adjustBorder = 0, adjustOutside = 1 };
      virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);

      
         // get immediate child with given ID
      using ::user::interaction::get_child_by_id;
      void get_child_by_id(id id, oswindow* poswindow_) const;
         // as above, but returns oswindow
      using ::user::interaction::GetDescendantWindow;
      sp(::user::interaction) GetDescendantWindow(id id);
         // like get_child_by_id but recursive
      void SendMessageToDescendants(UINT message, WPARAM wParam = 0, lparam lParam = 0, bool bDeep = TRUE, bool bOnlyPerm = FALSE);
      sp(::user::frame_window) GetParentFrame();
      sp(::user::frame_window) EnsureParentFrame();
      sp(::user::interaction) EnsureTopLevelParent();
      sp(::user::interaction) GetTopLevelOwner();
      sp(::user::interaction) GetParentOwner();
      sp(::user::frame_window) GetTopLevelFrame();
      static ::window_sp get_safe_owner(::window_sp pParent = NULL, oswindow* pWndTop = NULL);

      virtual bool IsWindow();

   #if(WINVER >= 0x0500)

      ::window_sp GetAncestor(UINT gaFlags) const;

   #endif   // WINVER >= 0x0500

      LRESULT send_message(UINT message, WPARAM wParam = 0, lparam lParam = NULL);
      bool post_message(UINT message, WPARAM wParam = 0, lparam lParam = NULL);

      bool SendNotifyMessage(UINT message, WPARAM wParam, lparam lParam);
      //bool SendChildNotifyLastMsg(LRESULT* pResult = NULL);

      bool DragDetect(POINT pt) const;



   // Window Text Functions
      void SetWindowText(const char * lpszString);
      strsize GetWindowText(LPTSTR lpszStringBuf, strsize nMaxCount);
      void GetWindowText(string & rString);
      strsize GetWindowTextLength();
      void SetFont(::draw2d::font* pFont, bool bRedraw = TRUE);
      ::draw2d::font* GetFont();

      inline mutex * mutex_graphics() { if(m_pmutexGraphics != NULL) return m_pmutexGraphics; m_pmutexGraphics = new mutex(get_app()); return m_pmutexGraphics; }

   // Window size and position Functions
      virtual bool IsIconic();
      virtual bool IsZoomed();
      void MoveWindow(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight,
               bool bRepaint = TRUE);
      void MoveWindow(LPCRECT lpRect, bool bRepaint = TRUE);
      int32_t SetWindowRgn(HRGN hRgn, bool bRedraw);
      int32_t GetWindowRgn(HRGN hRgn);

      //static const window wndTop; // SetWindowPos's pWndInsertAfter
      //static const window wndBottom; // SetWindowPos's pWndInsertAfter
      //static const window wndTopMost; // SetWindowPos pWndInsertAfter
      //static const window wndNoTopMost; // SetWindowPos pWndInsertAfter

      virtual bool SetWindowPos(int32_t z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags);
      virtual UINT ArrangeIconicWindows();
      virtual bool BringWindowToTop();
      using interaction::GetWindowRect;
      virtual void GetWindowRect(__rect64 * lpRect);
      using interaction::GetClientRect;
      virtual void GetClientRect(__rect64 * lpRect);

      void ClientToScreen(LPRECT lprect);
      void ClientToScreen(LPPOINT lppoint);
      void ClientToScreen(__rect64 * lprect);
      void ClientToScreen(__point64 * lppoint);
      void ScreenToClient(LPRECT lprect);
      void ScreenToClient(LPPOINT lppoint);
      void ScreenToClient(__rect64 * lprect);
      void ScreenToClient(__point64 * lppoint);

      virtual bool GetWindowPlacement(WINDOWPLACEMENT* lpwndpl);
      virtual bool SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);

      virtual void MapWindowPoints(::window_sp pwndTo, LPPOINT lpPoint, UINT nCount);
      virtual void MapWindowPoints(::window_sp pwndTo, LPRECT lpRect);

      virtual void Print(::draw2d::graphics * pgraphics, uint32_t dwFlags) const;
      virtual void PrintClient(::draw2d::graphics * pgraphics, uint32_t dwFlags) const;

      virtual void UpdateWindow();
      virtual void SetRedraw(bool bRedraw = TRUE);
      virtual bool GetUpdateRect(LPRECT lpRect, bool bErase = FALSE);
      virtual int32_t GetUpdateRgn(::draw2d::region* pRgn, bool bErase = FALSE);
      virtual void Invalidate(bool bErase = TRUE);
      virtual void InvalidateRect(LPCRECT lpRect, bool bErase = TRUE);
      virtual void InvalidateRgn(::draw2d::region* pRgn, bool bErase = TRUE);
      virtual void ValidateRect(LPCRECT lpRect);
      virtual void ValidateRgn(::draw2d::region* pRgn);
      virtual bool ShowWindow(int32_t nCmdShow);
      virtual void _001WindowMaximize();
      virtual void _001WindowRestore();
      virtual bool IsWindowVisible();
      virtual void ShowOwnedPopups(bool bShow = TRUE);

      virtual ::draw2d::graphics * GetDCEx(::draw2d::region* prgnClip, uint32_t flags);
      virtual bool LockWindowUpdate();
      virtual void UnlockWindowUpdate();
      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,
         ::draw2d::region* prgnUpdate = NULL,
         UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
      virtual bool EnableScrollBar(int32_t nSBFlags, UINT nArrowFlags = ESB_ENABLE_BOTH);

      virtual bool DrawAnimatedRects(int32_t idAni, CONST RECT *lprcFrom, CONST RECT *lprcTo);
      virtual bool DrawCaption(::draw2d::graphics * pgraphics, LPCRECT lprc, UINT uFlags);

   #if(WINVER >= 0x0500)

      virtual bool AnimateWindow(uint32_t dwTime, uint32_t dwFlags);

   #endif   // WINVER >= 0x0500

   #if(_WIN32_WINNT >= 0x0501)

      virtual bool PrintWindow(::draw2d::graphics * pgraphics, UINT nFlags) const;

   #endif   // _WIN32_WINNT >= 0x0501

   // Layered Window

   #if(_WIN32_WINNT >= 0x0500)

      virtual bool SetLayeredWindowAttributes(COLORREF crKey, BYTE bAlpha, uint32_t dwFlags);
      virtual bool UpdateLayeredWindow(::draw2d::graphics * pDCDst, POINT *pptDst, SIZE *psize, 
         ::draw2d::graphics * pDCSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, uint32_t dwFlags);

   #endif   // _WIN32_WINNT >= 0x0500

   #if(_WIN32_WINNT >= 0x0501)

      virtual bool GetLayeredWindowAttributes(COLORREF *pcrKey, BYTE *pbAlpha, uint32_t *pdwFlags) const;

   #endif   // _WIN32_WINNT >= 0x0501


   // timer Functions
      virtual uint_ptr SetTimer(uint_ptr nIDEvent, UINT nElapse,
         void (CALLBACK* lpfnTimer)(oswindow, UINT, uint_ptr, uint32_t));
      virtual bool KillTimer(uint_ptr nIDEvent);

   // Window State Functions
      virtual bool is_window_enabled();
      virtual bool enable_window(bool bEnable = TRUE);

      // the active window applies only to top-level (frame windows)
      virtual sp(::user::interaction) GetActiveWindow();
      virtual sp(::user::interaction) SetActiveWindow();

      // the foreground window applies only to top-level windows (frame windows)
      virtual bool SetForegroundWindow();
      static ::window_sp GetForegroundWindow();

      virtual id SetDlgCtrlId(id id);
      virtual id GetDlgCtrlId();

      

      // capture and focus apply to all windows
      static ::window_sp GetCapture();
      virtual sp(::user::interaction) set_capture(sp(::user::interaction) pinterface = NULL);
      virtual sp(::user::interaction) release_capture();
      virtual sp(::user::interaction) get_capture();
      virtual sp(::user::interaction) SetFocus();

      static  sp(::user::interaction) GetFocus();

      static ::window_sp GetDesktopWindow();

   // Obsolete and non-portable APIs - not recommended for new code
      virtual void CloseWindow();
      virtual bool OpenIcon();

   // Dialog-Box Item Functions
   // (NOTE: Dialog-Box Items/Controls are not necessarily in dialog boxes!)
      virtual void CheckDlgButton(int32_t nIDButton, UINT nCheck);
      virtual void CheckRadioButton(int32_t nIDFirstButton, int32_t nIDLastButton,
                  int32_t nIDCheckButton);
      virtual int32_t GetCheckedRadioButton(int32_t nIDFirstButton, int32_t nIDLastButton);
      virtual int32_t DlgDirList(__inout_z LPTSTR lpPathSpec, __in int32_t nIDListBox,
                  __in int32_t nIDStaticPath, __in UINT nFileType);
      virtual int32_t DlgDirListComboBox(__inout_z LPTSTR lpPathSpec, __in int32_t nIDComboBox,
                  __in int32_t nIDStaticPath, __in UINT nFileType);
      virtual bool DlgDirSelect(__out_ecount_z(nSize) LPTSTR lpString, __in int32_t nSize, __in int32_t nIDListBox);
      virtual bool DlgDirSelectComboBox(__out_ecount_z(nSize) LPTSTR lpString, __in int32_t nSize, __in int32_t nIDComboBox);

      virtual UINT GetChildByIdInt(int32_t nID, BOOL * lpTrans = NULL, bool bSigned = TRUE) const;
      virtual int32_t GetChildByIdText(__in int32_t nID, __out_ecount_part_z(nMaxCount, return + 1) LPTSTR lpStr, __in int32_t nMaxCount) const;
      virtual int32_t GetChildByIdText(int32_t nID, string & rString) const;
      virtual ::window_sp GetNextDlgGroupItem(::window_sp pWndCtl, bool bPrevious = FALSE) const;
      virtual ::window_sp GetNextDlgTabItem(::window_sp pWndCtl, bool bPrevious = FALSE) const;
      virtual UINT IsDlgButtonChecked(int32_t nIDButton) const;
      virtual LRESULT SendDlgItemMessage(int32_t nID, UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
      virtual void SetDlgItemInt(int32_t nID, UINT nValue, bool bSigned = TRUE);
      virtual void SetDlgItemText(int32_t nID, const char * lpszString);

   // Scrolling Functions
      virtual int32_t GetScrollPos(int32_t nBar) const;
      virtual void GetScrollRange(int32_t nBar, LPINT lpMinPos, LPINT lpMaxPos) const;
      virtual void ScrollWindow(int32_t xAmount, int32_t yAmount,
                  LPCRECT lpRect = NULL,
                  LPCRECT lpClipRect = NULL);
      virtual int32_t SetScrollPos(int32_t nBar, int32_t nPos, bool bRedraw = TRUE);
      virtual void SetScrollRange(int32_t nBar, int32_t nMinPos, int32_t nMaxPos,
            bool bRedraw = TRUE);
      virtual void ShowScrollBar(UINT nBar, bool bShow = TRUE);
      virtual void EnableScrollBarCtrl(int32_t nBar, bool bEnable = TRUE);
//      virtual CScrollBar* GetScrollBarCtrl(int32_t nBar) const;
            // return sibling scrollbar control (or NULL if none)

      virtual int32_t ScrollWindowEx(int32_t dx, int32_t dy,
               LPCRECT lpRectScroll, LPCRECT lpRectClip,
               ::draw2d::region* prgnUpdate, LPRECT lpRectUpdate, UINT flags);
      virtual bool SetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo,
         bool bRedraw = TRUE);
      virtual bool GetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
      virtual int32_t GetScrollLimit(int32_t nBar);

   #if(WINVER >= 0x0500)

      virtual bool GetScrollBarInfo(LONG idObject, PSCROLLBARINFO psbi) const;

   #endif   // WINVER >= 0x0500

   // Window Access Functions
      virtual sp(::user::interaction) ChildWindowFromPoint(POINT point);
      virtual sp(::user::interaction) ChildWindowFromPoint(POINT point, UINT nFlags);
      static ::window_sp FindWindow(const char * lpszClassName, const char * lpszWindowName);
      static ::window_sp FindWindowEx(oswindow oswindowParent, oswindow oswindowChildAfter, const char * lpszClass, const char * lpszWindow);

      virtual sp(::user::interaction) GetNextWindow(UINT nFlag = GW_HWNDNEXT);
      virtual sp(::user::interaction) GetTopWindow();

      virtual sp(::user::interaction) GetWindow(UINT nCmd);
      virtual sp(::user::interaction) GetLastActivePopup();

      virtual bool IsChild(sp(::user::interaction)  pwindow);
      virtual ::user::interaction * get_parent();
      using ::user::interaction::set_parent;
      ::window_sp set_parent(::window_sp pWndNewParent);
      static ::window_sp WindowFromPoint(POINT point);

   // Alert Functions

      bool FlashWindow(bool bInvert);

      virtual int32_t message_box(const char * lpszText, const char * lpszCaption = NULL, UINT nType = MB_OK);


   #if(WINVER >= 0x0500)

      virtual bool FlashWindowEx(uint32_t dwFlags, UINT  uCount, uint32_t dwTimeout);

   #endif   // WINVER >= 0x0500

   // Clipboard Functions
      virtual bool ChangeClipboardChain(oswindow oswindow_Next);
      virtual ::oswindow  SetClipboardViewer();
      virtual bool OpenClipboard();
      static ::window_sp GetClipboardOwner();
      static ::window_sp GetClipboardViewer();
      static ::window_sp GetOpenClipboardWindow();

   // Caret Functions
      virtual void CreateCaret(::draw2d::bitmap* pBitmap);
      virtual void CreateSolidCaret(int32_t nWidth, int32_t nHeight);
      virtual void CreateGrayCaret(int32_t nWidth, int32_t nHeight);
      static point GetCaretPos();
      static void SetCaretPos(POINT point);
      virtual void HideCaret();
      virtual void ShowCaret();

   // Shell Interaction Functions
      virtual void DragAcceptFiles(bool bAccept = TRUE);

   // icon Functions
      virtual HICON SetIcon(HICON hIcon, bool bBigIcon);
      virtual HICON GetIcon(bool bBigIcon) const;

   // Context Help Functions
      virtual bool SetWindowContextHelpId(uint32_t dwContextHelpId);
      virtual uint32_t GetWindowContextHelpId() const;

   // Dialog Data support
      //virtual bool update_data(bool bSaveAndValidate = TRUE);
            // data wnd must be same type as this

   // Help Command Handlers
      void OnHelp();          // F1 (uses current context)
      void OnHelpIndex();     // ID_HELP_INDEX
      void OnHelpFinder();    // ID_HELP_FINDER, ID_DEFAULT_HELP
      void OnHelpUsing();     // ID_HELP_USING
      virtual void WinHelp(uint_ptr dwData, UINT nCmd = HELP_CONTEXT);
      //virtual void HtmlHelp(uint_ptr dwData, UINT nCmd = 0x000F);
      virtual void WinHelpInternal(uint_ptr dwData, UINT nCmd = HELP_CONTEXT);

   // layout and other functions
   /*   void RepositionBars(const char * pszPrefix, const char * pszIdLeftOver,
         UINT nFlag = reposDefault, LPRECT lpRectParam = NULL,
         LPCRECT lpRectClient = NULL, bool bStretch = TRUE);*/

      void RepositionBars(UINT nIDFirst, UINT nIDLast, id nIDLeftOver,
         UINT nFlags = reposDefault, LPRECT lpRectParam = NULL,
         LPCRECT lpRectClient = NULL, bool bStretch = TRUE);


      // dialog support
      void UpdateDialogControls(command_target* pTarget, bool bDisableIfNoHndler);
      void CenterWindow(sp(::user::interaction) pAlternateOwner = NULL);
      virtual id   RunModalLoop(uint32_t dwFlags = 0, ::core::live_object * pliveobject = NULL);
      virtual bool ContinueModal(int32_t iLevel);
      virtual void EndModalLoop(id nResult);
      virtual void EndAllModalLoops(id nResult);

   // Window-Management message handler member functions
      virtual bool OnCommand(::message::base * pbase);
      virtual bool OnNotify(::message::base * pbase);

      void OnActivate(UINT nState, ::window_sp pWndOther, bool bMinimized);
      void OnActivateApp(bool bActive, uint32_t dwThreadID);
      LRESULT OnActivateTopLevel(WPARAM, LPARAM);
      void OnCancelMode();
      void OnChildActivate();
      void OnClose();
      void OnContextMenu(::window_sp pwindow, point pos);
      bool OnCopyData(::window_sp pwindow, COPYDATASTRUCT* pCopyDataStruct);
      DECL_GEN_SIGNAL(_001OnCreate);


      HBRUSH OnCtlColor(::draw2d::graphics * pgraphics, ::window_sp pwindow, UINT nCtlColor);

      DECL_GEN_SIGNAL(_001OnDestroy);
      void OnEnable(bool bEnable);
      void OnEndSession(bool bEnding);
      void OnEnterIdle(UINT nWhy, ::window_sp pWho);
      bool OnEraseBkgnd(::draw2d::graphics * pgraphics);
      void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
      bool OnHelpInfo(HELPINFO* lpHelpInfo);
      void OnIconEraseBkgnd(::draw2d::graphics * pgraphics);
      void OnKillFocus(::window_sp pNewWnd);
      LRESULT OnMenuChar(UINT nChar, UINT nFlags, ::user::menu* pMenu);
      void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
      void OnMove(int32_t x, int32_t y);
      DECL_GEN_SIGNAL(_001OnPaint)
      DECL_GEN_SIGNAL(_001OnPrint)
      DECL_GEN_SIGNAL(_001OnCaptureChanged)
      void OnParentNotify(UINT message, LPARAM lParam);
      HCURSOR OnQueryDragIcon();
      bool OnQueryEndSession();
      bool OnQueryNewPalette();
      bool OnQueryOpen();
      void OnSetFocus(::window_sp pOldWnd);
      void OnShowWindow(bool bShow, UINT nStatus);
      void OnSize(UINT nType, int32_t cx, int32_t cy);
      void OnTCard(UINT idAction, uint32_t dwActionData);
      void OnWindowPosChanging(WINDOWPOS* lpwndpos);
      void OnWindowPosChanged(WINDOWPOS* lpwndpos);

      void OnChangeUIState(UINT nAction, UINT nUIElement);
      void OnUpdateUIState(UINT nAction, UINT nUIElement);
      UINT OnQueryUIState();

   // Nonclient-Area message handler member functions
      bool OnNcActivate(bool bActive);
      void OnNcCalcSize(bool bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
      bool OnNcCreate(LPCREATESTRUCT lpCreateStruct);
      DECL_GEN_SIGNAL(_001OnNcDestroy)
      DECL_GEN_SIGNAL(_001OnNcCalcSize)
      LRESULT OnNcHitTest(point point);
      void OnNcLButtonDblClk(UINT nHitTest, point point);
      void OnNcLButtonDown(UINT nHitTest, point point);
      void OnNcLButtonUp(UINT nHitTest, point point);
      void OnNcMButtonDblClk(UINT nHitTest, point point);
      void OnNcMButtonDown(UINT nHitTest, point point);
      void OnNcMButtonUp(UINT nHitTest, point point);
      void OnNcMouseMove(UINT nHitTest, point point);
      void OnNcPaint();
      void OnNcRButtonDblClk(UINT nHitTest, point point);
      void OnNcRButtonDown(UINT nHitTest, point point);
      void OnNcRButtonUp(UINT nHitTest, point point);

   // System message handler member functions
      void OnDropFiles(HDROP hDropInfo);
      void OnPaletteIsChanging(::window_sp pRealizeWnd);
      void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnSysCommand(UINT nID, LPARAM lParam);
      void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnCompacting(UINT nCpuTime);
      void OnDevModeChange(__in_z LPTSTR lpDeviceName);
      void OnFontChange();
      void OnPaletteChanged(::window_sp pFocusWnd);
      void OnSpoolerStatus(UINT nStatus, UINT nJobs);
      void OnSysColorChange();
      void OnTimeChange();
      void OnSettingChange(UINT uFlags, const char * lpszSection);
      void OnWinIniChange(const char * lpszSection);

   // Input message handler member functions
      void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
//      void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  //    void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
      void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnLButtonDblClk(UINT nFlags, point point);
      void OnLButtonDown(UINT nFlags, point point);
      void OnLButtonUp(UINT nFlags, point point);
      void OnMButtonDblClk(UINT nFlags, point point);
      void OnMButtonDown(UINT nFlags, point point);
      void OnMButtonUp(UINT nFlags, point point);
      int32_t OnMouseActivate(::window_sp pDesktopWnd, UINT nHitTest, UINT message);
      void OnMouseMove(UINT nFlags, point point);
      bool OnMouseWheel(UINT nFlags, int16_t zDelta, point pt);
      LRESULT OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam);
      void OnRButtonDblClk(UINT nFlags, point point);
      void OnRButtonDown(UINT nFlags, point point);
      void OnRButtonUp(UINT nFlags, point point);
      DECL_GEN_SIGNAL(_001OnSetCursor)
      void OnTimer(uint_ptr nIDEvent);

   // Initialization message handler member functions
      void OnInitMenu(::user::menu* pMenu);
      void OnInitMenuPopup(::user::menu* pPopupMenu, UINT nIndex, bool bSysMenu);

   // Clipboard message handler member functions
      void OnAskCbFormatName(__in UINT nMaxCount, __out_ecount_z(nMaxCount) LPTSTR lpszString);
      void OnChangeCbChain(oswindow oswindow_Remove, oswindow oswindow_After);
      void OnDestroyClipboard();
      void OnDrawClipboard();
      void OnHScrollClipboard(::window_sp pClipAppWnd, UINT nSBCode, UINT nPos);
      void OnPaintClipboard(::window_sp pClipAppWnd, HGLOBAL hPaintStruct);
      void OnRenderAllFormats();
      void OnRenderFormat(UINT nFormat);
      void OnSizeClipboard(::window_sp pClipAppWnd, HGLOBAL hRect);
      void OnVScrollClipboard(::window_sp pClipAppWnd, UINT nSBCode, UINT nPos);

   // control message handler member functions
      int32_t OnCompareItem(int32_t nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct);
      void OnDeleteItem(int32_t nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
      void OnDrawItem(int32_t nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
      UINT OnGetDlgCode();
      void OnMeasureItem(int32_t nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

   // MDI message handler member functions
      void OnMDIActivate(bool bActivate,
         ::window_sp pActivateWnd, ::window_sp pDeactivateWnd);

   // menu loop notification messages
      void OnEnterMenuLoop(bool bIsTrackPopupMenu);
      void OnExitMenuLoop(bool bIsTrackPopupMenu);

   // Win4 messages
      void OnStyleChanged(int32_t nStyleType, LPSTYLESTRUCT lpStyleStruct);
      void OnStyleChanging(int32_t nStyleType, LPSTYLESTRUCT lpStyleStruct);
      void OnSizing(UINT nSide, LPRECT lpRect);
      void OnMoving(UINT nSide, LPRECT lpRect);
      void OnCaptureChanged(::window_sp pwindow);
      bool OnDeviceChange(UINT nEventType, uint_ptr dwData);

   // Overridables and other helpers (for implementation of derived classes)
      // for deriving from a standard control
      virtual WNDPROC* GetSuperWndProcAddr();

      // for dialog data exchange and validation
//      virtual void do_data_exchange(CDataExchange* pDX);

      // for modality
      virtual void BeginModalState();
      virtual void EndModalState();

      // for translating Windows messages in main message pump
      virtual void pre_translate_message(signal_details * pobj);


      // for processing Windows messages
      virtual void message_handler(signal_details * pobj);
      //virtual bool OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

      // for handling default processing
      LRESULT Default();
      virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, lparam lParam);

      // for custom cleanup after WM_NCDESTROY
      virtual void PostNcDestroy();

      // for notifications from parent
      virtual bool OnChildNotify(::message::base * pbase);
         // return TRUE if parent should not process this message
      virtual bool ReflectChildNotify(::message::base * pbase);
      static bool ReflectMessage(oswindow oswindow_Child, ::message::base * pbase);

   // Implementation
      virtual bool CheckAutoCenter();
      virtual bool GrayCtlColor(HDC hDC, oswindow oswindow, UINT nCtlColor,
         HBRUSH hbrGray, COLORREF clrText);


      // helper routines for implementation
      bool HandleFloatingSysCommand(UINT nID, LPARAM lParam);
      bool IsTopParentActive();
      void ActivateTopParent();
      virtual void WalkPreTranslateTree(sp(::user::interaction) puiStop, signal_details * pobj);
      static sp(::user::interaction) GetDescendantWindow(sp(::user::interaction) oswindow, id id);
      static void SendMessageToDescendants(oswindow  oswindow, UINT message, WPARAM wParam, lparam lParam, bool bDeep, bool bOnlyPerm);
      virtual bool is_frame_window(); // is_kind_of(System.type_info < frame_window > ()))
      virtual void on_final_release();
      static bool ModifyStyle(oswindow oswindow, uint32_t dwRemove, uint32_t dwAdd, UINT nFlags);
      static bool ModifyStyleEx(oswindow oswindow, uint32_t dwRemove, uint32_t dwAdd, UINT nFlags);
      static void _FilterToolTipMessage(MSG* pMsg, ::window_sp pwindow);
      bool _EnableToolTips(bool bEnable, UINT nFlag);
      static oswindow get_safe_owner(::oswindow oswindow, ::oswindow * pWndTop);
      void PrepareForHelp();

      //UINT m_nFlags;      // see WF_ flags above

      WNDPROC m_pfnSuper; // for subclassing of controls
      static const UINT m_nMsgDragList;
      int32_t m_nModalResult; // for return values from window::RunModalLoop

      ::draw2d::font_sp m_pfont;

      friend class frame_window;

      // for creating dialogs and dialog-like windows
      bool CreateDlg(const char * lpszTemplateName, ::window_sp pParentWnd);
      //bool CreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate, ::window_sp pParentWnd,
        // HINSTANCE hInst);


      // implementation of message dispatch/hooking
      CLASS_DECL_win friend LRESULT CALLBACK __send_message_hook(int32_t, WPARAM, LPARAM);
      //CLASS_DECL_win friend void _gen::StandardSubclass;
      CLASS_DECL_win friend LRESULT CALLBACK __cbt_filter_hook(int32_t, WPARAM, LPARAM);
      CLASS_DECL_win friend LRESULT __call_window_procedure(sp(::user::interaction) pwindow, oswindow oswindow, UINT nMsg, WPARAM wParam, LPARAM lParam);

      // standard message implementation
      LRESULT OnNTCtlColor(WPARAM wParam, LPARAM lParam);
      LRESULT OnDisplayChange(WPARAM, LPARAM);
      LRESULT OnDragList(WPARAM, LPARAM);

      static BOOL CALLBACK GetAppsEnumWindowsProc(oswindow oswindow, LPARAM lParam);


      static void get_app_wnda(user::oswindow_array & wnda);

      virtual void _001DeferPaintLayeredWindowBackground(HDC hdc);

      virtual LONG GetWindowLong(int32_t nIndex);
      virtual LONG SetWindowLong(int32_t nIndex, LONG lValue);

      virtual void _001BaseWndInterfaceMap();


      void _001UpdateWindow();


      void _001OnTriggerMouseInside();

      void set_view_port_org(::draw2d::graphics * pgraphics);

      string calc_icon_window_class(uint32_t dwDefaultStyle, const char * pszMatter);
      string calc_window_class();

      void on_set_parent(sp(::user::interaction) pui);


   };


} // namespace win





