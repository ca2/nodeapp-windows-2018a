#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class CLASS_DECL_AUDIO_MIXER_MMSYSTEM thread :
         public ::thread,
         public ::database::client
      {
      public:


         enum e_message
         {
            MessageMixerThread = WM_USER + 113,
         };

         bool                                m_bPendingShowCommand;

         sp(::multimedia::audio_mixer::department) m_psection;


         thread(::aura::application * papp);
         virtual ~thread();


         static LRESULT CALLBACK MessageProc(
         int32_t code,       // hook code
         WPARAM wParam,  // undefined
         LPARAM lParam   // address of structure with message data
         );


         void install_message_routing(::message::sender * pinterface);

         virtual bool init_thread() override;
         virtual void term_thread() override;
         // virtual bool on_idle(LONG lCount);
         virtual void pre_translate_message(::message::message * pobj);


         DECL_GEN_SIGNAL(OnMixerMessage);
         DECL_GEN_SIGNAL(OnVmsmException);
         DECL_GEN_SIGNAL(OnUserMessage);


      };


   } // namespace audio_mixer_mmsystem


} // namespace multimedia





