#ifndef __LV_GUI_H_
#define __LV_GUI_H_

#include "lvgl/lvgl.h"
#include "lv_gui_conf.h"

lv_obj_t *chBar;
int32_t value;
extern char *__CHLabelShort[];

void lv_styles_create(void);

void lv_screen_change(lv_obj_t *screen);

void lv_gui_create(void);

void lv_gui();

#endif