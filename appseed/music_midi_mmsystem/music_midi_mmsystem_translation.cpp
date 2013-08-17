#include "framework.h"


namespace music
{

   namespace midi_mmsystem
   {

::multimedia::result TranslateSMFResult(
         e_file_result               smfrc)
      {
         switch(smfrc)
         {
         case Success:
            return MMSYSERR_NOERROR;

         case ENoMemory:
            return MCIERR_OUT_OF_MEMORY;

         case EInvalidFile:
         case EOpenFailed:
         case EInvalidTrack:
            return MCIERR_INVALID_FILE;

         default:
            return MCIERR_UNSUPPORTED_FUNCTION;
         }
      }



   } // namespace midi_mmsystem


} // namespace music      