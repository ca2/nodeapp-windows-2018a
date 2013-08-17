#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM midi_listener_set :
         public ::ca2::signalizable_array < midi_listener >
      {
      public:


         ::ca2::signal           m_signalAttributeChange;


         void send_attribute_change(e_attribute eattribute)
         {

            attribute_message change(&m_signalAttributeChange);

            change.m_eattribute = eattribute;

            m_signalAttributeChange.emit(&change);

         }

         virtual void install_handlers(::ca2::signalizable * psignalizable)
         {

            m_signalAttributeChange.connect(dynamic_cast < midi_listener * > (psignalizable), &midi_listener::on_attribute_change);

         }


      };


   } // namespace midi_mmsystem


} // namespace music






