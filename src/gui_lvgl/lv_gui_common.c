#include "lv_gui_common.h"
#include "lv_gui_conf.h"
#include "lv_gui.h"
#include "lv_gui_styles.h"
#include "lv_port_disp.h"

lv_obj_t *lv_button_back(lv_obj_t *parent, lv_event_cb_t event_cb)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 4, -4);
    lv_obj_set_size(btn, 40, 20);

    if (event_cb != NULL)
    {
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, NULL);
    }

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_LEFT);
    lv_obj_center(label);

    return btn;
};

lv_obj_t *lv_button(lv_obj_t *parent, lv_event_cb_t event_cb, const char *title)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 4, -4);
    lv_obj_set_size(btn, 72, 24);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, title);
    lv_obj_center(label);

    return btn;
};

lv_obj_t *lv_label(lv_obj_t *parent, lv_text_align_t value, lv_event_cb_t event_cb, const char *ini)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, ini);
    lv_obj_set_size(label, 32, 18);
    lv_label_set_text(label, ini);
    lv_obj_set_style_text_align(label, value, 0);

    if (event_cb != NULL)
    {
        lv_obj_add_event_cb(label, event_cb, LV_EVENT_DRAW_MAIN_END, NULL);
    }

    return label;
};

lv_obj_t *lv_title(lv_obj_t *parent, const char *title)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_size(label, 140, 20);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);
    lv_label_set_text(label, title);

    return label;
};

lv_obj_t *lv_screen(const char *title, lv_event_cb_t event_cb)
{
    lv_indev_encoder_group_create();

    lv_obj_t *screen = lv_obj_create(NULL);

    lv_obj_t *header_title = lv_title(screen, title);
    lv_obj_t *back_button = lv_button_back(screen, event_cb);

    lv_obj_align(header_title, LV_ALIGN_TOP_MID, 0, GUI_MARGIN);
    lv_obj_align(back_button, LV_ALIGN_TOP_LEFT, GUI_MARGIN, GUI_MARGIN);

    return screen;
};

static void lv_add_center_line(lv_obj_t *parent)
{
    static lv_point_t line_points[] = {{2, 0}, {2, GUI_SLIDER_CONTAINER_HEIGHT}};

    lv_obj_t *line = lv_line_create(parent);
    lv_line_set_points(line, line_points, 2); /*Set the points*/
    lv_obj_add_style(line, &bar_center_line_style, 0);
    lv_obj_center(line);
}

/*
Dynamic label
 */

static void lv_label_timer_cb(lv_timer_t *timer)
{
    lv_obj_t *label = timer->user_data;

    void (*event_cb)(lv_obj_t * label) = lv_obj_get_user_data(label);

    event_cb(label);
};

static void lv_delete_timer_cb(lv_event_t *e)
{
    lv_timer_t *timer = e->user_data;

    lv_timer_del(timer);
};

lv_obj_t *lv_dynamic_label(lv_obj_t *parent, lv_text_align_t value, const void (*event_cb)(lv_obj_t *label), const char *ini)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, ini);
    lv_obj_set_size(label, 32, 18);
    lv_label_set_text(label, ini);
    lv_obj_set_style_text_align(label, value, 0);
    lv_obj_set_user_data(label, event_cb);

    if (event_cb != NULL)
    {
        lv_timer_t *timer = lv_timer_create(lv_label_timer_cb, GUI_DATA_UPDATE_PERIOD, label);
        lv_obj_add_event_cb(label, lv_delete_timer_cb, LV_EVENT_DELETE, timer);
    }

    return label;
};

/*
    Menu
 */

static void menu_close_handle(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");

        lv_obj_del(e->current_target->parent);
    }
}

static void menu_button_handle(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked %d", e->current_target->user_data);

        lv_obj_t *(*event_cb)(void) = lv_obj_get_user_data(e->current_target);

        lv_screen_change(event_cb());
    }
}

static void menu_destroy_handle(lv_event_t *e)
{
    lv_group_t *group = e->user_data;
    lv_group_remove_all_objs(group);
    lv_group_del(group);
}

lv_obj_t *lv_menu(lv_obj_t *parent, const char *title, const char *items[], lv_obj_t *(*items_callbak[])(void), void *back_cb(lv_event_t *e))
{
    lv_group_t *group = lv_group_get_default();
    lv_group_remove_all_objs(group);

    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, 180, 290);
    lv_obj_align(container, LV_ALIGN_CENTER, 2, 0);
    lv_obj_t *header_title = lv_title(container, title);

    lv_obj_t *button;
    uint16_t i = 0;
    while (items_callbak[i] != NULL)
    {
        button = lv_button(container, menu_button_handle, items[i]);
        lv_obj_set_user_data(button, items_callbak[i]);
        lv_obj_align(button, LV_ALIGN_TOP_MID, 0, 30 + i * (GUI_BUTTON_MENU_HEIGHT + GUI_MARGIN * 2));
        lv_obj_set_width(button, GUI_BUTTON_MENU_WIDTH);
        lv_obj_set_height(button, GUI_BUTTON_MENU_HEIGHT);

        lv_group_add_obj(group, button);

        i++;
    }

    lv_obj_t *back_button;
    if (back_cb != NULL)
    {
        back_button = lv_button_back(container, back_cb);
    }
    else
    {
        back_button = lv_button_back(container, menu_close_handle);
    }

    lv_obj_align(header_title, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_align(back_button, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    return container;
}

/*
    Ack box
*/

static void lv_ack_box_ok_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked ok");

        void (*event_cb)() = lv_obj_get_user_data(e->current_target);

        event_cb();
    }
}

