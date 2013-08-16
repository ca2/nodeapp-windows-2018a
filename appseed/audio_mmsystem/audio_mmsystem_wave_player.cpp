#include "framework.h"


namespace audio
{


   wave_player::wave_player(sp(::ca2::application) papp) :
      ca2(papp),
      thread(papp),
      m_decoderset(papp)
   {
      m_pdecoder           = NULL;
      m_pwaveout           = NULL;
      m_oswindowCallback       = NULL;
      m_estate             = state_initial;
   }

   wave_player::~wave_player()
   {

   }

   void wave_player::install_message_handling(::ca2::message::dispatch * pinterface)
   {
      IGUI_WIN_MSG_LINK(audMessageCommand, pinterface, this, &wave_player::OnaudCommandMessage);
   }

   bool wave_player::DecoderOpen(wave_player_command & command)
   {

      if(DecoderIsActive())
      {
         DecoderClose();
      }

      string strDecoder;
      if(command.GetCommand() == command_open_decoder)
      {
         m_iOutBufferSampleCount =  8 * 1024;
         m_pdecoder = command.m_pdecoderOpen;
      }
      else if(command.GetCommand() == command_open_file)
      {
         if(command.m_bSeekable)
         {
            m_iOutBufferSampleCount =  48 * 1024;
            m_pdecoder = m_decoderset.GetNewDecoder(NULL, command.GetOpenFile(), command.m_bSeekable);
         }
         else
         {
            m_iOutBufferSampleCount =  1024;
            m_pdecoder = m_decoderset.GetNewDecoder(NULL, command.GetOpenFile(), command.m_bSeekable, 1024);
         }
      }
      else
      {
         switch(command.GetCommand())
         {
         case command_open_file_aiff:
            //m_pdecoder = new _vmsaiffdec::decoder();
            return false;
            break;
         case command_open_file_cda:
            {
               strDecoder = "audio_decode_cd.dll";
            }
            break;
         case command_open_file_mp3:
         case command_open_file_rtp:
            {
               strDecoder = "audio_decode_libmpg123.dll";
            }
            break;
         case command_open_file_vqf:
            //m_pdecoder = new _vmstwinvqdec::decoder();
            return false;
            break;
         case command_open_file_wm:
            {
               strDecoder = "audio_decode_window_media.dll";
            }
            break;
         case command_open_file_wav:
            {
               strDecoder = "audio_decode_wave.dll";
            }
            break;
         default:
            return false;
            break;
         }
         m_iOutBufferSampleCount =  48 * 1024;

         audio_decode::decoder_plugin * pplugin = m_decoderset.LoadPlugin(strDecoder);
         if(pplugin == NULL)
            return false;

         m_pdecoder = pplugin->NewDecoder();
         if(m_pdecoder != NULL)
         {
            if(command.GetCommand() == command_open_file_rtp)
            {
               if(m_pdecoder->DecoderSetSeekable(false))
               {
                  delete m_pdecoder;
                  return false;
               }
               if(!m_pdecoder->DecoderSetRealTime())
               {
                  delete m_pdecoder;
                  return false;
               }
               if(m_pdecoder->DecoderSetReadBlockSize(1024) != 1024)
               {
                  delete m_pdecoder;
                  return false;
               }
               m_iOutBufferSampleCount = 2 * ((44100 * (1024 * 8 * 2 * 2 * 4 / 128)) /  1000); // samples are samples, bytes are bytes
               // each sample may have e.g. n channels * sizeof(int16_t) bytes
            }
            if(!m_pdecoder->DecoderInitialize(command.GetOpenFile(), false))
            {
               delete m_pdecoder;
               return false;
            }
         }
      }

      return m_pdecoder != NULL;

   }


   void wave_player::DecoderInitialize(sp(::ca2::file)pfile)
   {
      UNREFERENCED_PARAMETER(pfile);
      //   ASSERT(DecoderIsActive());
      //spfile->seek_to_begin();
      //m_pdecoder->DecoderInitialize(pfile);
   }

   bool wave_player::DecoderIsActive()
   {
      return m_pdecoder != NULL;
   }

   void wave_player::DecoderClose()
   {

      if(m_pdecoder != NULL)
      {
         try
         {
            m_pdecoder->DecoderFinalize();
         }
         catch(...)
         {
         }
      }

      ::ca::release(m_pdecoder.m_p);

   }

