#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      event_v2::event_v2(sp(::music::midi::file) pFile, track * pTrack)
      {
         m_abEvent[0] = 0;
         m_abEvent[1] = 0;
         m_abEvent[2] = 0;
         m_cbParm  = 0;
         m_hpbParm = 0;
         //   m_idxParam = 0;
         m_tkDelta = 0;
         m_bAutoAllocation = false;
         m_dwAllocation = 0;
         m_dwAllocationAddUp = 512;
         //   m_hpbImage =  0;
         m_cbImage = 0;
         m_hpbAllocation = NULL;
         m_pFile = pFile;
         m_pTrack = pTrack;

         m_iFlags = 0;

      }


      event_v2::event_v2(sp(::music::midi::file) pFile, track * pTrack, event_v1 * pEvent)
      {
         m_abEvent[0] = pEvent->m_abEvent[0];
         m_abEvent[1] = pEvent->m_abEvent[1];
         m_abEvent[2] = pEvent->m_abEvent[2];
         SetParam(pEvent->m_hpbParm, pEvent->m_cbParm);
         //   m_idxParam = 0;
         m_tkDelta = 0;
         m_bAutoAllocation = false;
         m_dwAllocation = 0;
         m_dwAllocationAddUp = 512;
         //   m_hpbImage =  0;
         m_cbImage = 0;
         m_hpbAllocation = NULL;
         m_pFile = pFile;
         m_pTrack = pTrack;

         m_iFlags = 0;
      }

      event_v2::~event_v2()
      {
         if(m_bAutoAllocation && m_hpbAllocation != NULL)
         {
            free(m_hpbAllocation);
         }
      }

      event_v2 & event_v2::operator = (const event_v2 &eventSrc)
      {
         m_abEvent[0]      = eventSrc.m_abEvent[0];
         m_abEvent[1]      = eventSrc.m_abEvent[1];
         m_abEvent[2]      = eventSrc.m_abEvent[2];
         m_cbParm            = eventSrc.m_cbParm;
         m_hpbParm         = eventSrc.m_hpbParm;
         //m_idxParam      = eventSrc.m_idxParam;
         m_tkDelta         = eventSrc.m_tkDelta;
         m_tkPosition      = eventSrc.m_tkPosition;
         //   m_hpbImage         = eventSrc.m_hpbImage;
         m_idxImage         = eventSrc.m_idxImage;
         m_cbImage         = eventSrc.m_cbImage;
         m_pFile            = eventSrc.m_pFile;
         m_pTrack          = eventSrc.m_pTrack;
         return *this;
      }




      //event_v2 * event_v2::operator =(event_v2 *pEventSrc)
      //{
      //   abEvent[0]   = pEventSrc->abEvent[0];
      //   abEvent[1]   = pEventSrc->abEvent[1];
      //   abEvent[2]   = pEventSrc->abEvent[2];
      //   cbParm      = pEventSrc->cbParm;
      //   hpbParm      = pEventSrc->hpbParm;
      //   tkDelta      = pEventSrc->tkDelta;
      //   return this;
      //}


      bool event_v2::IsAutoAllocated()
      {
         return m_bAutoAllocation;
      }

      void event_v2::AutoAllocate()
      {
         ASSERT(!m_bAutoAllocation);
         m_bAutoAllocation = true;
         byte * hpbImage = (byte *) malloc(m_cbImage);
         memcpy(hpbImage, GetImage(), m_cbImage);
         m_hpbAllocation = hpbImage;
      }


      uint32_t event_v2::SetVDWord(
         byte * hpbImage,
         uint32_t dw)
      {
         uint32_t                   dwUsed  = 0;
         uint32_t               dwBuffer;

         ASSERT(m_bAutoAllocation);
         ASSERT(dw <= 0x0fffffff);


         dwBuffer = dw & 0x7f;
         dwUsed++;
         while ((dw >>= 7) > 0)
         {
            dwBuffer <<= 8;
            dwBuffer |= 0x80;
            dwBuffer |= (dw & 0x7f);
            dwUsed++;
         }
         if(!allocate_add_up(dwUsed))
            return 0;
         //   byte *   hpbImage = m_hpbImage;
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
      bool event_v2::allocate(uint32_t dwNewLength)
      {
         ASSERT(m_bAutoAllocation);
         if(m_hpbAllocation == NULL)
         {
            m_hpbAllocation = (byte *) malloc(dwNewLength);
            if(m_hpbAllocation!= NULL)
            {
               m_dwAllocation = dwNewLength;
               m_idxImage = 0;
               //m_hpbImage = m_hpbAllocation;
               m_cbImage = dwNewLength;
               return TRUE;
            }
            else
            {
               return FALSE;
            }
         }
         else
         {
            if(dwNewLength > m_dwAllocation)
            {
               uint32_t dwAllocation = dwNewLength +  m_dwAllocationAddUp;
               LPVOID lpVoid = realloc(m_hpbAllocation, dwAllocation);
               if(lpVoid == NULL)
               {
                  //            uint32_t dw = GetLastError();
                  TRACELASTERROR();
                  return FALSE;
               }
               else
               {
                  m_dwAllocation = dwAllocation;
                  //            m_hpbImage = (byte *) lpVoid + (m_hpbImage - m_hpbAllocation);
                  m_hpbAllocation = (byte *) lpVoid;
                  m_cbImage = dwNewLength;
                  return TRUE;
               }
            }
            else
            {
               m_cbImage = dwNewLength;
               return TRUE;
            }

         }
      }

      bool event_v2::allocate_add_up(uint32_t dwAddUp)
      {
         ASSERT(m_bAutoAllocation);
         return allocate(m_cbImage + dwAddUp);
      }

      void event_v2::SetAutoAllocation(bool bValue)
      {
         if(bValue)
         {
            if(!m_bAutoAllocation)
            {
               ASSERT(m_dwAllocation == 0);
               ASSERT(!m_bAutoAllocation);
               //         m_hpbImage = NULL;
               m_idxImage = 0;
               m_bAutoAllocation = TRUE;
            }
         }
         else
         {
            if(m_bAutoAllocation)
            {
               if(m_hpbAllocation != NULL)
               {
                  free(m_hpbAllocation);
                  m_hpbAllocation = NULL;
                  m_dwAllocation = 0;
               }
            }
            m_bAutoAllocation = FALSE;
         }
      }

      SMFRESULT event_v2::CreateXFInfoHeaderEvent(imedia::position tkDelta, ::music::xf::info_header *pXfih)
      {

         SMFRESULT smfrc;

         primitive::memory memstorage;

         memstorage.allocate(pXfih->GetDataCount());

         pXfih->get_data((char *) memstorage.get_data(), (int32_t) memstorage.get_size());

         if(Success != (smfrc = CreateMetaEvent(tkDelta, ::music::xf::MetaXFInfoHdr, (byte *) memstorage.get_data(), (uint32_t) memstorage.get_size())))
         {
            return smfrc;
         }

         return Success;

      }

      SMFRESULT event_v2::CreateMetaEvent(imedia::position tkDelta, BYTE bMetaType, byte * hpbParam, uint32_t cbParam)
      {

         uint32_t dwUsed;

         m_cbImage = 0;
         m_tkDelta = tkDelta;
         SetFullType(Meta);
         SetMetaType(bMetaType);
         m_cbParm = cbParam;

         SetAutoAllocation();

         if(!(dwUsed = SetVDWord(m_tkDelta)))
         {
            //      TRACE("event_v2::CreateMetaEvent NoMemory");
            return (SMFRESULT) ENoMemory;
         }

         m_idxImage = dwUsed;

         if(!allocate_add_up(2))
         {
            //TRACE("event_v2::CreateMetaEvent NoMemory");
            return (SMFRESULT) ENoMemory;
         }

         *GetImage() = Meta;
         m_idxImage++;
         *GetImage() = GetMetaType();
         m_idxImage++;

         if(!(dwUsed = SetVDWord(cbParam)))
         {
            //      TRACE("event_v2::CreateMetaEvent NoMemory");
            return (SMFRESULT) ENoMemory;
         }

         m_idxImage += dwUsed;

         if(!allocate_add_up(cbParam))
         {
            //TRACE("event_v2::CreateMetaEvent NoMemory");
            return (SMFRESULT) ENoMemory;
         }

         m_hpbParm = GetImage();

         memcpy(m_hpbParm, hpbParam, cbParam);

         //   m_hpbImage = m_hpbAllocation;
         m_idxParam = m_idxImage;
         m_idxImage = 0;

         return Success;

      }

      uint32_t event_v2::SetVDWord(uint64_t ui)
      {
         uint32_t dw = throw_cast < uint32_t > (ui);
         uint32_t dwUsed = 0;
         uint32_t dwBuffer;

         ASSERT(m_bAutoAllocation);
         ASSERT(dw <= 0x0fffffff);


         dwBuffer = dw & 0x7f;
         dwUsed++;
         while ((dw >>= 7) > 0)
         {
            dwBuffer <<= 8;
            dwBuffer |= 0x80;
            dwBuffer |= (dw & 0x7f);
            dwUsed++;
         }
         if(!allocate_add_up(dwUsed))
            return 0;
         byte *   hpbImage = GetImage();
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

      void event_v2::SetImage(byte * hpbImage)
      {
         if(m_bAutoAllocation)
         {
            m_idxImage = (uint32_t) (hpbImage - m_hpbAllocation);
         }
         else
         {
            m_idxImage = (uint32_t) (hpbImage - (m_pFile->GetImage() + m_pTrack->m_idxTrack));
         }

      }

      byte * event_v2::GetImage() const
      {
         if(m_bAutoAllocation)
         {
            return m_hpbAllocation + m_idxImage;
         }
         else
         {
            return m_pFile->GetImage() + m_pTrack->m_idxTrack + m_idxImage;
         }

      }

      byte * event_v2::GetParam() const
      {
         if(m_bAutoAllocation)
         {
            return m_hpbAllocation + m_idxParam;
         }
         else
         {
            return m_pFile->GetImage()+
               m_pTrack->m_idxTrack +
               m_idxImage +

               m_idxParam;
         }

      }

      /*void event_v2::SetParam(byte * hpbParam)
      {
      if(m_bAutoAllocation)
      {
      m_idxParam = hpbParam - m_hpbAllocation;
      }
      else
      {
      m_idxParam = hpbParam -
      m_pFile->m_hpbImage +
      m_pTrack->m_idxTrack +
      m_idxImage +
      m_idxParam;
      }

      }*/

      void event_v2::SetParam(void * hpbParam, ::primitive::memory_size iSize)
      {
         m_cbParm = (uint32_t) iSize;
         if(m_bAutoAllocation)
         {
            m_idxParam = (uint32_t) ((byte *) hpbParam - m_hpbAllocation);
         }
         else
         {
            m_idxParam = (uint32_t) ((byte *) hpbParam - m_pFile->GetImage() + m_pTrack->m_idxTrack + m_idxImage + m_idxParam);
         }

      }

      VMSRESULT event_v2::SetParam(::ca2::file & file, ::primitive::memory_size iLength)
      {
         UNREFERENCED_PARAMETER(file);
         UNREFERENCED_PARAMETER(iLength);
         ASSERT(FALSE);
         return VMSR_E_FAIL;
      }


      int32_t event_v2::GetFlags() const
      {
         return m_iFlags;
      }

      void event_v2::SetFlags(int32_t iFlags)
      {
         m_iFlags = iFlags;
      }

      void event_v2::clear()
      {
         m_tkDelta = 0;
         m_tkPosition= 0;
         m_abEvent[0]= 0;
         m_abEvent[1]= 0;
         m_abEvent[2]= 0;
         m_cbParm= 0;
         m_hpbParm= 0;
         m_cbImage= 0;
         m_idxImage= 0;
         m_idxParam= 0;
         m_pFile.release();
         m_pTrack= 0;
         m_hpbAllocation= 0;
         m_dwAllocation= 0;
         m_bAutoAllocation= 0;
         m_dwAllocationAddUp= 0;

         m_iFlags= 0;



      }



      imedia::position event_v2::GetDelta() const
      {
         return m_tkDelta;
      }

      void event_v2::SetDelta(imedia::position tkDelta)
      {
         m_tkDelta = tkDelta;
      }

      imedia::position event_v2::get_position() const
      {
         return m_tkPosition;
      }

      void event_v2::SetPosition(imedia::position tkPosition)
      {
         m_tkPosition = tkPosition;
      }

      ::primitive::memory_size event_v2::GetImageSize() const
      {
         return m_cbImage;
      }

      BYTE event_v2::GetFullType() const
      {
         return m_abEvent[0];
      }
      void event_v2::SetFullType(BYTE bValue)
      {
         m_abEvent[0] = bValue;
      }
      BYTE event_v2::get_type() const
      {
         return m_abEvent[0] & 0xf0;
      }
      void event_v2::set_type(BYTE bValue)
      {
         m_abEvent[0] &= 0x0f;
         m_abEvent[0] |= bValue & 0xf0;
      }
      BYTE event_v2::GetTrack() const
      {
         return m_abEvent[0] & 0x0f;
      }
      void event_v2::SetTrack(BYTE bValue)
      {
         m_abEvent[0] &= 0xf0;
         m_abEvent[0] |= bValue & 0x0f;
      }
      BYTE event_v2::GetMetaType() const
      {
         return m_abEvent[1];
      }
      void event_v2::SetMetaType(BYTE bValue)
      {
         m_abEvent[1] = bValue;
      }
      BYTE event_v2::GetChB1() const
      {
         return m_abEvent[1];
      }
      void event_v2::SetChB1(BYTE bValue)
      {
         m_abEvent[1] = bValue;
      }
      BYTE event_v2::GetChB2() const
      {
         return m_abEvent[2];
      }
      void event_v2::SetChB2(BYTE bValue)
      {
         m_abEvent[3] = bValue;
      }
      BYTE event_v2::GetNotePitch() const
      {
         return m_abEvent[1];
      }
      void event_v2::SetNotePitch(BYTE bValue)
      {
         m_abEvent[1] = bValue;
      }
      BYTE event_v2::GetNoteVelocity() const
      {
         return m_abEvent[2];
      }
      void event_v2::SetNoteVelocity(BYTE bValue)
      {
         m_abEvent[2] = bValue;
      }
      void event_v2::SetImageSize(::primitive::memory_size cbSize)
      {
         m_cbImage = (uint32_t) cbSize;
      }
      //   void event_v2::SetParamSize(uint32_t cbSize)
      //   {
      //      m_cbParm = cbSize;
      //   }
      ::primitive::memory_size event_v2::GetParamSize() const
      {
         return m_cbParm;
      }


   } // namespace midi_mmsystem


} // namespace music









