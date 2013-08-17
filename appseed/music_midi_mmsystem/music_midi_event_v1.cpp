#include "framework.h"


namespace music
{


   namespace midi
   {


      event_v1::event_v1()
      {
         m_abEvent[0] = 0;
         m_abEvent[1] = 0;
         m_abEvent[2] = 0;
         m_cbParm  = 0;
         m_hpbParm = 0;
         m_tkDelta = 0;
         m_hpbImage =  0;
         m_cbImage = 0;
         m_pFile = NULL;
         m_pTrack = NULL;
         m_iFlags = 0;

      }

      event_v1::~event_v1()
      {

      }



      //event_v1 * event_v1::operator =(event_v1 *pEventSrc)
      //{
      //   abEvent[0]   = pEventSrc->abEvent[0];
      //   abEvent[1]   = pEventSrc->abEvent[1];
      //   abEvent[2]   = pEventSrc->abEvent[2];
      //   cbParm      = pEventSrc->cbParm;
      //   hpbParm      = pEventSrc->hpbParm;
      //   tkDelta      = pEventSrc->tkDelta;
      //   return this;
      //}






      event_v1 & event_v1::operator =(const event_v1 &eventSrc)
      {
         _SetFullType   (eventSrc._GetFullType());
         _SetChB1       (eventSrc._GetChB1());
         _SetChB2       (eventSrc._GetChB2());
         _SetParam      (eventSrc._GetParam(), eventSrc._GetParamSize());
         _SetDelta      (eventSrc._GetDelta());
         _SetPosition   (eventSrc._GetPosition());
         _SetImage      (eventSrc._GetImage());
         _SetImageSize  (eventSrc._GetImageSize());
         _SetFlags      (eventSrc._GetFlags());
         return *this;
      }

      /*event_base & event_v1::operator = (const event_v5 &eventSrc)
      {
      _SetFullType   (eventSrc._GetFullType());
      _SetChB1       (eventSrc._GetChB1());
      _SetChB2       (eventSrc._GetChB2());
      _SetParam      (eventSrc._GetParam(), eventSrc._GetParamSize());
      _SetDelta      (eventSrc._GetDelta());
      _SetPosition   (eventSrc._GetPosition());
      _SetImage      (eventSrc._GetImage());
      _SetImageSize  (eventSrc._GetImageSize());
      _SetFlags      (eventSrc._GetFlags());
      return *this;
      }

      event_base & event_v1::operator =(const event_base &eventSrc)
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
      }*/

      int32_t event_v1::GetFlags() const
      {
         return m_iFlags;
      }

      void event_v1::SetFlags(int32_t iFlags)
      {
         m_iFlags = iFlags;
      }


      VMSRESULT event_v1::SetParam(::ca2::file & file, ::primitive::memory_size iLength)
      {
         UNREFERENCED_PARAMETER(file);
         UNREFERENCED_PARAMETER(iLength);
         ASSERT(FALSE);
         return VMSR_E_FAIL;
      }


      void event_v1::clear()
      {
         m_abEvent[0] = 0;
         m_abEvent[1] = 0;
         m_abEvent[2] = 0;
         m_cbImage = 0;
         m_cbParm = 0;
         m_hpbImage = 0;
         m_hpbParm = 0;
         m_iFlags = 0;
         m_pFile.release();
         m_pTrack = 0;
         m_tkDelta = 0;
         m_tkPosition = 0;


      }



      imedia::position event_v1::GetDelta() const
      {
         return m_tkDelta;
      }

      void event_v1::SetDelta(imedia::position tkDelta)
      {
         m_tkDelta = tkDelta;
      }

      imedia::position event_v1::get_position() const
      {
         return m_tkPosition;
      }

      void event_v1::SetPosition(imedia::position tkPosition)
      {
         m_tkPosition = tkPosition;
      }
      BYTE event_v1::GetFullType() const
      {
         return m_abEvent[0];
      }
      void event_v1::SetFullType(BYTE bValue)
      {
         m_abEvent[0] = bValue;
      }
      BYTE event_v1::get_type() const
      {
         return m_abEvent[0] & 0xf0;
      }

      void event_v1::set_type(BYTE bValue)
      {
         m_abEvent[0] &= 0x0f;
         m_abEvent[0] |= bValue & 0xf0;
      }

      BYTE event_v1::GetTrack() const
      {
         return m_abEvent[0] & 0x0f;
      }
      void event_v1::SetTrack(BYTE bValue)
      {
         m_abEvent[0] &= 0xf0;
         m_abEvent[0] |= bValue & 0x0f;
      }
      BYTE event_v1::GetMetaType() const
      {
         return m_abEvent[1];
      }
      void event_v1::SetMetaType(BYTE bValue)
      {
         m_abEvent[1] = bValue;
      }
      BYTE event_v1::GetChB1() const
      {
         return m_abEvent[1];
      }
      void event_v1::SetChB1(BYTE bValue)
      {
         m_abEvent[1] = bValue;
      }
      BYTE event_v1::GetChB2() const
      {
         return m_abEvent[2];
      }
      void event_v1::SetChB2(BYTE bValue)
      {
         m_abEvent[2] = bValue;
      }
      BYTE event_v1::GetNotePitch() const
      {
         return m_abEvent[1];
      }
      void event_v1::SetNotePitch(BYTE bValue)
      {
         m_abEvent[1] = bValue;
      }
      BYTE event_v1::GetNoteVelocity() const
      {
         return m_abEvent[2];
      }
      void event_v1::SetNoteVelocity(BYTE bValue)
      {
         m_abEvent[2] = bValue;
      }
      byte * event_v1::GetImage() const
      {
         return m_hpbImage;
      }
      void event_v1::SetImage(byte * hpbImage)
      {
         m_hpbImage = hpbImage;
      }