   void wave_player::DecoderRun()
   {
      //   m_pdecoder->DecoderRun();
   }


   bool wave_player::initialize_instance()
   {
   #ifdef WINDOWS
      CoInitialize(NULL);
#endif
      m_pwaveout = __begin_thread < wave_out >(get_app(), ::ca2::scheduling_priority_above_normal);
      if(m_pwaveout == NULL)
         return false;

      m_pwaveout->m_pplayer = this;

      return true;
   }

   int32_t wave_player::exit_instance()
   {
   #ifdef WINDOWS
      CoUninitialize();
#endif
      if(m_pwaveout != NULL)
      {
         event ev(get_app());
         m_pwaveout->AttachEndEvent(&ev);
         m_pwaveout->post_thread_message(WM_QUIT);
         ev.wait(seconds(60));
      }

      DecoderClose();

      return thread::exit_instance();
   }



   void wave_player::ExecuteCommand(wave_player_command & command)
   {
      switch(command.GetCommand())
      {
      case command_execute_stop:
      case command_close_device:
         {
            audCommandMessageProcedure(command);
         }
         break;
      default:
         {
            post_thread_message(audMessageCommand, (WPARAM) command.GetCommand(), (LPARAM) (new wave_player_command(command)));
         }
      }

   }

   wave_player_command::wave_player_command()
   {
      m_pbResult = NULL;
      m_bSeekable = true;
   }

   wave_player_command::wave_player_command(const wave_player_command & command)
   {
      operator =(command);
   }

   wave_player_command & wave_player_command::operator =(const wave_player_command & command)
   {
      SetCommand(command.GetCommand());
      SetOpenFile(command.GetOpenFile());
      SetOpenDecoder(command.m_pdecoderOpen);
      m_bResult      = command.m_bResult;
      m_pbResult     = command.m_pbResult;
      m_evptra       = command.m_evptra;
      m_bSeekable    = command.m_bSeekable;
      m_position     = command.m_position;
      return *this;

   }

   e_command wave_player_command::GetCommand() const
   {
      return m_ecommand;
   }

   void wave_player_command::SetCommand(e_command ecommand)
   {
      m_ecommand = ecommand;
   }

   sp(::ca2::file) wave_player_command::GetOpenFile() const
   {
      return m_pfileOpen;
   }

   void wave_player_command::SetOpenFile(sp(::ca2::file) pfile)
   {
      m_pfileOpen = pfile;
   }

   void wave_player_command::SetOpenDecoder(::audio_decode::decoder * pdecoder)
   {
      m_pdecoderOpen = pdecoder;
   }

   void wave_player_command::OpenDecoder(::audio_decode::decoder * pdecoder)
   {
      SetCommand(command_open_decoder);
      SetOpenDecoder(pdecoder);
   }

   void wave_player_command::OpenRtpFile(sp(::ca2::file) pfile)
   {
      SetCommand(command_open_file_rtp);
      SetOpenFile(pfile);
   }

   void wave_player_command::OpenMp3File(sp(::ca2::file) pfile)
   {
      SetCommand(command_open_file_mp3);
      SetOpenFile(pfile);
   }

   void wave_player_command::OpenAiffFile(sp(::ca2::file) pfile)
   {
      SetCommand(command_open_file_aiff);
      SetOpenFile(pfile);
   }

   void wave_player_command::OpenCdaFile(sp(::ca2::file) pfile)
   {
      SetCommand(command_open_file_cda);
      SetOpenFile(pfile);
   }

   void wave_player_command::OpenVqfFile(sp(::ca2::file)pfile)
   {
      SetCommand(command_open_file_vqf);
      SetOpenFile(pfile);
   }

   void wave_player_command::OpenWmFile(sp(::ca2::file) pfile)
   {
      SetCommand(command_open_file_wm);
      SetOpenFile(pfile);
   }

   void wave_player_command::OpenWavFile(sp(::ca2::file) pfile)
   {
      SetCommand(command_open_file_wav);
      SetOpenFile(pfile);
   }

   void wave_player_command::OpenFile(sp(::ca2::file) pfile, bool bSeekable)
   {
      SetCommand(command_open_file);
      SetOpenFile(pfile);
      m_bSeekable = bSeekable;
   }

