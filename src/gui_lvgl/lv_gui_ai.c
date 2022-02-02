#include "lv_gui_ai.h"
#include "lv_gui_common.h"
#include "lv_gui.h"
#include "lv_gui_main_screen.h"
#include "stconfig.h"
#include "core/filter.h"

#define GUI_ADC_CH_NUM ADC1_CH_NUM

static lv_obj_t *ADC_ch_buuf_label[GUI_ADC_CH_NUM] = {0};

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_screen_change(lv_gui_main_screen());
    }
}

static void channels_update_handler(lv_timer_t *timer)
{
    for (uint16_t i = 0; i < GUI_ADC_CH_NUM; i++)
    {
        lv_label_set_text_fmt(ADC_ch_buuf_label[i], "AI %d: %d", i,  ADCbuff[i]);
    }
}

static void screen_close_handler(lv_event_t *e)
{
    lv_timer_t *timer = e->user_data;

    lv_timer_del(timer);
}

lv_obj_t *lv_gui_ai(void)
{
    lv_obj_t *screen = lv_screen("Analog input", back_button_handler);

    for (uint16_t i = 0; i < GUI_ADC_CH_NUM; i++)
    {
        ADC_ch_buuf_label[i] = lv_label(screen, LV_TEXT_ALIGN_LEFT, NULL, "");
        lv_obj_set_width(ADC_ch_buuf_label[i], 100);
        lv_obj_align_to(ADC_ch_buuf_label[i], screen, LV_ALIGN_TOP_LEFT, GUI_MARGIN * 2, i * (GUI_CH_BAR_HEIGHT + GUI_MARGIN * 1.5) + GUI_SCREEN_START_MARGIN * 2);
    }

    lv_timer_t *timer = lv_timer_create(channels_update_handler, GUI_DATA_UPDATE_PERIOD_FAST, NULL);

    lv_obj_add_event_cb(screen, screen_close_handler, LV_EVENT_DELETE, timer);

    return screen;
}