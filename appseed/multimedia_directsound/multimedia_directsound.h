#pragma once


#include "app-core/appseed/multimedia/multimedia/multimedia.h"
#include <dsound.h>


#if defined(_MULTIMEDIA_DIRECTSOUND_LIBRARY)
   #define CLASS_DECL_MULTIMEDIA_DIRECTSOUND  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_MULTIMEDIA_DIRECTSOUND  CLASS_DECL_IMPORT
#endif


#include "multimedia_directsound_translation.h"


#include "multimedia_directsound_factory_exchange.h"