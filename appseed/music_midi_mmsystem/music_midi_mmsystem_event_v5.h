#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class event_v1;


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM event_v5 :
         public event_v7
      {
      protected:
         imedia::position          m_tkPosition;
      public:
         event_v5();
         event_v5(const ::music::midi::event_base &eventSrc);
         event_v5(const event_v1 &eventSrc);
         event_v5(const event_v5 &eventSrc);
         virtual ~event_v5();

         virtual ::music::midi::event_base & operator = (const ::music::midi::event_base & eventSrc) { return ::music::midi::event_base::operator = (eventSrc); }
         virtual ::music::midi::event_base & operator = (const event_v5 & eventSrc) { return ::music::midi::event_base::operator = (eventSrc); }
         virtual event_v8 & operator = (const event_v8 & eventSrc) { return event_v8::operator = (eventSrc); }
         virtual event_v7 & operator = (const event_v7 & eventSrc) { return event_v7::operator = (eventSrc); }
         virtual event_v5 & operator = (const event_v1 & eventSrc);

         imedia::position get_position() const;
         void SetPosition(imedia::position tkPosition);
         imedia::position GetDelta() const;
         void SetDelta(imedia::position tkDelta);
         BYTE GetFullType() const;
         void SetFullType(BYTE bValue);
         BYTE get_type() const;
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

         imedia::position _GetPosition() const;
         void _SetPosition(imedia::position tkPosition);

         byte * _GetParam() const;
         ::primitive::memory_size _GetParamSize() const;

#ifndef DEBUG
         void * operator new(size_t st);
         void operator delete (void *);
#endif

      };


   } // namespace midi_mmsystem


} // namespace music





