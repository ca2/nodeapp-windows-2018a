#include "framework.h"


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {

         mutex & get_midi_mutex();

         out::out(::aura::application * papp) :
            ::object(papp),
            ::music::midi::object(papp),
            ::music::midi::out(papp)
         {

            m_hmidiout = NULL;

         }


         out::~out()
         {

            close();

         }


         ::multimedia::e_result out::open()
         {

            synch_lock sl(&get_midi_mutex());

            MMRESULT mmrc;

            uint32_t uDeviceID = 1;

            m_hmidiout = NULL;

            mmrc = midiOutOpen(&m_hmidiout, uDeviceID,  0, 0, CALLBACK_NULL);

            if(mmrc != MMSYSERR_NOERROR)
               return ::multimedia::result_error;


            return ::multimedia::result_success;

         }

         ::multimedia::e_result out::close()
         {

            if(m_hmidiout != NULL)
            {

               midiOutClose(m_hmidiout);

            }

            return ::multimedia::result_success;

         }

         ::multimedia::e_result out::send_short_message(::music::midi::e_type etype, int iChannel, int iData1, int iData2)
         {

            return translate_os_result(midiOutShortMsg(m_hmidiout, MIDIMSG(((int) etype) >> 4, iChannel, iData1, iData2)), "out::send_short_message");

         }


      } // 


   } // namespace midi_mmsystem


} // namespace music




