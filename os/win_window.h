#pragma once


namespace win
{


   CLASS_DECL_win LRESULT CALLBACK __send_message_hook(int32_t, WPARAM, LPARAM);
   //CLASS_DECL_win void _gen::StandardSubclass;
   CLASS_DECL_win LRESULT CALLBACK __cbt_filter_hook(int32_t, WPARAM, LPARAM);
   CLASS_DECL_win LRESULT __call_window_procedure(::user::interaction * pWnd, oswindow oswindow, UINT nMsg, WPARAM wParam, LPARAM lParam);


   class CLASS_DECL_win window : 
      virtual public ::ca::window,
      virtual public oswindow_handle
   {
   public:


      pha(::user::interaction)      m_guieptraMouseHover;
      ::ca::window_callback *       m_pcallback;
      string                        m_strWindowText;
      //visual::dib_sp                m_spdib;
      //visual::dib_sp                m_spdibMultAlphaWork;


      window();
      window(::ca::application * papp);
      virtual ~window();


      virtual void construct(oswindow oswindow);


      virtual void on_delete(::ca::ca * poc);


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      virtual void mouse_hover_add(::user::interaction* pinterface);
      virtual void mouse_hover_remove(::user::interaction* pinterface);

      bool create_message_window(const char * pszName, ::ca::window_callback * pcallback = NULL);

      static const MSG* PASCAL GetCurrentMessage();

      virtual void install_message_handling(::gen::message::dispatch * pinterface);

      bool operator==(const ::ca::window& wnd) const;
      bool operator!=(const ::ca::window& wnd) const;

      DWORD GetStyle();
      DWORD GetExStyle();
      bool ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
      bool ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);

      //virtual ::user::interaction * get_owner();
      virtual void set_owner(::user::interaction * pOwnerWnd);

      virtual oswindow get_handle() const;

      virtual bool _001OnCmdMsg(BaseCmdMsg * pcmdmsg);   

      virtual bool BaseOnControlEvent(::user::control_event * pevent);

      void _002OnDraw(::ca::graphics * pdc);

      DECL_GEN_SIGNAL(_001OnEraseBkgnd)
      DECL_GEN_SIGNAL(_001OnMove)
      DECL_GEN_SIGNAL(_001OnSize)
      DECL_GEN_SIGNAL(_001OnShowWindow)
      DECL_GEN_SIGNAL(_001OnProdevianSynch)

      ::user::window_interface *      m_pbasewnd;
      ::user::interaction *        m_pguieCapture;

   #if(WINVER >= 0x0500)

      bool GetWindowInfo(PWINDOWINFO pwi) const;
      bool GetTitleBarInfo(PTITLEBARINFO pti) const;

   #endif   // WINVER >= 0x0500

      virtual ::ca::window * from_os_data(void * pdata);
      virtual void * get_os_data() const;

      static window * PASCAL from_handle(oswindow oswindow);
      static window * PASCAL FromHandlePermanent(oswindow oswindow);
      static void PASCAL DeleteTempMap();
      bool attach(oswindow oswindow_New);
      oswindow detach();

      // subclassing/unsubclassing functions
      virtual void pre_subclass_window();
      bool subclass_window(oswindow oswindow);
      bool SubclassDlgItem(UINT nID, ::ca::window * pParent);
      oswindow unsubclass_window();

      // handling of RT_DLGINIT resource (extension to RT_DIALOG)
      bool ExecuteDlgInit(const char * lpszResourceName);
      bool ExecuteDlgInit(LPVOID lpResource);

      using ::user::interaction::create;
      // for child windows, views, panes etc
      virtual bool create(const char * lpszClassName,
         const char * lpszWindowName, DWORD dwStyle,
         const RECT& rect,
         ::user::interaction * pParentWnd, id id,
         ::ca::create_context* pContext = NULL);

      // advanced creation (allows access to extended styles)
      virtual bool CreateEx(DWORD dwExStyle, const char * lpszClassName,
         const char * lpszWindowName, DWORD dwStyle,
         int32_t x, int32_t y, int32_t nWidth, int32_t nHeight,
         oswindow oswindow_Parent, id id, LPVOID lpParam = NULL);

      virtual bool CreateEx(DWORD dwExStyle, const char * lpszClassName,
         const char * lpszWindowName, DWORD dwStyle,
         const RECT& rect,
         ::user::interaction* pParentWnd, id id,
         LPVOID lpParam = NULL);

