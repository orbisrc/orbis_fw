#include "lv_gui_ai.h"
#include "lv_gui_common.h"
#include "lv_gui.h"
#include "lv_gui_main_screen.h"
#include "core/iosettings.h"
#include "core/rcchannel.h"
#include "core/buffer.h"
#include "core/rccurve.h"

uint16_t current_channel = {0};

lv_obj_t *map_dd;
lv_obj_t *trim;
lv_obj_t *endpoint;
lv_obj_t *expo_type;
lv_obj_t *expo_rate;
lv_obj_t *curve_chart;
lv_chart_series_t *chart_line;
lv_obj_t *invert_switch;

static void ch_settnigs_load(uint16_t channel);

static void channel_change_handle(lv_event_t *e)
{
    uint16_t selected = lv_dropdown_get_selected(e->current_target);
    current_channel = selected;
    ch_settnigs_load(current_channel);
}

static void map_change_handle(lv_event_t *e)
{
    uint16_t selected = lv_dropdown_get_selected(e->current_target);
    RCChanelBufferSetItem(selected, &RCChanel[current_channel]);
    settings_changed();
}

static void curve_type_change_handle(lv_event_t *e)
{
    lv_obj_t *dd = e->current_target;

    uint16_t selected = lv_dropdown_get_selected(dd);

    if (selected == 0)
    {
        lv_obj_add_flag(expo_rate, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(expo_rate, LV_OBJ_FLAG_HIDDEN);
    }

    RCChanelSetCurveType(selected, &RCChanel[current_channel]);
    STcurveFill(current_channel);
    lv_chart_refresh(curve_chart);
    settings_changed();
}

static void invert_switch_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        RCChanelSetInvertState(lv_obj_has_state(obj, LV_STATE_CHECKED) ? 1 : 0, &RCChanel[current_channel]);
    }
    settings_changed();
}

static void trim_change_handle(lv_obj_t *obj)
{
    int32_t value = lv_slider_get_value(obj);
    RCChanelSetTrim(value, &RCChanel[current_channel]);
    settings_changed();
}

static void endpoint_change_handle(lv_obj_t *obj)
{
    uint32_t value_left = lv_slider_get_left_value(obj);
    uint32_t value_right = lv_slider_get_value(obj);

    RCChanelSetHightRate((uint16_t)value_right, &RCChanel[current_channel]);
    RCChanelSetLowRate((uint16_t) ~(value_left) + 1, &RCChanel[current_channel]);
    settings_changed();
}

static void expo_rate_change_handle(lv_obj_t *obj)
{
    uint32_t value = lv_slider_get_value(obj);
    RCChanelSetExpoX((uint16_t)value, &RCChanel[current_channel]);
    STcurveFill(current_channel);
    lv_chart_refresh(curve_chart);
    settings_changed();
}

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_screen_change(lv_gui_main_screen());
    }
}

static lv_obj_t *lv_dropdown(lv_obj_t *parent, lv_event_cb_t event_cb, const char *items)
{
    lv_obj_t *dd = lv_dropdown_create(parent);
    lv_obj_set_width(dd, 90);
    lv_dropdown_set_options_static(dd, items);
    lv_dropdown_set_dir(dd, LV_DIR_BOTTOM);
    lv_dropdown_set_symbol(dd, LV_SYMBOL_DOWN);

    if (event_cb != NULL)
    {
        lv_obj_add_event_cb(dd, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    }

    return dd;
}

static lv_obj_t *lv_chart(lv_obj_t *parent)
{
    lv_obj_t *chart = lv_chart_create(parent);
    lv_obj_set_size(chart, 150, 150);
    lv_obj_center(chart);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE); /*Show lines and points too*/
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1000);
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_point_count(chart, 1000);
    lv_chart_set_div_line_count(chart, 7, 7);

    chart_line = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(chart, chart_line, (lv_coord_t *)&___Curve[current_channel]);

    return chart;
}