      ::primitive::memory_size event_v1::GetImageSize() const
      {
         return m_cbImage;
      }
      void event_v1::SetImageSize(::primitive::memory_size cbImage)
      {

         m_cbImage = (uint32_t) cbImage;

      }
      //   void event_v1::SetParam(byte * hpbParam)
      //   {
      //      m_hpbParm = hpbParam;
      //   }
      void event_v1::SetParam(void * pVoid, ::primitive::memory_size iSize)
      {

         m_hpbParm = (byte *) pVoid;
         m_cbParm = (uint32_t) iSize;

      }

      byte * event_v1::GetParam() const
      {
         return m_hpbParm;
      }

      ::primitive::memory_size event_v1::GetParamSize() const
      {
         return m_cbParm;
      }

      index event_v1::CompareTkPosition(event_v1 * p1, event_v1 * p2)
      {
         if(p1->m_tkPosition > p2->m_tkPosition)
            return 1;
         else if(p1->m_tkPosition < p2->m_tkPosition)
            return -1;
         else 
            return 0;
      }
      void event_v1::swap(event_v1 * p1, event_v1 * p2)
      {
         event_v1 ev;
         ev = *p1;
         *p1 = *p2;
         *p2 = ev;
      }





      imedia::position event_v1::_GetDelta() const
      {
         return m_tkDelta;
      }

      void  event_v1::_SetDelta(imedia::position tkDelta)
      {
         m_tkDelta = tkDelta;
      }

      imedia::position event_v1::_GetPosition() const
      {
         return m_tkPosition;
      }

      void  event_v1::_SetPosition(imedia::position tkPosition)
      {
         m_tkPosition = tkPosition;
      }

      BYTE  event_v1::_GetFullType() const
      {
         return m_abEvent[0];
      }

      void  event_v1::_SetFullType(BYTE bValue)
      {
         m_abEvent[0] = bValue;
      }

      BYTE  event_v1::_GetType() const
      {
         return m_abEvent[0] & 0xf0;
      }

      void event_v1::_SetType(BYTE bValue)
      {
         m_abEvent[0] &= 0x0f;
         m_abEvent[0] |= bValue & 0xf0;
      }

      BYTE event_v1::_GetTrack() const
      {
         return m_abEvent[0] & 0x0f;
      }

      void event_v1::_SetTrack(BYTE bValue)
      {
         m_abEvent[0] &= 0xf0;
         m_abEvent[0] |= bValue & 0x0f;
      }

      BYTE event_v1::_GetMetaType() const
      {
         return m_abEvent[1];
      }

      void event_v1::_SetMetaType(BYTE bValue)
      {
         m_abEvent[1] = bValue;
      }

      BYTE event_v1::_GetChB1() const
      {
         return m_abEvent[1];
      }

      void event_v1::_SetChB1(BYTE bValue)
      {
         m_abEvent[1] = bValue;
      }

      BYTE event_v1::_GetChB2() const
      {
         return m_abEvent[2];
      }

      void event_v1::_SetChB2(BYTE bValue)
      {
         m_abEvent[2] = bValue;
      }

      BYTE event_v1::_GetNotePitch() const
      {
         return m_abEvent[1];
      }

      void event_v1::_SetNotePitch(BYTE bValue)
      {
         m_abEvent[1] = bValue;
      }

      BYTE event_v1::_GetNoteVelocity() const
      {
         return m_abEvent[2];
      }

      void event_v1::_SetNoteVelocity(BYTE bValue)
      {
         m_abEvent[2] = bValue;
      }

      byte * event_v1::_GetImage() const
      {
         return m_hpbImage;
      }

      void event_v1::_SetImage(byte * hpbImage)
      {
         m_hpbImage = hpbImage;
      }

      ::primitive::memory_size event_v1::_GetImageSize() const
      {
         return m_cbImage;
      }

      void event_v1::_SetImageSize(::primitive::memory_size cbImage)
      {
         m_cbImage = (uint32_t) cbImage;
      }

      void event_v1::_SetParam(void * pVoid, ::primitive::memory_size iSize)
      {
         m_hpbParm = (byte *) pVoid;
         m_cbParm = (uint32_t) iSize;
      }

      byte * event_v1::_GetParam() const
      {
         return m_hpbParm;
      }

      ::primitive::memory_size event_v1::_GetParamSize() const
      {
         return m_cbParm;
      }

      int32_t event_v1::_GetFlags() const
      {
         return m_iFlags;
      }

      void event_v1::_SetFlags(int32_t iFlags)
      {
         m_iFlags = iFlags;
      }


   } // namespace midi


} // namespace music









