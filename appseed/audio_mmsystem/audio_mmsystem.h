#pragma once


#include "app-core/appseed/multimedia/multimedia/multimedia.h"
#include <mmsystem.t.h>


#if defined(_AUDIO_MMSYSTEM_LIBRARY)
   #define CLASS_DECL_AUDIO_MMSYSTEM  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_AUDIO_MMSYSTEM  CLASS_DECL_IMPORT
#endif


#include "audio_mmsystem_global.h"


#include "audio_mmsystem_wave_player_command.h"
#include "audio_mmsystem_wave_recorder_command.h"


#include "audio_mmsystem_wave_buffer.h"
#include "audio_mmsystem_wave_file.h"



#include "audio_mmsystem_wave_in_listener.h"
#include "audio_mmsystem_wave_in_listener_set.h"

#include "audio_mmsystem_wave_player.h"
#include "audio_mmsystem_wave_player_container.h"
#include "audio_mmsystem_wave_pre_buffer.h"
#include "audio_mmsystem_wave_out.h"
#include "audio_mmsystem_wave_recorder.h"
#include "audio_mmsystem_wave_recorder_container.h"
#include "audio_mmsystem_wave_in.h"

#include "audio_mmsystem_wave_effect_crescendo.h"
#include "audio_mmsystem_wave_effect_devocalizer.h"


#include "audio_mmsystem_wave.h"