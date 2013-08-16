#pragma once


#ifdef LINUX
#include <alsa/asoundlib.h>
#endif


#include "app-core/appseed/multimedia/iaudio/iaudio.h"
#include "app-core/appseed/multimedia/audio_decode/audio_decode.h"
#include "math/math/math.h" // kissfft


#include "app-core/appseed/multimedia/ortp/ortp.h"
#include "app-core/appseed/multimedia/srtp/srtp.h"


#include "audio_global.h"
#include "audio_command.h"


#include "audio_wave_player_command.h"
#include "audio_wave_recorder_command.h"


#include "audio_wave_buffer.h"
#include "audio_wave_file.h"



#include "audio_wave_in_listener.h"
#include "audio_wave_in_listener_set.h"

#include "audio_wave_player.h"
#include "audio_wave_player_container.h"
#include "audio_wave_pre_buffer.h"
#include "audio_wave_out.h"
#include "audio_wave_recorder.h"
#include "audio_wave_recorder_container.h"
#include "audio_wave_in.h"

#include "audio_wave_effect_crescendo.h"
#include "audio_wave_effect_devocalizer.h"


#include "audio_wave.h"