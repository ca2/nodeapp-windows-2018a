#pragma once


namespace audio
{

   class wave_in;
   class wave_in_listener;


   class wave_in_listener_set :
      public comparable_array < wave_in_listener *, wave_in_listener *>
   {
   public:


      wave_in_listener_set();
      virtual ~wave_in_listener_set();


      virtual void WaveInDataProc(wave_in * pwavein, uint32_t dwSampleTime, LPWAVEHDR lpwavehdr);



   };


} // namespace audio