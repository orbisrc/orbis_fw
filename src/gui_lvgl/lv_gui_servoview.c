#include "lv_gui_servoview.h"
#include "lv_gui_conf.h"
#include "lv_gui.h"
#include "lv_gui_styles.h"
#include "lv_gui_main_screen.h"
#include "lv_gui_common.h"

static lv_obj_t *___lv_ch_bar[NUMBER_OF_CHANNELS];
static lv_style_t style_line;

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");

        lv_screen_change(lv_gui_main_screen());
    }
}



static void lv_ch_list_set_channel_value(lv_event_t *event)
{
    char buff[32];
    uint16_t i;

    i = lv_obj_get_user_data(event->target);

    lv_snprintf(buff, 32, "%d", i);

    LV_LOG_USER(buff);

    for (uint16_t i = 0; i < NUMBER_OF_CHANNELS - 1; i++)
    {
        // if (!___lv_ch_bar[i])
        //     return;
        // lv_bar_set_value(___lv_ch_bar[i], value, LV_ANIM_OFF);
    }
}

static void lv_bar_center_line_style_init()
{
    /*Create style*/
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 4);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_line_rounded(&style_line, false);
}

static lv_style_t *get_ch_bar_center_line_style()
{
    return &style_line;
}

static void lv_bar_center_line(lv_obj_t *parent)
{
    static lv_point_t line_points[] = {{2, 0}, {2, GUI_CH_BAR_HEIGHT}};

    lv_obj_t *line = lv_line_create(parent);
    lv_line_set_points(line, line_points, 2); /*Set the points*/
    lv_obj_add_style(line, get_ch_bar_center_line_style(), 0);
    lv_obj_center(line);
}

static lv_obj_t *lv_ch_label(lv_obj_t *parent, char *text)
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

    lv_obj_add_event_cb(bar, lv_ch_list_set_channel_value, LV_EVENT_DRAW_MAIN_END, NULL);

    return bar;
}

static lv_obj_t *lv_ch_bar_w_desc(lv_obj_t *parent, const char *text, lv_coord_t y_ofs)
{
    lv_obj_t *bar = lv_ch_bar(parent, 0);
    lv_obj_t *label = lv_ch_label(parent, text);
    lv_obj_t *value = lv_ch_value(parent, "-125");

    lv_obj_align_to(bar, parent, LV_ALIGN_TOP_MID, 0, y_ofs + 28);
    lv_obj_align_to(label, bar, LV_ALIGN_OUT_LEFT_MID, -4, 0);
    lv_obj_align_to(value, bar, LV_ALIGN_OUT_RIGHT_MID, 4, 0);

    return bar;
}

static void lv_ch_list(lv_obj_t *parent)
{
    static uint16_t chIds[NUMBER_OF_CHANNELS] = {0};

    lv_bar_center_line_style_init();

    for (uint16_t i = 0; i < NUMBER_OF_CHANNELS; i++)
    {
        ___lv_ch_bar[i] = lv_ch_bar_w_desc(parent, CHLabelShort[i], i * 18);
        chIds[i] = i;
        lv_obj_set_user_data(___lv_ch_bar[i], i);
    }
}

lv_obj_t *lv_gui_servoview(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_t *title = lv_title(screen, "Servo view");
    lv_obj_t *back_button = lv_button_back(screen, back_button_handler);

    lv_ch_list(screen);

    lv_obj_align(back_button, LV_ALIGN_TOP_LEFT, 4, 4);

    return screen;
}