   void wave_player_command::Play(const imedia::position & position)
   {
      SetCommand(command_execute_play);
      m_position = position;
   }

   void wave_player::OnaudCommandMessage(::ca2::signal_object * pobj)
   {
      //WPARAM wparam, // audCommand enumeration
      //LPARAM lparam  // audCommand object pointer
      SCAST_PTR(::ca2::message::base, pbase, pobj);
      wave_player_command * pcommand = (wave_player_command *) pbase->m_lparam.m_lparam;
      audCommandMessageProcedure(*pcommand);
      if(pcommand->m_pbResult != NULL)
      {
         *pcommand->m_pbResult = pcommand->m_bResult;
      }
      delete pcommand;
   }

   bool wave_player::audCommandMessageProcedure(wave_player_command &command)
   {
      command.m_bResult = true;
      e_command ecommand = command.GetCommand();
      switch(ecommand)
      {
      case command_open_decoder:
      case command_open_file_cda:
      case command_open_file_mp3:
      case command_open_file_rtp:
      case command_open_file_aiff:
      case command_open_file_vqf:
      case command_open_file_wm:
      case command_open_file_wav:
      case command_open_file:
         {

            if(IsPlaying())
            {
               return true;
            }

            if(DecoderIsActive())
            {
               DecoderClose();
            }

            if(!DecoderOpen(command))
            {
               command.m_bResult = false;
               break;
            }

            if(!DecoderIsActive())
            {
               command.m_bResult = false;
               break;
            }

            m_pwaveout->SetDecoder(m_pdecoder);

            OnEvent(EventOpenDecoder);

         }
         break;
      case command_execute_play:
         {
            if(m_commandlistStopOpen.get_count() > 0)
            {
               if(IsPlaying())
               {
                  m_commandlistOpenPlay.add_tail(command);
                  return true;
               }
               else
               {
                  m_commandlistStopOpen.remove_all();
               }
            }
            if(m_pdecoder != NULL)
            {
               if(0 != m_pwaveout->OpenEx(
                  this,
                  16,
                  m_iOutBufferSampleCount,
                  m_pdecoder->DecoderGetSamplesPerSecond(),
                  m_pdecoder->DecoderGetChannelCount(),
                  m_pdecoder->DecoderGetBitsPerSample()))
               {
                  System.simple_message_box_timeout(NULL, "Could not play the spfile->", 5000, MB_OK | MB_ICONEXCLAMATION);
                  command.m_bResult = false;
                  break;
               }
               OnEvent(EventOpenDevice);
               m_pwaveout->Start(command.m_position);
               OnEvent(EventPlay);
            }
            else
            {
               command.m_bResult = false;
               break;
            }
         }
         break;
      case command_execute_stop:
         {
            FadeOutAndStop();
         }
         break;
      case command_execute_pause:
         {
            if(m_commandlistStopOpen.get_count() > 0)
            {
               return false;
            }
            m_pwaveout->Pause();
         }
         break;
      case command_execute_restart:
         {
            if(m_commandlistStopOpen.get_count() > 0)
            {
               return false;
            }
            m_pwaveout->Restart();
         }
         break;
      case command_close_device:
         {
            m_pwaveout->close();
            OnEvent(EventCloseDevice);
         }
         break;
      default:
         ASSERT(FALSE);
         break;
      }
      command.SetEvents();
      return command.m_bResult;
   }


   void wave_player::AttachEndEvent(event *pevent)
   {
      if(m_pwaveout)
      {
         m_pwaveout->AttachEndEvent(pevent);
      }
   }

   void wave_player_command::AttachEvent(event *pevent)
   {
      m_evptra.add(pevent);
   }

   void wave_player_command::SetEvents()
   {
      for(int32_t i = 0;  i < m_evptra.get_size(); i++)
      {
         m_evptra[i]->SetEvent();
      }
   }

   void wave_player_command::Stop()
   {
      SetCommand(command_execute_stop);
   }

   void wave_player_command::CloseDevice()
   {
      SetCommand(command_close_device);
   }

   void wave_player_command::OpenDevice()
   {
      SetCommand(command_open_device);
   }

   void wave_player::ExecuteStop()
   {
      m_pwaveout->Stop();
      //   m_pdecoder->DecoderStop();
   }

