#include "lv_gui_styles.h"
#include "lvgl/lvgl.h"

// Bar
lv_style_t bar_bg;
lv_style_t bar_indic;
lv_style_t bar_knob;

// Labels
lv_style_t timer_style;

void lv_styles_init(void)
{
    /* BAR */
    lv_style_init(&bar_bg);
    lv_style_set_border_color(&bar_bg, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&bar_bg, 2);
    lv_style_set_pad_all(&bar_bg, 4);
    lv_style_set_radius(&bar_bg, 4);
    lv_style_set_anim_time(&bar_bg, 100);

    lv_style_init(&bar_indic);
    // lv_style_set_bg_opa(&bar_indic, LV_OPA_COVER);
    // lv_style_set_bg_color(&bar_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_radius(&bar_indic, 2);

    lv_style_init(&bar_knob);
    lv_style_set_bg_opa(&bar_knob, LV_OPA_COVER);
    lv_style_set_bg_color(&bar_knob, lv_color_darken(lv_palette_main(LV_PALETTE_BLUE), 50));
    lv_style_set_border_color(&bar_knob, lv_palette_darken(LV_PALETTE_BLUE_GREY, 3));
    lv_style_set_width(&bar_knob, 4);
    lv_style_set_border_width(&bar_knob, 2);
    lv_style_set_radius(&bar_knob, 2);
    lv_style_set_pad_all(&bar_knob, 2);

    /* Labels */
    lv_style_init(&timer_style);
    // lv_style_set_text_color(&timer_style, lv_color_black());
    lv_style_set_text_font(&timer_style, &lv_font_montserrat_28);
}
