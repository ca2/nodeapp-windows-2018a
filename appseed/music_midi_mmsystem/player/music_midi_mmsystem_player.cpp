#include "framework.h"
#include <math.h>


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         namespace player
         {


            player::player(::aura::application * papp) :
               ::object(papp),
               thread(papp),
               ::music::midi::player::player(papp)
            {

                  m_psequencethread = NULL;

               m_puie               = NULL;

            }

            player::~player()
            {

            }

            
            bool player::initialize_thread()
            {

               m_psequencethread->ResumeThread();

               TRACE("player::initialize_instance %X\n", get_os_int());
               //SetMainWnd(NULL);
               //ASSERT(GetMainWnd() == NULL);

               set_thread_priority(::multithreading::priority_normal);

               m_evInitialized.SetEvent();

               return true;
            }

            int32_t player::exit_thread()
            {
               // TODO:  perform any per-thread cleanup here
               //   if(!get_sequence()->IsNull())
               //   {
               //      delete get_sequence();
               //   }
               //    if(m_pmidicallbackdata != NULL)
               //  {
               //    delete m_pmidicallbackdata;
               ///  m_pmidicallbackdata = NULL;
               //}
               return thread::exit_thread();
            }

            void player::install_message_handling(::message::dispatch * pinterface)
            {
               ::music::midi::player::player::install_message_handling(pinterface);
               IGUI_WIN_MSG_LINK(MM_MOM_DONE, pinterface, this, &player::OnMultimediaMidiOutputMessageDone);
               IGUI_WIN_MSG_LINK(MM_MOM_POSITIONCB, pinterface, this, &player::OnMultimediaMidiOutputMessagePositionCB);
            }

            bool player::Play(imedia_position tkStart, uint32_t dwEllapse)
            {

               sp(::music::midi::player::command) pcommand = canew(::music::midi::player::command(get_app()));

               pcommand->m_ecommand = ::music::midi::player::command_play;

               if (dwEllapse != (uint32_t)-1)
               {

                  pcommand->m_dwEllapse = dwEllapse;

               }
               
               pcommand->m_flags.signalize(::music::midi::player::command::flag_ticks);
               pcommand->m_ticks = tkStart;

               //ASSERT(m_psequencethread->get_sequence() == get_sequence());

               //m_psequencethread->m_psequence = get_sequence();

               m_psequencethread->ExecuteCommand(pcommand);

               bool bFinished = pcommand->wait_ready();

//               pcommand->release();

               return bFinished && pcommand->m_eresult == ::multimedia::result_success;

            }


            bool player::Play(double dRate, uint32_t dwEllapse)
            {

               sp(::music::midi::player::command) pcommand = canew(::music::midi::player::command(get_app()));

               pcommand->m_ecommand = ::music::midi::player::command_play;
               if (dwEllapse != (uint32_t)-1)
               {

                  pcommand->m_dwEllapse = dwEllapse;

               }
               pcommand->m_flags.signalize(::music::midi::player::command::flag_dRate);
               pcommand->m_dRate = dRate;

               m_psequencethread->ExecuteCommand(pcommand);

               bool bFinished = pcommand->wait_ready();

               //pcommand->release();

               return bFinished && pcommand->m_eresult == ::multimedia::result_success;

            }

            imedia_position player::RateToTicks(double dRate)
            {
               ASSERT(dRate >= 0.0);
               ASSERT(dRate <= 1.0);
               return (imedia_position) (int32_t) (get_sequence()->m_tkLength * dRate);
            }



            bool player::IsPlaying()
            {
               ASSERT(!get_sequence()->IsNull());
               return (bool) get_sequence()->IsPlaying();

            }

            bool player::ExecuteCommand(::music::midi::player::e_command ecommand, uint32_t dwEllapse)
            {
               
               return ::music::midi::player::player::ExecuteCommand(ecommand, dwEllapse);

               /*
               ::music::midi::player::command * pcommand = new ::music::midi::player::command(get_app());

               pcommand->m_ecommand = ecommand;
               pcommand->m_dwEllapse = dwEllapse;

               m_psequencethread->ExecuteCommand(pcommand);

               bool bFinished = pcommand->wait_ready();

               pcommand->release();

               return bFinished;*/
            }


            void player::CloseFile()
            {

               e_result            mmrc;

               if(success != (mmrc = get_sequence()->CloseFile()) && mmrc != EFunctionNotSupported)
               {
                  
                  throw new exception(get_app(), EMidiPlayerClose);

               }

            }

            void player::Pause()
            {

               if (get_sequence()->GetState() == ::music::midi::sequence::status_paused)
               {

                  get_sequence()->Restart();

               }
               else
               {

                  get_sequence()->Pause();

               }

            }

            void player::SetPosition(double dRate)
            {
               if (::music::midi::sequence::status_playing != get_sequence()->GetState() &&
                  ::music::midi::sequence::status_stopping != get_sequence()->GetState() &&
                  ::music::midi::sequence::status_opened != get_sequence()->GetState())
                  return;

               if(get_sequence()->IsPlaying())
               {
                  ::music::midi::sequence::PlayerLink & link = get_sequence()->GetPlayerLink();
                  link.ModifyFlag(
                     ::music::midi::sequence::FlagSettingPos,
                     ::music::midi::sequence::FlagNull);
                  link.m_tkRestart = RateToTicks(dRate);
                  get_sequence()->Stop();
               }

            }

            void player::OnMmsgDone(::music::midi::sequence * pSeq)
            {
               ASSERT(FALSE);
               ASSERT(FALSE);
               ASSERT(FALSE);
               ASSERT(FALSE);
               ASSERT(FALSE);
               ASSERT(FALSE);
               ASSERT(FALSE);
               ASSERT(FALSE);
               ASSERT(pSeq == get_sequence());
               if(pSeq->IsSettingPosition())
               {
                  get_sequence()->SetSettingPositionFlag(FALSE);
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
                  get_sequence()->Start();
               }
               else
               {
               }

            }

            void player::pre_translate_message(::signal_details * pobj) 
            {
               SCAST_PTR(::message::base, pbase, pobj);
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
                  OnMmsgDone((::music::midi::sequence *) pbase->m_wparam);
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
               
               e_result            mmrc;

               if((mmrc = get_sequence()->SaveFile(lpszPathName)) != success)
               {

                  throw new exception(get_app(), EMidiPlayerSave);

               }

            }


            void player::OnUserMessage(::signal_details * pobj)
            {
               SCAST_PTR(::message::base, pbase, pobj);
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


            ::multimedia::e_result player::set_client(::music::midi::player::player_client * pclient)
            {

               m_pclient = pclient;

               get_sequence()->m_pthread   = m_psequencethread;

               m_psequencethread->m_psequence = get_sequence(); 

               m_psequencethread->m_pplayer = this; 

               PostNotifyEvent(::music::midi::player::notify_event_set_sequence);

               return ::multimedia::result_success;

            }


            bool player::SetMidiOutDevice(uint32_t uiDevice)
            {
               UNREFERENCED_PARAMETER(uiDevice);
               OnMidiOutDeviceChange();
               return true;
            }



            ::music::e_result player::SetTempoShift(double dTempoShift)
            {
               if(fabs(get_sequence()->GetTempoShift() - dTempoShift) >= 1.0)
               {
                  //   if(IsPlaying())
                  {/*
                   get_sequence()->SetTempoChangeFlag();
                   ::music::midi::sequence::PlayerLink & link = get_sequence()->GetPlayerLink();
                   link.ModifyFlag(
                   ::music::midi::sequence::FlagTempoChange,
                   ::music::midi::sequence::FlagNull);
                   imedia_position tk = get_sequence()->GetPositionTicks();
                   get_sequence()->m_evMmsgDone.ResetEvent();
                   link.m_tkRestart = tk + get_sequence()->m_tkBase;
                   //m_bChangingTempo = true;
                   get_sequence()->Stop();
                   //get_sequence()->m_evMmsgDone.lock();
                   */
                     bool bPlay = IsPlaying();
                     imedia_position ticks = 0;
                     if(bPlay)
                     {
                        ticks = get_sequence()->GetPositionTicks();
                        get_sequence()->Stop();
                     }
                     ::music::e_result eresult;
                     if(failed(eresult = get_sequence()->SetTempoShift(dTempoShift)))
                        return eresult;
                     if(bPlay)
                     {
                        get_sequence()->m_pthread->PrerollAndWait(ticks);
                        get_sequence()->Start();
                     }

                  }
               }
               return ::music::success;
            }


            void player::PostNotifyEvent(::music::midi::player::e_notify_event eevent)
            {

               if(m_puie != NULL)
               {

                  sp(::music::midi::player::notify_event) pdata(canew(::music::midi::player::notify_event));

                  pdata->m_pplayer = this;

                  pdata->m_enotifyevent = eevent;

                  m_puie->post_message(::music::midi::player::message_notify_event, 0 , pdata);      

               }

            }


            void player::SendMmsgDone(::music::midi::sequence *pSeq, ::music::midi::LPMIDIDONEDATA lpmdd)
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


            void player::SetCallbackUi(sp(::user::interaction) puie)
            {

               m_puie = puie;

            }


            void player::on_attribute_change(::signal_details * pobj)
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
               get_sequence()->SetMidiOutDevice(GetMidiOutDevice());
               if(get_sequence()->IsPlaying())
               {
                  imedia_position tkPosition = 0;
                  get_sequence()->GetPosition(tkPosition);
                  ::music::midi::sequence::PlayerLink & link = get_sequence()->GetPlayerLink();
                  link.ModifyFlag(
                     ::music::midi::sequence::FlagTempoChange,
                     ::music::midi::sequence::FlagNull);
                  link.m_tkRestart = tkPosition;
                  get_sequence()->Stop();
               }

            }


            void player::OnMultimediaMidiOutputMessageDone(::signal_details * pobj)
            {

               SCAST_PTR(::message::base, pbase, pobj);

               HMIDISTRM hmidistream = (HMIDISTRM) pbase->m_wparam;

               LPMIDIHDR lpmidihdr = (LPMIDIHDR) pbase->m_lparam.m_lparam;

               sp(sequence) sequence = get_sequence();

               ASSERT(sequence->m_hstream == hmidistream);

               sequence->OnDone(hmidistream, lpmidihdr);

            }

            void player::OnMultimediaMidiOutputMessagePositionCB(::signal_details * pobj)
            {
               SCAST_PTR(::message::base, pbase, pobj);
               LPMIDIHDR lpmidihdr = (LPMIDIHDR) pbase->m_wparam;
               //          get_sequence()->OnPositionCB(lpmidihdr);

               sp(sequence) sequence = get_sequence();

               //            ASSERT(sequence->m_hstream == hmidistream);

               sequence->OnPositionCB(lpmidihdr);
            }




            //void player::OnNotifyEvent(::signal_details * pobj)
            //{
            //   SCAST_PTR(::message::base, pbase, pobj);
            //   ::music::midi::player::notify_event * pdata = (::music::midi::player::notify_event *) pbase->m_lparam.m_lparam;
            //   pdata->m_pplayer = this;
            //   if(m_puie != NULL)
            //   {
            //      m_puie->post_message(::music::midi::player::message_notify_event, 0 , (LPARAM) pdata);      
            //   }
            //   else
            //   {
            //      delete pdata;
            //   }
            //}






         } // namespace player


      } // namespace mmsystem


   } // namespace midi_mmsystem


} // namespace music









