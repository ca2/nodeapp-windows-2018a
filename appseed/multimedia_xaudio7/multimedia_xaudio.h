#pragma once

#include <sdkddkver.h>

#define _WIN32_WINNT _WIN32_WINNT_WIN8


#include "app-core/appseed/multimedia/multimedia/multimedia.h"
//#include <Mmreg.h>
#include <xaudio2.h>



#if defined(_MULTIMEDIA_XAUDIO_LIBRARY)
   #define CLASS_DECL_MULTIMEDIA_XAUDIO  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_MULTIMEDIA_XAUDIO  CLASS_DECL_IMPORT
#endif


#include "multimedia_xaudio_translation.h"


#include "multimedia_xaudio_factory_exchange.h"