#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      namespace player
      {


         /*commandcommand(e_command ecommand, uint32_t dwEllapse)
         {
         m_ecommand = ecommand;
         m_dwEllapse = dwEllapse;

         }
         */

         command::command(sp(::ca2::application) papp) :
            ca2(papp)
         {
            m_iRefCount = 0;
            m_bReady = false;
         }

         e_command command::GetCommand()
         {
            return m_ecommand;
         }

         uint32_t command::GetEllapse()
         {
            return m_dwEllapse;
         }

         bool command::wait_ready()
         {
            uint32_t dwStart = ::get_tick_count();
            while(!m_bReady && (::get_tick_count() - dwStart) < m_dwEllapse)
            {
               Sleep(84);
            }
            return m_bReady;
         }

         void command::OnFinish()
         {
            m_bReady = true;
         }


      } // namespace player


   } // namespace midi_mmsystem


} // namespace music









