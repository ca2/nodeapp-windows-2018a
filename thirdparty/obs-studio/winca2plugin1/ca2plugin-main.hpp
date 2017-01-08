#pragma once

struct ca2pluginMain_private;

class ca2pluginMain
{
	public:
	static bool init();
	static void deinit();

	static obs_properties_t *properties();
	static void defaults(obs_data_t *settings);

	ca2pluginMain(obs_data_t *settings, obs_source_t *source);
	~ca2pluginMain();

	void updateSettings(obs_data_t *settings);

	void tick(float seconds);
	void render(gs_effect_t *effect);

	uint32_t width();
	uint32_t height();

	ca2pluginMain_private *p;
	private:
};
