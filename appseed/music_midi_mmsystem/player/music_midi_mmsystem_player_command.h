#pragma once


namespace music
{


   namespace midi_mmsystem
   {



      namespace player
      {


         class command :
            virtual public ::ca2::object
         {
         public:


            enum eflag
            {

               flag_ticks,
               flag_dRate,

            };


            e_command                  m_ecommand;
            imedia::position           m_ticks;
            double                     m_dRate;
            uint32_t                   m_dwEllapse;
            bool                       m_bReady;
            int32_t                    m_iRefCount;
            flags < eflag >            m_flags;


            command(sp(::ca2::application) papp);
            //command(::music::midi::player::e_command ecommand, uint32_t dwEllapse);


            uint32_t GetEllapse();
            bool wait_ready();
            void OnFinish();
            ::music::midi::player::e_command GetCommand();
         private:
            using ::ca2::object::lock;
         };



      } // namespace player


   } // namespace midi_mmsystem


} // namespace music



