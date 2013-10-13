#pragma once


#include "nodeapp/appseed/multimedia_mmsystem/multimedia_mmsystem.h"


#if defined(_AUDIO_MMSYSTEM_LIBRARY)
   #define CLASS_DECL_AUDIO_MMSYSTEM  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_AUDIO_MMSYSTEM  CLASS_DECL_IMPORT
#endif


#include "audio_mmsystem_wave_out.h"
#include "audio_mmsystem_wave_in.h"


#include "audio_mmsystem_factory_exchange.h"