#include "lv_gui_main_screen.h"
#include "lv_gui_styles.h"
#include "lv_gui_servoview.h"
#include "lv_gui_ai.h"
#include "lv_gui_info.h"
#include "lv_gui.h"
#include "lv_gui_common.h"
#include "lv_gui_ch_settings.h"
#include "lv_gui_models.h"
#include "lv_gui_calibrate.h"
#include "core/auxiliary.h"
#include "core/iosettings.h"

static lv_obj_t *lv_menu_model(lv_obj_t *parent, const char *title);

static lv_obj_t *lv_menu_system(lv_obj_t *parent, const char *title);

static void servoview_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");

        lv_screen_change(lv_gui_servoview());
    }
}

static void model_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");

        lv_menu_model(e->current_target->parent, "Model");
    }
}

static void menu_back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");

        lv_screen_change(lv_gui_main_screen());
    }
}

static void settings_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");

        lv_menu_system(e->current_target->parent, "System");
    }
}

static void set_timer2_data(lv_obj_t *label)
{
    lv_label_set_text_fmt(label, "%02dm:%02ds", RCStimerGetMinute(&RCTimer2), RCStimerGetSecond(&RCTimer2));
}

static void set_timer1_data(lv_obj_t *label)
{
    lv_label_set_text_fmt(label, "%02dm:%02ds", RCStimerGetMinute(&RCTimer1), RCStimerGetSecond(&RCTimer1));
}

static lv_obj_t *lv_model_name(lv_obj_t *parent)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_width(label, 100);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, 0);
    lv_label_set_text(label, ModelSettings[STgetCurrentModelID()].Name);

    lv_obj_remove_style_all(label); /*Remove the styles coming from the theme*/
    lv_obj_add_style(label, &model_name_style, LV_PART_MAIN);

    return label;
};

static lv_obj_t *lv_timer(lv_obj_t *parent, void (*event_cb)(lv_obj_t *obj))
{
    lv_obj_t *label = lv_dynamic_label(parent, LV_TEXT_ALIGN_LEFT, event_cb, "00:00");
    lv_obj_set_width(label, 160);
    lv_obj_set_height(label, 30);
    lv_obj_add_style(label, &timer_style, LV_PART_MAIN);

    return label;
};

static lv_obj_t *lv_slider(lv_obj_t *parent, uint32_t ini_value)
{
    lv_obj_t *slider = lv_slider_create(parent);
    lv_slider_set_mode(slider, LV_SLIDER_MODE_SYMMETRICAL);
    lv_slider_set_range(slider, GUI_TRIM_MIN, GUI_TRIM_MAX);

    lv_obj_add_style(slider, &bar_bg, LV_PART_MAIN);
    lv_obj_add_style(slider, &bar_indic, LV_PART_INDICATOR);
    lv_obj_add_style(slider, &bar_knob, LV_PART_KNOB);
    lv_obj_set_size(slider, 160, 16);
    lv_obj_center(slider);

    return slider;
}

static lv_obj_t *lv_menu_system(lv_obj_t *parent, const char *title)
{
    const char *lv_menu_buttons_label[] = {
        "CALIBRATE",
        "ANALOG INPUT",
        "DISCRETE INPUT",
        "BASIC SETTINGS",
        "INFO"};

    lv_obj_t *(*lv_menu_buttons_callback[])(void) = {lv_gui_calibrate,
                                                     lv_gui_ai,
                                                     lv_gui_main_screen,
                                                     lv_gui_main_screen,
                                                     lv_gui_info,
                                                     NULL,
                                                     NULL};

    lv_obj_t *container = lv_menu(parent, title, lv_menu_buttons_label, lv_menu_buttons_callback, menu_back_button_handler);

    return container;
}

