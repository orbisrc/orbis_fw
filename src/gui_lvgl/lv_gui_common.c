#include "lv_gui_common.h"

lv_obj_t *lv_button_back(lv_obj_t *parent, lv_event_cb_t event_cb)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 4, -4);
    lv_obj_set_size(btn, 40, 20);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, NULL);

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
    lv_obj_t *screen = lv_obj_create(NULL);

    lv_obj_t *header_title = lv_title(screen, title);
    lv_obj_t *back_button = lv_button_back(screen, event_cb);

    lv_obj_align(back_button, LV_ALIGN_TOP_LEFT, 4, 4);

    return screen;
};

/*
Dynamic label
 */

static void lv_label_timer_cb(lv_timer_t *timer)
{
    lv_obj_t *label = timer->user_data;

    lv_event_cb_t event_cb = lv_obj_get_user_data(label);

    event_cb(label);
};

static void lv_delete_timer_cb(lv_event_t *e)
{
    lv_timer_t *timer = e->user_data;

    lv_timer_del(timer);
};

lv_obj_t *lv_dynamic_label(lv_obj_t *parent, lv_text_align_t value, void *event_cb(lv_obj_t *label), const char *ini)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, ini);
    lv_obj_set_size(label, 32, 18);
    lv_label_set_text(label, ini);
    lv_obj_set_style_text_align(label, value, 0);
    lv_obj_set_user_data(label, event_cb);

    if (event_cb != NULL)
    {
        lv_timer_t *timer = lv_timer_create(lv_label_timer_cb, 500, label);
        lv_obj_add_event_cb(label, lv_delete_timer_cb, LV_EVENT_DELETE, timer);
    }

    return label;
};