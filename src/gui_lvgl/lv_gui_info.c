#include "lv_gui_info.h"
#include "lv_gui_conf.h"
#include "lv_gui.h"
#include "lv_gui_styles.h"
#include "lv_gui_main_screen.h"
#include "lv_gui_common.h"
#include "core/common.h"
#include "stconfig.h"

lv_indev_t *enc_dev;

static lv_obj_t *lv_msgbox_info(lv_obj_t *parent);

static void dfu_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");
    }
}

static void reset_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");
    }
}

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_screen_change(lv_gui_main_screen());
    }
}
static void set_main_loop_data(lv_obj_t *label)
{
    lv_label_set_text_fmt(label, "%d Hz", (uint16_t)CommonLoopGetFreq(&MainLoop));
}

static void set_adc_loop_data(lv_obj_t *label)
{
    lv_label_set_text_fmt(label, "%d Hz", (uint16_t)CommonLoopGetFreq(&ADCloop));
}

static void set_rc_loop_data(lv_obj_t *label)
{
    lv_label_set_text_fmt(label, "%d Hz", (uint16_t)CommonLoopGetFreq(&RCchLoop));
}

static void dfu_aply_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_current_target(e);

    LV_LOG_USER("Button %s clicked", lv_msgbox_get_active_btn_text(obj));
}

static lv_obj_t *lv_info_label(lv_obj_t *parent, const char *ini)
{
    lv_obj_t *label = lv_label(parent, LV_TEXT_ALIGN_LEFT, NULL, ini);
    lv_obj_set_width(label, 100);

    return label;
}

static lv_obj_t *lv_info_label_dynamic(lv_obj_t *parent, void *event_cb(lv_obj_t *label), const char *ini)
{
    lv_obj_t *label = lv_dynamic_label(parent, LV_TEXT_ALIGN_LEFT, event_cb, ini);
    lv_obj_set_width(label, 100);

    return label;
}

lv_obj_t *lv_gui_info(void)
{
    lv_obj_t *screen = lv_screen("Info", back_button_handler);

    lv_obj_t *hw_version = lv_info_label(screen, "HW version");
    lv_obj_t *sw_version = lv_info_label(screen, "SW version");
    lv_obj_t *date = lv_info_label(screen, "Date");

    lv_obj_t *hw_version_value = lv_info_label(screen, HW_VERSION);
    lv_obj_t *sw_version_value = lv_info_label(screen, SW_VERSION);
    lv_obj_t *date_value = lv_info_label(screen, SW_DATE);

    lv_obj_t *main_loop = lv_info_label(screen, "Main loop");
    lv_obj_t *adc_loop = lv_info_label(screen, "ADC loop");
    lv_obj_t *rc_loop = lv_info_label(screen, "RC loop");

    lv_obj_t *main_loop_value = lv_info_label_dynamic(screen, set_main_loop_data, "xxxx");
    lv_obj_t *adc_loop_value = lv_info_label_dynamic(screen, set_adc_loop_data, "xxxx");
    lv_obj_t *rc_loop_value = lv_info_label_dynamic(screen, set_rc_loop_data, "xxxx");

    lv_obj_t *reset_button = lv_button(screen, reset_button_handler, "Reset");
    lv_obj_t *dfu_button = lv_button(screen, dfu_button_handler, "DFU");

    lv_obj_align(hw_version, LV_ALIGN_TOP_LEFT, GUI_MARGIN * 2, GUI_SCREEN_START_MARGIN * 2);
    lv_obj_align_to(sw_version, hw_version, LV_ALIGN_OUT_BOTTOM_LEFT, 0, GUI_MARGIN);
    lv_obj_align_to(date, sw_version, LV_ALIGN_OUT_BOTTOM_LEFT, 0, GUI_MARGIN);

    lv_obj_align_to(hw_version_value, hw_version, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);
    lv_obj_align_to(sw_version_value, sw_version, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);
    lv_obj_align_to(date_value, date, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(main_loop, date, LV_ALIGN_OUT_BOTTOM_LEFT, 0, GUI_MARGIN);
    lv_obj_align_to(adc_loop, main_loop, LV_ALIGN_OUT_BOTTOM_LEFT, 0, GUI_MARGIN);
    lv_obj_align_to(rc_loop, adc_loop, LV_ALIGN_OUT_BOTTOM_LEFT, 0, GUI_MARGIN);

    lv_obj_align_to(main_loop_value, main_loop, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);
    lv_obj_align_to(adc_loop_value, adc_loop, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);
    lv_obj_align_to(rc_loop_value, rc_loop, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align(dfu_button, LV_ALIGN_BOTTOM_RIGHT, -GUI_MARGIN, -GUI_MARGIN);
    lv_obj_align(reset_button, LV_ALIGN_BOTTOM_LEFT, GUI_MARGIN, -GUI_MARGIN);

    return screen;
}