#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      thread::thread(::aura::application * papp) :
         ::object(papp),
         ::thread(papp)
      {

         m_bPendingShowCommand = false;

      }


      thread::~thread()
      {
         

      }


      bool thread::initialize_thread()
      {

//         set_auto_delete(false);

         return true;

      }


      int32_t thread::exit_thread()
      {
         // TODO:  perform any per-thread cleanup here
         //    CVMSApp * pApp = (CVMSApp *) &System;
         //MessageFilterHookManager * pmfmh;
         //MessageFilterHookManager::AppGetMessageFilterHookManager(&pmfmh);
         //pmfmh->MessageFilterUnhook();

         CoUninitialize();
         return ::thread::exit_thread();
      }

      void thread::install_message_routing(::message::sender * pinterface)
      {
         IGUI_WIN_MSG_LINK(thread::MessageMixerThread, pinterface, this, &thread::OnMixerMessage);
         IGUI_WIN_MSG_LINK(WM_USER, pinterface, this, &thread::OnUserMessage);
      }

      // BEGIN_MESSAGE_MAP(thread, thread)
      //{{AFX_MSG_MAP(thread)
      // NOTE - the ClassWizard will add and remove mapping macros here.
      //}}AFX_MSG_MAP
      //    ON_THREAD_MESSAGE(thread::MessageMixerThread, OnMixerMessage)
      /* xxx   ON_THREAD_MESSAGE(VMSM_V001, OnVmsmV001)
      ON_THREAD_MESSAGE(VMSM_EXCEPTION, OnVmsmException) */
      //    ON_THREAD_MESSAGE(WM_USER, OnUserMessage)
      // END_MESSAGE_MAP()

      /////////////////////////////////////////////////////////////////////////////
      // thread message handlers


      void thread::OnMixerMessage(::message::message * pobj)
      {
         SCAST_PTR(::message::base, pbase, pobj);

         if(pbase->m_wparam ==  23)
         {
         }
         else if(pbase->m_wparam ==  21)
         {
         }
         else if(pbase->m_wparam == 19)
         {
            ASSERT(FALSE);
            //        CDBCentral * pgraphics = (CDBCentral *) lParam;
            //      pgraphics->SongsCreateBuildAlbumThread(
            //        (CDataCentral::EBuildSongsAlbumOperation)
            //      (CDataCentral::eBuildSongsAlbumDeleteNotFound |
            //    CDataCentral::eBuildSongsAlbumAddNew |
            //  CDataCentral::eBuildSongsAlbumCreateSearchReferences),
            //false, this, this);
         }
         else if(pbase->m_wparam == 20)
         {
            ASSERT(FALSE);
            //        CDBCentral * pgraphics = (CDBCentral *) lParam;
            //      pgraphics->SongsCreateBuildAlbumThread(
            //        (CDataCentral::EBuildSongsAlbumOperation)
            //      (CDataCentral::eBuildSongsAlbumAddNew |
            //    CDataCentral::eBuildSongsAlbumCreateSearchReferences),
            //  true, this, this);
         }
         else if(pbase->m_wparam == 810)
         {
            ASSERT(FALSE);
            //        CDBCentral * pgraphics = (CDBCentral *) lParam;
            //      pgraphics->SongsCreateBuildAlbumThread(
            //        (CDataCentral::EBuildSongsAlbumOperation)
            //      CDataCentral::eBuildSongsAlbumDeleteNotFound,
            //    false, this, this);
         }
         else if(pbase->m_wparam == 20191817)
         {
            ASSERT(FALSE);
         }
         else if(pbase->m_wparam == 67985)
         {
            ASSERT(FALSE);
            //        m_pMixerDocTemplate->close_all_documents(lParam != 0);
         }


         //    if(m_pMixerDocTemplate != NULL)
         //  {
         //    m_pMixerDocTemplate->open_new_document();
         //  if(GetMainWnd() != NULL)
         //{
         //  GetMainWnd()->SetForegroundWindow();
         //}
         //}
      }




      //bool thread::on_idle(LONG lCount)
      //{
      //   // TODO: add your specialized code here and/or call the base class
      //   return ::thread::on_idle(lCount);
      //}

      void thread::OnVmsmException(::message::message * pobj)
      {
         UNREFERENCED_PARAMETER(pobj);
         //sp(::axis::application) pApp = (sp(::axis::application)) &System;
         //pApp->post_thread_message(VMSM_EXCEPTION, wParam, lParam);
      }


      void thread::OnUserMessage(::message::message * pobj)
      {
         SCAST_PTR(::message::base, pbase, pobj);

         switch(pbase->m_wparam)
         {

         case 21:
            //                    ASSERT(FALSE);
            //                  //ReserveSong_(pMsg->lParam);
            //GetPlayerDocTemplate()->ReserveSong(lParam, false, true);
            break;
         case 22:
            {
               //  CDWArray * pdwa = (CDWArray *) lParam;
               //GetPlayerDocTemplate()->ReserveSong((LPINT) &pdwa->get_data()[1], pdwa->get_at(0), false, true);
               //delete pdwa;
            }
            break;
         case 23:
            {
               //CStrArray * pstra = (CStrArray *) lParam;
               //GetPlayerDocTemplate()->ReserveSong(pstra, false, true);
               //delete pstra;
            }
            break;
         case 543:
            {

               //            if(m_poptionsview == NULL)
               {
                  //                m_poptionsview = new COptionsDialog();
                  //              m_poptionsview->create(NULL);
               }

               //            if(m_poptionsview != NULL)
               {
                  //            m_poptionsview->ShowWindow(SW_SHOWNORMAL);
               }

            }
            break;

         }

         switch(pbase->m_wparam)
         {
         case 131415:
            // ViewToolsAlbum();
            break;
         case 565758:
            {
               /*CDBBuildAlbumThreadData * lpdata =
               (CDBBuildAlbumThreadData *)lParam;
               sp(single_document_template) pAlbumDocTemplate = NULL;
               if(GetAlbumThread())
               {
               pAlbumDocTemplate = GetAlbumThread()->GetMixerDocTemplate();
               }
               bool bVisible = false;
               if(pAlbumDocTemplate != NULL)
               {
               bVisible = pAlbumDocTemplate->get_document_count() != NULL;

               // avoid thread quit on close all documents
               GetAlbumThread()->GetMainWnd() = NULL;

               GetAlbumThread()->GetMixerDocTemplate()->close_all_documents(false);
               }
               //CMixerFrameWnd * pAlbum = GetMixerDocTemplate();
               //bool bVisible;
               //if(pAlbum != NULL)
               //{
               //  bVisible = (pAlbum->GetStyle() & WS_VISIBLE) != 0;
               //if(bVisible)
               //  pAlbum->ShowWindow(SW_HIDE);
               //}
               lpdata->bVisible = bVisible;*/
            }
            break;
         case 676869:
            {
               //ASSERT(FALSE);
               /*            CDBBuildAlbumThreadData * lpdata =
               (CDBBuildAlbumThreadData *)lParam;
               if(!lpdata->m_ptaskdlg->create(IDD_TASK))
               {
               System.simple_message_box("Could not create task dialog");
               return;
               }

               GetAlbumThread()->GetMainWnd() = lpdata->m_ptaskdlg;
               */
            }
         case 787970:
            {
               /*CDBBuildAlbumThreadData * lpdata =
               (CDBBuildAlbumThreadData *)lParam;
               if(m_pMixerDocTemplate != NULL)
               {
               if(lpdata->bVisible)
               {
               lpdata->lpDataCentral->m_csBuildAlbumThreadData.lock();
               lpdata->m_evStarted.lock();
               //lpdata->m_ptaskdlg->m_pCloseWindow = pAlbum;
               //            lpdata->m_ptaskdlg->m_lparamClose = 10;
               lpdata->lpDataCentral->m_csBuildAlbumThreadData.unlock();
               }
               }*/
            }
            break;
         case 345456:
            {

            }
            break;
         case 321432:
            {
               // On Task Dialog Stop
               //ASSERT(m_pdbcentral != NULL);
               //m_pdbcentral->SetBuildAlbumThreadsCancelFlag();
            }
            break;
         case 3214:
            {
               //OnChangeEconoMode();

            }
            break;
         case 534231:
            {
               //GetKarWnd()->SendMessageToDescendants(WM_USER, 534231);
            }
            break;

         default:;
            break;

         }

      }


      void thread::pre_translate_message(::message::message * pobj)
      {

         return ::thread::pre_translate_message(pobj);

      }


   } // namespace audio_mixer_mmsystem


} // namespace multimedia






