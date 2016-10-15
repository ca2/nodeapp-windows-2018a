#include "StdAfx.h"
#include <process.h>

void simple_se_translator(unsigned int uiCode, EXCEPTION_POINTERS * ppointers)
{
   //throw 0;
}




namespace npca2
{

   plugin::plugin()
   {
      m_bCa2JustInstalled     = false;
      m_iHealingSurface       = 0;
      m_pszUrl                = NULL;
      m_iEdge                 = -1;
      m_bAppStarted           = false;
      m_hEventReady           = NULL;


   }


   plugin::~plugin()
   {
   }





/*   UINT __cdecl plugin::ThreadProcStart(LPVOID lpv)
   {
      plugin * pplugin = (plugin *) lpv;
      try
      {
         return pplugin->start_bergedge_cube();
      }
      catch(...)
      {
      }
      return -1;
   }*/



   void plugin::on_prepare_memory()
   {
   }

   void plugin::start_ca2()
   {


      update_ca2_installed();
   

      if(is_ca2_installed())
      {
         HMODULE hmodule = ::LoadLibraryA("ca2.dll");
         FN_NEW_HOTPLUGIN fn_new_hotplugin = (FN_NEW_HOTPLUGIN) ::GetProcAddress(hmodule, "new_hotplugin");
         m_phost->m_pplugin = fn_new_hotplugin();
         m_phost->m_pplugin->m_phost = m_phost;
         m_phost->start_ca2_system();
         delete this;
         return;
      }
      else
      {
         starter_start("mplite");
      }

   }






   bool plugin::hist(const char * pszUrl)
   {
      return open_url(pszUrl);
   }


   void plugin::run_start_install(const char * pszInstall)
   {
   
      {
         XNode node;
         // remove install tag : should be turned into a function dependant of spalib at maximum
         if(!node.Load(file_get_contents_dup(dir::appdata("spa_install.xml"))))
            goto install;
         XNode * lpnodeInstalled = node.GetChild("installed");
         if(lpnodeInstalled == NULL)
            goto install;
         XNode * pnode = lpnodeInstalled->GetChildByAttr("application", "id", pszInstall);
         if(pnode == NULL)
            goto install;
         lpnodeInstalled->RemoveChild(pnode);
         file_put_contents_dup(dir::appdata("spa_install.xml"), node.GetXML(NULL));
      }
install:


      starter_start(pszInstall);


   }


   void plugin::on_paint(HDC hdcWindow, LPCRECT lprect)
   {
      RECT rectWindow;
      get_window_rect(&rectWindow);

      int cx = rectWindow.right - rectWindow.left;
      int cy = rectWindow.bottom - rectWindow.top;

      RECT rect;
      rect.left         = 0;
      rect.top          = 0;
      rect.right        = cx;
      rect.bottom       = cy;

      HBITMAP hbmp      = ::CreateCompatibleBitmap(hdcWindow, cx, cy);
      HDC hdc           = ::CreateCompatibleDC(hdcWindow);
      HBITMAP hbmpOld   =  (HBITMAP) ::SelectObject(hdc, (HGDIOBJ) hbmp);
   
      ::BitBlt(hdc, 0, 0, cx, cy, hdcWindow, m_rect.left, m_rect.top, SRCCOPY);

      HFONT hfontOld = NULL;
      HFONT hfont = NULL;
      if(is_installing_ca2())
      {
         ca2_install_canvas_on_paint(hdc, &rect, m_iHealingSurface);
      }
      else if(!is_ca2_installed())
      {
         RECT rect;
         rect.left      = 0;
         rect.top       = 0;
         rect.bottom    = cx;
         rect.right     = cy;
         ::FillSolidRect_dup(hdc, &rect, RGB(255, 255, 255));
         ::SetTextColor(hdc, RGB(255, 0, 255));
         const char * psz = "ca2 is not installed! You may try to install using low level spaboot_install.exe.";
         ::TextOutU_dup(hdc, 10, 10, psz, strlen_dup(psz));
      }
      else
      {
      }
      POINT pointViewport;
      ::SetViewportOrgEx(hdc, 0, 0, &pointViewport);
      ::BitBlt(hdcWindow, lprect->left, lprect->top, lprect->right - lprect->left, lprect->bottom - lprect->top, 
               hdc,       lprect->left - m_rect.left, lprect->top - m_rect.top, SRCCOPY);
      ::SelectObject(hdc, (HGDIOBJ) hbmpOld);
      if(hfontOld != NULL)
      {
         ::SelectObject(hdc,(HGDIOBJ)  hfontOld);
      }
      if(hfont != NULL)
      {
         ::DeleteObject(hfont);
      }
      ::DeleteObject(hbmp);
      ::DeleteDC(hdc);

      if(m_bCa2JustInstalled)
      {
         m_bCa2JustInstalled = false;
         initialize();
      }
      on_bare_paint(hdcWindow, lprect);

   }




/*   UINT plugin::start_bergedge_cube()
   {
   
      ::cube8::system * papp = m_psystem;
      
      if(!papp->initialize_instance())
         return 0;

//      if(!papp->bergedge_start())
  //       return 0;


    //  papp->m_puiInitialPlaceHolderContainer = papp->get_edge(0, true)->get_bergedge_view();



      papp->m_bShouldInitializeGTwf = false;
/*      win::thread * pthread = dynamic_cast < win::thread *> (papp->smart_pointer < ::ca::thread > ::m_p);
      win::thread * pthreadCur = dynamic_cast < win::thread *> (papp->GetThread()->m_p);
      pthread->m_hThread = pthreadCur->m_hThread;
      pthread->m_nThreadID = pthreadCur->m_nThreadID;

      papp->_001InstallMessageHandling(pthreadCur);*/

