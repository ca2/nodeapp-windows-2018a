#pragma once


#include "nodeapp/appseed/multimedia_xaudio7/multimedia_xaudio7.h"



#if defined(_AUDIO_XAUDIO7_LIBRARY)
   #define CLASS_DECL_AUDIO_XAUDIO7  CLASS_DECL_EXPORT
#else
#define  CLASS_DECL_AUDIO_XAUDIO7  CLASS_DECL_IMPORT
#endif


#include "audio_xaudio7_wave_out.h"
#include "audio_xaudio7_wave_in.h"


#include "audio_xaudio7_factory_exchange.h"