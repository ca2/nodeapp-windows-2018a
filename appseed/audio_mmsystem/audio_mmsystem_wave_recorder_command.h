#pragma once


namespace audio_mmsystem
{

   class wave_out;
   class wave_pre_buffer;

   class  CLASS_DECL_AUDIO_MMSYSTEM wave_recorder_command
   {
   protected:
      ::audio_mmsystem::e_command                         m_ecommand;
      sp(::ca2::file)                         m_pfileOpen;
      array < event *, event * >   m_evptra;
   public:
      bool                                m_bResult;
      bool *                              m_pbResult;
      //      void CloseDevice();
      //    void OpenDevice();
      void SetEvents();
      void AttachEvent(event * pevent);
      wave_recorder_command();
      wave_recorder_command(const wave_recorder_command & command);
      wave_recorder_command & operator =(const wave_recorder_command & command);
      void OpenWavFile(sp(::ca2::file) pfile);
      void Record();
      void Stop();
      void SetOpenFile(sp(::ca2::file) pfile);
      sp(::ca2::file) GetOpenFile() const;
      void SetCommand(::audio_mmsystem::e_command ecommand);
      ::audio_mmsystem::e_command GetCommand() const;
   };


} //  namespace audio_mmsystem




