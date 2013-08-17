#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      track_v8::track_v8()
      {
      }

      track_v8::~track_v8()
      {

      }

      track_v8 & track_v8::operator =(const track_v8 &eventSrc)
      {
         
         m_chunkHeader  = eventSrc.m_chunkHeader;
         
         m_speventa = eventSrc.m_speventa;
         
         return *this;

      }

      void track_v8::insert_at(index iIndex, event_v5 & event)
      {
         m_speventa.insert_at(iIndex, event);
      }

      void track_v8::insert_at(index iIndex, imedia::position tkDelta, event_v5 & event)
      {
         m_speventa.insert_at(iIndex, event);
         m_speventa.element_at(iIndex)._SetDelta(tkDelta);
      }

      void track_v8::remove_all()
      {
         m_speventa.remove_all();
      }

      void track_v8::remove_at(index iIndex)
      {
         ASSERT(iIndex >= 0);
         ASSERT(iIndex < m_speventa.get_size());
         m_speventa.remove_at(iIndex);
      }

      void track_v8::clear()
      {
         remove_all();
      }




      ::count track_v8::GetEventCount() const
      {
         return m_speventa.get_size();
      }

      event_v5 & track_v8::EventAt(index iIndex)
      {
         ASSERT(iIndex >= 0);
         ASSERT(iIndex < m_speventa.get_size());
         return m_speventa.element_at(iIndex);
      }

      event_v5 track_v8::EventAt(index iIndex) const
      {
         ASSERT(iIndex >= 0);
         ASSERT(iIndex < m_speventa.get_size());
         return m_speventa.element_at(iIndex);
      }

      void track_v8::add(event_base &event)
      {
         m_eventAdd = event;
         add(m_eventAdd);
      }

      void track_v8::add(event_v1 &event)
      {
         m_eventAdd = event;
         add(m_eventAdd);
      }

      void track_v8::add(event_v5 &event)
      {
         insert_at(GetEventCount(), event);
      }

      void track_v8::add(imedia::position tkDelta, event_v5 &event)
      {
         insert_at(GetEventCount(), tkDelta, event);
      }

      event_v5 & track_v8::LastEvent()
      {
         ASSERT(GetEventCount() > 0);
         return EventAt(GetLastEventIndex());
      }

      ::count track_v8::GetLastEventIndex()
      {
         return GetEventCount() - 1;
      }


   } // namespace midi_mmsystem


} // namespace music









