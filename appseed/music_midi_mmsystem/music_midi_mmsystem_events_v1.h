#pragma once


namespace music
{


   namespace midi
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA events_v1
      {
      public:


         imedia::position_array        m_tkaEventsPosition;
         uint_ptr_array                m_dwaEventsData;



         events_v1();
         virtual ~events_v1();


         ::count get_size();

         imedia::position get_position(::index iIndex);
         void GetEvent(event_v1 * pEvent, ::index iIndex);


         void InsertEvent(::index iIndex, const event_v1 & event);
         void AddEvent(event_v5 * pevent);
         void AddEvent(event_base * pevent, imedia::position tkPosition);
         void AddEvent(event_base * pevent);

         void copy(events_v1 &events);


         void remove_all();


      };


   } // namespace midi


} // namespace music




