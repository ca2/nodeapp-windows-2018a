#pragma once


namespace music
{


   namespace midi
   {


      class event_v1;


      class CLASS_DECL_VERIWELL_MULTIMEDIA event_v7 :
         public event_v8
      {
      protected:


         imedia::position m_tkDelta;


      public:

         event_v7(event_v7 & event);
         event_v7();
         virtual ~event_v7();

         virtual void clear();


         virtual ::music::midi::event_base & operator = (const ::music::midi::event_base & eventSrc) { return ::music::midi::event_base::operator = (eventSrc); }
         virtual ::music::midi::event_base & operator = (const event_v5 & eventSrc) { return ::music::midi::event_base::operator = (eventSrc); }
         virtual event_v8 & operator = (const event_v8 & eventSrc) { return event_v8::operator = (eventSrc); }
         virtual event_v7 & operator = (const event_v7 & eventSrc);


         virtual VMSRESULT WriteParam(LPVOID lpData, ::primitive::memory_size iSize);
         virtual VMSRESULT FromData(LPVOID lpData, LPINT lpiLeft, BYTE &bRunningStatus);
         VMSRESULT GetMidiEventV001(event_v1 * lpMidiEventV001,  uint32_t dwPositionTicks);

         imedia::position GetDelta() const;
         void SetDelta(imedia::position tkDelta);
         imedia::position get_position() const;
         void SetPosition(imedia::position tkPosition);
         BYTE GetFullType() const;
         void SetFullType(BYTE bValue);
         BYTE GetType_() const;
         void SetType_(BYTE bValue);
         BYTE GetTrack() const;
         void SetTrack(BYTE bValue);
         BYTE GetMetaType() const;
         void SetMetaType(BYTE bValue);
         BYTE GetChB1() const;
         void SetChB1(BYTE bValue);
         BYTE GetChB2() const;
         void SetChB2(BYTE bValue);
         BYTE GetNotePitch() const;
         void SetNotePitch(BYTE bValue);
         BYTE GetNoteVelocity() const;
         void SetNoteVelocity(BYTE bValue);
         byte * GetParam() const;
         ::primitive::memory_size GetParamSize() const;

         imedia::position _GetDelta() const;
         void _SetDelta(imedia::position tkDelta);

      };

      inline imedia::position event_v7::GetDelta() const
      {
         return m_tkDelta;
      }

      inline void event_v7::SetDelta(imedia::position tkDelta)
      {
         m_tkDelta = tkDelta;
      }

      inline imedia::position event_v7::get_position() const
      {
         return 0xffffffff;
      }

      inline void event_v7::SetPosition(imedia::position tkPosition)
      {
         UNREFERENCED_PARAMETER(tkPosition);
         ASSERT(FALSE);
      }

      inline BYTE event_v7::GetFullType() const
      {
         return m_lpbData[0];
      }
      inline void event_v7::SetFullType(BYTE bValue)
      {
         m_lpbData[0] = bValue;
      }
      inline BYTE event_v7::GetType_() const
      {
         return m_lpbData[0] & 0xf0;
      }
      inline void event_v7::SetType_(BYTE bValue)
      {
         m_lpbData[0] &= 0x0f;
         m_lpbData[0] |= bValue & 0xf0;
      }
      inline BYTE event_v7::GetTrack() const
      {
         return m_lpbData[0] & 0x0f;
      }
      inline void event_v7::SetTrack(BYTE bValue)
      {
         m_lpbData[0] &= 0xf0;
         m_lpbData[0] |= bValue & 0x0f;
      }
      inline BYTE event_v7::GetMetaType() const
      {
         return m_lpbData[1];
      }
      inline void event_v7::SetMetaType(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }
      inline BYTE event_v7::GetChB1() const
      {
         return m_lpbData[1];
      }
      inline void event_v7::SetChB1(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }
      inline BYTE event_v7::GetChB2() const
      {
         return m_lpbData[2];
      }
      inline void event_v7::SetChB2(BYTE bValue)
      {
         m_lpbData[2] = bValue;
      }
      inline BYTE event_v7::GetNotePitch() const
      {
         return m_lpbData[1];
      }
      inline void event_v7::SetNotePitch(BYTE bValue)
      {
         m_lpbData[1] = bValue;
      }
      inline BYTE event_v7::GetNoteVelocity() const
      {
         return m_lpbData[2];
      }
      inline void event_v7::SetNoteVelocity(BYTE bValue)
      {
         m_lpbData[2] = bValue;
      }


      inline byte * event_v7::GetParam() const
      {
         return (byte *) m_lpdwData[2];
      }

      inline ::primitive::memory_size event_v7::GetParamSize() const
      {
         return m_lpdwData[1];
      }

      inline imedia::position event_v7::_GetDelta() const
      {
         return m_tkDelta;
      }

      inline void event_v7::_SetDelta(imedia::position tkDelta)
      {
         m_tkDelta = tkDelta;
      }


   } // namespace midi


} // namespace music





