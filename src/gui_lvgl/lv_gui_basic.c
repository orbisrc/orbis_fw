#include <stdio.h>
#include "lv_gui_ai.h"
#include "lv_gui_common.h"
#include "lv_gui.h"
#include "lv_gui_main_screen.h"
#include "core/multiprotocol.h"
#include "core/iosettings.h"

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_screen_change(lv_gui_main_screen());
    }

    settings_changed();
}

static void beeper_switch_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        /* code will be here */
    }
}

static void trim_beeper_switch_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t *obj = lv_event_get_target(e);

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
        CommonSettings.USBmode = lv_dropdown_get_selected(obj);
    }
}

static void battery_adjust_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        CommonSettings.BatteryAdjustment = (int16_t)lv_spinbox_get_value(obj);
    }
}

static void protocol_mode_dd_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t *obj = lv_event_get_target(e);
        lv_obj_t *multi_sub_protocols = lv_obj_get_user_data(obj);
        uint16_t selected = lv_dropdown_get_selected(obj);

        const MULTITX_PROTO_ItemTypedef *protocol = getProtocolByItemNumber(selected);

        multiprotocolSetProtocol(protocol->code, &sbus);
        multiprotocolSetSubProtocol(0, &sbus);

        const MULTITX_SUB_PROTO_ItemTypedef *subProtocol = protocol->subProtocols;

        if (subProtocol != NULL)
        {
            lv_obj_clear_flag(multi_sub_protocols, LV_OBJ_FLAG_HIDDEN);
            lv_dropdown_set_options(multi_sub_protocols, getSubProtocolsOptions(protocol));
            lv_dropdown_set_selected(multi_sub_protocols, multiprotocolGetSubProtocol(&sbus));
        }
        else
        {
            // lv_dropdown_set_selected(multi_sub_protocols, multiprotocolGetSubProtocol(&sbus));
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
        multiprotocolSetSubProtocol(selected, &sbus);
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
    lv_obj_set_size(dd, 124, 36);

    return dd;
}

static lv_obj_t *lv_current_label(lv_obj_t *parrent, const char *text)
{
    lv_obj_t *label = lv_label(parrent, LV_TEXT_ALIGN_LEFT, NULL, text);
    lv_obj_set_size(label, 98, 22);

    return label;
}

static lv_obj_t *lv_battery_adjust(lv_obj_t *parent)
{
    lv_obj_t *spinbox = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox, -1 * BATTERY_VOLTAGE_MAX, BATTERY_VOLTAGE_MAX);
    lv_spinbox_set_digit_format(spinbox, 4, 2);
    lv_spinbox_step_prev(spinbox);
    lv_obj_set_size(spinbox, 124, 36);
    lv_obj_center(spinbox);
    lv_obj_add_event_cb(spinbox, battery_adjust_handler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_spinbox_set_value(spinbox, CommonSettings.BatteryAdjustment);
    lv_spinbox_set_pos(spinbox, 0);

    return spinbox;
}

lv_obj_t *lv_gui_basic(void)
{
    const uint16_t protocolItemNumber = getProtocolItemNumberByCode(multiprotocolGetProtocol(&sbus));
    const MULTITX_PROTO_ItemTypedef *protocol = getProtocolByItemNumber(protocolItemNumber);

    lv_obj_t *screen = lv_screen("Basic settings", back_button_handler);

    lv_obj_t *beeper_label = lv_current_label(screen, "Beeper");
    lv_obj_t *beeper_switch = lv_gui_switch(screen, beeper_switch_handler);

    lv_obj_t *trim_beeper_label = lv_current_label(screen, "Trim beep");
    lv_obj_t *trim_beeper_switch = lv_gui_switch(screen, trim_beeper_switch_handler);

    static const char *usb_mode_opts = "COM\n"
                                       "Joystick\n"
                                       "Media";

    lv_obj_t *usb_mode_label = lv_current_label(screen, "USB mode");
    lv_obj_t *usb_mode_dd = lv_current_dropdawn(screen, usb_mode_dd_handler, usb_mode_opts);
    lv_dropdown_set_selected(usb_mode_dd, CommonSettings.USBmode);

    lv_obj_t *battery_adjust_label = lv_current_label(screen, "Battery adj");
    lv_obj_t *battery_adjust = lv_battery_adjust(screen);

    lv_obj_t *protocol_label = lv_current_label(screen, "TX");
    lv_obj_t *bind_button = lv_button(screen, bind_button_handler, "Bind");
    lv_obj_t *protocols = lv_current_dropdawn(screen, protocol_mode_dd_handler, getProtocolsOptions());
    lv_dropdown_set_selected(protocols, protocolItemNumber);
    lv_obj_t *multi_sub_protocols = lv_current_dropdawn(screen, multi_sub_protocol_mode_dd_handler, "Empty");
    lv_obj_set_user_data(protocols, multi_sub_protocols);
    if (protocol->subProtocols == NULL)
    {
        lv_obj_add_flag(multi_sub_protocols, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        const uint16_t selectedSubProtocolItem = getSubProtocolItemNumberByCode(protocol, multiprotocolGetSubProtocol(&sbus));
        lv_dropdown_set_options(multi_sub_protocols, getSubProtocolsOptions(protocol));
        lv_dropdown_set_selected(multi_sub_protocols, selectedSubProtocolItem);
    }

    lv_obj_align_to(beeper_label, screen, LV_ALIGN_TOP_LEFT, GUI_MARGIN * 2, GUI_SCREEN_START_MARGIN * 1.5);
    lv_obj_align_to(beeper_switch, beeper_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(trim_beeper_label, beeper_label, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, GUI_MARGIN * 2);
    lv_obj_align_to(trim_beeper_switch, trim_beeper_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(usb_mode_label, trim_beeper_label, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, GUI_MARGIN * 2);
    lv_obj_align_to(usb_mode_dd, usb_mode_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(battery_adjust_label, usb_mode_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, GUI_MARGIN * 4);
    lv_obj_align_to(battery_adjust, battery_adjust_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(protocol_label, battery_adjust_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, GUI_MARGIN * 4);
    lv_obj_align_to(protocols, protocol_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);
    lv_obj_align_to(bind_button, protocols, LV_ALIGN_OUT_LEFT_MID, -GUI_MARGIN, 0);
    lv_obj_align_to(multi_sub_protocols, protocols, LV_ALIGN_OUT_BOTTOM_MID, 0, GUI_MARGIN);

    return screen;
}