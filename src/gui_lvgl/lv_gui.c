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
#endif

uint16_t change_settings_counter;

static void settings_update_handler(lv_timer_t *timer)
{
	static uint16_t prev_change_settings_counter = 0;

	if (change_settings_counter == prev_change_settings_counter && prev_change_settings_counter != 0)
	{
		STsaveSettingsToFlash();
		prev_change_settings_counter = 0;
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