   bool wave_player::DeviceIsOpened()
   {
      return GetDeviceState() == DeviceStateOpened;
   }

   bool wave_player::DecoderIsOpened()
   {
      return GetDecoderState() == DecoderStateOpened;
   }

   bool wave_player::IsPlaying()
   {
      return (GetDeviceState() == DeviceStatePlaying
         || GetDeviceState() == DeviceStateStopping
         || m_estate == StateFadingOutToStop)
         && m_pwaveout->m_iBufferInMMSys > 0;

   }

   bool wave_player::GetPlayEnable()
   {
      return GetDecoderState() == DecoderStateOpened
         && !IsPlaying();
   }

   bool wave_player::GetStopEnable()
   {
      return IsPlaying();
   }




   void wave_player::OnEvent(e_event eevent)
   {
      switch(eevent)
      {
      case EventOpenDevice:
         SetDeviceState(DeviceStateOpened);
         break;
      case EventOpenDecoder:
         SetDecoderState(DecoderStateOpened);
         if(m_commandlistOpenPlay.get_count() > 0)
         {
            wave_player_command command = m_commandlistOpenPlay.remove_head();
            ExecuteCommand(command);
            m_commandlistOpenPlay.remove_all();
         }
         break;
      case EventPlay:
         SetDeviceState(DeviceStatePlaying);
         SetState(StatePlaying);
         break;
      case EventStop:
         SetDeviceState(DeviceStateStopping);
         SetState(state_initial);
         break;
      case EventFadeOutAndStop:
         SetState(StateFadingOutToStop);
         break;
      case EventExecutePause:
         SetDeviceState(DeviceStatePaused);
         SetState(StatePaused);
         break;
      case EventExecuteRestart:
         SetDeviceState(DeviceStatePlaying);
         SetState(StatePlaying);
         break;
      case EventEOF:
         break;
      case EventPlaybackEnd:
         {
            m_pwaveout->close();
            if(m_pdecoder != NULL)
            {
               SetDecoderState(DecoderStateOpened);
            }
            SetState(state_initial);
            SetDeviceState(DeviceStateInitial);
            m_estate = state_initial;
            if(m_commandlistStopOpen.get_count() > 0)
            {
               wave_player_command command = m_commandlistStopOpen.remove_head();
               ExecuteCommand(command);
               m_commandlistStopOpen.remove_all();
            }
         }
         break;
      case EventStopped:
         {
            m_pwaveout->close();
            if(m_pdecoder != NULL)
            {
               SetDecoderState(DecoderStateOpened);
               m_pdecoder->DecoderSeekBegin();
            }
            SetState(state_initial);
            SetDeviceState(DeviceStateInitial);
            if(m_commandlistStopOpen.get_count() > 0)
            {
               wave_player_command command = m_commandlistStopOpen.remove_head();
               ExecuteCommand(command);
               m_commandlistStopOpen.remove_all();
            }
         }
         break;
      case EventCloseDevice:
         SetDeviceState(DeviceStateInitial);
         break;
      case EventCloseDecoder:
         SetDecoderState(DecoderStateInitial);
         break;

      }

      if(m_oswindowCallback != NULL)
      {
         m_oswindowCallback->post_message(WM_APP + 119, (WPARAM) eevent);
      }

   }
   void wave_player::SetDeviceState(EDeviceState estate)
   {
      m_edevicestate = estate;
   }

   void wave_player::SetDecoderState(EDecoderState estate)
   {
      m_edecoderstate = estate;
   }

   void wave_player::SetState(e_state estate)
   {
      m_estate = estate;
   }


   wave_player::EDeviceState wave_player::GetDeviceState()
   {
      return m_edevicestate;
   }

   wave_player::EDecoderState wave_player::GetDecoderState()
   {
      return m_edecoderstate;
   }

   wave_player * wave_player::BeginThread(sp(::ca2::application) papp)
   {
      return __begin_thread < wave_player > (papp, ::ca2::scheduling_priority_highest);
   }

   wave_out * wave_player::GetWaveOut()
   {
      return m_pwaveout;
   }

   void wave_player::set_callback(sp(::user::interaction) oswindow)
   {
      m_oswindowCallback = oswindow;
   }