static void lv_ack_box_cancel_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked cancell");

        lv_obj_del(e->current_target->parent);
    }
}

lv_obj_t *lv_ack_box(lv_obj_t *parent, void (*event_cb)(), const char *text)
{
    lv_obj_t *container = lv_obj_create(parent);

    lv_obj_set_size(container, 200, 100);
    lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label = lv_label(container, LV_TEXT_ALIGN_CENTER, NULL, text);
    lv_obj_set_size(label, 160, 40);

    lv_obj_t *cancell_button = lv_button(container, lv_ack_box_cancel_handler, "Cancel");
    lv_obj_t *ok_button = lv_button(container, lv_ack_box_ok_handler, "Ok");

    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_align(ok_button, LV_ALIGN_BOTTOM_RIGHT, -GUI_MARGIN, -GUI_MARGIN);
    lv_obj_align(cancell_button, LV_ALIGN_BOTTOM_LEFT, GUI_MARGIN, -GUI_MARGIN);

    lv_obj_set_user_data(ok_button, event_cb);

    return container;
}

/*
    Trim bar
*/

static void trim_value_changed_cb(lv_event_t *e)
{
    lv_obj_t *value_label = e->user_data;
    int32_t value = lv_slider_get_value(e->current_target);

    lv_label_set_text_fmt(value_label, "%ld", value);
}

static void trim_call_cb(lv_event_t *e)
{
    st_gui_event_cb_t event_cb = e->user_data;
    event_cb(e->current_target);
}

lv_obj_t *lv_trim(lv_obj_t *parent, st_gui_event_cb_t event_cb, int32_t ini, const char *name)
{

    lv_obj_t *container = lv_obj_create(parent);

    lv_obj_set_size(container, 220, GUI_SLIDER_CONTAINER_HEIGHT);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(container, 0, LV_OPA_0);
    lv_obj_set_style_border_opa(container, 0, LV_OPA_0);

    lv_obj_t *slider = lv_slider_create(container);
    lv_slider_set_mode(slider, LV_SLIDER_MODE_SYMMETRICAL);
    lv_slider_set_range(slider, GUI_TRIM_MIN, GUI_TRIM_MAX);
    lv_slider_set_value(slider, ini, LV_ANIM_OFF);

    lv_obj_add_style(slider, &bar_bg, LV_PART_MAIN);
    lv_obj_add_style(slider, &bar_indic, LV_PART_INDICATOR);
    lv_obj_add_style(slider, &bar_knob, LV_PART_KNOB);
    lv_obj_set_size(slider, 150, 16);
    lv_obj_center(slider);

    lv_add_center_line(slider);

    lv_obj_t *slider_name = lv_label(container, LV_TEXT_ALIGN_RIGHT, NULL, name);
    lv_obj_t *value = lv_label(container, LV_TEXT_ALIGN_LEFT, NULL, "0");

    lv_obj_align_to(slider_name, slider, LV_ALIGN_OUT_LEFT_MID, -GUI_MARGIN * 2, 0);
    lv_obj_align_to(value, slider, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN * 2, 0);

    lv_obj_add_event_cb(slider, trim_value_changed_cb, LV_EVENT_ALL, value);

    lv_obj_set_user_data(container, slider);

    if (event_cb != NULL)
    {
        lv_obj_add_event_cb(slider, trim_call_cb, LV_EVENT_VALUE_CHANGED, event_cb);
    }

    return container;
}

/*
    Trim bar vertical
*/
static void trim_vertical_value_changed_cb(lv_event_t *e)
{
    lv_obj_t *value_label = e->user_data;
    int32_t value = lv_slider_get_value(e->current_target);

    lv_label_set_text_fmt(value_label, "%ld", value);
}

static void trim_vertical_call_cb(lv_event_t *e)
{
    st_gui_event_cb_t event_cb = e->user_data;
    event_cb(e->current_target);
}

