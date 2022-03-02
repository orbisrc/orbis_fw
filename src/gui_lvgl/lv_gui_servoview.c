#include "lv_gui_servoview.h"
#include "lv_gui_conf.h"
#include "lv_gui.h"
#include "lv_gui_styles.h"
#include "lv_gui_main_screen.h"
#include "lv_gui_common.h"
#include "core/rcchannel.h"

static lv_obj_t *___lv_ch_bar[NUMBER_OF_CHANNELS];
static lv_obj_t *___lv_ch_value[NUMBER_OF_CHANNELS];

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");

        lv_screen_change(lv_gui_main_screen());
    }
}

static void channels_update_handler(lv_timer_t *timer)
{
    for (uint16_t i = 0; i < NUMBER_OF_CHANNELS; i++)
    {
        lv_bar_set_value(___lv_ch_bar[i], RCChanelGetGUIValue(&RCChanel[i]), LV_ANIM_OFF);
        lv_label_set_text_fmt(___lv_ch_value[i], "%d", RCChanelGetGUIValue(&RCChanel[i]));
    }
}

static void screen_close_handler(lv_event_t *e)
{
    lv_timer_t *timer = e->user_data;

    lv_timer_del(timer);
}

static void lv_bar_center_line(lv_obj_t *parent)
{
    static lv_point_t line_points[] = {{2, 0}, {2, GUI_CH_BAR_HEIGHT}};

    lv_obj_t *line = lv_line_create(parent);
    lv_line_set_points(line, line_points, 2); /*Set the points*/
    lv_obj_add_style(line, &bar_center_line_style, 0);
    lv_obj_center(line);
}

static lv_obj_t *lv_ch_label(lv_obj_t *parent, const char *text)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_width(label, 35);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_label_set_text(label, text);

    return label;
};

static lv_obj_t *lv_ch_value(lv_obj_t *parent, char *text)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_width(label, 35);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, 0);
    lv_label_set_text(label, text);

    return label;
};

static lv_obj_t *lv_ch_bar(lv_obj_t *parent, uint32_t ini_value)
{

    lv_obj_t *bar = lv_bar_create(parent);

    lv_obj_add_style(bar, &bar_bg, 0);
    lv_obj_add_style(bar, &bar_indic, LV_PART_INDICATOR);
    lv_bar_set_mode(bar, LV_BAR_MODE_SYMMETRICAL);
    lv_bar_set_range(bar, GUI_BAR_MIN, GUI_BAR_MAX);

    lv_obj_set_size(bar, GUI_CH_BAR_WIDTH, GUI_CH_BAR_HEIGHT);
    lv_obj_center(bar);
    lv_bar_center_line(bar);
    lv_bar_set_value(bar, ini_value, LV_ANIM_ON);

    return bar;
}

lv_obj_t *lv_gui_servoview(void)
{
    lv_obj_t *screen = lv_screen("View", back_button_handler);

    for (uint16_t i = 0; i < NUMBER_OF_CHANNELS; i++)
    {
        ___lv_ch_bar[i] = lv_ch_bar(screen, 0);
        lv_obj_t *label = lv_ch_label(screen, CHLabelShort[i]);
        ___lv_ch_value[i] = lv_ch_value(screen, "-125");

        lv_obj_align_to(___lv_ch_bar[i], screen, LV_ALIGN_TOP_MID, 0, i * (GUI_CH_BAR_HEIGHT + GUI_MARGIN * 1.5) + GUI_SCREEN_START_MARGIN);
        lv_obj_align_to(label, ___lv_ch_bar[i], LV_ALIGN_OUT_LEFT_MID, -GUI_MARGIN, 0);
        lv_obj_align_to(___lv_ch_value[i], ___lv_ch_bar[i], LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);
    }

    lv_timer_t *timer = lv_timer_create(channels_update_handler, 20, NULL);

    lv_obj_add_event_cb(screen, screen_close_handler, LV_EVENT_DELETE, timer);

    return screen;
}