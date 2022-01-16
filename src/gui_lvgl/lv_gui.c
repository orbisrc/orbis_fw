#include "lv_gui.h"
#include "lvgl/lvgl.h"
#include "lv_gui_servoview.h"

extern lv_obj_t *chBar;

void lv_gui_init(void)
{
    lv_init();
    lv_port_disp_init();

    lv_gui_servoview();
}

void lv_gui()
{
    static int32_t value = 0;
    static int32_t increment = 1;
    // lv_bar_set_value(chBar,  value++, LV_ANIM_OFF);
    // if (value > 125) value = -125;
    value += increment;
    for (uint16_t i = 0; i < NUMBER_OF_CHANNELS - 1; i++)
    {
        lv_ch_list_set_channel_value(i, value);
    }
    if (value > 125)
        increment = -1;
    if (value < -125)
        increment = 1;
}