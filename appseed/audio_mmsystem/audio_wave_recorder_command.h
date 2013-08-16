#pragma once


namespace audio
{

   class wave_out;
   class ::audio::wave_pre_buffer;

   class CLASS_DECL_CA2_MULTIMEDIA wave_recorder_command
   {
   protected:
      ::audio::e_command                         m_ecommand;
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
      void SetCommand(::audio::e_command ecommand);
      ::audio::e_command GetCommand() const;
   };


} //  namespace audio




