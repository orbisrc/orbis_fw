#ifndef __LV_GUI_SERVOVIEW_H_
#define __LV_GUI_SERVOVIEW_H_

#include <stdint.h>
#include "lvgl/lvgl.h"
#include "lv_gui_conf.h"

void lv_gui_servoview(void);

void lv_ch_list_set_channel_value(uint16_t ch, int32_t value);

#endif