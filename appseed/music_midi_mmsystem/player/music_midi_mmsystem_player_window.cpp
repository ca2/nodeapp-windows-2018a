#include "framework.h"


namespace music
{


   namespace midi
   {


      namespace player
      {


         window::window(sp(::ca2::application) papp) :
            ca2(papp)
         {
            m_pcallback = NULL;
         }

         window::~window()
         {
         }

         void window::install_message_handling(::ca2::message::dispatch * pinterface)
         {
            ::user::interaction::install_message_handling(pinterface);
            IGUI_WIN_MSG_LINK(MMSG_DONE                        , pinterface, this, &window::_001OnMmsgDone);
            IGUI_WIN_MSG_LINK(player::MessageNotifyEvent   , pinterface, this, &window::_001OnNotifyEvent);
            IGUI_WIN_MSG_LINK(WM_USER                          , pinterface, this, &window::_001OnUserMessage);
            IGUI_WIN_MSG_LINK(WM_APP + 3388                    , pinterface, this, &window::_001OnApp3388Message);
         }

         void window::_001OnMmsgDone(::ca2::signal_object * pobj)
         {
            SCAST_PTR(::ca2::message::base, pbase, pobj);
            if(get_callback() != NULL)
            {
               ::music::midi::sequence * pseq = (::music::midi::sequence *) pbase->m_wparam;
               ::music::midi::LPMIDIDONEDATA lpmdd = (::music::midi::LPMIDIDONEDATA) pbase->m_lparam.m_lparam;


               get_callback()->OnMmsgDone(pseq, lpmdd);
               pbase->set_lresult(0);
            }
            else
            {
               pbase->set_lresult(-1);
            }
         }

         void window::_001OnNotifyEvent(::ca2::signal_object * pobj)
         {
            SCAST_PTR(::ca2::message::base, pbase, pobj);
            notify_event * pdata = (notify_event *) pbase->m_lparam.m_lparam;
            if(get_callback() != NULL)
            {
               get_callback()->OnMidiPlayerNotifyEvent(pdata);
               pbase->set_lresult(0);
            }
            else
            {
               pbase->set_lresult(-1);
            }
            if(pdata != NULL)
               delete pdata;
         }


         callback * window::get_callback()
         {
            return m_pcallback;
         }

         void window::set_callback(callback *pcallback)
         {
            m_pcallback = pcallback;
         }


         bool window::create()
         {
            if(!create_message_window("::ca2::::music::music::midiwindow"))
            {
               return false;
            }
            return true;
         }

         void window::_001OnUserMessage(::ca2::signal_object * pobj)
         {
            SCAST_PTR(::ca2::message::base, pbase, pobj);
            switch(pbase->m_wparam)
            {
            case 3388:
               {
                  // Lyric ::view Event 
                  if(get_callback() != NULL)
                  {
                     get_callback()->OnMidiLyricEvent((array<::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1&> *) pbase->m_lparam.m_lparam);
                     delete (array<::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1&> *) pbase->m_lparam.m_lparam;
                  }
               }
               break;
            }
            pbase->set_lresult(0);
         }

         void window::_001OnApp3388Message(::ca2::signal_object * pobj)
         {
            throw not_implemented(get_app());
            SCAST_PTR(::ca2::message::base, pbase, pobj);
/*            MSG msg;

            while(PeekMessage(
               &msg,
               get_handle(),
               WM_APP + 3388,
               WM_APP + 3388,
               PM_REMOVE))
            {
               if(pbase->m_wparam == 3388)
               {
                  delete (array<::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1&> * ) pbase->m_lparam.m_lparam;
               }

               pbase->m_wparam = msg.wParam;
               pbase->m_lparam = msg.lParam;
            }
            switch(pbase->m_wparam)
            {
            case 3388:
               {
                  // Lyric ::view Event 
                  if(get_callback() != NULL)
                  {
                     get_callback()->OnMidiLyricEvent((array<::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1&> *) pbase->m_lparam.m_lparam);
                     delete (array<::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1&> *) pbase->m_lparam.m_lparam;
                  }
               }
               break;
            }*/

            pbase->set_lresult(0);
         }


      } // namespace player


   } // namespace midi


} // namespace music









