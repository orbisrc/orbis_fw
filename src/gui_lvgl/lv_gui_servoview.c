#include "lv_gui_servoview.h"
#include "lv_gui_conf.h"


static lv_obj_t *  ___lv_ch_bar [NUMBER_OF_CHANNELS];
static lv_style_t style_bg;
static lv_style_t style_indic;
static lv_style_t style_line;

void lv_ch_list_set_channel_value(uint16_t ch, int32_t value){
    lv_bar_set_value(___lv_ch_bar[ch], value , LV_ANIM_OFF); 
}

static void lv_bar_center_line_style_init(){
    /*Create style*/
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 4);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_line_rounded(&style_line, false);
}

static  lv_style_t * lv_ch_bar_bg_style_init () {

    lv_style_init(&style_bg);
    lv_style_set_border_color(&style_bg, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&style_bg, 2);
    lv_style_set_pad_all(&style_bg, 4); /*To make the indicator smaller*/
    lv_style_set_radius(&style_bg, 4);
    lv_style_set_anim_time(&style_bg, 100);

    return &style_bg;
}

static  lv_style_t * lv_ch_bar_indic_style_init () {

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_radius(&style_indic, 1);

    return &style_indic;
}

static lv_style_t * get_ch_bar_indic_style(){
    return &style_indic;
}

static lv_style_t * get_ch_bar_bg_style(){
    return &style_bg;
}

static lv_style_t * get_ch_bar_center_line_style(){
    return &style_line;
}

static void lv_bar_center_line(lv_obj_t * parent)
{
    static lv_point_t line_points[] = { {2, 0}, {2, CH_BAR_HEIGHT}};

    lv_obj_t * line = lv_line_create(parent);
    lv_line_set_points(line, line_points, 2);     /*Set the points*/
    lv_obj_add_style(line, get_ch_bar_center_line_style(), 0);
    lv_obj_center(line);
}

static lv_obj_t * lv_title(lv_obj_t * parent, char * title){

    lv_obj_t * label = lv_label_create(parent);
    lv_obj_set_width(label, 240);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT); 
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);
    lv_label_set_text(label, title);

    return label;
};


static lv_obj_t * lv_ch_label(lv_obj_t * parent, char * text){

    lv_obj_t * label = lv_label_create(parent);
    lv_obj_set_width(label, 30);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT); 
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_label_set_text(label, text);

    return label;
};


static lv_obj_t * lv_ch_bar(lv_obj_t * parent, uint32_t ini_value){

    lv_obj_t * bar = lv_bar_create(parent);
    lv_obj_remove_style_all(bar);  /*To have a clean start*/
    lv_obj_add_style(bar, get_ch_bar_bg_style(), 0);
    lv_obj_add_style(bar, get_ch_bar_indic_style(), LV_PART_INDICATOR);
    lv_bar_set_mode(bar, LV_BAR_MODE_SYMMETRICAL);
    lv_bar_set_range(bar, -125, 125);

    lv_obj_set_size(bar, CH_BAR_WIDTH, CH_BAR_HEIGHT);
    lv_obj_center(bar);
    lv_bar_center_line(bar);
    lv_bar_set_value(bar, ini_value, LV_ANIM_ON);

    //lv_ch_label(parent, "CHN");

    return bar;
}

static void lv_back_btn(lv_obj_t * parent)
{
    lv_obj_t * label;

    lv_obj_t * btn = lv_btn_create(parent);
    // lv_obj_add_event_cb(btn, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(btn, 22, 16);
    label = lv_label_create(btn);
    lv_label_set_text(label, "Back");
    lv_obj_center(label);

}

static void lv_ch_list(lv_obj_t * parent){

    static lv_style_t style_list;

    lv_ch_bar_bg_style_init ();
    lv_ch_bar_indic_style_init();
    lv_bar_center_line_style_init();
    lv_style_init(&style_list);
    lv_style_set_pad_row(&style_list, 4);
    lv_style_set_border_color(&style_list, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&style_list, 0);
    lv_obj_t * cont_col = lv_obj_create(parent);
    lv_obj_add_style(cont_col, &style_list, 0);
    lv_obj_set_size(cont_col, 240  , 280);
    lv_obj_align(cont_col, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont_col, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_CENTER , LV_FLEX_ALIGN_SPACE_BETWEEN );

    for (uint16_t i = 0; i <= NUMBER_OF_CHANNELS - 1; i++)
    {
        ___lv_ch_bar[i] = lv_ch_bar(cont_col, 0);
         
    }    
}

void lv_gui_servoview(void){

    lv_title(lv_scr_act(), "Servo view");
    lv_ch_list(lv_scr_act());
    lv_back_btn(lv_scr_act());
}