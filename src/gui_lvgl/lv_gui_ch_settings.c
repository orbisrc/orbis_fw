#include "lv_gui_ai.h"
#include "lv_gui_common.h"
#include "lv_gui.h"
#include "lv_gui_main_screen.h"

uint16_t current_channel = {0};

static void channel_change_handle(lv_event_t *e)
{
    uint16_t selected = lv_dropdown_get_selected(e->current_target);

    current_channel = selected;
    LV_LOG_USER("Option channel: %d", selected);
}

static void map_change_handle(lv_event_t *e)
{
    uint16_t selected = lv_dropdown_get_selected(e->current_target);
    LV_LOG_USER("Option map: %d", selected);
}

static void curve_type_change_handle(lv_event_t *e)
{
    uint16_t selected = lv_dropdown_get_selected(e->current_target);
    LV_LOG_USER("Option map: %d", selected);
}

static void trim_change_handle(lv_obj_t *obj)
{
    uint32_t value = lv_slider_get_value(obj);
    LV_LOG_USER("Slider value: %d", value);
}

static void endpoint_change_handle(lv_obj_t *obj)
{
    uint32_t value_left = lv_slider_get_left_value(obj);
    uint32_t value_right = lv_slider_get_value(obj);
    LV_LOG_USER("Slider value left: %d", value_left);
    LV_LOG_USER("Slider value right: %d", value_right);
}

static void expo_rate_change_handle(lv_obj_t *obj)
{
    uint32_t value = lv_slider_get_value(obj);
    LV_LOG_USER("Slider value: %d", value);
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
    lv_obj_set_size(chart, 160, 160);
    lv_obj_center(chart);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE); /*Show lines and points too*/

    lv_chart_series_t *ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    // lv_chart_set_ext_y_array(curve_chart, ser1, (lv_coord_t *)ecg_sample);

    return chart;
}

lv_obj_t *lv_gui_ch_settings(void)
{
    lv_obj_t *screen = lv_screen("Ch settings", back_button_handler);

    static const char *opts = "A\n"
                              "E\n"
                              "T\n"
                              "R\n"
                              "PT1\n"
                              "PT2\n"
                              "PT3\n"
                              "PT4\n"
                              "SWA\n"
                              "SWB\n"
                              "SWC\n"
                              "SWD\n"
                              "SWE\n"
                              "SWF\n"
                              "SWG\n"
                              "SWH\n"
                              "SWI\n"
                              "SWJ\n"
                              "S1\n"
                              "S2\n"
                              "S3\n"
                              "S4\n"
                              "S5\n"
                              "S6\n"
                              "S7\n"
                              "S8\n"
                              "S9\n"
                              "S10\n"
                              "S11\n"
                              "S12\n"
                              "S13\n"
                              "S14";

    static const char *channels = "Aileron \n"
                                  "Elevon\n"
                                  "Throttle\n"
                                  "Rudder\n"
                                  "CH5\n"
                                  "CH6\n"
                                  "CH7\n"
                                  "CH8\n"
                                  "CH9\n"
                                  "CH10\n"
                                  "CH11\n"
                                  "CH12\n"
                                  "CH13\n"
                                  "CH14\n"
                                  "CH15\n"
                                  "CH16";

    static const char *curve_types = "Lin \n"
                                     "Exp\n";

    lv_obj_t *ch_label = lv_label(screen, LV_TEXT_ALIGN_LEFT, NULL, "CH: ");
    lv_obj_t *ch_dd = lv_dropdown(screen, channel_change_handle, channels);
    lv_obj_t *map_dd = lv_dropdown(screen, map_change_handle, opts);
    lv_obj_t *trim = lv_trim(screen, trim_change_handle, 0, "Trim");
    lv_obj_t *endpoint = lv_endpoint(screen, endpoint_change_handle, -100, 100);
    lv_obj_t *expo_type = lv_dropdown(screen, curve_type_change_handle, curve_types);
    lv_obj_t *expo_rate = lv_trim_vertical(screen, expo_rate_change_handle, 0, "Trim");
    lv_obj_t *curve_chart = lv_chart(screen);

    lv_dropdown_set_selected(ch_dd, current_channel);

    lv_obj_set_width(expo_type, 68);

    lv_obj_align_to(ch_label, screen, LV_ALIGN_TOP_LEFT, GUI_MARGIN * 2, GUI_SCREEN_START_MARGIN + GUI_MARGIN);
    lv_obj_align_to(ch_dd, ch_label, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);
    lv_obj_align_to(map_dd, ch_dd, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(trim, screen, LV_ALIGN_TOP_MID, 0, GUI_MARGIN * 16);
    lv_obj_align_to(endpoint, trim, LV_ALIGN_OUT_BOTTOM_MID, 0, GUI_MARGIN);

    lv_obj_align(curve_chart, LV_ALIGN_BOTTOM_RIGHT, -GUI_MARGIN, -GUI_MARGIN * 5);
    lv_obj_align_to(expo_type, curve_chart, LV_ALIGN_OUT_LEFT_TOP, -GUI_MARGIN, 0);
    lv_obj_align_to(expo_rate, expo_type, LV_ALIGN_OUT_BOTTOM_MID, 0, GUI_MARGIN);

    return screen;
}