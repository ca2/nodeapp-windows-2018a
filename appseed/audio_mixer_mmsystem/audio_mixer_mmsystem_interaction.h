#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class audio_mixer;


      class interaction : 
         public ::user::interaction
      {
      public:


         ::multimedia::audio_mixer::audio_mixer                    * m_pmixer;


         interaction(sp(::axis::application) papp);
         virtual ~interaction();


         void install_message_handling(::message::dispatch * pinterface);


         DECL_GEN_SIGNAL(_001OnMixerControlChange);
            DECL_GEN_SIGNAL(_001OnMixerLineChange);


      };


   } // namespace audio_mixer_mmsystem


} // namespace multimedia



