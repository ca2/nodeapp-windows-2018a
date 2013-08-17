#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      events_v1::events_v1()
      {

      }

      events_v1::~events_v1()
      {

      }

      void events_v1::GetEvent(event_v1 *pEvent, ::index iIndex)
      {

         uint32_t dw = (uint32_t) m_dwaEventsData.get_at(iIndex);

         memcpy(pEvent->m_abEvent , &dw, 3);

         pEvent->m_tkPosition = m_tkaEventsPosition.get_at(iIndex);

      }

      imedia::position events_v1::get_position(::index iIndex)
      {
         return m_tkaEventsPosition.get_at(iIndex);
      }

      /*void events_v1::AddEvent(event_v1 *pEvent)
      {
      uint32_t dw = 0;
      memcpy(&dw, pEvent->m_abEvent, 3);
      m_dwaEventsData.add(dw);
      m_tkaEventsPosition.add(pEvent->m_tkPosition);
      }*/
      void events_v1::AddEvent(event_base * pevent)
      {
         AddEvent(pevent, pevent->get_position());
      }

      void events_v1::AddEvent(event_base * pevent, imedia::position tkPosition)
      {
         uint32_t dw(0);
         dw |= pevent->GetFullType();
         dw |= pevent->GetChB1() << 8;
         dw |= pevent->GetChB2() << 16;
         m_dwaEventsData.add(dw);
         m_tkaEventsPosition.add(imedia::position(tkPosition));
      }

      void events_v1::AddEvent(event_v5 * pevent)
      {
         uint32_t dw(0);
         dw |= pevent->_GetFullType();
         dw |= pevent->_GetChB1() << 8;
         dw |= pevent->_GetChB2() << 16;
         m_dwaEventsData.add(dw);
         m_tkaEventsPosition.add(imedia::position(pevent->_GetPosition()));
      }

      ::count events_v1::get_size()
      {
         ASSERT(m_dwaEventsData.get_size() == m_tkaEventsPosition.get_size());
         return m_dwaEventsData.get_size();
      }

      void events_v1::copy(events_v1 &events)
      {
         ASSERT(events.m_dwaEventsData.get_size() ==
            events.m_tkaEventsPosition.get_size());
         m_dwaEventsData.copy(events.m_dwaEventsData);
         m_tkaEventsPosition.copy(events.m_tkaEventsPosition);

      }

      void events_v1::remove_all()
      {
         m_dwaEventsData.remove_all();
         m_tkaEventsPosition.remove_all();
      }

      void events_v1::InsertEvent(::index iIndex, const event_v1 &event)
      {

         uint32_t dw = 0;
         memcpy(&dw, event.m_abEvent, 3);

         m_dwaEventsData.insert_at(iIndex, dw);

         m_tkaEventsPosition.insert_at(iIndex, imedia::position(event.m_tkPosition));

      }


   } // namespace midi_mmsystem


} // namespace music









