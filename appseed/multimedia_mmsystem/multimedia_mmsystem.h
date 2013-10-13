#pragma once


#include "app-core/appseed/multimedia/multimedia/multimedia.h"
#include <mmsystem.h>


#if defined(_MULTIMEDIA_MMSYSTEM_LIBRARY)
   #define CLASS_DECL_MULTIMEDIA_MMSYSTEM  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_MULTIMEDIA_MMSYSTEM  CLASS_DECL_IMPORT
#endif


#include "multimedia_mmsystem_translation.h"


#include "multimedia_mmsystem_factory_exchange.h"