#include "framework.h"


namespace music
{


   namespace midi
   {


      //extern uint32_t grbChanMsgLen[];
      event_v7::event_v7()
      {
         m_tkDelta = 0;
         m_iFlags = 0;
      }

      event_v7::event_v7(event_v7 & event)
      {
         operator =(event);
      }

      event_v7::~event_v7()
      {

      }




      //CMidiEventV007 * CMidiEventV007::operator =(CMidiEventV007 *pEventSrc)
      //{
      //   abEvent[0]   = pEventSrc->abEvent[0];
      //   abEvent[1]   = pEventSrc->abEvent[1];
      //   abEvent[2]   = pEventSrc->abEvent[2];
      //   cbParm      = pEventSrc->cbParm;
      //   hpbParm      = pEventSrc->hpbParm;
      //   tkDelta      = pEventSrc->tkDelta;
      //   return this;
      //}


      void event_v7::clear()
      {
         m_tkDelta = 0;
         event_v8::clear();
      }
      VMSRESULT event_v7::GetMidiEventV001(event_v1 * lpMidiEventV001,  uint32_t dwPositionTicks)
      {
         UNREFERENCED_PARAMETER(dwPositionTicks);
         lpMidiEventV001->SetFullType(GetFullType());
         lpMidiEventV001->SetChB1(GetChB1());
         lpMidiEventV001->SetChB2(GetChB2());
         //   lpMidiEventV001->SetParamSize(GetParamSize());
         lpMidiEventV001->SetParam(GetParam(), GetParamSize());
         lpMidiEventV001->SetDelta(GetDelta());
         lpMidiEventV001->SetPosition(get_position());
         lpMidiEventV001->SetImage(NULL);
         lpMidiEventV001->SetImageSize(GetImageSize());
         return VMSR_SUCCESS;
      }


      /*event_base & event_v7::operator =(event_base &eventSrc)
      {
      SetFullType(eventSrc.GetFullType());
      SetChB1(eventSrc.GetChB1());
      SetChB2(eventSrc.GetChB2());
      //SetParamSize(eventSrc.GetParamSize());
      //SetParam(eventSrc.GetParam());
      WriteParam(eventSrc.GetParam(), eventSrc.GetParamSize());
      SetDelta(eventSrc.GetDelta());
      //SetPositionTicks(eventSrc.GetPositionTicks());
      //SetImage(eventSrc.GetImage());
      //SetImageSize(eventSrc.GetImageSize());
      SetFlags(eventSrc.GetFlags());
      return *this;
      }*/

      event_v7 & event_v7::operator =(const event_v7 &eventSrc)
      {

         if(&eventSrc == this)
            return *this;

         SetFullType(eventSrc.GetFullType());
         SetChB1(eventSrc.GetChB1());
         SetChB2(eventSrc.GetChB2());
         WriteParam(eventSrc.GetParam(), eventSrc.GetParamSize());
         SetDelta(eventSrc.GetDelta());
         SetFlags(eventSrc.GetFlags());

         return *this;

      }



      VMSRESULT event_v7::WriteParam(LPVOID lpData, ::primitive::memory_size iSize)
      {

         return event_v8::WriteParam(lpData, iSize);
         
      }

      VMSRESULT event_v7::FromData(LPVOID lpData, LPINT lpiLeft, BYTE &bRunningStatus)
      {
         BYTE                    bEvent;
         int32_t                     iGotTotal;
         uint32_t                   dwGot;
         uint32_t                   cbEvent;
         byte *                  hpbImage = (byte *) lpData;

         if (*lpiLeft < 3)
         {
            return VMSR_E_INVALIDFILE;
         }

         bEvent = *hpbImage++;

         if (Msg > bEvent)
         {
            ASSERT(FALSE);
            iGotTotal = 1;
            SetFullType(bRunningStatus);
            SetChB1(*hpbImage++);
            if (3 == grbChanMsgLen[(bEvent >> 4) & 0x0F])
            {
               SetChB2(*hpbImage++);
               iGotTotal++;
            }
         }
         else if (SysEx > bEvent)
         {
            bRunningStatus = bEvent;

            iGotTotal = 2;
            SetFullType(bEvent);
            SetChB1(*hpbImage++);
            if (3 == GetMessageLen(bEvent))
            {
               SetChB2(*hpbImage++);
               iGotTotal++;
            }
         }
         else
         {
            bRunningStatus = 0;
            if (Meta == bEvent)
            {
               SetFullType(Meta);
               SetMetaType(*hpbImage++);
               if (MetaEOT == GetMetaType())
               {

                  //                m_fdwTrack |= SMF_TF_EOT;
                  //            m_smti.m_tkLength = m_tkPosition;
               }

               iGotTotal = 2;
            }
            else if (SysEx == bEvent || SysExEnd == bEvent)
            {
               SetFullType(bEvent);
               iGotTotal = 1;
            }
            else
            {
               return VMSR_E_INVALIDFILE;
            }

            if (0 == (dwGot = event_base::GetVDWord(hpbImage, *lpiLeft - iGotTotal, &cbEvent)))
            {
               return VMSR_E_INVALIDFILE;
            }

            hpbImage  += dwGot;
            iGotTotal  += dwGot;

            if (iGotTotal + (int32_t) cbEvent > *lpiLeft)
            {
               return VMSR_E_INVALIDFILE;
            }

            WriteParam(hpbImage, cbEvent);

            hpbImage += cbEvent;
            iGotTotal += cbEvent;
         }

         ASSERT(*lpiLeft >= iGotTotal);

         *lpiLeft -= iGotTotal;

         return VMSR_SUCCESS;

      }



   } // namespace midi


} // namespace music









