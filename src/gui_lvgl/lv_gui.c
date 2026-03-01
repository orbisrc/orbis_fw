#include "lv_gui.h"
#include "lvgl/lvgl.h"
#include "lv_gui_servoview.h"
#include "lv_gui_main_screen.h"
#include "lv_gui_styles.h"
#include "lv_port_disp.h"
#include "core/iosettings.h"
#include "stconfig.h"

#ifdef __ORBIS_AIR__
const char *CHLabel[] = {"Aileron",
						 "Elevator",
						 "Throttle",
						 "Rudder",
						 "CH5",
						 "CH6",
						 "CH7",
						 "CH8",
						 "CH9",
						 "CH10",
						 "CH11",
						 "CH12",
						 "CH13",
						 "CH14",
						 "CH15",
						 "CH16"};

const char *CHLabelShort[] = {"Ail",
							  "Ele",
							  "Thr",
							  "Rud",
							  "CH5",
							  "CH6",
							  "CH7",
							  "CH8",
							  "CH9",
							  "CH10",
							  "CH11",
							  "CH12",
							  "CH13",
							  "CH14",
							  "CH15",
							  "CH16"};

const char *channelsDD = "Aileron \n"
						 "Elevon\n"
						 "Throttle\n"
						 "Rudder\n"
						 "CH5\n"
						 "CH6\n"
						 "CH7\n"
						 "CH8\n"
						 "CH9\n"
						 "CH10\n"
						 "CH11\n"
						 "CH12\n"
						 "CH13\n"
						 "CH14\n"
						 "CH15\n"
						 "CH16";

const char *optionsDD = "A\n"
						"E\n"
						"T\n"
						"R\n"
						"PT1\n"
						"PT2\n"
						"PT3\n"
						"PT4\n"
						"SWA\n"
						"SWB\n"
						"SWC\n"
						"SWD\n"
						"SWE\n"
						"SWF\n"
						"SWG\n"
						"SWH\n"
						"SWI\n"
						"SWJ\n"
						"S1\n"
						"S2\n"
						"S3\n"
						"S4\n"
						"S5\n"
						"S6\n"
						"S7\n"
						"S8\n"
						"S9\n"
						"S10\n"
						"S11\n"
						"S12\n"
						"S13\n"
						"S14";

#endif

#ifdef __ORBIS_SURFACE__
const char *CHLabel[] = {"ST",
						 "TH",
						 "CH3",
						 "CH4",
						 "CH5",
						 "CH6",
						 "CH7",
						 "CH8",
						 "CH9",
						 "CH10",
						 "CH11",
						 "CH12",
						 "CH13",
						 "CH14",
						 "CH15",
						 "CH16"};

const char *CHLabelShort[] = {"ST",
							  "TH",
							  "CH3",
							  "CH4",
							  "CH5",
							  "CH6",
							  "CH7",
							  "CH8",
							  "CH9",
							  "CH10",
							  "CH11",
							  "CH12",
							  "CH13",
							  "CH14",
							  "CH15",
							  "CH16"};

const char *channelsDD = "ST \n"
						 "TH\n"
						 "INA3\n"
						 "INA4\n"
						 "CH5\n"
						 "CH6\n"
						 "CH7\n"
						 "CH8\n"
						 "CH9\n"
						 "CH10\n"
						 "CH11\n"
						 "CH12\n"
						 "CH13\n"
						 "CH14\n"
						 "CH15\n"
						 "CH16";

const char *optionsDD = "ST\n"
						"TH\n"
						"INA3\n"
						"INA4\n"
						"PT1\n"
						"PT2\n"
						"PT3\n"
						"PT4\n"
						"SWA\n"
						"SWB\n"
						"SWC\n"
						"SWD\n"
						"SWE\n"
						"SWF\n"
						"SWG\n"
						"SWH\n"
						"SWI\n"
						"SWJ\n"
						"S1\n"
						"S2\n"
						"S3\n"
						"S4\n"
						"S5\n"
						"S6\n"
						"S7\n"
						"S8\n"
						"S9\n"
						"S10\n"
						"S11\n"
						"S12\n"
						"S13\n"
						"S14";
#endif

static uint16_t change_settings_counter = 0;
static uint16_t prev_change_settings_counter = 0;

static void settings_update_handler(lv_timer_t *timer)
{
	if (change_settings_counter == prev_change_settings_counter && prev_change_settings_counter != 0)
	{
		STrequestSettingsSave();
		change_settings_counter = 0;
	}

	prev_change_settings_counter = change_settings_counter;
}

void settings_changed(void)
{
	change_settings_counter++;
}

void lv_screen_change(lv_obj_t *screen)
{
	lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
}

void lv_styles_create(void)
{
	lv_styles_init();
}

void lv_gui_create(void)
{
	lv_init();
	lv_port_disp_init();

	lv_styles_create();

	lv_scr_load(lv_gui_main_screen());

	lv_timer_create(settings_update_handler, 5000, NULL);
}

void lv_gui()
{
}