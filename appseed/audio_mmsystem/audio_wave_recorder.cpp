#include "framework.h"


namespace audio
{


   wave_recorder::wave_recorder(sp(::ca2::application) papp) :
      ca2(papp),
      thread(papp),
      m_pluginset(papp),
      m_eventStopped(papp, FALSE, TRUE)
   {

      m_pwavein = new wave_in(papp);

      m_pencoder = NULL;

   }

   wave_recorder::~wave_recorder()
   {
   }

   bool wave_recorder::initialize_instance()
   {

      if(m_pwavein == NULL)
         return false;

      m_pwavein->begin(::ca2::scheduling_priority_above_normal);

      //m_pwavein->m_pplayer = this;

      return true;
   }

   int32_t wave_recorder::exit_instance()
   {
      // TODO:  perform any per-thread cleanup here
      return thread::exit_instance();
   }

   void wave_recorder::install_message_handling(::ca2::message::dispatch * pinterface)
   {
      IGUI_WIN_MSG_LINK(audMessageCommand, pinterface, this, &wave_recorder::OnaudCommandMessage);
   }

   /* // BEGIN_MESSAGE_MAP(wave_recorder, thread)
   ON_THREAD_MESSAGE(audMessageCommand, OnaudCommandMessage)
   // END_MESSAGE_MAP()
   */

   // wave_recorder message handlers

   void wave_recorder::ExecuteCommand(wave_recorder_command & command)
   {
      m_eventStopped.ResetEvent();
      switch(command.GetCommand())
      {
      case ::audio::command_execute_stop:
      case ::audio::command_close_device:
         {
            audCommandMessageProcedure(command);
         }
         break;
      default:
         {
            post_thread_message(
               audMessageCommand,
               (WPARAM) command.GetCommand(),
               (LPARAM) (new wave_recorder_command(command)));
         }
         break;
      }

   }


   void wave_recorder::OnaudCommandMessage(::ca2::signal_object * pobj)
      //WPARAM wparam, // audCommand enumeration
      //LPARAM lparam  // audCommand object pointer
      //)
   {
      SCAST_PTR(::ca2::message::base, pbase, pobj);
      wave_recorder_command * pcommand = (wave_recorder_command *) pbase->m_lparam.m_lparam;
      audCommandMessageProcedure(*pcommand);
      if(pcommand->m_pbResult != NULL)
      {
         *pcommand->m_pbResult = pcommand->m_bResult;
      }
      delete pcommand;
   }

   bool wave_recorder::audCommandMessageProcedure(wave_recorder_command &command)
   {

      command.m_bResult = true;

      switch(command.GetCommand())
      {
      case command_open_file_wav:
         {

            m_iOutBufferSampleCount = 48 * 1024;

            m_spfile = command.GetOpenFile();

            if(m_pencoder == NULL)
            {

               try
               {
                  m_pencoder  = m_pluginset.GetNewEncoder("*.wav");
                  if(m_pencoder == NULL)
                     return false;
               }
               catch(...)
               {
                  command.m_bResult = false;
                  return false;
               }

            }

            m_pwavein->m_pencoder = m_pencoder;

            m_pencoder->EncoderSetFile(m_spfile);

         }
         break;

      case ::audio::command_execute_record:
         {
            /*if(m_pencoder == NULL)
            {
            command.m_bResult = false;
            return false;
            }*/
            m_pwavein->open(8, m_iOutBufferSampleCount);
            m_pwavein->Start();
         }
         break;
      case ::audio::command_close_device:
      case ::audio::command_execute_stop:
         {

            m_eventStopped.ResetEvent();
            m_pwavein->Stop();
            m_pwavein->m_eventStopped.wait(millis((1984 + 1977) * 20));
            m_pwavein->close();
            m_pencoder->EncoderFinalize();
            m_eventStopped.SetEvent();

            ::ca::release(m_spfile.m_p);

         }
         break;
      default:
         ASSERT(FALSE);
         break;
      }
      command.SetEvents();
      return command.m_bResult;
   }


   wave_recorder_command::wave_recorder_command()
   {
      m_pbResult = NULL;
   }

   wave_recorder_command::wave_recorder_command(const wave_recorder_command & command)
   {
      operator =(command);
   }

   wave_recorder_command & wave_recorder_command::operator =(const wave_recorder_command & command)
   {
      SetCommand(command.GetCommand());
      SetOpenFile(command.GetOpenFile());
      m_bResult = command.m_bResult;
      m_pbResult = command.m_pbResult;
      m_evptra = command.m_evptra;
      return *this;

   }

   e_command wave_recorder_command::GetCommand() const
   {
      return m_ecommand;
   }

   void wave_recorder_command::SetCommand(e_command ecommand)
   {
      m_ecommand = ecommand;
   }

   sp(::ca2::file) wave_recorder_command::GetOpenFile() const
   {
      return m_pfileOpen;
   }

   void wave_recorder_command::SetOpenFile(sp(::ca2::file)pfile)
   {
      m_pfileOpen = pfile;
   }

   void wave_recorder_command::OpenWavFile(sp(::ca2::file) pfile)
   {
      SetCommand(command_open_file_wav);
      SetOpenFile(pfile);
   }


   void wave_recorder_command::SetEvents()
   {
      for(int32_t i = 0;  i < m_evptra.get_size(); i++)
      {
         m_evptra[i]->SetEvent();
      }
   }
   void wave_recorder_command::Stop()
   {
      SetCommand(::audio::command_execute_stop);

   }

   void wave_recorder_command::Record()
   {
      SetCommand(::audio::command_execute_record);

   }


   void wave_recorder::WaveInDataProc(wave_in * pwavein, uint32_t dwSampleTime, LPWAVEHDR lpwavehdr)
   {

      UNREFERENCED_PARAMETER(pwavein);
      UNREFERENCED_PARAMETER(dwSampleTime);
#ifdef WINDOWS
      m_spfile->write(lpwavehdr->lpData, lpwavehdr->dwBufferLength);
#endif
   }

   bool wave_recorder::IsRecording()
   {

      return m_pwavein->IsRecording();

   }


} // namespace audio



