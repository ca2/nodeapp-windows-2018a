#include "framework.h"


namespace music
{


   namespace midi
   {


      sequence_thread::sequence_thread(sp(::ca2::application) papp) :
         ca2(papp),
         thread(papp),
         m_eventStop(papp)
      {
      }

      sequence_thread::~sequence_thread()
      {
      }

      bool sequence_thread::initialize_instance()
      {
         set_thread_priority(::ca2::scheduling_priority_highest);
         return true;
      }

      int32_t sequence_thread::exit_instance()
      {
         return thread::exit_instance();
      }

      void sequence_thread::install_message_handling(::ca2::message::dispatch * pinterface)
      {
         IGUI_WIN_MSG_LINK(MIDIPLAYERMESSAGE_COMMAND, pinterface, this, &sequence_thread::OnCommand);
         IGUI_WIN_MSG_LINK(MIDISEQUENCEMESSAGE_EVENT, pinterface, this, &sequence_thread::OnMidiSequenceEvent);
      }

      void sequence_thread::Stop(imedia::time msEllapse)
      {
         GetSequence()->Stop();
         m_eventStop.wait(millis(msEllapse));
      }

      sequence * sequence_thread::GetSequence()
      {
         return m_psequence;
      }

      bool sequence_thread::PostMidiSequenceEvent(sequence * pseq, sequence::e_event eevent)
      {
         return PostMidiSequenceEvent(pseq, eevent, (LPMIDIHDR) NULL);
      }

      bool sequence_thread::PostMidiSequenceEvent(sequence * pseq, sequence::e_event eevent, LPMIDIHDR lpmh)
      {
         sequence::Event * pevent = new sequence::Event;
         pevent->m_psequence  = pseq;
         pevent->m_eevent     = eevent;
         pevent->m_lpmh       = lpmh;
         return post_thread_message(MIDISEQUENCEMESSAGE_EVENT,  (WPARAM) pseq, (LPARAM) pevent);
      }

      void sequence_thread::OnMidiSequenceEvent(::ca2::signal_object * pobj)
      {
         SCAST_PTR(::ca2::message::base, pbase, pobj);

         sequence::Event * pevent = (sequence::Event *) pbase->m_lparam.m_lparam;
         sequence * pseq = (sequence *) pevent->m_psequence;

         pseq->OnEvent(pevent);

         switch(pevent->m_eevent)
         {
         case sequence::EventStopped:
            {
               sequence::PlayerLink & link = GetSequence()->GetPlayerLink();
               if(link.TestFlag(sequence::FlagStop))
               {
                  link.ModifyFlag(sequence::FlagNull, sequence::FlagStop);
                  link.OnFinishCommand(player::command_stop);
                  PostNotifyEvent(player::notify_event_playback_stop);

               }
               else if(link.TestFlag(sequence::FlagTempoChange))
               {
                  PrerollAndWait(link.m_tkRestart);
                  GetSequence()->SetTempoChangeFlag(false);
                  GetSequence()->Start();
                  link.ModifyFlag(sequence::FlagNull, sequence::FlagTempoChange);
               }
               else if(link.TestFlag(sequence::FlagSettingPos))
               {
                  link.ModifyFlag(sequence::FlagNull, sequence::FlagSettingPos);
                  try
                  {
                     PrerollAndWait(link.m_tkRestart);
                  }
                  catch(exception *pe)
                  {
                     //               SendMmsgDone(pseq, NULL);      
                     pe->Delete();
                     return;
                  }
                  GetSequence()->Start();
                  PostNotifyEvent(player::notify_event_position_set);
               }
               else if(link.TestFlag(
                  sequence::FlagMidiOutDeviceChange))
               {
                  link.ModifyFlag(sequence::FlagNull, sequence::FlagMidiOutDeviceChange);
                  try
                  {
                     PrerollAndWait(link.m_tkRestart);
                  }
                  catch(exception *pe)
                  {
                     //               SendMmsgDone(pseq, NULL);      
                     pe->Delete();
                     return;
                  }
                  GetSequence()->Start();
                  /*CNotifyEventData data;
                  data.m_pplayer = this;
                  data.m_enotifyevent = player::notify_event_position_set;
                  SendMessage(
                  m_oswindow_,
                  MIDIPLAYERMESSAGE_NOTIFYEVENT,
                  (WPARAM) &data,
                  0);      */

               }
               else if(link.TestFlag(sequence::FlagStopAndRestart))
               {
                  link.ModifyFlag(sequence::FlagNull, sequence::FlagStopAndRestart);
                  try
                  {
                     PrerollAndWait(link.m_tkRestart);
                  }
                  catch(exception *pe)
                  {
                     //               SendMmsgDone(pseq, NULL);      
                     pe->Delete();
                     return;
                  }
                  GetSequence()->Start();
                  //PostNotifyEvent(player::notify_event_position_set);
               }

            }
            break;
         case sequence::EventSpecialModeV001End:
            {
               PostNotifyEvent(player::notify_event_generic_mmsg_done);
            }
            break;
         case sequence::EventMidiPlaybackStart:
            {
               PostNotifyEvent(player::notify_event_playback_start);
            }
            break;
         case sequence::EventMidiStreamOut:
            {
               PostNotifyEvent(player::notify_event_midi_stream_out);
            }
            break;
         case sequence::EventMidiPlaybackEnd:
            {
               PostNotifyEvent(player::notify_event_playback_end);
            }
            break;

         }
         delete pevent;
      }

      void sequence_thread::PostNotifyEvent(player::e_notify_event eevent)
      {
         if(m_pplayer != NULL)
         {
            player::notify_event * pdata = new player::notify_event;
            pdata->m_enotifyevent = eevent;
            m_pplayer->post_thread_message(message_notify_event, 0, (LPARAM) pdata);      
         }
      }

