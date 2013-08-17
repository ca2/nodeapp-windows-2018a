#pragma once


namespace audio_mmsystem
{

   class wave_out;
   class wave_pre_buffer;

   class  CLASS_DECL_AUDIO_MMSYSTEM wave_player_command
   {
   public:


      ::audio_mmsystem::e_command                         m_ecommand;
      ::ca2::filesp                         m_pfileOpen;
      array < event *, event * >     m_evptra;
      bool                                m_bResult;
      bool *                              m_pbResult;
      bool                                m_bSeekable;
      imedia::position                    m_position;
      ::::multimedia::audio_decode::decoder *           m_pdecoderOpen;


      wave_player_command();
      wave_player_command(const wave_player_command & command);


      void SetOpenFile(sp(::ca2::file) pfile);
      void SetOpenDecoder(::::multimedia::audio_decode::decoder * pdecoder);
      sp(::ca2::file) GetOpenFile() const;
      void SetCommand(::audio_mmsystem::e_command ecommand);
      ::audio_mmsystem::e_command GetCommand() const;


      void SetEvents();
      void AttachEvent(event * pevent);


      void OpenDecoder(::::multimedia::audio_decode::decoder * pdecoder);
      void OpenRtpFile(sp(::ca2::file) pfile);
      void OpenMp3File(sp(::ca2::file) pfile);
      void OpenAiffFile(sp(::ca2::file) pfile);
      void OpenCdaFile(sp(::ca2::file) pfile);
      void OpenWmFile(sp(::ca2::file) pfile);
      void OpenWavFile(sp(::ca2::file) pfile);
      void OpenFile(sp(::ca2::file) pfile, bool bSeekable = true);
      void OpenVqfFile(sp(::ca2::file) pfile);

      void Play(const imedia::position & position);
      void Stop();
      void OpenDevice();
      void CloseDevice();

      wave_player_command & operator =(const wave_player_command & command);

   };


} // namespace audio_mmsystem



