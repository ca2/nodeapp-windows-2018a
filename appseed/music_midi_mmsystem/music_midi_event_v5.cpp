#include "framework.h"


namespace music
{


   namespace midi
   {


      //extern uint32_t grbChanMsgLen[];

#define ROUND(x,y) (((x)+(y-1))&~(y-1))
#define ROUND4(x) ROUND(x, 4)

      __STATIC fixed_alloc _vmsMidiEventV008Alloc(ROUND4(sizeof(event_v5)), ROUND4(1024));


      event_v5::event_v5()
      {
      }

      event_v5::event_v5(const event_base &eventSrc)
      {
         operator =(eventSrc);
      }

      event_v5::event_v5(const event_v1 &eventSrc)
      {
         operator =(eventSrc);
      }

      event_v5::event_v5(const event_v5 &eventSrc)
      {
         operator =(eventSrc);
      }

      event_v5::~event_v5()
      {

      }

      /*event_base & event_v5::operator=(const event_base &eventSrc)
      {
      _SetFullType   (eventSrc.GetFullType());
      _SetChB1       (eventSrc.GetChB1());
      _SetChB2       (eventSrc.GetChB2());
      SetParam      (eventSrc.GetParam(), eventSrc.GetParamSize());
      _SetDelta      (eventSrc.GetDelta());
      _SetPosition   (eventSrc.get_position());
      _SetFlags      (eventSrc.GetFlags());
      return *this;
      }
      */

      event_v5 & event_v5::operator = (const event_v1 & eventSrc)
      {
         _SetFullType   (eventSrc._GetFullType());
         _SetChB1       (eventSrc._GetChB1());
         _SetChB2       (eventSrc._GetChB2());
         SetParam      (eventSrc._GetParam(), eventSrc._GetParamSize());
         _SetDelta      (eventSrc._GetDelta());
         _SetPosition   (eventSrc._GetPosition());
         _SetFlags      (eventSrc._GetFlags());

         // _SetImage      (eventSrc._GetImage());
         // _SetImageSize  (eventSrc._GetImageSize());
         return *this;
      }


      /*event_v5 & event_v5::operator =(const event_v5 &eventSrc)
      {
      _SetFullType   (eventSrc._GetFullType());
      _SetChB1       (eventSrc._GetChB1());
      _SetChB2       (eventSrc._GetChB2());
      _SetDelta      (eventSrc._GetDelta());
      _SetPosition   (eventSrc._GetPosition());
      _SetFlags      (eventSrc._GetFlags());
      SetParam      (eventSrc._GetParam(), eventSrc._GetParamSize());

      //   _SetImage(eventSrc._GetImage());
      //   _SetImageSize(eventSrc._GetImageSize());
      return *this;
      }*/

#ifndef DEBUG


      void * event_v5::operator new(size_t st)
      {
         return _vmsMidiEventV008Alloc.Alloc();
      }

      void event_v5::operator delete (void * p)
      {
         _vmsMidiEventV008Alloc.Free(p);
      }

#endif DEBUG



      imedia::position event_v5::get_position() const
      {
         return m_tkPosition;
      }
      void event_v5::SetPosition(imedia::position tkPosition)
      {
         m_tkPosition = tkPosition;
      }

      imedia::position event_v5::GetDelta() const
      {
         return m_tkDelta;
      }

      void event_v5::SetDelta(imedia::position tkDelta)
      {
         m_tkDelta = tkDelta;
      }


      BYTE event_v5::GetFullType() const
      {
         return m_lpbData[0];
      }
      void event_v5::SetFullType(BYTE bValue)
      {
         m_lpbData[0] = bValue;
      }
      BYTE event_v5::get_type() const
      {
         return m_lpbData[0] & 0xf0;
      }
      void event_v5::SetType_(BYTE bValue)
      {
         m_lpbData[0] &= 0x0f;
         m_lpbData[0] |= bValue & 0xf0;
      }
      BYTE event_v5::GetTrack() const
      {
         return m_lpbData[0] & 0x0f;
      }
      void event_v5::SetTrack(BYTE bValue)
      {
         m_lpbData[0] &= 0xf0;
         m_lpbData[0] |= bValue & 0x0f;
      }
      BYTE event_v5::GetMetaType() const
      {
         return m_lpbData[1];
      }
      void event_v5::SetMetaType(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }
      BYTE event_v5::GetChB1() const
      {
         return m_lpbData[1];
      }
      void event_v5::SetChB1(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }
      BYTE event_v5::GetChB2() const
      {
         return m_lpbData[2];
      }
      void event_v5::SetChB2(BYTE bValue)
      {
         m_lpbData[2] = bValue;
      }
      BYTE event_v5::GetNotePitch() const
      {
         return m_lpbData[1];
      }
      void event_v5::SetNotePitch(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }
      BYTE event_v5::GetNoteVelocity() const
      {
         return m_lpbData[2];
      }
      void event_v5::SetNoteVelocity(BYTE bValue)
      {
         m_lpbData[2] = bValue;
      }


      byte * event_v5::GetParam() const
      {
         return (byte *) m_lpdwData[2];
      }

      ::primitive::memory_size event_v5::GetParamSize() const
      {
         return m_lpdwData[1];
      }


      inline imedia::position event_v5::_GetPosition() const
      {
         return m_tkPosition;
      }
      inline void event_v5::_SetPosition(imedia::position tkPosition)
      {
         m_tkPosition = tkPosition;
      }

      byte * event_v5::_GetParam() const
      {
         return (byte *) m_lpdwData[2];
      }

      ::primitive::memory_size event_v5::_GetParamSize() const
      {
         return m_lpdwData[1];
      }




   } // namespace midi


} // namespace music









