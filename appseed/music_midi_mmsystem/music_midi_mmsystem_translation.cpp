#include "framework.h"


namespace music
{

   namespace midi_mmsystem
   {

      ::multimedia::e_result translate_mmr(MMRESULT mmr)
      {

         if(mmr == MMSYSERR_NOERROR)
         {

            return ::multimedia::result_success;

         }
         else
         {

            return ::multimedia::result_error;

         }

      }

   } // namespace midi_mmsystem


} // namespace music      