      virtual bool DestroyWindow();

      // special pre-creation and ::ca::window rect adjustment hooks
      virtual bool pre_create_window(CREATESTRUCT& cs);

      // Advanced: virtual AdjustWindowRect
      enum AdjustType { adjustBorder = 0, adjustOutside = 1 };
      virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);

      
         // get immediate child with given ID
      using ::user::interaction::get_child_by_id;
      void get_child_by_id(id id, oswindow* poswindow_) const;
         // as above, but returns oswindow
      using ::user::interaction::GetDescendantWindow;
      ::user::interaction * GetDescendantWindow(id id);
         // like get_child_by_id but recursive
      void SendMessageToDescendants(UINT message, WPARAM wParam = 0,
         LPARAM lParam = 0, bool bDeep = TRUE, bool bOnlyPerm = FALSE);
      frame_window* GetParentFrame();
      frame_window* EnsureParentFrame();
      ::user::interaction* GetTopLevelParent();
      ::user::interaction* EnsureTopLevelParent();
      ::user::interaction* GetTopLevelOwner();
      ::user::interaction* GetParentOwner();
      frame_window* GetTopLevelFrame();
      static ::ca::window * PASCAL get_safe_owner(::ca::window * pParent = NULL, oswindow* pWndTop = NULL);

      virtual bool IsWindow();

   #if(WINVER >= 0x0500)

      ::ca::window * GetAncestor(UINT gaFlags) const;

   #endif   // WINVER >= 0x0500

      LRESULT send_message(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
      bool PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

      bool SendNotifyMessage(UINT message, WPARAM wParam, LPARAM lParam);
      bool SendChildNotifyLastMsg(LRESULT* pResult = NULL);

      bool DragDetect(POINT pt) const;



   // Window Text Functions
      void SetWindowText(const char * lpszString);
      strsize GetWindowText(LPTSTR lpszStringBuf, strsize nMaxCount);
      void GetWindowText(string & rString);
      strsize GetWindowTextLength();
      void SetFont(::ca::font* pFont, bool bRedraw = TRUE);
      ::ca::font* GetFont();


   // Window size and position Functions
      virtual bool IsIconic();
      virtual bool IsZoomed();
      void MoveWindow(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight,
               bool bRepaint = TRUE);
      void MoveWindow(LPCRECT lpRect, bool bRepaint = TRUE);
      int32_t SetWindowRgn(HRGN hRgn, bool bRedraw);
      int32_t GetWindowRgn(HRGN hRgn);

      //static const ::ca::window wndTop; // SetWindowPos's pWndInsertAfter
      //static const ::ca::window wndBottom; // SetWindowPos's pWndInsertAfter
      //static const ::ca::window wndTopMost; // SetWindowPos pWndInsertAfter
      //static const ::ca::window wndNoTopMost; // SetWindowPos pWndInsertAfter

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

   // Coordinate Mapping Functions
      virtual void MapWindowPoints(::ca::window * pwndTo, LPPOINT lpPoint, UINT nCount);
      virtual void MapWindowPoints(::ca::window * pwndTo, LPRECT lpRect);

   // Update/Painting Functions
      virtual ::ca::graphics * GetDC();
      virtual ::ca::graphics * GetWindowDC();
      virtual bool ReleaseDC(::ca::graphics * pgraphics);
      virtual void Print(::ca::graphics * pgraphics, DWORD dwFlags) const;
      virtual void PrintClient(::ca::graphics * pgraphics, DWORD dwFlags) const;

      virtual void UpdateWindow();
      virtual void SetRedraw(bool bRedraw = TRUE);
      virtual bool GetUpdateRect(LPRECT lpRect, bool bErase = FALSE);
      virtual int32_t GetUpdateRgn(::ca::region* pRgn, bool bErase = FALSE);
      virtual void Invalidate(bool bErase = TRUE);
      virtual void InvalidateRect(LPCRECT lpRect, bool bErase = TRUE);
      virtual void InvalidateRgn(::ca::region* pRgn, bool bErase = TRUE);
      virtual void ValidateRect(LPCRECT lpRect);
      virtual void ValidateRgn(::ca::region* pRgn);
      virtual bool ShowWindow(int32_t nCmdShow);
      virtual void _001WindowMaximize();
      virtual void _001WindowRestore();
      virtual bool IsWindowVisible();
      virtual void ShowOwnedPopups(bool bShow = TRUE);

      virtual ::ca::graphics * GetDCEx(::ca::region* prgnClip, DWORD flags);
      virtual bool LockWindowUpdate();
      virtual void UnlockWindowUpdate();
      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,
         ::ca::region* prgnUpdate = NULL,
         UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
      virtual bool EnableScrollBar(int32_t nSBFlags, UINT nArrowFlags = ESB_ENABLE_BOTH);

      virtual bool DrawAnimatedRects(int32_t idAni, CONST RECT *lprcFrom, CONST RECT *lprcTo);
      virtual bool DrawCaption(::ca::graphics * pgraphics, LPCRECT lprc, UINT uFlags);

   #if(WINVER >= 0x0500)

      virtual bool AnimateWindow(DWORD dwTime, DWORD dwFlags);

   #endif   // WINVER >= 0x0500

   #if(_WIN32_WINNT >= 0x0501)

      virtual bool PrintWindow(::ca::graphics * pgraphics, UINT nFlags) const;

   #endif   // _WIN32_WINNT >= 0x0501

   // Layered Window

   #if(_WIN32_WINNT >= 0x0500)

      virtual bool SetLayeredWindowAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
      virtual bool UpdateLayeredWindow(::ca::graphics * pDCDst, POINT *pptDst, SIZE *psize, 
         ::ca::graphics * pDCSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);

   #endif   // _WIN32_WINNT >= 0x0500

   #if(_WIN32_WINNT >= 0x0501)

      virtual bool GetLayeredWindowAttributes(COLORREF *pcrKey, BYTE *pbAlpha, DWORD *pdwFlags) const;

   #endif   // _WIN32_WINNT >= 0x0501


   // Timer Functions
      virtual uint_ptr SetTimer(uint_ptr nIDEvent, UINT nElapse,
         void (CALLBACK* lpfnTimer)(oswindow, UINT, uint_ptr, DWORD));
      virtual bool KillTimer(uint_ptr nIDEvent);

   // Window State Functions
      virtual bool IsWindowEnabled();
      virtual bool EnableWindow(bool bEnable = TRUE);

      // the active ::ca::window applies only to top-level (frame windows)
      virtual ::user::interaction * GetActiveWindow();
      virtual ::user::interaction * SetActiveWindow();

      // the foreground ::ca::window applies only to top-level windows (frame windows)
      virtual bool SetForegroundWindow();
      static ::ca::window * PASCAL GetForegroundWindow();

      virtual id SetDlgCtrlId(id id);
      virtual id GetDlgCtrlId();

      

      // capture and focus apply to all windows
      static ::ca::window * PASCAL GetCapture();
      virtual ::user::interaction * set_capture(::user::interaction * pinterface = NULL);
      virtual ::user::interaction * release_capture();
      virtual ::user::interaction * get_capture();
      static ::ca::window * PASCAL GetFocus();
      ::ca::window * SetFocus();

      static ::ca::window * PASCAL GetDesktopWindow();

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
      virtual ::ca::window * GetNextDlgGroupItem(::ca::window * pWndCtl, bool bPrevious = FALSE) const;
      virtual ::ca::window * GetNextDlgTabItem(::ca::window * pWndCtl, bool bPrevious = FALSE) const;
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
      virtual CScrollBar* GetScrollBarCtrl(int32_t nBar) const;
            // return sibling scrollbar control (or NULL if none)

      virtual int32_t ScrollWindowEx(int32_t dx, int32_t dy,
               LPCRECT lpRectScroll, LPCRECT lpRectClip,
               ::ca::region* prgnUpdate, LPRECT lpRectUpdate, UINT flags);
      virtual bool SetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo,
         bool bRedraw = TRUE);
      virtual bool GetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
      virtual int32_t GetScrollLimit(int32_t nBar);

   #if(WINVER >= 0x0500)

      virtual bool GetScrollBarInfo(LONG idObject, PSCROLLBARINFO psbi) const;

   #endif   // WINVER >= 0x0500

   // Window Access Functions
      virtual ::ca::window * ChildWindowFromPoint(POINT point);
      virtual ::ca::window * ChildWindowFromPoint(POINT point, UINT nFlags);
      static ::ca::window * PASCAL FindWindow(const char * lpszClassName, const char * lpszWindowName);
      static ::ca::window * FindWindowEx(oswindow oswindowParent, oswindow oswindowChildAfter, const char * lpszClass, const char * lpszWindow);

      virtual ::user::interaction * GetNextWindow(UINT nFlag = GW_HWNDNEXT);
      virtual ::user::interaction * GetTopWindow();

      virtual ::user::interaction * GetWindow(UINT nCmd);
      virtual ::user::interaction * GetLastActivePopup();

      virtual bool IsChild(::user::interaction *  pWnd);
      virtual ::user::interaction * get_parent() const;
      using ::user::interaction::set_parent;
      ::ca::window * set_parent(::ca::window * pWndNewParent);
      static ::ca::window * PASCAL WindowFromPoint(POINT point);

   // Alert Functions

      bool FlashWindow(bool bInvert);

      virtual int32_t message_box(const char * lpszText, const char * lpszCaption = NULL, UINT nType = MB_OK);


   #if(WINVER >= 0x0500)

      virtual bool FlashWindowEx(DWORD dwFlags, UINT  uCount, DWORD dwTimeout);

   #endif   // WINVER >= 0x0500

   // Clipboard Functions
      virtual bool ChangeClipboardChain(oswindow oswindow_Next);
      virtual ::oswindow  SetClipboardViewer();
      virtual bool OpenClipboard();
      static ::ca::window * PASCAL GetClipboardOwner();
      static ::ca::window * PASCAL GetClipboardViewer();
      static ::ca::window * PASCAL GetOpenClipboardWindow();

   // Caret Functions
      virtual void CreateCaret(::ca::bitmap* pBitmap);
      virtual void CreateSolidCaret(int32_t nWidth, int32_t nHeight);
      virtual void CreateGrayCaret(int32_t nWidth, int32_t nHeight);
      static point PASCAL GetCaretPos();
      static void PASCAL SetCaretPos(POINT point);
      virtual void HideCaret();
      virtual void ShowCaret();

   // Shell Interaction Functions
      virtual void DragAcceptFiles(bool bAccept = TRUE);

   // icon Functions
      virtual HICON SetIcon(HICON hIcon, bool bBigIcon);
      virtual HICON GetIcon(bool bBigIcon) const;

   // Context Help Functions
      virtual bool SetWindowContextHelpId(DWORD dwContextHelpId);
      virtual DWORD GetWindowContextHelpId() const;

   // Dialog Data support
      //virtual bool update_data(bool bSaveAndValidate = TRUE);
            // data wnd must be same type as this

   // Help Command Handlers
      void OnHelp();          // F1 (uses current context)
      void OnHelpIndex();     // ID_HELP_INDEX
      void OnHelpFinder();    // ID_HELP_FINDER, ID_DEFAULT_HELP
      void OnHelpUsing();     // ID_HELP_USING
      virtual void WinHelp(dword_ptr dwData, UINT nCmd = HELP_CONTEXT);
      //virtual void HtmlHelp(dword_ptr dwData, UINT nCmd = 0x000F);
      virtual void WinHelpInternal(dword_ptr dwData, UINT nCmd = HELP_CONTEXT);

   // layout and other functions
   /*   void RepositionBars(const char * pszPrefix, const char * pszIdLeftOver,
         UINT nFlag = reposDefault, LPRECT lpRectParam = NULL,
         LPCRECT lpRectClient = NULL, bool bStretch = TRUE);*/

      void RepositionBars(UINT nIDFirst, UINT nIDLast, id nIDLeftOver,
         UINT nFlags = reposDefault, LPRECT lpRectParam = NULL,
         LPCRECT lpRectClient = NULL, bool bStretch = TRUE);


      // dialog support
      void UpdateDialogControls(command_target* pTarget, bool bDisableIfNoHndler);
      void CenterWindow(::user::interaction * pAlternateOwner = NULL);
      virtual id   RunModalLoop(DWORD dwFlags = 0, ::ca::live_object * pliveobject = NULL);
      virtual bool ContinueModal(int32_t iLevel);
      virtual void EndModalLoop(id nResult);
      virtual void EndAllModalLoops(id nResult);

   // Window-Management message handler member functions
      virtual bool OnCommand(WPARAM wParam, LPARAM lParam);
      virtual bool OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

      void OnActivate(UINT nState, ::ca::window * pWndOther, bool bMinimized);
      void OnActivateApp(bool bActive, DWORD dwThreadID);
      LRESULT OnActivateTopLevel(WPARAM, LPARAM);
      void OnCancelMode();
      void OnChildActivate();
      void OnClose();
      void OnContextMenu(::ca::window * pWnd, point pos);
      bool OnCopyData(::ca::window * pWnd, COPYDATASTRUCT* pCopyDataStruct);
      DECL_GEN_SIGNAL(_001OnCreate);


      HBRUSH OnCtlColor(::ca::graphics * pgraphics, ::ca::window * pWnd, UINT nCtlColor);

      DECL_GEN_SIGNAL(_001OnDestroy);
      void OnEnable(bool bEnable);
      void OnEndSession(bool bEnding);
      void OnEnterIdle(UINT nWhy, ::ca::window * pWho);
      bool OnEraseBkgnd(::ca::graphics * pgraphics);
      void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
      bool OnHelpInfo(HELPINFO* lpHelpInfo);
      void OnIconEraseBkgnd(::ca::graphics * pgraphics);
      void OnKillFocus(::ca::window * pNewWnd);
      LRESULT OnMenuChar(UINT nChar, UINT nFlags, ::userbase::menu* pMenu);
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
      void OnSetFocus(::ca::window * pOldWnd);
      void OnShowWindow(bool bShow, UINT nStatus);
      void OnSize(UINT nType, int32_t cx, int32_t cy);
      void OnTCard(UINT idAction, DWORD dwActionData);
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
      void OnPaletteIsChanging(::ca::window * pRealizeWnd);
      void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnSysCommand(UINT nID, LPARAM lParam);
      void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnCompacting(UINT nCpuTime);
      void OnDevModeChange(__in_z LPTSTR lpDeviceName);
      void OnFontChange();
      void OnPaletteChanged(::ca::window * pFocusWnd);
      void OnSpoolerStatus(UINT nStatus, UINT nJobs);
      void OnSysColorChange();
      void OnTimeChange();
      void OnSettingChange(UINT uFlags, const char * lpszSection);
      void OnWinIniChange(const char * lpszSection);

   // Input message handler member functions
      void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
      void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
      void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
      void OnLButtonDblClk(UINT nFlags, point point);
      void OnLButtonDown(UINT nFlags, point point);
      void OnLButtonUp(UINT nFlags, point point);
      void OnMButtonDblClk(UINT nFlags, point point);
      void OnMButtonDown(UINT nFlags, point point);
      void OnMButtonUp(UINT nFlags, point point);
      int32_t OnMouseActivate(::ca::window * pDesktopWnd, UINT nHitTest, UINT message);
      void OnMouseMove(UINT nFlags, point point);
      bool OnMouseWheel(UINT nFlags, int16_t zDelta, point pt);
      LRESULT OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam);
      void OnRButtonDblClk(UINT nFlags, point point);
      void OnRButtonDown(UINT nFlags, point point);
      void OnRButtonUp(UINT nFlags, point point);
      DECL_GEN_SIGNAL(_001OnSetCursor)
      void OnTimer(uint_ptr nIDEvent);

   // Initialization message handler member functions
      void OnInitMenu(::userbase::menu* pMenu);
      void OnInitMenuPopup(::userbase::menu* pPopupMenu, UINT nIndex, bool bSysMenu);

   // Clipboard message handler member functions
      void OnAskCbFormatName(__in UINT nMaxCount, __out_ecount_z(nMaxCount) LPTSTR lpszString);
      void OnChangeCbChain(oswindow oswindow_Remove, oswindow oswindow_After);
      void OnDestroyClipboard();
      void OnDrawClipboard();
      void OnHScrollClipboard(::ca::window * pClipAppWnd, UINT nSBCode, UINT nPos);
      void OnPaintClipboard(::ca::window * pClipAppWnd, HGLOBAL hPaintStruct);
      void OnRenderAllFormats();
      void OnRenderFormat(UINT nFormat);
      void OnSizeClipboard(::ca::window * pClipAppWnd, HGLOBAL hRect);
      void OnVScrollClipboard(::ca::window * pClipAppWnd, UINT nSBCode, UINT nPos);

   // control message handler member functions
      int32_t OnCompareItem(int32_t nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct);
      void OnDeleteItem(int32_t nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
      void OnDrawItem(int32_t nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
      UINT OnGetDlgCode();
      void OnMeasureItem(int32_t nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

   // MDI message handler member functions
      void OnMDIActivate(bool bActivate,
         ::ca::window * pActivateWnd, ::ca::window * pDeactivateWnd);

   // menu loop notification messages
      void OnEnterMenuLoop(bool bIsTrackPopupMenu);
      void OnExitMenuLoop(bool bIsTrackPopupMenu);

   // Win4 messages
      void OnStyleChanged(int32_t nStyleType, LPSTYLESTRUCT lpStyleStruct);
      void OnStyleChanging(int32_t nStyleType, LPSTYLESTRUCT lpStyleStruct);
      void OnSizing(UINT nSide, LPRECT lpRect);
      void OnMoving(UINT nSide, LPRECT lpRect);
      void OnCaptureChanged(::ca::window * pWnd);
      bool OnDeviceChange(UINT nEventType, dword_ptr dwData);

   // Overridables and other helpers (for implementation of derived classes)
      // for deriving from a standard control
      virtual WNDPROC* GetSuperWndProcAddr();

      // for dialog data exchange and validation
//      virtual void do_data_exchange(CDataExchange* pDX);

      // for modality
      virtual void BeginModalState();
      virtual void EndModalState();

      // for translating Windows messages in main message pump
      virtual void pre_translate_message(gen::signal_object * pobj);


      // for processing Windows messages
      virtual void message_handler(gen::signal_object * pobj);
      //virtual bool OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

      // for handling default processing
      LRESULT Default();
      virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

      // for custom cleanup after WM_NCDESTROY
      virtual void PostNcDestroy();

      // for notifications from parent
      virtual bool OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
         // return TRUE if parent should not process this message
      bool ReflectChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
      static bool PASCAL ReflectLastMsg(oswindow oswindow_Child, LRESULT* pResult = NULL);

   // Implementation
      virtual bool CheckAutoCenter();
      static bool PASCAL GrayCtlColor(HDC hDC, oswindow oswindow, UINT nCtlColor,
         HBRUSH hbrGray, COLORREF clrText);


      // helper routines for implementation
      bool HandleFloatingSysCommand(UINT nID, LPARAM lParam);
      bool IsTopParentActive();
      void ActivateTopParent();
      virtual void WalkPreTranslateTree(::user::interaction * puiStop, gen::signal_object * pobj);
      static ::user::interaction * PASCAL GetDescendantWindow(::user::interaction * oswindow, id id);
      static void PASCAL SendMessageToDescendants(oswindow  oswindow, UINT message, WPARAM wParam, LPARAM lParam, bool bDeep, bool bOnlyPerm);
      virtual bool IsFrameWnd(); // is_kind_of(System.template type_info < frame_window > ()))
      virtual void on_final_release();
      static bool PASCAL ModifyStyle(oswindow oswindow, DWORD dwRemove, DWORD dwAdd, UINT nFlags);
      static bool PASCAL ModifyStyleEx(oswindow oswindow, DWORD dwRemove, DWORD dwAdd, UINT nFlags);
      static void PASCAL _FilterToolTipMessage(MSG* pMsg, ::ca::window * pWnd);
      bool _EnableToolTips(bool bEnable, UINT nFlag);
      static oswindow PASCAL get_safe_owner(::oswindow oswindow, ::oswindow * pWndTop);
      void PrepareForHelp();

      //UINT m_nFlags;      // see WF_ flags above

      WNDPROC m_pfnSuper; // for subclassing of controls
      static const UINT m_nMsgDragList;
      int32_t m_nModalResult; // for return values from ::ca::window::RunModalLoop

      ::ca::font * m_pfont;

      friend class frame_window;

      // for creating dialogs and dialog-like windows
      bool CreateDlg(const char * lpszTemplateName, ::ca::window * pParentWnd);
      //bool CreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate, ::ca::window * pParentWnd,
        // HINSTANCE hInst);


      // implementation of message dispatch/hooking
      CLASS_DECL_win friend LRESULT CALLBACK __send_message_hook(int32_t, WPARAM, LPARAM);
      //CLASS_DECL_win friend void _gen::StandardSubclass;
      CLASS_DECL_win friend LRESULT CALLBACK __cbt_filter_hook(int32_t, WPARAM, LPARAM);
      CLASS_DECL_win friend LRESULT __call_window_procedure(::user::interaction * pWnd, oswindow oswindow, UINT nMsg, WPARAM wParam, LPARAM lParam);

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


   };


} // namespace win


