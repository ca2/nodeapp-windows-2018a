#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class event_base;
      class event_v1;
      class event_v5;


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM track_base :
         virtual public ::ca2::object
      {
      public:


         enum e_type
         {

            TypeMidi,
            TypeLyric,

         };


         track_base(sp(::ca2::application) papp);
         virtual ~track_base();

         virtual bool IsType(e_type e_type);
         virtual e_type get_type() = 0;
         virtual e_file_result get_position(imedia::position &tkPosition, imedia::position tkMax) = 0;
         virtual file_flags & GetFlags() = 0;
         virtual e_file_result GetEvent(event_base * & pevent, imedia::position tkMax, bool bTkMaxInclusive) = 0;
         virtual e_file_result GetEvent(event_v1 * & pevent, imedia::position tkMax, bool bTkMaxInclusive) = 0;
         virtual imedia::position get_position() = 0;
         virtual e_file_result seek_begin() = 0;
         virtual e_file_result MoveNext() = 0;
         virtual imedia::position WorkGetPosition() = 0;
         virtual e_file_result WorkMoveNext() = 0;
         virtual void WorkGetEvent(event_base * & pevent, imedia::position tkMax, bool bTkMaxInclusive) = 0;
         virtual void WorkGetEvent(event_v5 * & pevent, imedia::position tkMax, bool bTkMaxInclusive) = 0;
         virtual bool WorkIsEOT() = 0;
         virtual e_file_result WorkSeekBegin() = 0;
         virtual e_file_result WorkSeekEnd() = 0;
      };


   } // namespace midi_mmsystem


} // namespace music