      void sequence_thread::Play(imedia::position tkStart)
      {
         ASSERT(GetSequence() != NULL);
         ASSERT(GetSequence()->GetState() == sequence::StatusOpened);

         PrerollAndWait(tkStart);
         GetSequence()->Start();
      }

      void sequence_thread::Play(double dRate)
      {
         ASSERT(GetSequence() != NULL);
         ASSERT(GetSequence()->GetState() == sequence::StatusOpened);

         PrerollAndWait(dRate);
         GetSequence()->Start();
      }


      void sequence_thread::PrerollAndWait(imedia::position tkStart)
      {
         PREROLL                 preroll;

         preroll.tkBase = tkStart;
         preroll.tkEnd  = GetSequence()->m_tkLength;

         GetSequence()->SetMidiOutDevice(m_pplayer->GetMidiOutDevice());

         try
         {
            GetSequence()->Preroll(this, &preroll, true);
         }
         catch (exception * pme)
         {
            string str;
            ASSERT(FALSE);

            /* super merge module      CVmsMusDll::load_string(str, IDS_PREROLLUSERERROR001);
            pme->SetUserText(str);*/
            throw pme;
         }

      }


      void sequence_thread::PrerollAndWait(double dRate)
      {
         PREROLL                 preroll;

         ::ca2::math::math::MaxClip(&dRate, 1.0);
         ::ca2::math::math::MinClip(&dRate,  0.0);

         preroll.tkBase = (imedia::position) (int32_t) ((double) GetSequence()->m_tkLength * dRate);
         preroll.tkEnd  = GetSequence()->m_tkLength;

         GetSequence()->SetMidiOutDevice(m_pplayer->GetMidiOutDevice());

         try
         {
            GetSequence()->Preroll(this, &preroll, true);
         }
         catch (exception * pme)
         {
            throw not_implemented(get_app());
            /*string str;
            str.load_string(IDS_PREROLLUSERERROR001);
            pme->SetUserText(str);*/
            throw pme;
         }

         //    if(!GetSequence()->IsInSpecialModeV001())
         //  {
         //    SendMessage(m_oswindow_, WM_USER, 33, preroll.tkBase);
         //}
      }


      void sequence_thread::PostGMReset()
      {
         ASSERT(!GetSequence()->IsPlaying());
         GetSequence()->SetSpecialModeV001Operation(sequence::SPMV001GMReset);
         PrerollAndWait(0.0);
         GetSequence()->Start();

      }

      void sequence_thread::PostTempoChange()
      {
         ASSERT(!GetSequence()->IsPlaying());
         GetSequence()->SetSpecialModeV001Operation(sequence::SPMV001TempoChange);
         PrerollAndWait(0.0);
         GetSequence()->Start();
      }

      void sequence_thread::SendTempoChange()
      {
         ASSERT(!GetSequence()->IsPlaying());
         GetSequence()->m_evMmsgDone.ResetEvent();
         PostTempoChange();
         GetSequence()->m_evMmsgDone.wait();
      }


      void sequence_thread::ExecuteCommand(::ca::smart_pointer < player::command > spcommand)
      {
         spcommand->add_ref();
         post_thread_message(
            MIDIPLAYERMESSAGE_COMMAND,
            0,
            (LPARAM) (player::command *) spcommand);
      }


      void sequence_thread::OnCommand(::ca2::signal_object * pobj)
      {
         SCAST_PTR(::ca2::message::base, pbase, pobj);
         ::ca::smart_pointer < player::command > spcommand;
         spcommand = (player::command *) pbase->m_lparam.m_lparam;
         try
         {
            _ExecuteCommand(spcommand);
         }
         catch(exception * pe)
         {
            pe->Delete();
         }
         catch(...)
         {
         }
      }


      void sequence_thread::_ExecuteCommand(::ca::smart_pointer < player::command > spcommand)
      {
         switch(spcommand->GetCommand())
         {
         case player::command_play:
            {
               if(spcommand->m_flags.is_signalized(player::command::flag_dRate))
               {
                  Play(spcommand->m_dRate);
               }
               else if(spcommand->m_flags.is_signalized(player::command::flag_ticks))
               {
                  Play(spcommand->m_ticks);
               }
               else 
               {
                  Play();
               }
               spcommand->OnFinish();
            }
            break;
         case player::command_close_device:
            {
               if(GetSequence() != NULL)
               {
                  GetSequence()->CloseFile();
               }
               spcommand->OnFinish();
            }
            break;
         case player::command_stop:
            {
               m_eventStop.ResetEvent();
               ::multimedia::result            mmrc;
               sequence::PlayerLink & link = GetSequence()->GetPlayerLink();
               link.SetCommand(spcommand);
               link.ModifyFlag(sequence::FlagStop, sequence::FlagNull);
               if(MMSYSERR_NOERROR != (mmrc = GetSequence()->Stop()))
               {
                  throw new exception(get_app(), mmrc, MIDIPLAYERSTOPEXCEPTION);
               }
            }
            break;
         case player::command_stop_and_restart:
            {
               ::multimedia::result            mmrc;
               sequence::PlayerLink & link = GetSequence()->GetPlayerLink();
               link.SetCommand(spcommand);
               link.ModifyFlag(
                  sequence::FlagStopAndRestart,
                  sequence::FlagNull);
               link.m_tkRestart = GetSequence()->GetPositionTicks();
               if(MMSYSERR_NOERROR != (mmrc = GetSequence()->Stop()))
               {
                  throw new exception(get_app(), mmrc, MIDIPLAYERSTOPEXCEPTION);
               }
            }
            break;
         }
      }




   } // namespace midi


} // namespace music