lv_obj_t *lv_trim_vertical(lv_obj_t *parent, st_gui_event_cb_t event_cb, int32_t ini, const char *name)
{

    lv_obj_t *container = lv_obj_create(parent);

    lv_obj_set_size(container, GUI_SLIDER_CONTAINER_HEIGHT, 130);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(container, 0, LV_OPA_0);
    lv_obj_set_style_border_opa(container, 0, LV_OPA_0);

    lv_obj_t *slider = lv_slider_create(container);
    lv_slider_set_mode(slider, LV_SLIDER_MODE_SYMMETRICAL);
    lv_slider_set_range(slider, GUI_TRIM_MIN, GUI_TRIM_MAX);
    lv_slider_set_value(slider, ini, LV_ANIM_OFF);

    lv_obj_add_style(slider, &bar_bg, LV_PART_MAIN);
    lv_obj_add_style(slider, &bar_indic, LV_PART_INDICATOR);
    // lv_style_set_bg_grad_dir(&bar_indic, LV_GRAD_DIR_VER);
    lv_obj_add_style(slider, &bar_knob, LV_PART_KNOB);
    lv_obj_set_size(slider, 16, 100);
    lv_obj_center(slider);

    lv_obj_t *value = lv_label(container, LV_TEXT_ALIGN_CENTER, NULL, "0");

    lv_obj_align_to(value, slider, LV_ALIGN_OUT_TOP_MID, 0, 0);

    lv_obj_add_event_cb(slider, trim_vertical_value_changed_cb, LV_EVENT_ALL, value);

    lv_obj_set_user_data(container, slider);

    if (event_cb != NULL)
    {
        lv_obj_add_event_cb(slider, trim_vertical_call_cb, LV_EVENT_VALUE_CHANGED, event_cb);
    }

    return container;
}

/*
    Endpoint bar
*/

static void endpoint_left_value_changed_cb(lv_event_t *e)
{
    lv_obj_t *value_label = e->user_data;
    lv_obj_t *slider = e->current_target;

    int32_t value_left = lv_slider_get_left_value(slider);

    if (value_left >= 0)
    {
        value_left = 0;
        lv_slider_set_left_value(slider, value_left, LV_ANIM_OFF);
    }

    lv_label_set_text_fmt(value_label, "%ld", value_left);
}

static void endpoint_right_value_changed_cb(lv_event_t *e)
{
    lv_obj_t *slider = e->current_target;
    lv_obj_t *value_label = e->user_data;

    int32_t value_right = lv_slider_get_value(slider);

    if (value_right <= 0)
    {
        value_right = 0;
        lv_slider_set_value(slider, value_right, LV_ANIM_OFF);
    }

    lv_label_set_text_fmt(value_label, "%ld", value_right);
}

static void endpoint_call_cb(lv_event_t *e)
{
    st_gui_event_cb_t event_cb = e->user_data;
    event_cb(e->current_target);
}

lv_obj_t *lv_endpoint(lv_obj_t *parent, st_gui_event_cb_t event_cb, int32_t ini_left, int32_t ini_right)
{

    lv_obj_t *container = lv_obj_create(parent);

    lv_obj_set_size(container, 220, GUI_SLIDER_CONTAINER_HEIGHT);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(container, 0, LV_OPA_0);
    lv_obj_set_style_border_opa(container, 0, LV_OPA_0);

    lv_obj_t *slider = lv_slider_create(container);
    lv_slider_set_mode(slider, LV_SLIDER_MODE_RANGE);
    lv_slider_set_range(slider, GUI_ENDPOINT_MIN, GUI_ENDPOINT_MAX);
    lv_slider_set_value(slider, ini_right, LV_ANIM_OFF);
    lv_slider_set_left_value(slider, ini_left, LV_ANIM_OFF);

    lv_obj_add_style(slider, &bar_bg, LV_PART_MAIN);
    lv_obj_add_style(slider, &bar_indic, LV_PART_INDICATOR);
    lv_obj_add_style(slider, &bar_knob, LV_PART_KNOB);
    lv_obj_set_size(slider, 150, 16);
    lv_obj_center(slider);

    lv_add_center_line(slider);

    lv_obj_t *left_value = lv_label(container, LV_TEXT_ALIGN_RIGHT, NULL, "0");
    lv_obj_t *right_value = lv_label(container, LV_TEXT_ALIGN_LEFT, NULL, "0");

    lv_label_set_text_fmt(left_value, "%ld", ini_left);
    lv_label_set_text_fmt(right_value, "%ld", ini_right);

    lv_obj_align_to(left_value, slider, LV_ALIGN_OUT_LEFT_MID, -GUI_MARGIN * 2, 0);
    lv_obj_align_to(right_value, slider, LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN * 2, 0);

    lv_obj_add_event_cb(slider, endpoint_left_value_changed_cb, LV_EVENT_ALL, left_value);
    lv_obj_add_event_cb(slider, endpoint_right_value_changed_cb, LV_EVENT_ALL, right_value);

    lv_obj_set_user_data(container, slider);

    if (event_cb != NULL)
    {
        lv_obj_add_event_cb(slider, endpoint_call_cb, LV_EVENT_VALUE_CHANGED, event_cb);
    }

    return container;
}

/*
    Dropdawn
*/

lv_obj_t *lv_gui_dropdown(lv_obj_t *parent, lv_event_cb_t event_cb, const char *items)
{
    lv_obj_t *dd = lv_dropdown_create(parent);
    lv_obj_set_size(dd, 96, 32);
    lv_dropdown_set_options_static(dd, items);
    lv_dropdown_set_dir(dd, LV_DIR_BOTTOM);
    lv_dropdown_set_symbol(dd, LV_SYMBOL_DOWN);

    if (event_cb != NULL)
    {
        lv_obj_add_event_cb(dd, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    }

    return dd;
}
