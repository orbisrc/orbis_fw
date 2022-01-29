#ifndef __LV_GUI_COMMON_H_
#define __LV_GUI_COMMON_H_

#include "lvgl/lvgl.h"

lv_obj_t *lv_button_back(lv_obj_t *parent, lv_event_cb_t event_cb);

lv_obj_t *lv_button(lv_obj_t *parent, lv_event_cb_t event_cb, const char *title);

lv_obj_t *lv_label(lv_obj_t *parent, lv_text_align_t value, lv_event_cb_t event_cb, const char *ini);

lv_obj_t *lv_title(lv_obj_t *parent, const char *title);

#endif