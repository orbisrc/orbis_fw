#include "lv_gui_calibrate.h"
#include "lv_gui_common.h"
#include "lv_gui.h"
#include "lv_gui_main_screen.h"
#include "core/rcchannel.h"
#include "core/analog.h"
#include "core/iosettings.h"

#define GUI_CALIBRATE_CH_NUM 2

static uint16_t calibrate_step = {0};
static uint16_t Amin[GUI_CALIBRATE_CH_NUM] = {0};
static uint16_t Amax[GUI_CALIBRATE_CH_NUM] = {0};

static lv_obj_t *ADC_current_label[GUI_CALIBRATE_CH_NUM] = {0};
static lv_obj_t *ADC_min_label[GUI_CALIBRATE_CH_NUM] = {0};
static lv_obj_t *ADC_max_label[GUI_CALIBRATE_CH_NUM] = {0};

enum calibrate_stem_enum_typedef 
{
    START_STEP,
    MIN_MAX_SET,
    CENTER_SET,
    CALIBRATE_DONE,
    RETURN_TO_MAIN
};

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_screen_change(lv_gui_main_screen());
    }
}

static void calibrate_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        calibrate_step++;
    }
}

static void calibrate_handler(lv_timer_t *timer)
{
    for (uint16_t i = 0; i < GUI_CALIBRATE_CH_NUM; i++)
    {
        if (Amin[i] > AIgetADCValue(&AnalogChannel[i]))
            Amin[i] = AIgetADCValue(&AnalogChannel[i]);
        if (Amax[i] < AIgetADCValue(&AnalogChannel[i]))
            Amax[i] = AIgetADCValue(&AnalogChannel[i]);

        lv_label_set_text_fmt(ADC_current_label[i], "AI%u: %u", i + 1, AIgetADCValue(&AnalogChannel[i]));
        lv_label_set_text_fmt(ADC_min_label[i], "min: %u", Amin[i]);
        lv_label_set_text_fmt(ADC_max_label[i], "max: %u", Amax[i]);
    }

    lv_obj_t *button_label = timer->user_data;

    switch (calibrate_step)
    {
    case START_STEP:
        lv_label_set_text(button_label, "Start calibrate");

        break;

    case MIN_MAX_SET:
        lv_label_set_text(button_label, "Set min/max");
        for (uint16_t i = 0; i < GUI_CALIBRATE_CH_NUM; i++)
        {
            lv_obj_clear_flag(ADC_min_label[i], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ADC_max_label[i], LV_OBJ_FLAG_HIDDEN);
        }
        break;

    case CENTER_SET:
        lv_label_set_text(button_label, "Set center");
        for (uint16_t i = 0; i < GUI_CALIBRATE_CH_NUM; i++)
        {
            AIsetADCMin(Amin[i], &AnalogChannel[i]);
            AIsetADCMax(Amax[i], &AnalogChannel[i]);
        }
        break;

    case CALIBRATE_DONE:
        lv_label_set_text(button_label, "Done");
        for (uint16_t i = 0; i < GUI_CALIBRATE_CH_NUM; i++)
        {
            AIsetADCCenter(AIgetADCValue(&AnalogChannel[i]), &AnalogChannel[i]);
        }
        break;

    case RETURN_TO_MAIN:
        lv_screen_change(lv_gui_main_screen());
        STsaveSettingsToFlash();
        // settings_changed();
        break;
    default:
        break;
    }
}

static void screen_close_handler(lv_event_t *e)
{
    lv_timer_t *timer = e->user_data;

    lv_timer_del(timer);
}

lv_obj_t *lv_gui_calibrate(void)
{
    calibrate_step = 0;

    lv_obj_t *screen = lv_screen("qw", back_button_handler);

    lv_obj_t *calibrate_button = lv_btn_create(screen);
    lv_obj_align(calibrate_button, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(calibrate_button, 160, 24);
    lv_obj_add_event_cb(calibrate_button, calibrate_button_handler, LV_EVENT_ALL, NULL);

    lv_obj_t *label = lv_label_create(calibrate_button);
    lv_label_set_text(label, "Start calibrate");
    lv_obj_center(label);

    lv_obj_align(calibrate_button, LV_ALIGN_CENTER, 0, 0);

    for (uint16_t i = 0; i < GUI_CALIBRATE_CH_NUM; i++)
    {
        ADC_current_label[i] = lv_label(screen, LV_TEXT_ALIGN_LEFT, NULL, "");
        ADC_min_label[i] = lv_label(screen, LV_TEXT_ALIGN_LEFT, NULL, "");
        ADC_max_label[i] = lv_label(screen, LV_TEXT_ALIGN_LEFT, NULL, "");

        lv_obj_set_width(ADC_current_label[i], 75);
        lv_obj_set_width(ADC_min_label[i], 75);
        lv_obj_set_width(ADC_max_label[i], 75);

        lv_obj_align_to(ADC_current_label[i], screen, LV_ALIGN_TOP_LEFT, GUI_MARGIN, i * (GUI_CH_BAR_HEIGHT + GUI_MARGIN * 1.5) + GUI_SCREEN_START_MARGIN * 2);
        lv_obj_align_to(ADC_min_label[i], ADC_current_label[i], LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);
        lv_obj_align_to(ADC_max_label[i], ADC_min_label[i], LV_ALIGN_OUT_RIGHT_MID, GUI_MARGIN, 0);

        lv_obj_add_flag(ADC_min_label[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ADC_max_label[i], LV_OBJ_FLAG_HIDDEN);

        Amin[i] = AIgetADCValue(&AnalogChannel[i]);
        Amax[i] = AIgetADCValue(&AnalogChannel[i]);
    }

    lv_timer_t *timer = lv_timer_create(calibrate_handler, GUI_DATA_UPDATE_PERIOD, label);

    lv_obj_add_event_cb(screen, screen_close_handler, LV_EVENT_DELETE, timer);

    return screen;
}