static void ch_settnigs_load(uint16_t channel)
{
    lv_dropdown_set_selected(map_dd, 1);
    lv_dropdown_set_selected(expo_type, 1);

    lv_obj_t *rate = lv_obj_get_user_data(expo_rate);
    lv_obj_t *trim_bar = lv_obj_get_user_data(trim);
    lv_obj_t *endpoint_bar = lv_obj_get_user_data(endpoint);

    lv_slider_set_value(rate, (int32_t)RCChanelGetExpoX(&RCChanel[current_channel]), LV_ANIM_OFF);
    lv_slider_set_value(trim_bar, (int32_t)RCChanelGetTrim(&RCChanel[current_channel]), LV_ANIM_OFF);
    lv_slider_set_left_value(endpoint_bar, (int32_t) ~(RCChanelGetLowRate(&RCChanel[current_channel])) + 1, LV_ANIM_OFF);
    lv_slider_set_value(endpoint_bar, (int32_t)RCChanelGetHighRate(&RCChanel[current_channel]), LV_ANIM_OFF);
    lv_dropdown_set_selected(map_dd, RCChanelBufferGetItem(&RCChanel[current_channel]));
    lv_dropdown_set_selected(expo_type, RCChanelGetCurveType(&RCChanel[current_channel]));
    lv_obj_add_state(invert_switch, RCChanelGetInvertState(&RCChanel[current_channel]));

    RCChanelGetInvertState(&RCChanel[current_channel]) != 0 ? lv_obj_add_state(invert_switch, LV_STATE_CHECKED) : lv_obj_clear_state(invert_switch, LV_STATE_CHECKED);

    lv_chart_remove_series(curve_chart, chart_line);
    chart_line = lv_chart_add_series(curve_chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(curve_chart, chart_line, (lv_coord_t *)&___Curve[current_channel]);

    lv_chart_refresh(curve_chart);
}

lv_obj_t *lv_gui_ch_settings(void)
{
    lv_obj_t *screen = lv_screen("Ch settings", back_button_handler);

    static const char *curve_types = "Lin \n"
                                     "Exp";

    lv_obj_t *ch_label = lv_label(screen, LV_TEXT_ALIGN_LEFT, NULL, "CH: ");
    lv_obj_t *ch_dd = lv_dropdown(screen, channel_change_handle, channelsDD);

    map_dd = lv_dropdown(screen, map_change_handle, optionsDD);
    lv_dropdown_set_selected(map_dd, RCChanelGetExpoX(&RCChanel[current_channel]));
    trim = lv_trim(screen, trim_change_handle, RCChanelBufferGetItem(&RCChanel[current_channel]), "Trim");
    endpoint = lv_endpoint(screen, endpoint_change_handle, ~(RCChanelGetLowRate(&RCChanel[current_channel])) + 1, RCChanelGetHighRate(&RCChanel[current_channel]));

    lv_obj_t *invert_label = lv_label(screen, LV_TEXT_ALIGN_LEFT, NULL, "Invert");
    lv_obj_set_size(invert_label, 60, 22);
    invert_switch = lv_gui_switch(screen, invert_switch_handler);
    lv_obj_add_state(invert_switch, RCChanelGetInvertState(&RCChanel[current_channel]));

    expo_type = lv_dropdown(screen, curve_type_change_handle, curve_types);
    expo_rate = lv_trim_vertical(screen, expo_rate_change_handle, RCChanelGetTrim(&RCChanel[current_channel]), "Trim");
    curve_chart = lv_chart(screen);
    lv_dropdown_set_selected(ch_dd, current_channel);
    lv_obj_set_width(expo_type, 68);

    lv_obj_align_to(ch_label, screen, LV_ALIGN_TOP_LEFT, GUI_MARGIN * 2, GUI_SCREEN_START_MARGIN + GUI_MARGIN);
    lv_obj_align_to(ch_dd, ch_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);
    lv_obj_align_to(map_dd, ch_dd, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(trim, screen, LV_ALIGN_TOP_MID, 0, GUI_MARGIN * 17);
    lv_obj_align_to(endpoint, trim, LV_ALIGN_OUT_BOTTOM_MID, 0, GUI_MARGIN);
    lv_obj_align_to(invert_label, endpoint, LV_ALIGN_OUT_BOTTOM_LEFT, -GUI_MARGIN, GUI_MARGIN);
    lv_obj_align_to(invert_switch, invert_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align(curve_chart, LV_ALIGN_BOTTOM_RIGHT, -GUI_MARGIN, -GUI_MARGIN * 5);
    lv_obj_align_to(expo_type, curve_chart, LV_ALIGN_OUT_LEFT_TOP, -GUI_MARGIN, 0);
    lv_obj_align_to(expo_rate, expo_type, LV_ALIGN_OUT_BOTTOM_MID, 0, GUI_MARGIN);

    ch_settnigs_load(current_channel);

    return screen;
}