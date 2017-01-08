#include <obs-module.h>

#include "ca2plugin-main.hpp"

obs_source_t *get_transition(ca2pluginMain *ss);

static void* ca2plugin_create(obs_data_t *settings, obs_source_t *source)
{
	return new ca2pluginMain(settings, source);
}

static void ca2plugin_destroy(void *data)
{
	ca2pluginMain* cc = (ca2pluginMain*)data;
	delete cc;
}

static void ca2plugin_video_tick(void* data, float seconds)
{
	ca2pluginMain* cc = (ca2pluginMain*)data;
	cc->tick(seconds);
}

static void ca2plugin_video_render(void* data, gs_effect_t *effect)
{
	ca2pluginMain* cc = (ca2pluginMain*)data;
	cc->render(effect);
}

static uint32_t ca2plugin_getwidth(void* data)
{
	ca2pluginMain* cc = (ca2pluginMain*)data;
	return cc->width();
}

static uint32_t ca2plugin_getheight(void* data)
{
	ca2pluginMain* cc = (ca2pluginMain*)data;
	return cc->height();
}

static obs_properties_t *ca2plugin_props(void *unused)
{
	UNUSED_PARAMETER(unused);

	return ca2pluginMain::properties();
}

void ca2plugin_defaults(obs_data_t *settings)
{
	ca2pluginMain::defaults(settings);
}

void ca2plugin_update(void *data, obs_data_t *settings)
{
	ca2pluginMain* cc = (ca2pluginMain*)data;
	cc->updateSettings(settings);
}

static const char* ca2plugin_getname(void*)
{
	return obs_module_text("ca2plugin_capture");
}

static inline bool ss_audio_render_(obs_source_t *transition, uint64_t *ts_out,
   struct obs_source_audio_mix *audio_output,
   uint32_t mixers, size_t channels, size_t sample_rate)
{
   struct obs_source_audio_mix child_audio;
   uint64_t source_ts;

   if (obs_source_audio_pending(transition))
      return false;

   source_ts = obs_source_get_audio_timestamp(transition);
   if (!source_ts)
      return false;

   obs_source_get_audio_mix(transition, &child_audio);
   for (size_t mix = 0; mix < MAX_AUDIO_MIXES; mix++) {
      if ((mixers & (1 << mix)) == 0)
         continue;

      for (size_t ch = 0; ch < channels; ch++) {
         float *out = audio_output->output[mix].data[ch];
         float *in = child_audio.output[mix].data[ch];

         memcpy(out, in, AUDIO_OUTPUT_FRAMES *
            MAX_AUDIO_CHANNELS * sizeof(float));
      }
   }

   *ts_out = source_ts;

   UNUSED_PARAMETER(sample_rate);
   return true;
}

static bool ss_audio_render(void *data, uint64_t *ts_out,
   struct obs_source_audio_mix *audio_output,
   uint32_t mixers, size_t channels, size_t sample_rate)
{
	ca2pluginMain* wc = (ca2pluginMain*)data;
   obs_source_t *transition = get_transition(wc);
   bool success;

   if (!transition)
      return false;

   success = ss_audio_render_(transition, ts_out, audio_output, mixers,
      channels, sample_rate);

   obs_source_release(transition);
   return success;
}

extern "C" void xcomposite_load(void)
{
	if (!ca2pluginMain::init())
		return;

	obs_source_info sinfo;
	memset(&sinfo, 0, sizeof(obs_source_info));

	sinfo.id = "ca2plugin";
	sinfo.type = OBS_SOURCE_TYPE_INPUT;
	sinfo.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW | OBS_SOURCE_COMPOSITE;


	sinfo.audio_render = ss_audio_render;

	sinfo.get_name       = ca2plugin_getname;
	sinfo.create         = ca2plugin_create;
	sinfo.destroy        = ca2plugin_destroy;
	sinfo.get_properties = ca2plugin_props;
	sinfo.get_defaults   = ca2plugin_defaults;
	sinfo.update         = ca2plugin_update;
	sinfo.video_tick     = ca2plugin_video_tick;
	sinfo.video_render   = ca2plugin_video_render;
	sinfo.get_width      = ca2plugin_getwidth;
	sinfo.get_height     = ca2plugin_getheight;

	obs_register_source(&sinfo);
}

extern "C" void xcomposite_unload(void)
{
	ca2pluginMain::deinit();
}
