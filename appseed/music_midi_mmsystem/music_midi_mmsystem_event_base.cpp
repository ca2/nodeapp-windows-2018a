#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      uint32_t event_base::SetVDWord(
         byte * hpbImage,
         uint32_t dwLeft,
         uint32_t dw)
      {
         uint32_t                   dwUsed  = 0;
         uint32_t               dwBuffer;

         ASSERT(hpbImage != NULL);
         ASSERT(dw <= 0x0fffffff);


         if(!dwLeft)
            return 0;
         dwBuffer = dw & 0x7f;
         dwLeft--;
         dwUsed++;
         while ((dw >>= 7) > 0)
         {
            if(!dwLeft)
               return 0;
            dwBuffer <<= 8;
            dwBuffer |= 0x80;
            dwBuffer |= (dw & 0x7f);
            dwLeft--;
            dwUsed++;
         }
         while (TRUE)
         {
            *hpbImage++ = (BYTE) dwBuffer;
            if (dwBuffer & 0x80)
               dwBuffer >>= 8;
            else
               break;
         }


         return dwUsed;
      }


      event_base & event_base::operator = (const event_base &eventSrc)
      {
         SetFullType(eventSrc.GetFullType());
         SetChB1(eventSrc.GetChB1());
         SetChB2(eventSrc.GetChB2());
         SetParam(eventSrc.GetParam(), eventSrc.GetParamSize());
         SetDelta(eventSrc.GetDelta());
         SetPosition(eventSrc.get_position());
         SetImage(eventSrc.GetImage());
         SetImageSize(eventSrc.GetImageSize());
         SetFlags(eventSrc.GetFlags());
         return *this;
      }

      event_base & event_base::operator = (const event_v5 &eventSrc)
      {
         SetFullType(eventSrc.GetFullType());
         SetChB1(eventSrc.GetChB1());
         SetChB2(eventSrc.GetChB2());
         SetParam(eventSrc.GetParam(), eventSrc.GetParamSize());
         SetDelta(eventSrc.GetDelta());
         SetPosition(eventSrc.get_position());
         SetFlags(eventSrc.GetFlags());
         return *this;
      }

      uint32_t event_base::GetVDWord(
         byte * hpbImage,
         uint32_t dwLeft,
         uint32_t * pDw)
      {
         BYTE                    b;
         uint32_t                   dwUsed  = 0;

         ASSERT(hpbImage != NULL);
         ASSERT(pDw != NULL);

         *pDw = 0;

         do
         {
            if (!dwLeft)
            {
               return 0;
            }

            b = *hpbImage++;
            dwLeft--;
            dwUsed++;

            *pDw = (*pDw << 7) | (b & 0x7F);
         } while (b&0x80);

         return dwUsed;
      }

      void event_base::SetParam(primitive::memory & memstorage)
      {

         SetParam(memstorage.get_data(), (int32_t) memstorage.get_size());

      }

      int32_t event_base::GetPitchBendLevel() const
      {

         ASSERT(get_type() == PitchBend);

         return GetChB1() | (GetChB2() << 8);

      }


   } // namespace midi_mmsystem


} // namespace music









