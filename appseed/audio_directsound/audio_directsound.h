#pragma once


#include "nodeapp/appseed/multimedia_directsound/multimedia_directsound.h"


#if defined(_AUDIO_DIRECTSOUND_LIBRARY)
   #define CLASS_DECL_AUDIO_DIRECTSOUND  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_AUDIO_DIRECTSOUND  CLASS_DECL_IMPORT
#endif


#include "audio_directsound_wave_out.h"
#include "audio_directsound_wave_in.h"


#include "audio_directsound_factory_exchange.h"