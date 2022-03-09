#include "lv_gui_ai.h"
#include "lv_gui_common.h"
#include "lv_gui.h"
#include "lv_gui_main_screen.h"
#include "lv_gui_basic_multi_desc.h"
#include "core/multiprotocol.h"

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_screen_change(lv_gui_main_screen());
    }
}

static void beeper_switch_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        /* code will be here */
    }
}

static void trim_beeper_switch_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        /* code will be here */
    }
}

static void usb_mode_dd_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        /* code will be here */
    }
}

static void protocol_mode_dd_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t *obj = lv_event_get_target(e);
        uint16_t selected = lv_dropdown_get_selected(obj);

        /* code will be here */
    }
}

static void multi_protocol_mode_dd_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t *obj = lv_event_get_target(e);
        lv_obj_t *multi_sub_protocols = lv_obj_get_user_data(obj);
        uint16_t selected = lv_dropdown_get_selected(obj);

        multiprotocolSetProtocol(selected, &sbus);
        multiprotocolSetSubProtocol(0, &sbus);
        settings_changed();

        if (sub_protocols_opts[selected] != NULL)
        {
            lv_obj_clear_flag(multi_sub_protocols, LV_OBJ_FLAG_HIDDEN);
            lv_dropdown_set_options(multi_sub_protocols, sub_protocols_opts[selected]);
            lv_dropdown_set_selected(multi_sub_protocols, multiprotocolGetSubProtocol(&sbus));
        }
        else
        {
            lv_dropdown_set_selected(multi_sub_protocols, multiprotocolGetSubProtocol(&sbus));
            lv_obj_add_flag(multi_sub_protocols, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void multi_sub_protocol_mode_dd_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t *obj = lv_event_get_target(e);
        uint16_t selected = lv_dropdown_get_selected(obj);
        multiprotocolSetProtocol(selected, &sbus);
        settings_changed();
    }
}

static void finish_bind_handler(lv_obj_t *obj)
{
    multiprotocolBindDisable(&sbus);
}

static void bind_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        multiprotocolBindEnable(&sbus);
        lv_info_box(e->current_target->parent, finish_bind_handler, "Binding......");
    }
}

static lv_obj_t *lv_current_dropdawn(lv_obj_t *parent, lv_event_cb_t event_cb, const char *items)
{
    lv_obj_t *dd = lv_gui_dropdown(parent, event_cb, items);
    lv_obj_set_size(dd, 124, 32);

    return dd;
}

static lv_obj_t *lv_current_label(lv_obj_t *parrent, const char *text)
{
    lv_obj_t *label = lv_label(parrent, LV_TEXT_ALIGN_LEFT, NULL, text);
    lv_obj_set_size(label, 98, 22);

    return label;
}

lv_obj_t *lv_gui_basic(void)
{
    lv_multi_desc_init();

    lv_obj_t *screen = lv_screen("Basic settings", back_button_handler);

    lv_obj_t *beeper_label = lv_current_label(screen, "Beeper");
    lv_obj_t *beeper_switch = lv_gui_switch(screen, beeper_switch_handler);

    lv_obj_t *trim_beeper_label = lv_current_label(screen, "Trim beep");
    lv_obj_t *trim_beeper_switch = lv_gui_switch(screen, trim_beeper_switch_handler);

    static const char *usb_mode_opts = "COM\n"
                                       "Joystick\n"
                                       "Media";

    static const char *tx_protocol_opts = "PPM\n"
                                          "Multi";

    lv_obj_t *usb_mode_label = lv_current_label(screen, "USB mode");
    lv_obj_t *usb_mode_dd = lv_current_dropdawn(screen, usb_mode_dd_handler, usb_mode_opts);
    lv_obj_t *protocol_label = lv_current_label(screen, "TX");
    lv_obj_t *bind_button = lv_button(screen, bind_button_handler, "Bind");
    lv_obj_t *protocols = lv_current_dropdawn(screen, protocol_mode_dd_handler, tx_protocol_opts);
    lv_dropdown_set_selected(protocols, 1);
    lv_obj_t *multi_protocols = lv_current_dropdawn(screen, multi_protocol_mode_dd_handler, multi_protocols_opts);
    lv_obj_t *multi_sub_protocols = lv_current_dropdawn(screen, multi_sub_protocol_mode_dd_handler, sub_protocols_opts[1]);
    lv_dropdown_set_selected(multi_protocols, multiprotocolGetProtocol(&sbus));
    lv_obj_set_user_data(multi_protocols, multi_sub_protocols);
    if (sub_protocols_opts[multiprotocolGetProtocol(&sbus)] == NULL)
    {
        lv_obj_add_flag(multi_sub_protocols, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_dropdown_set_options(multi_sub_protocols, sub_protocols_opts[multiprotocolGetProtocol(&sbus)]);
        lv_dropdown_set_selected(multi_sub_protocols, multiprotocolGetSubProtocol(&sbus));
    }

    lv_obj_align_to(beeper_label, screen, LV_ALIGN_TOP_LEFT, GUI_MARGIN * 2, GUI_SCREEN_START_MARGIN * 1.5);
    lv_obj_align_to(beeper_switch, beeper_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(trim_beeper_label, beeper_label, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, GUI_MARGIN * 2);
    lv_obj_align_to(trim_beeper_switch, trim_beeper_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(usb_mode_label, trim_beeper_label, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, GUI_MARGIN * 2);
    lv_obj_align_to(usb_mode_dd, usb_mode_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(protocol_label, usb_mode_label, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, GUI_MARGIN * 3);
    lv_obj_align_to(protocols, protocol_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);
    lv_obj_align_to(bind_button, protocols, LV_ALIGN_OUT_LEFT_MID, -GUI_MARGIN, 0);
    lv_obj_align_to(multi_protocols, protocols, LV_ALIGN_OUT_BOTTOM_MID, 0, GUI_MARGIN);
    lv_obj_align_to(multi_sub_protocols, multi_protocols, LV_ALIGN_OUT_BOTTOM_MID, 0, GUI_MARGIN);

    return screen;
}