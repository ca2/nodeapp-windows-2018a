#pragma once


#include "nodeapp/appseed/multimedia_xaudio/multimedia_xaudio.h"



#if defined(_AUDIO_XAUDIO_LIBRARY)
   #define CLASS_DECL_AUDIO_XAUDIO  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_AUDIO_XAUDIO  CLASS_DECL_IMPORT
#endif


#include "audio_xaudio_wave_out.h"
#include "audio_xaudio_wave_in.h"


#include "audio_xaudio_factory_exchange.h"