static lv_obj_t *lv_menu_model(lv_obj_t *parent, const char *title)
{
    const char *lv_menu_buttons_label[] = {"MODEL SELECT",
                                           "CH SETTINGS",
                                           "END POINT",
                                           "SUB TRIM",
                                           "CH MAPPING",
                                           "CH INVERT",
                                           "\n"};

    lv_obj_t *(*lv_menu_buttons_callback[])(void) = {lv_gui_models,
                                                     lv_gui_ch_settings,
                                                     lv_gui_main_screen,
                                                     lv_gui_main_screen,
                                                     lv_gui_main_screen,
                                                     lv_gui_main_screen,
                                                     NULL};

    lv_obj_t *container = lv_menu(parent, title, lv_menu_buttons_label, lv_menu_buttons_callback, menu_back_button_handler);

    return container;
}

lv_obj_t *lv_gui_main_screen(void)
{
    lv_group_t *group = lv_group_get_default();
    lv_group_remove_all_objs(group);

    lv_obj_t *screen = lv_obj_create(NULL);

    lv_obj_t *rssi_label = lv_label(screen, LV_TEXT_ALIGN_LEFT, NULL, "RS");
    lv_obj_t *battery_label = lv_label(screen, LV_TEXT_ALIGN_RIGHT, NULL, "0.0v");
    lv_obj_t *battery_icon = lv_label(screen, LV_TEXT_ALIGN_RIGHT, NULL, LV_SYMBOL_BATTERY_2);
    lv_obj_t *th_trim = lv_slider(screen, 0);
    lv_obj_t *st_trim = lv_slider(screen, 0);
    lv_obj_t *model_name = lv_model_name(screen);
    lv_obj_t *timer1 = lv_timer(screen, set_timer1_data);
    lv_obj_t *timer2 = lv_timer(screen, set_timer2_data);
    lv_obj_t *servoview_button = lv_button(screen, servoview_button_handler, "Monitor");
    lv_obj_t *model_button = lv_button(screen, model_button_handler, "Model");
    lv_obj_t *settings_button = lv_button(screen, settings_button_handler, "Settings");
    lv_obj_t *st_name = lv_label(screen, LV_TEXT_ALIGN_RIGHT, NULL, "ST");
    lv_obj_t *th_name = lv_label(screen, LV_TEXT_ALIGN_RIGHT, NULL, "TH");
    lv_obj_t *st_value = lv_label(screen, LV_TEXT_ALIGN_LEFT, NULL, "0");
    lv_obj_t *th_value = lv_label(screen, LV_TEXT_ALIGN_LEFT, NULL, "0");

    lv_obj_align(rssi_label, LV_ALIGN_TOP_LEFT, GUI_MARGIN, 0);
    lv_obj_align(battery_icon, LV_ALIGN_TOP_RIGHT, -GUI_MARGIN, 0);
    lv_obj_align_to(battery_label, battery_icon, LV_ALIGN_OUT_LEFT_MID, GUI_MARGIN, 0);

    lv_obj_align_to(model_name, rssi_label, LV_ALIGN_OUT_BOTTOM_LEFT, 16, 10);
    lv_obj_align_to(timer1, model_name, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_align_to(timer2, timer1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    lv_obj_align(st_trim, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align_to(st_name, st_trim, LV_ALIGN_OUT_LEFT_MID, -8, 0);
    lv_obj_align_to(st_value, st_trim, LV_ALIGN_OUT_RIGHT_MID, 8, 0);

    lv_obj_align_to(th_trim, st_trim, LV_ALIGN_OUT_BOTTOM_MID, 0, 16);
    lv_obj_align_to(th_name, th_trim, LV_ALIGN_OUT_LEFT_MID, -8, 0);
    lv_obj_align_to(th_value, th_trim, LV_ALIGN_OUT_RIGHT_MID, 8, 0);

    lv_obj_align(servoview_button, LV_ALIGN_BOTTOM_RIGHT, -GUI_MARGIN, -GUI_MARGIN);
    lv_obj_align(model_button, LV_ALIGN_BOTTOM_MID, 0, -GUI_MARGIN);
    lv_obj_align(settings_button, LV_ALIGN_BOTTOM_LEFT, GUI_MARGIN, -GUI_MARGIN);

    return screen;
}
