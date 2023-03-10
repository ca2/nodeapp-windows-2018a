#include "framework.h"


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         sequence_thread::sequence_thread(::aura::application * papp) :
            ::object(papp),
            thread(papp),
            ::music::midi::sequence_thread(papp)
         {
         }

         sequence_thread::~sequence_thread()
         {
         }


         bool sequence_thread::init_thread()
         {

            set_thread_priority(::multithreading::priority_highest);

            return true;

         }


         void sequence_thread::term_thread()
         {

            thread::term_thread();

         }


         void sequence_thread::install_message_routing(::message::sender * pinterface)
         {
            ::music::midi::sequence_thread::install_message_routing(pinterface);
            //IGUI_MSG_LINK(::music::midi::player::message_command, pinterface, this, &sequence_thread::OnCommand);
            IGUI_MSG_LINK(::music::midi::sequence::message_event, pinterface, this, &sequence_thread::OnMidiSequenceEvent);
            IGUI_MSG_LINK(MM_MOM_DONE, pinterface, this, &sequence_thread::OnDone);
            IGUI_MSG_LINK(MM_MOM_POSITIONCB, pinterface, this, &sequence_thread::OnPositionCB);
         }

         void sequence_thread::Stop(imedia_time msEllapse)
         {
            get_sequence()->Stop();
            m_eventStop.wait(millis(msEllapse));
         }

         ::music::midi::sequence * sequence_thread::get_sequence()
         {
            return m_psequence;
         }


         bool sequence_thread::PostMidiSequenceEvent(::music::midi::sequence * pseq, ::music::midi::sequence::e_event eevent)
         {

            return post_object(::music::midi::sequence::message_event,  (WPARAM) pseq, pseq->create_new_event(eevent));

         }


         bool sequence_thread::PostMidiSequenceEvent(::music::midi::sequence * pseq, ::music::midi::sequence::e_event eevent, LPMIDIHDR lpmh)
         {

            sp(sequence) seq = pseq;

            return post_object(::music::midi::sequence::message_event,  (WPARAM) pseq, seq->create_new_event(eevent, lpmh));

         }

         void sequence_thread::OnMidiSequenceEvent(::message::message * pobj)
         {

            SCAST_PTR(::message::base, pbase, pobj);

            sp(::music::midi::sequence::event) pevent(pbase->m_lparam);
            ::music::midi::mmsystem::sequence * pseq = dynamic_cast < ::music::midi::mmsystem::sequence * > (pevent->m_psequence);

            pseq->OnEvent(pevent);

            switch(pevent->m_eevent)
            {
            case ::music::midi::sequence::EventMidiPlaybackEnd:
            {

               ::music::midi::sequence::PlayerLink & link = get_sequence()->GetPlayerLink();

               if(link() & ::music::midi::sequence::FlagTempoChange)
               {
                  PrerollAndWait(link.m_tkRestart);
                  get_sequence()->SetTempoChangeFlag(false);
                  get_sequence()->Start();
                  link() -=  ::music::midi::sequence::FlagTempoChange;
               }
               else if(link()  & ::music::midi::sequence::FlagSettingPos)
               {
                  link() -= ::music::midi::sequence::FlagSettingPos;
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
                  get_sequence()->Start();
                  PostNotifyEvent(::music::midi::player::notify_event_position_set);
               }
               else if(link() & ::music::midi::sequence::FlagMidiOutDeviceChange)
               {
                  link() -= ::music::midi::sequence::FlagMidiOutDeviceChange;
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
                  get_sequence()->Start();
                  /*CNotifyEventData data;
                  data.m_pplayer = this;
                  data.m_enotifyevent = player::notify_event_position_set;
                  SendMessage(
                  m_oswindow_,
                  MIDIPLAYERMESSAGE_NOTIFYEVENT,
                  (WPARAM) &data,
                  0);      */

               }
               else if(link() & ::music::midi::sequence::FlagStopAndRestart)
               {
                  link() -= ::music::midi::sequence::FlagStopAndRestart;
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
                  get_sequence()->Start();
                  //PostNotifyEvent(player::notify_event_position_set);
               }
               else // if (link.TestFlag(::music::midi::sequence::FlagStop))
               {

                  bool bStopWasSetRemarkJustForDebugging = link() & ::music::midi::sequence::FlagStop;

                  link() -= ::music::midi::sequence::FlagStop;

                  link.OnFinishCommand(::music::midi::player::command_stop);

                  PostNotifyEvent(::music::midi::player::notify_event_playback_end);

               }

            }
            break;
            case ::music::midi::sequence::EventSpecialModeV001End:
            {
               PostNotifyEvent(::music::midi::player::notify_event_generic_mmsg_done);
            }
            break;
            case ::music::midi::sequence::EventMidiPlaybackStart:
            {

               pseq->mm_start();


               PostNotifyEvent(::music::midi::player::notify_event_playback_start);
            }
            break;
            case ::music::midi::sequence::EventMidiStreamOut:
            {
               PostNotifyEvent(::music::midi::player::notify_event_midi_stream_out);
            }
            break;
            }

         }


         void sequence_thread::PostNotifyEvent(::music::midi::player::e_notify_event eevent)
         {

            if(m_pplayer != NULL)
            {

               sp(::music::midi::player::notify_event) pdata = canew(::music::midi::player::notify_event);

               pdata->m_enotifyevent = eevent;

               m_pplayer->post_object(::music::midi::player::message_notify_event, 0, pdata);

            }

         }


         void sequence_thread::Play(imedia_position tkStart)
         {
            ASSERT(get_sequence() != NULL);
            ASSERT(get_sequence()->get_status() == ::music::midi::sequence::status_opened);

            PrerollAndWait(tkStart);
            get_sequence()->Start();
         }


         void sequence_thread::PlayRate(double dRate)
         {

            ASSERT(get_sequence() != NULL);

            ASSERT(get_sequence()->get_status() == ::music::midi::sequence::status_opened);

            PrerollRateAndWait(dRate);

            get_sequence()->Start();

         }


         void sequence_thread::PrerollAndWait(imedia_position tkStart)
         {

            ::music::midi::PREROLL                 preroll;

            preroll.tkBase = tkStart;
            preroll.tkEnd  = get_sequence()->m_tkLength;

            get_sequence()->SetMidiOutDevice(m_pplayer->GetMidiOutDevice());

            try
            {
               get_sequence()->Preroll(this, &preroll, true);
            }
            catch (exception * pme)
            {
               string str;
               //ASSERT(FALSE);

               /* super merge module      CVmsMusDll::load_string(str, IDS_PREROLLUSERERROR001);
               pme->SetUserText(str);*/
               throw pme;
            }

         }


         void sequence_thread::PrerollRateAndWait(double dRate)
         {
            ::music::midi::PREROLL                 preroll;

            ::math::math::MaxClip(&dRate, 1.0);
            ::math::math::MinClip(&dRate,  0.0);

            preroll.tkBase = (imedia_position) (int32_t) ((double) get_sequence()->m_tkLength * dRate);
            preroll.tkEnd  = get_sequence()->m_tkLength;

            get_sequence()->SetMidiOutDevice(m_pplayer->GetMidiOutDevice());

            try
            {
               get_sequence()->Preroll(this, &preroll, true);
            }
            catch (exception * pme)
            {
               throw not_implemented(get_app());
               /*string str;
               str.load_string(IDS_PREROLLUSERERROR001);
               pme->SetUserText(str);*/
               throw pme;
            }

            //    if(!get_sequence()->IsInSpecialModeV001())
            //  {
            //    SendMessage(m_oswindow_, WM_USER, 33, preroll.tkBase);
            //}
         }


         void sequence_thread::PostGMReset()
         {
            ASSERT(!get_sequence()->IsPlaying());
            get_sequence()->SetSpecialModeV001Operation(::music::midi::sequence::SPMV001GMReset);
            PrerollAndWait(0.0);
            get_sequence()->Start();

         }

         void sequence_thread::PostTempoChange()
         {
            ASSERT(!get_sequence()->IsPlaying());
            get_sequence()->SetSpecialModeV001Operation(::music::midi::sequence::SPMV001TempoChange);
            PrerollAndWait(0.0);
            get_sequence()->Start();
         }

         void sequence_thread::SendTempoChange()
         {
            ASSERT(!get_sequence()->IsPlaying());
            get_sequence()->m_evMmsgDone.ResetEvent();
            PostTempoChange();
            get_sequence()->m_evMmsgDone.wait();
         }


         void sequence_thread::ExecuteCommand(::music::midi::player::command * spcommand)
         {

            post_object(::music::midi::player::message_command, 0, spcommand);

         }


         //void sequence_thread::OnCommand(::message::message * pobj)
         //{
         //   SCAST_PTR(::message::base, pbase, pobj);
         //   smart_pointer < ::music::midi::player::command > spcommand;
         //   spcommand = (::music::midi::player::command *) pbase->m_lparam.m_lparam;
         //   spcommand->m_eresult = ::multimedia::result_success;
         //   try
         //   {
         //      _ExecuteCommand(spcommand);
         //   }
         //   catch(exception * pe)
         //   {
         //      pe->Delete();
         //      spcommand->m_eresult = ::multimedia::result_error;
         //   }
         //   catch(...)
         //   {
         //      spcommand->m_eresult = ::multimedia::result_error;
         //   }
         //
         //   spcommand->OnFinish();

         //}


         void sequence_thread::_ExecuteCommand(::music::midi::player::command * spcommand)
         {
            switch(spcommand->GetCommand())
            {
            case ::music::midi::player::command_play:
            {
               if(spcommand->m_flags.is_signalized(::music::midi::player::command::flag_dRate))
               {
                  PlayRate(spcommand->m_dRate);
               }
               else if(spcommand->m_flags.is_signalized(::music::midi::player::command::flag_ticks))
               {
                  Play(spcommand->m_ticks);
               }
               else
               {
                  PlayRate();
               }
            }
            break;
            case ::music::midi::player::command_close_file:
            {

               if(get_sequence() != NULL)
               {

                  get_sequence()->close_file();

               }

            }
            break;
            case ::music::midi::player::command_close_device:
            {

               if(get_sequence() != NULL)
               {

                  get_sequence()->close_device();

               }

            }
            break;
            case ::music::midi::player::command_fade_out_and_stop:
            {
               m_eventStop.ResetEvent();
               ::multimedia::e_result            mmrc;
               sequence::PlayerLink & link = get_sequence()->GetPlayerLink();
               link.SetCommand(spcommand);
               link() |= sequence::FlagFadeOutAndStop;
               if (::multimedia::result_success != (mmrc = get_sequence()->FadeOutAndStop()))
               {
                  _throw(exception(get_app(), ::music::EMidiPlayerStop, mmrc));
               }
            }
            break;
            case ::music::midi::player::command_stop:
            {
               m_eventStop.ResetEvent();
               ::multimedia::e_result            mmrc;
               ::music::midi::sequence::PlayerLink & link = get_sequence()->GetPlayerLink();
               link.SetCommand(spcommand);
               link() |= ::music::midi::sequence::FlagStop;
               if(MMSYSERR_NOERROR != (mmrc = get_sequence()->Stop()))
               {
                  throw new exception(get_app(), EMidiPlayerStop);
               }
            }
            break;
            case ::music::midi::player::command_stop_and_restart:
            {
               ::multimedia::e_result            mmrc;
               ::music::midi::sequence::PlayerLink & link = get_sequence()->GetPlayerLink();
               link.SetCommand(spcommand);
               link() |= ::music::midi::sequence::FlagStopAndRestart;

               link.m_tkRestart = get_sequence()->get_position_ticks();

               if(::multimedia::result_success != (mmrc = get_sequence()->Stop()))
               {
                  throw new exception(get_app(), EMidiPlayerStop);
               }
            }
            break;
            }
         }


         void sequence_thread::OnDone(::message::message * pobj)
         {

            SCAST_PTR(::message::base, pbase, pobj);

            if (get_sequence() == NULL)
               return;

            sp(sequence) pseq = get_sequence();

            HMIDISTRM hmidistrm = (HMIDISTRM) pbase->m_wparam;

            LPMIDIHDR lpmidihdr = pbase->m_lparam.cast < MIDIHDR >();

            pseq->OnDone(hmidistrm, lpmidihdr);

         }


         void sequence_thread::OnPositionCB(::message::message * pobj)
         {

            SCAST_PTR(::message::base, pbase, pobj);

            sp(sequence) pseq = get_sequence();

            LPMIDIHDR lpmidihdr = (LPMIDIHDR) pbase->m_wparam;

            pseq->OnPositionCB(lpmidihdr);

         }


      } // namespace mmsystem


   } // namespace midi


} // namespace music









