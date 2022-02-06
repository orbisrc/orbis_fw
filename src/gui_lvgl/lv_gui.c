#include "lv_gui.h"
#include "lvgl/lvgl.h"
#include "lv_gui_servoview.h"
#include "lv_gui_main_screen.h"
#include "lv_gui_styles.h"
#include "lv_port_disp.h"

char *CHLabelShort[] = {"Ail",
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
}

void lv_gui()
{
}