  /*    ::SetEvent(m_hEventReady);
      

      /*if(papp->does_launch_window_on_startup())
      {
         int iRetry = 100;
         while(papp->m_pwndMain == NULL && (iRetry > 0))
         {
            Sleep(100);
            iRetry--;
         }
         if(papp->m_pwndMain != NULL)
         {
   //            papp->m_pwndMain->SetParent(pview);
            papp->m_pwndMain->ModifyStyle(0, WS_CHILD);
            papp->m_pwndMain->SetWindowPos(NULL, 0, 0, 400, 400, SWP_SHOWWINDOW);
         }
      }*/


   //   pplugin->m_pframe = dynamic_cast < bergedge::frame * > (pplugin->m_papp->get_edge(0, true)->get_bergedge_view()->GetParentFrame());


   /*   papp->m_puiInitialPlaceHolderContainer = pplugin->m_pframe;

      App(pplugin->m_papp).GetThread()->SetMainWnd(pplugin->m_pframe);
      pplugin->m_pframe->m_bCustomFrame = false;
      pplugin->m_pframe->GetParent()->on_layout();*/



      /*try
      {
         papp->run();
      }
      catch(...)
      {
      }

      int iExitCode = -1;

      try
      {
         iExitCode = papp->exit_instance();
         //TRACE("Exit Code from retract_app %d (exit_instance %s)", iExitCode, typeid(*papp).raw_name());
      }
      catch(...)
      {
      }
      pthread->m_hThread = NULL;
      pthread->m_nThreadID = NULL;
   
      WIN_THREAD(papp->smart_pointer < ::ca::thread >::m_p)->m_bRun = false;
      ::SetEvent(m_hEventReady);
      //papp->m_bRun = false;

      return iExitCode;
   }*/












   UINT_PTR plugin::message_handler(UINT_PTR uiMessage, WPARAM wparam, LPARAM lparam)
   {
      switch(uiMessage)
      {
      case WM_TIMER:
         {
            switch(wparam)
            {
            case 8477:
               {
                  KillTimer(get_host_window(), 8477);
                  start_ca2();
               }
               break;
            }
         }
         return 0;
      default:
         {
            if(!is_spa_installed() || !is_spa_updated())
            {
               return 1;
            }

            if((uiMessage == WM_LBUTTONUP
            || uiMessage == WM_RBUTTONUP
            || uiMessage == WM_MBUTTONUP) &&
               is_installing_ca2())
            {
               m_iHealingSurface++;
            }
         }
      }
      return 0;      
   }


   bool plugin::initialize()
   {
      start_ca2();
      return true;
   }

} // namespace npca2





::hotplugin::plugin * new_hotplugin()
{
   return new ::npca2::plugin();
}