#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      namespace player
      {


         player::player(sp(::ca2::application) papp) :
            ca2(papp),
            thread(papp),
            m_eventPlaybackEnd(papp),
            m_evInitialized(papp)
         {

            m_psequencethread = __begin_thread < sequence_thread >(papp, ::ca2::scheduling_priority_normal, 0, CREATE_SUSPENDED);

            m_puie               = NULL;

         }

         player::~player()
         {

         }

         bool player::initialize_instance()
         {

            m_psequencethread->ResumeThread();

            TRACE("player::initialize_instance %X\n", get_os_int());
            //SetMainWnd(NULL);
            //ASSERT(GetMainWnd() == NULL);

            set_thread_priority(::ca2::scheduling_priority_normal);

            m_evInitialized.SetEvent();

            return true;
         }

         int32_t player::exit_instance()
         {
            // TODO:  perform any per-thread cleanup here
            //   if(!GetSequence().IsNull())
            //   {
            //      delete GetSequence();
            //   }
            //    if(m_pmidicallbackdata != NULL)
            //  {
            //    delete m_pmidicallbackdata;
            ///  m_pmidicallbackdata = NULL;
            //}
            return thread::exit_instance();
         }

         void player::install_message_handling(::ca2::message::dispatch * pinterface)
         {
            IGUI_WIN_MSG_LINK(MessageNotifyEvent, pinterface, this, &player::OnNotifyEvent);
            IGUI_WIN_MSG_LINK(MM_MOM_DONE, pinterface, this, &player::OnMultimediaMidiOutputMessageDone);
            IGUI_WIN_MSG_LINK(MM_MOM_POSITIONCB, pinterface, this, &player::OnMultimediaMidiOutputMessagePositionCB);
            IGUI_WIN_MSG_LINK(WM_USER, pinterface, this, &player::OnUserMessage);
         }

         bool player::Play(imedia::position tkStart, uint32_t dwEllapse)
         {

            command * pcommand = new command(get_app());

            pcommand->m_ecommand = command_play;
            pcommand->m_dwEllapse = dwEllapse;
            pcommand->m_flags.signalize(command::flag_ticks);
            pcommand->m_ticks = tkStart;

            m_psequencethread->ExecuteCommand(pcommand);

            bool bFinished = pcommand->wait_ready();

            pcommand->release();

            return bFinished;
         }

         bool player::Play(double dRate, uint32_t dwEllapse)
         {

            command * pcommand = new command(get_app());

            pcommand->m_ecommand = command_play;
            pcommand->m_dwEllapse = dwEllapse;
            pcommand->m_flags.signalize(command::flag_dRate);
            pcommand->m_dRate = dRate;

            m_psequencethread->ExecuteCommand(pcommand);

            bool bFinished = pcommand->wait_ready();

            pcommand->release();

            return bFinished;

         }

         imedia::position player::RateToTicks(double dRate)
         {
            ASSERT(dRate >= 0.0);
            ASSERT(dRate <= 1.0);
            return (imedia::position) (int32_t) (GetSequence().m_tkLength * dRate);
         }



         bool player::IsPlaying()
         {
            ASSERT(!GetSequence().IsNull());
            return (bool) GetSequence().IsPlaying();

         }

         bool player::ExecuteCommand(e_command ecommand, uint32_t dwEllapse)
         {

            command * pcommand = new command(get_app());

            pcommand->m_ecommand = ecommand;
            pcommand->m_dwEllapse = dwEllapse;

            m_psequencethread->ExecuteCommand(pcommand);

            bool bFinished = pcommand->wait_ready();

            pcommand->release();

            return bFinished;
         }


         void player::CloseFile()
         {
            ::multimedia::result            mmrc;
            if(MMSYSERR_NOERROR != (mmrc = GetSequence().CloseFile()) &&
               mmrc != MCIERR_UNSUPPORTED_FUNCTION)
            {
               throw new exception(get_app(), mmrc, MIDIPLAYERCLOSEEXCEPTION);
            }
         }

         void player::Pause()
         {
            if (GetSequence().GetState() == sequence::StatusPaused)
            {
               GetSequence().Restart();
            }
            else
            {
               GetSequence().Pause();
            }

         }

         void player::SetPosition(double dRate)
         {
            if (sequence::StatusPlaying != GetSequence().GetState() &&
               sequence::StatusStopping != GetSequence().GetState() &&
               sequence::StatusOpened != GetSequence().GetState())
               return;

            if(GetSequence().IsPlaying())
            {
               sequence::PlayerLink & link = GetSequence().GetPlayerLink();
               link.ModifyFlag(
                  sequence::FlagSettingPos,
                  sequence::FlagNull);
               link.m_tkRestart = RateToTicks(dRate);
               GetSequence().Stop();
            }

         }

         void player::OnMmsgDone(sequence * pSeq)
         {
            ASSERT(FALSE);
            ASSERT(FALSE);
            ASSERT(FALSE);
            ASSERT(FALSE);
            ASSERT(FALSE);
            ASSERT(FALSE);
            ASSERT(FALSE);
            ASSERT(FALSE);
            ASSERT(pSeq == &GetSequence());
            if(pSeq->IsSettingPosition())
            {
               GetSequence().SetSettingPositionFlag(FALSE);
               try
               {
                  //         PrerollAndWait(m_dNextPositionRate);
               }
               catch(exception *pe)
               {
                  SendMmsgDone(pSeq, NULL);
                  pe->Delete();
                  return;
               }
               GetSequence().Start();
            }
            else
            {
            }

         }

         void player::pre_translate_message(::ca2::signal_object * pobj) 
         {
            SCAST_PTR(::ca2::message::base, pbase, pobj);
            //ASSERT(GetMainWnd() == NULL);
            //   if(pMsg->message == MM_MOM_DONE ||
            //      pMsg->message == MM_MOM_POSITIONCB ||
            //      pMsg->message == MM_MOM_OPEN ||
            //      pMsg->message == MM_MOM_OPEN)
            //   {
            //      OnMidiOutMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
            //      return TRUE;
            //   }
            if(pbase->m_pwnd == NULL)
            {
               switch(pbase->m_uiMessage)
               {
               case WM_USER + 100:
                  {
                     LPDOUBLESTRUCT lpds = (LPDOUBLESTRUCT) pbase->m_wparam;
                     SetPosition(lpds->d);
                     delete lpds;
                  }
                  return;
               }
            }
            if(pbase->m_uiMessage == MMSG_DONE)
            {
               OnMmsgDone((sequence *) pbase->m_wparam);
               pbase->m_bRet = true;
               return;
            }
            else if(pbase->m_uiMessage == WM_USER)
            {
               //      OnUserMessage(pMsg->wParam, pMsg->lParam);
            }
            return thread::pre_translate_message(pobj);
         }

         void player::SaveFile(const char * lpszPathName)
         {
            ::multimedia::result            mmrc;
            if((mmrc = GetSequence().SaveFile(lpszPathName)) != 
               MMSYSERR_NOERROR)
            {
               throw new exception(get_app(), mmrc, MIDIPLAYERSAVEEXCEPTION);
            }

         }
         /*
         void player::OnMidiPlaybackEnd(
         WPARAM wParam, LPARAM lParam)
         {
         sequence * pSeq = (sequence *) wParam;
         LPMIDIHDR lpmh = (LPMIDIHDR) lParam;

         ASSERT(pSeq == GetSequence());

         m_eventPlaybackEnd.SetEvent();
         //    GetSequence().OnMidiPlaybackEnd(lpmh, m_pmidicallbackdata);
         GetSequence().OnEvent(sequence::EventMidiPlaybackEnd);
         }*/

         //void player::OnMidiSequenceEvent(::ca2::signal_object * pobj)
         //{
         /*   sequence::Event * pevent = (sequence::Event *) lparam;
         sequence * pseq = (sequence *) pevent->m_psequence;

         if(pevent->m_eevent == sequence::EventMidiPlaybackEnd)
         {
         m_eventPlaybackEnd.SetEvent();
         }

         pseq->OnEvent(pevent->m_eevent);

         switch(pevent->m_eevent)
         {
         case sequence::EventStopped:
         {
         sequence::PlayerLink & link = GetSequence().GetPlayerLink();
         if(link.TestFlag(
         sequence::FlagStop))
         {
         link.ModifyFlag(
         sequence::FlagNull,
         sequence::FlagStop);
         PostNotifyEvent(:music::midi:player::notify_event_playback_stop);
         }
         else if(link.TestFlag(
         sequence::FlagTempoChange))
         {
         PrerollAndWait(link.m_tkRestart);
         GetSequence().SetTempoChangeFlag(false);
         GetSequence().Start();
         link.ModifyFlag(
         sequence::FlagNull,
         sequence::FlagTempoChange);
         }
         else if(link.TestFlag(
         sequence::FlagSettingPos))
         {
         link.ModifyFlag(
         sequence::FlagNull,
         sequence::FlagSettingPos);
         try
         {
         PrerollAndWait(link.m_tkRestart);
         }
         catch(exception *pe)
         {
         SendMmsgDone(pseq, NULL);      
         pe->Delete();
         return;
         }
         GetSequence().Start();
         PostNotifyEvent(:music::midi:player::notify_event_position_set);
         }
         else if(link.TestFlag(
         sequence::FlagMidiOutDeviceChange))
         {
         link.ModifyFlag(
         sequence::FlagNull,
         sequence::FlagMidiOutDeviceChange);
         try
         {
         PrerollAndWait(link.m_tkRestart);
         }
         catch(exception *pe)
         {
         SendMmsgDone(pseq, NULL);      
         pe->Delete();
         return;
         }
         GetSequence().Start();
         /*:notify_event data;
         data.m_pplayer = this;
         data.m_enotifyevent = :music::midi:player::notify_event_position_set;
         SendMessage(
         m_oswindow_,
         MIDIPLAYERMESSAGE_NOTIFYEVENT,
         (WPARAM) &data,
         0);      */

         /*}
         else if(link.TestFlag(
         sequence::FlagStopAndRestart))
         {
         link.ModifyFlag(
         sequence::FlagNull,
         sequence::FlagStopAndRestart);
         try
         {
         PrerollAndWait(link.m_tkRestart);
         }
         catch(exception *pe)
         {
         SendMmsgDone(pseq, NULL);      
         pe->Delete();
         return;
         }
         GetSequence().Start();
         //PostNotifyEvent(:music::midi:player::notify_event_position_set);
         }

         }
         break;
         case sequence::EventSpecialModeV001End:
         {
         PostNotifyEvent(:music::midi:player::notify_event_generic_mmsg_done);
         }
         break;
         case sequence::EventMidiPlaybackEnd:
         {
         PostNotifyEvent(:music::midi:player::notify_event_playback_end);
         }
         break;

         }
         delete pevent;*/
         //}


         void player::OnUserMessage(::ca2::signal_object * pobj)
         {
            SCAST_PTR(::ca2::message::base, pbase, pobj);
            if(pbase->m_wparam == 3377)
            {
               m_puie->send_message(WM_USER, pbase->m_wparam, pbase->m_lparam);
            }

         }


         void player::PostGMReset()
         {
            m_psequencethread->PostGMReset();
         }

         void player::PostTempoChange()
         {
            m_psequencethread->PostTempoChange();
         }

         void player::SendTempoChange()
         {
            m_psequencethread->SendTempoChange();
         }


         VMSRESULT player::Initialize(thread * pthread)
         {
            UNREFERENCED_PARAMETER(pthread);
            return VMSR_SUCCESS;
         }

         //void player::SetView(CXfplayerView *pview)
         //{
         //    m_pView = pview;
         //}

         VMSRESULT player::SetInterface(player_interface * pinterface)
         {
            m_pinterface = pinterface;
            GetSequence().m_pthread   = m_psequencethread;
            GetSequence().m_midicallbackdata.m_psequence = &GetSequence(); 
            m_psequencethread->m_psequence = &GetSequence(); 
            m_psequencethread->m_pplayer = this; 
            PostNotifyEvent(notify_event_set_sequence);
            return VMSR_SUCCESS;
         }


         bool player::SetMidiOutDevice(uint32_t uiDevice)
         {
            UNREFERENCED_PARAMETER(uiDevice);
            OnMidiOutDeviceChange();
            return true;
         }



         bool player::SetTempoShift(int32_t iTempoShift)
         {
            //   if(IsPlaying())
            {/*
             GetSequence().SetTempoChangeFlag();
             sequence::PlayerLink & link = GetSequence().GetPlayerLink();
             link.ModifyFlag(
             sequence::FlagTempoChange,
             sequence::FlagNull);
             imedia::position tk = GetSequence().GetPositionTicks();
             GetSequence().m_evMmsgDone.ResetEvent();
             link.m_tkRestart = tk + GetSequence().m_tkBase;
             //m_bChangingTempo = true;
             GetSequence().Stop();
             //GetSequence().m_evMmsgDone.lock();
             */
               bool bPlay = IsPlaying();
               imedia::position ticks = 0;
               if(bPlay)
               {
                  ticks = GetSequence().GetPositionTicks();
                  GetSequence().Stop();
               }
               if(!GetSequence().SetTempoShift(iTempoShift))
                  return false;
               if(bPlay)
               {
                  GetSequence().m_pthread->PrerollAndWait(ticks);
                  GetSequence().Start();
               }

            }
            return true;
         }


         void player::PostNotifyEvent(e_notify_event eevent)
         {
            if(m_puie != NULL)
            {
               notify_event * pdata = new notify_event;
               pdata->m_pplayer = this;
               pdata->m_enotifyevent = eevent;
               m_puie->post_message(MessageNotifyEvent, 0 , (LPARAM) pdata);      
            }
         }

         void player::SendMmsgDone(sequence *pSeq, LPMIDIDONEDATA lpmdd)
         {
            if(m_puie != NULL)
            {
               m_puie->post_message(MMSG_DONE, (WPARAM) pSeq, (LPARAM) lpmdd);      
            }

         }

         uint32_t player::GetMidiOutDevice()
         {
            
            return Application.midi()->GetMidiOutDevice();
         }

         void player::SetCallbackWindow(sp(::user::interaction) puie)
         {
            m_puie = puie;
         }

         void player::on_attribute_change(::ca2::signal_object * pobj)
         {
            SCAST_PTR(::music::midi::attribute_message, pchange, pobj);

            switch(pchange->m_eattribute)
            {
            case ::music::midi::attribute_midi_out_device:
               OnMidiOutDeviceChange();
               break;
            }

         }

         void player::OnMidiOutDeviceChange()
         {
            GetSequence().SetMidiOutDevice(GetMidiOutDevice());
            if(GetSequence().IsPlaying())
            {
               imedia::position tkPosition = 0;
               GetSequence().get_position(tkPosition);
               sequence::PlayerLink & link = GetSequence().GetPlayerLink();
               link.ModifyFlag(
                  sequence::FlagTempoChange,
                  sequence::FlagNull);
               link.m_tkRestart = tkPosition;
               GetSequence().Stop();
            }

         }


         void player::OnMultimediaMidiOutputMessageDone(::ca2::signal_object * pobj)
         {
            SCAST_PTR(::ca2::message::base, pbase, pobj);
            HMIDISTRM hmidistream = (HMIDISTRM) pbase->m_wparam;
            LPMIDIHDR lpmidihdr = (LPMIDIHDR) pbase->m_lparam.m_lparam;
            ASSERT(GetSequence().m_hMidiStream == hmidistream);
            GetSequence().OnDone(hmidistream, lpmidihdr);
         }

         void player::OnMultimediaMidiOutputMessagePositionCB(::ca2::signal_object * pobj)
         {
            SCAST_PTR(::ca2::message::base, pbase, pobj);
            LPMIDIHDR lpmidihdr = (LPMIDIHDR) pbase->m_wparam;
            GetSequence().OnPositionCB(lpmidihdr);
         }




         void player::OnNotifyEvent(::ca2::signal_object * pobj)
         {
            SCAST_PTR(::ca2::message::base, pbase, pobj);
            notify_event * pdata = (notify_event *) pbase->m_lparam.m_lparam;
            pdata->m_pplayer = this;
            if(m_puie != NULL)
            {
               m_puie->post_message(MessageNotifyEvent, 0 , (LPARAM) pdata);      
            }
            else
            {
               delete pdata;
            }
         }




         void player::SendReset()
         {
            HMIDIOUT hmidiout = NULL;
            ::multimedia::result mmrc;
            uint32_t uDeviceID = 0;
            mmrc = midiOutOpen(&hmidiout, uDeviceID,  0, 0, CALLBACK_NULL);
            if(mmrc != MMSYSERR_NOERROR)
               return;
            Sleep(284);
            const uchar gmModeOn[] = {
               //        0x00, 0x00, 0x00, 0x00,
               //        0x00, 0x00, 0x00, 0x00,
               //        0x1b, 0x8a, 0x06, MEVT_TEMPO,
               0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00,
               0x06, 0x00, 0x00, MEVT_LONGMSG,
               0xf0, 0x7e, 0x7f, 0x09,
               0x01, 0xf7, 0x00, 0x00};
            //        0x70, 0x01, 0x00, 0x00,
            //      0x00, 0x00, 0x00, 0x00 };
            //      0x09, 0x00, 0x00, MEVT_LONGMSG,
            //      0x43, 0x10, 0x4c,
            //    0x00, 0x00, 0x7e, 0x00,
            //  0xf7, 0x00, 0x00, 0x00,};
            MIDIHDR mh;
            LPMIDIHDR lpmh = &mh;
            lpmh->lpData = (char *) gmModeOn;
            lpmh->dwBufferLength = sizeof(gmModeOn);
            lpmh->dwBytesRecorded = 0;
            lpmh->dwFlags = 0;
            mmrc = midiOutPrepareHeader( hmidiout, lpmh, sizeof(MIDIHDR));
            if(mmrc != MMSYSERR_NOERROR)
               goto End;
            lpmh->dwBytesRecorded = sizeof(gmModeOn);
            if(mmrc != MMSYSERR_NOERROR)
               goto End;
            mmrc = midiOutLongMsg( hmidiout, lpmh, sizeof(MIDIHDR));
            Sleep(284);
            mmrc = midiOutUnprepareHeader( hmidiout, lpmh, sizeof(MIDIHDR));
            if(mmrc != MMSYSERR_NOERROR)
               goto End;
End:
            midiOutClose( hmidiout);
         }

         sequence & player::GetSequence()
         {
            return m_pinterface->get_midi_sequence();
         }


      } // namespace player


   } // namespace midi_mmsystem


} // namespace music









