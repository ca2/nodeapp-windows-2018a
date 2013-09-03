#pragma once

namespace user
{
   class ::user::window_interface;
} // namespace user

namespace win
{

   class CLASS_DECL_win window_draw : 
      virtual public ::ca2::window_draw,
      virtual public ::ca2::message_window_simple_callback
   {
   public:


      enum EOptimize
      {
         OptimizeNone,
         OptimizeThis,
         OptimizeAllNext,
      };

      user::interaction_ptr_array                       m_wndpaOut;
      mutex                                  m_mutexRendering;
      mutex                                  m_mutexRgnUpdate;
      semaphore                              m_semaphoreBuffer;
      bool                                   m_bRender;
      DWORD                                  m_dwLastRedrawRequest;
      mutex                                  m_mutexRender;
      critical_section                       m_csWndInterfaceMap;
      typed_pointer_map <
         map_ptr_to_ptr,
         oswindow, 
         ::user::window_interface *>
                                             m_wndinterfacemap;
      DWORD                                  m_dwLastUpdate;
      DWORD                                  m_dwLastDelay;


      window_draw(sp(base_application) papp);
      virtual ~window_draw();


      bool UpdateBuffer(LPCRECT lpcrect);
      //void OnPaint(oswindow oswindow, CPaintDC & spgraphics);

      semaphore * TwfGetBufferSemaphore();

      // Bit blitting spread functions
      virtual bool ScreenOutput(
         user::buffer *,
         ::draw2d::region & rgnUpdate);

      virtual bool ScreenOutput();

      virtual bool ScreenOutput(user::buffer *, sp(::user::interaction) pwnd);

      // Transparent drawing operations
      // controllers
      virtual bool UpdateBuffer();

      virtual bool to(
         ::draw2d::graphics *       pdc,
         LPCRECT     lprectUpdate,
         user::oswindow_tree & oswindowtree,
         bool        bGdiLocked,
         bool        bExcludeParamWnd);

      virtual bool to(
         ::draw2d::graphics *       pdc,
         LPCRECT     lprectUpdate,
         user::oswindow_tree::Array & oswindowtreea,
         bool        bGdiLocked,
         bool        bExcludeParamWnd);

   /*     EOptimize TwfOptimizeRender2(
         user::oswindow_tree::Array & oswindowtreea,
         LPCRECT lpcrect);

      EOptimize TwfOptimizeRender2(
         user::oswindow_tree & oswindowtree,
         LPCRECT lpcrect);*/


      EOptimize TwfOptimizeRender(
         user::oswindow_tree::Array & oswindowtreea,
         LPCRECT lpcrect);

      EOptimize TwfOptimizeRender(
         user::oswindow_tree & oswindowtree,
         LPCRECT lpcrect);

      void TwfGetTopWindow(
         oswindow oswindow,
         user::oswindow_array & oswindowa,
         array < HRGN, HRGN > & hrgna,
         user::oswindow_tree::Array & oswindowtreea,
         LPCRECT lpcrect);

      bool TwfGetTopWindow(
         oswindow oswindow,
         user::oswindow_array & oswindowa,
         array < HRGN, HRGN > & hrgna,
         user::oswindow_tree::Array & oswindowtreea,
         HRGN hrgn);

      bool TwfGetTopWindow(
         oswindow oswindow,
         user::oswindow_array & oswindowa,
         array < HRGN, HRGN > & hrgna,
         user::oswindow_tree & oswindowtree,
         HRGN hrgn);

      void TwfGetTopWindowOptimizeOpaque(
         oswindow oswindowOpaque,
         user::oswindow_array & oswindowa,
         array < HRGN, HRGN > & hrgna);


      EOptimize TwfOptimizeRenderRemoveNextProper(
         user::oswindow_tree::Array & oswindowtreea,
         int32_t iIndex,
         LPCRECT lpcrect);

      void get_wnda(user::interaction_ptr_array & wndpa);
      void get_wnda(user::oswindow_array & oswindowa);
      
      virtual int32_t run();

      virtual UINT RedrawProc();

      virtual void asynch_redraw();
      virtual void _asynch_redraw();
      virtual void synch_redraw();
      virtual void _synch_redraw();

      virtual void message_window_message_handler(signal_details * pobj);

   };

} // namespace win