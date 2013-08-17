#include "framework.h"


namespace music
{


   namespace midi
   {


      event_v8::event_v8()
      {
         m_iFlags = 0;
         m_iAllocate = 0;
         m_pvoid = NULL;
         m_lpdwData[0] = 0;
         m_lpdwData[1] = 0;
         m_lpdwData[2] = 0;
      }

      event_v8::~event_v8()
      {

      }
#define ROUND(x,y) (((x)+(y-1))&~(y-1))
#define ROUND4(x) ROUND(x, 4)

      __STATIC fixed_alloc _vmsAlloc8(ROUND4(8));
      __STATIC fixed_alloc _vmsAlloc64(ROUND4(64));
      __STATIC fixed_alloc _vmsAlloc128(ROUND4(128));
      __STATIC fixed_alloc _vmsAlloc256(ROUND4(256));
      __STATIC fixed_alloc _vmsAlloc512(ROUND4(512));

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


      void event_v8::clear()
      {
         memset(m_lpbData, 0, sizeof(m_lpbData));
      }
      VMSRESULT event_v8::GetMidiEventV001(event_v1 * lpMidiEventV001,  uint32_t dwPositionTicks)
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


      /*event_base & event_v8::operator =(event_base &eventSrc)
      {
      _SetFullType(eventSrc.GetFullType());
      _SetChB1(eventSrc.GetChB1());
      _SetChB2(eventSrc.GetChB2());
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

      event_v8 & event_v8::operator =(const event_v8 &eventSrc)
      {

         if(&eventSrc == this)
            return *this;

         _SetFullType(eventSrc._GetFullType());
         _SetChB1(eventSrc._GetChB1());
         _SetChB2(eventSrc._GetChB2());
         WriteParam(eventSrc.GetParam(), eventSrc.GetParamSize());
         _SetFlags(eventSrc._GetFlags());

         return *this;

      }



      VMSRESULT event_v8::WriteParam(LPVOID lpData, ::primitive::memory_size iSize)
      {

         if(!_Allocate((int32_t) iSize))
            return VMSR_E_NOMEMORY;

         memcpy(GetParam(), lpData, iSize);

         return VMSR_SUCCESS;

      }

      VMSRESULT event_v8::SetParam(::ca2::file & file, ::primitive::memory_size iLength)
      {
         byte * hpbParam = GetParam();
         if(iLength <= 0 && iLength != -1)
         {
            //SetParam(NULL, 0);
            m_lpdwData[2] = 0;
            free(hpbParam);
            return VMSR_SUCCESS;
         }
         else if(iLength == -1)
         {
            iLength = (int32_t) file.get_length();
         }
         if(hpbParam == NULL)
         {   
            //        m_iDataRef = 1;
            hpbParam = (byte *) malloc(iLength);
         }
         else
         {
            if(_msize(hpbParam) < (uint32_t) iLength)
            {
               hpbParam = (byte *) realloc(hpbParam, iLength);
            }
         }
         if(hpbParam == NULL)
            return VMSR_E_NOMEMORY;
         file.read(hpbParam, iLength);
         //SetParam(hpbParam, iSize);
         //SetParamSize(iSize);
         m_lpdwData[2] = (uint32_t) hpbParam;
         m_lpdwData[1] = (uint32_t) iLength;

         return VMSR_SUCCESS;
      }
      VMSRESULT event_v8::FromData(LPVOID lpData, LPINT lpiLeft, BYTE &bRunningStatus)
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

      int32_t event_v8::GetFlags()  const
      {
         return m_iFlags;
      }

      void event_v8::SetFlags(int32_t iFlags)
      {
         m_iFlags = iFlags;
      }

      void event_v8::_Free()
      {
         void * pvoid = m_pvoid;
         if(pvoid == NULL)
            return;
         if(m_iAllocate == 8)
         {
            _vmsAlloc8.Free(pvoid);
         }
         else if(m_iAllocate == 64)
         {
            _vmsAlloc64.Free(pvoid);
         }
         else if(m_iAllocate == 128)
         {
            _vmsAlloc128.Free(pvoid);
         }
         else if(m_iAllocate == 256)
         {
            _vmsAlloc256.Free(pvoid);
         }
         else if(m_iAllocate == 512)
         {
            _vmsAlloc512.Free(pvoid);
         }
         else
         {
            free(pvoid);
         }
         m_pvoid = NULL;
         m_iAllocate = 0;
         m_lpdwData[1] = 0;
         m_lpdwData[2] = 0;
      }


      bool event_v8::_Allocate(int32_t iSize)
      {
         if(iSize == 0)
         {
            _Free();
            return true;
         }
         void * pvoid = m_pvoid;
         bool bFree = pvoid != NULL && (iSize > m_iAllocate);
         if(bFree)
         {
            _Free();
         }
         pvoid = m_pvoid;
         if(pvoid == NULL)
         {   
            if(iSize <= 8)
            {
               pvoid = _vmsAlloc8.Alloc();
               m_iAllocate = 8;
            }
            else if(iSize <= 64)
            {
               pvoid = _vmsAlloc64.Alloc();
               m_iAllocate = 64;
            }
            else if(iSize <= 128)
            {
               pvoid = _vmsAlloc128.Alloc();
               m_iAllocate = 128;
            }
            else if(iSize <= 256)
            {
               pvoid =  _vmsAlloc256.Alloc();
               m_iAllocate = 256;
            }
            else if(iSize <= 512)
            {
               pvoid =  _vmsAlloc512.Alloc();
               m_iAllocate = 512;
            }
            else
            {
               pvoid = malloc(iSize);
               m_iAllocate = iSize;
            }
            if(pvoid == NULL)
               return false;
         }
         else
         {
         }

         m_pvoid        = pvoid;
         m_lpdwData[1]  = iSize;
         m_lpdwData[2]  = (uint32_t) pvoid;

         return true;
      }

      void event_v8::SetParamSize(::primitive::memory_size iSize)
      {
         UNREFERENCED_PARAMETER(iSize);
         throw not_implemented(::ca2::get_thread_app());
      }



      imedia::position event_v8::GetDelta() const
      {
         return 0;
      }

      void event_v8::SetDelta(imedia::position tkDelta)
      {
         UNREFERENCED_PARAMETER(tkDelta);
         ASSERT(FALSE);
      }

      imedia::position event_v8::get_position() const
      {
         return 0xffffffff;
      }

      void event_v8::SetPosition(imedia::position tkPosition)
      {
         UNREFERENCED_PARAMETER(tkPosition);
         ASSERT(FALSE);
      }

      BYTE event_v8::GetFullType() const
      {
         return m_lpbData[0];
      }

      void event_v8::SetFullType(BYTE bValue)
      {
         m_lpbData[0] = bValue;
      }

      BYTE event_v8::get_type() const
      {
         return m_lpbData[0] & 0xf0;
      }

      void event_v8::set_type(BYTE bValue)
      {
         m_lpbData[0] &= 0x0f;
         m_lpbData[0] |= bValue & 0xf0;
      }
      BYTE event_v8::GetTrack() const
      {
         return m_lpbData[0] & 0x0f;
      }
      void event_v8::SetTrack(BYTE bValue)
      {
         m_lpbData[0] &= 0xf0;
         m_lpbData[0] |= bValue & 0x0f;
      }
      BYTE event_v8::GetMetaType() const
      {
         return m_lpbData[1];
      }
      void event_v8::SetMetaType(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }
      BYTE event_v8::GetChB1() const
      {
         return m_lpbData[1];
      }
      void event_v8::SetChB1(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }
      BYTE event_v8::GetChB2() const
      {
         return m_lpbData[2];
      }
      void event_v8::SetChB2(BYTE bValue)
      {
         m_lpbData[2] = bValue;
      }
      BYTE event_v8::GetNotePitch() const
      {
         return m_lpbData[1];
      }
      void event_v8::SetNotePitch(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }
      BYTE event_v8::GetNoteVelocity() const
      {
         return m_lpbData[2];
      }
      void event_v8::SetNoteVelocity(BYTE bValue)
      {
         m_lpbData[2] = bValue;
      }
      byte * event_v8::GetImage() const
      {
         return (byte *) m_lpdwData[0];
      }

      void event_v8::SetImage(byte * hpbImage)
      {
         UNREFERENCED_PARAMETER(hpbImage);
         ASSERT(FALSE);
      }

      ::primitive::memory_size event_v8::GetImageSize() const
      {
         return 0xffffffff;
      }
      void event_v8::SetImageSize(::primitive::memory_size cbImage)
      {
         UNREFERENCED_PARAMETER(cbImage);
         ASSERT(FALSE);
      }

      void event_v8::SetParam(void * pVoid, ::primitive::memory_size iSize)
      {
         WriteParam(pVoid, iSize);
      }

      byte * event_v8::GetParam() const
      {
         return (byte *) m_lpdwData[2];
      }

      ::primitive::memory_size event_v8::GetParamSize() const
      {
         return m_lpdwData[1];
      }

      BYTE event_v8::_GetType() const
      {
         return m_lpbData[0] & 0xf0;
      }

      BYTE event_v8::_GetTrack() const
      {
         return m_lpbData[0] & 0x0f;
      }

      BYTE event_v8::_GetFullType() const
      {
         return m_lpbData[0];
      }

      BYTE event_v8::_GetMetaType() const
      {
         return m_lpbData[1];
      }

      void event_v8::_SetMetaType(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }

      void event_v8::_SetFullType(BYTE bValue)
      {
         m_lpbData[0] = bValue;
      }

      BYTE event_v8::_GetChB1() const
      {
         return m_lpbData[1];
      }

      void event_v8::_SetChB1(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }

      BYTE event_v8::_GetChB2() const
      {
         return m_lpbData[2];
      }

      void event_v8::_SetChB2(BYTE bValue)
      {
         m_lpbData[2] = bValue;
      }

      byte * event_v8::_GetImage() const
      {
         return (byte *) m_lpdwData[0];
      }

      void event_v8::_SetImage(byte * hpbImage)
      {
         UNREFERENCED_PARAMETER(hpbImage);
         ASSERT(FALSE);
      }

      ::primitive::memory_size event_v8::_GetImageSize() const
      {
         return 0xffffffff;
      }

      void event_v8::_SetImageSize(::primitive::memory_size cbImage)
      {
         UNREFERENCED_PARAMETER(cbImage);
         ASSERT(FALSE);
      }

      int32_t event_v8::_GetFlags()  const
      {
         return m_iFlags;
      }

      void event_v8::_SetFlags(int32_t iFlags)
      {
         m_iFlags = iFlags;
      }


   } // namespace midi


} // namespace music









