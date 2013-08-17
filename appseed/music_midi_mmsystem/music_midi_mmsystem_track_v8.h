#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class event_v1;


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM track_v8
      {
      public:


         CHUNKHDR                            m_chunkHeader;    
         event_v5                            m_eventAdd;
         array<event_v5>                   m_speventa;


         track_v8();
         virtual ~track_v8();

         index GetLastEventIndex();
         event_v5 & LastEvent();
         void clear();
         void remove_at(index iIndex);
         event_v5 & EventAt(index iIndex);
         event_v5 EventAt(index iIndex) const;
         ::count GetEventCount() const;
         void remove_all();

         void add(event_v1 & event);
         void add(event_v5 & event);
         void add(imedia::position tkDelta, event_v5 & event);
         void add(event_base & event);
         void insert_at(index iIndex, event_v5 & event);
         void insert_at(index iIndex, imedia::position tkDelta, event_v5 & event);

         track_v8 & operator = (const track_v8 &eventSrc);

      };


   } // namespace midi_mmsystem


} // namespace music





