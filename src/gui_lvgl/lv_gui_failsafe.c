#include "lv_gui_failsafe.h"
#include "lv_gui_common.h"
#include "lv_gui.h"
#include "lv_gui_main_screen.h"
#include "core/rcchannel.h"
#include "stconfig.h"

#define FAILSAFE_CHANNEL_NUMBER NUMBER_OF_HALF_PART_CHANNELS

static lv_obj_t *___lv_failsafe_ch_bar[FAILSAFE_CHANNEL_NUMBER];

lv_obj_t *trim;

static uint16_t guiToValue(int16_t value)
{
    return (uint16_t)((value - GUI_TRIM_MIN) * (FAILSAFE_CH_MAX - FAILSAFE_CH_MIN) / (GUI_TRIM_MAX - GUI_TRIM_MIN) + FAILSAFE_CH_MIN);
}

static int16_t valueToGui(uint16_t value)
{
    return (int16_t)((value - FAILSAFE_CH_MIN) * (GUI_TRIM_MAX - GUI_TRIM_MIN) / (FAILSAFE_CH_MAX - FAILSAFE_CH_MIN) + GUI_TRIM_MIN);
}

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_screen_change(lv_gui_main_screen());
    }
}

static void failsafe_change_handler(lv_obj_t *obj)
{
    int32_t value = lv_slider_get_value(obj);

    uint16_t channel = *((uint16_t *)obj->user_data);

    RCChanelSetFailsafeValue(guiToValue(value), &RCChanel[channel]);
}

lv_obj_t *lv_gui_failsafe(void)
{
    lv_obj_t *screen = lv_screen("Failsafe", back_button_handler);

    static uint16_t chNumber[FAILSAFE_CHANNEL_NUMBER] = {0};

    for (uint16_t i = 0; i < FAILSAFE_CHANNEL_NUMBER; i++)
    {
        chNumber[i] = i;
        ___lv_failsafe_ch_bar[i] = lv_trim(screen, failsafe_change_handler, valueToGui(RCChanelGetFailsafeValue(&RCChanel[i])), CHLabelShort[i], chNumber + i);

        lv_obj_align_to(___lv_failsafe_ch_bar[i], screen, LV_ALIGN_TOP_MID, 0, i * (GUI_TRIM_BAR_HEIGHT + GUI_MARGIN * 1.5) + GUI_SCREEN_START_MARGIN);
    }

    return screen;
}