   bool wave_player::ExecuteIsPaused()
   {
      return m_estate == StatePaused;
   }

   void wave_player::_Stop()
   {
      if(IsPlaying())
      {
         m_pwaveout->m_eventStopped.ResetEvent();
         OnEvent(EventStop);
         m_pwaveout->Stop();
         m_pwaveout->m_eventStopped.wait(seconds(10));
         m_pdecoder->DecoderSeekBegin();
      }
   }



   /*uint32_t c_cdecl wave_player::ThreadProcBuffer(LPVOID lpparameter)
   {
   wave_player * pplayer = (wave_player *) lpparameter;

   while(true)
   {
   pplayer->m_evOutBufferDone.lock();
   pplayer->m_evPreBufferDone.lock();
   pplayer->post_thread_message(MessageKickBufferOut, 0, 0);
   }

   }*/

   bool wave_player::IsDevocalized()
   {
      return m_pwaveout->m_pprebuffer->IsDevocalized();
   }

   void wave_player::Devocalize(bool bSet)
   {
      m_pwaveout->m_pprebuffer->Devocalize(bSet);
   }

   void wave_player::FadeOutAndStop()
   {
      if(m_estate != StatePlaying)
         return;
      OnEvent(EventFadeOutAndStop);
      wave_effect_crescendo * pfadeout = new wave_effect_crescendo;

      pfadeout->m_bEnd = true;
      pfadeout->m_bSustain = true;
      pfadeout->m_bTriggerEnd = true;
      pfadeout->SetLengthMillis(5000 , m_pwaveout->GetBuffer().m_waveformatexOut.nSamplesPerSec);
      //pfadeout->m_iLength = 5000 * m_pwaveout->GetBuffer().m_waveformatexOut.nSamplesPerSec / 1000;
      pfadeout->SetFinalScale(0, (int16_t) pfadeout->m_iLength);
      pfadeout->Initialize();
      m_pwaveout->m_pprebuffer->m_pstreameffectOut = pfadeout;
      //single_lock sl(& m_pwaveout->m_csPreBuffer, TRUE);
      m_pwaveout->m_pprebuffer->ClearBuffer();
   }

   void wave_player::FillTitleInfo(
      stringa &wstraFormat,
      string2a & wstr2aTitle)
   {
      if(m_pdecoder == NULL)
         return;

      string strAttr;

      string str;
      string wstrAttr;
      if(m_pdecoder->DecoderGetAttribute(audio_decode::AttributeTitle, strAttr))
      {
         strAttr.ReleaseBuffer();
         wstrAttr = strAttr;
         wstraFormat.add("%0");
         wstr2aTitle.add_new();
         wstr2aTitle[wstr2aTitle.get_upper_bound()].add(wstrAttr);
      }

      if(m_pdecoder->DecoderGetAttribute(audio_decode::AttributeArtist, strAttr))
      {
         strAttr.ReleaseBuffer();
         wstrAttr = strAttr;
         wstraFormat.add("Performer: %0");
         wstr2aTitle.add_new();
         wstr2aTitle[wstr2aTitle.get_upper_bound()].add(wstrAttr);
      }

      if(m_pdecoder->DecoderGetAttribute(audio_decode::AttributeAlbum, strAttr))
      {
         strAttr.ReleaseBuffer();
         wstrAttr = strAttr;
         wstraFormat.add("Album: %0");
         wstr2aTitle.add_new();
         wstr2aTitle[wstr2aTitle.get_upper_bound()].add(wstrAttr);
      }

      if(m_pdecoder->DecoderGetAttribute(audio_decode::AttributeComposer, strAttr))
      {
         strAttr.ReleaseBuffer();
         wstrAttr = strAttr;
         wstraFormat.add(L"Composer: %0");
         wstr2aTitle.add_new();
         wstr2aTitle[wstr2aTitle.get_upper_bound()].add(wstrAttr);
      }

      if(m_pdecoder->DecoderGetAttribute(audio_decode::AttributeYear,strAttr))
      {
         strAttr.ReleaseBuffer();
         wstrAttr = strAttr;
         wstraFormat.add(L"Year: %0");
         wstr2aTitle.add_new();
         wstr2aTitle[wstr2aTitle.get_upper_bound()].add(wstrAttr);
      }
   }


} // namespace audio





