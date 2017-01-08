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
   struct window_capture *wc = data;
   obs_source_t *transition = get_transition(wc);
   bool success;

   if (!transition)
      return false;

   success = ss_audio_render_(transition, ts_out, audio_output, mixers,
      channels, sample_rate);

   obs_source_release(transition);
   return success;
}
