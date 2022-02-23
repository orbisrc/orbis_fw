#include "lv_gui_ai.h"
#include "lv_gui_common.h"
#include "lv_gui.h"
#include "lv_gui_main_screen.h"

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_screen_change(lv_gui_main_screen());
    }
}

void beeper_switch_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        /* code will be here */
    }
}

void trim_beeper_switch_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        /* code will be here */
    }
}

static lv_obj_t *lv_gui_label(lv_obj_t *parrent, const char *text)
{
    lv_obj_t *label = lv_label(parrent, LV_TEXT_ALIGN_LEFT, NULL, text);
    lv_obj_set_size(label, 104, 22);

    return label;
}

void usb_mode_dd_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        /* code will be here */
    }
}

lv_obj_t *lv_gui_basic(void)
{
    lv_obj_t *screen = lv_screen("Basic settings", back_button_handler);

    lv_obj_t *beeper_label = lv_gui_label(screen, "Beeper");
    lv_obj_t *beeper_switch = lv_gui_switch(screen, beeper_switch_handler);

    lv_obj_align_to(beeper_label, screen, LV_ALIGN_TOP_LEFT, GUI_MARGIN * 2, GUI_SCREEN_START_MARGIN * 1.5);
    lv_obj_align_to(beeper_switch, beeper_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_t *trim_beeper_label = lv_gui_label(screen, "Trim beep");
    lv_obj_t *trim_beeper_switch = lv_gui_switch(screen, trim_beeper_switch_handler);

    lv_obj_align_to(trim_beeper_label, beeper_label, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, GUI_MARGIN * 2);
    lv_obj_align_to(trim_beeper_switch, trim_beeper_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    static const char *opts = "COM\n"
                              "Joystick\n"
                              "Media";
    lv_obj_t *usb_mode_label = lv_gui_label(screen, "USB mode");
    lv_obj_t *usb_mode_dd = lv_gui_dropdown(screen, usb_mode_dd_handler, opts);

    lv_obj_align_to(usb_mode_label, trim_beeper_label, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, GUI_MARGIN * 2);
    lv_obj_align_to(usb_mode_dd, usb_mode_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    return screen;
}