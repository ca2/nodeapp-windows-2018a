#pragma once


#include "app-veriwell/appseed/multimedia/multimedia/multimedia.h"
#include <mmsystem.t.h>


#if defined(_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM_LIBRARY)
   #define CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM  CLASS_DECL_IMPORT
#endif



#include "music_midi_mmsystem_enum.h"
#include "music_midi_mmsystem_definition.h"

#include "music_midi_mmsystem_mmsystem_translation.h"




#include "music_midi_mmsystem_tempo_map.h"

#include "music_midi_mmsystem_exception.h"

#include "music_midi_mmsystem_attribute_message.h"
#include "music_midi_mmsystem_midi_listener.h"
#include "music_midi_mmsystem_midi_listener_set.h"
          
#include "music_midi_mmsystem_sequence.h"

#include "music_midi_mmsystem_player_notify_event.h"
#include "music_midi_mmsystem_player_callback_interface.h"
#include "music_midi_mmsystem_player_window.h"
#include "music_midi_mmsystem_player_callback.h"
#include "music_midi_mmsystem_player_interface.h"
#include "music_midi_mmsystem_player_command.h"
#include "music_midi_mmsystem_player.h"
#include "music_midi_mmsystem_exception.h"
#include "music_midi_mmsystem_file.h"
          
#include "music_midi_mmsystem_view_helper.h"

#include "music_midi_mmsystem_midi.h"


#include "music_midi_mmsystem_event_base.h"
#include "music_midi_mmsystem_event_v1.h"
#include "music_midi_mmsystem_event_v8.h"
#include "music_midi_mmsystem_event_v7.h"
#include "music_midi_mmsystem_event_v5.h"
#include "music_midi_mmsystem_event_v2.h"



#include "music_midi_mmsystem_events_v1.h"

#include "music_midi_mmsystem_track_base.h"
#include "music_midi_mmsystem_track_v8.h"
#include "music_midi_mmsystem_track.h"
#include "music_midi_mmsystem_lyric_track.h"

#include "music_midi_mmsystem_tracks.h"


#include "music_midi_mmsystem_util.h"
#include "music_midi_mmsystem_sequence_thread.h"
