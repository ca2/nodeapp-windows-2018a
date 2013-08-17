#pragma once



#include "app-core/appseed/multimedia/multimedia/multimedia.h"
#include <mmsystem.t.h>


#if defined(_AUDIO_MIXER_MMSYSTEM_LIBRARY)
   #define CLASS_DECL_AUDIO_MIXER_MMSYSTEM  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_AUDIO_MIXER_MMSYSTEM  CLASS_DECL_IMPORT
#endif





namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class callback;
      class device;
      class source;
      class destination;
      class thread;
      class audio_mixer;
      class section;


   } // namespace audio_mixer_mmsystem


} // namespace multimedia


#include "audio_mixer_mmsystem_control_data.h"
#include "audio_mixer_mmsystem_control_data_volume.h"
#include "audio_mixer_mmsystem_control_data_switch.h"


#include "audio_mixer_mmsystem_user_control.h"
#include "audio_mixer_mmsystem_user_level_control.h"
#include "audio_mixer_mmsystem_user_toggle_control.h"
#include "audio_mixer_mmsystem_user_label.h"


#include "audio_mixer_mmsystem_window.h"
#include "audio_mixer_mmsystem_mixer.h"
#include "audio_mixer_mmsystem_control.h"
#include "audio_mixer_mmsystem_source.h"
#include "audio_mixer_mmsystem_destination.h"
#include "audio_mixer_mmsystem_device.h"
#include "audio_mixer_mmsystem_line_info.h"


#include "audio_mixer_mmsystem_callback.h"


#include "audio_mixer_mmsystem_thread.h"          


#include "audio_mixer_mmsystem_section.h"



