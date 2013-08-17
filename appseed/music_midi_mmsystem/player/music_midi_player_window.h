#pragma once


namespace music
{


   namespace midi
   {


      namespace player
      {


         class callback;


         class CLASS_DECL_VERIWELL_MULTIMEDIA window :
            public ::user::interaction
         {
         public:


            callback * m_pcallback;


            window(sp(::ca2::application) papp);
            virtual ~window();

            callback * get_callback();

            using ::user::interaction::create;
            virtual bool create();

            void set_callback(callback * pcallback);

            void install_message_handling(::ca2::message::dispatch * pinterface);

            DECL_GEN_SIGNAL(_001OnNotifyEvent)
               DECL_GEN_SIGNAL(_001OnMmsgDone)
               DECL_GEN_SIGNAL(_001OnUserMessage)
               DECL_GEN_SIGNAL(_001OnApp3388Message)
         };

      
      } // namespace player


   } // namespace midi


} // namespace music



