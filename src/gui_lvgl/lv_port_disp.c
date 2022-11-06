/**
 * @file lv_port_disp.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "lvgl.h"
#include "gui/stdispdriver.h"
#include "core/periphery.h"
#include "core/encoder.h"
#include "stdint.h"
#include "tim.h"
#include "gui/logo.h"
#include "core/iosettings.h"

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_disp_drv_t disp_drv;

lv_indev_drv_t indev_drv;

lv_indev_t *enc_indev;

lv_group_t *default_group;
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_indev_encoder_group_create()
{

    lv_group_t *group = lv_group_get_default();

    if (group != NULL)
    {
        lv_group_del(group);
    }

    group = lv_group_create();

    lv_group_set_default(group);
    lv_indev_set_group(enc_indev, group);
}

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    static lv_color_t disp_buf1[LV_HOR_RES * 20];
    static lv_color_t disp_buf2[LV_HOR_RES * 20];
    static lv_disp_draw_buf_t buf;
    lv_disp_draw_buf_init(&buf, disp_buf1, disp_buf2, LV_HOR_RES * 20);

    lv_disp_drv_init(&disp_drv);

    disp_drv.draw_buf = &buf;
    disp_drv.flush_cb = disp_flush;
    disp_drv.hor_res = LV_HOR_RES;
    disp_drv.ver_res = LV_VER_RES;
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    lv_theme_t *th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);

    lv_disp_set_theme(disp, th);

    default_group = lv_group_create();
    lv_group_set_default(default_group);

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;

    enc_indev = lv_indev_drv_register(&indev_drv);
    lv_indev_set_group(enc_indev, default_group);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    STLCDinit();
    STLCDsetBrightness(CommonSettings.DisplayBrightness); //
    ShowLogo();
}

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    lv_draw_area(area->x1, area->y1, area->x2, area->y2, color_p);
}

static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    (void)indev_drv; /*Unused*/

    static int32_t encoder_diff = 0;

    data->state = buttonPressSimple(&hbtnEnter);
    data->enc_diff = EncoderGetCounter(&Encoder) - encoder_diff;

    encoder_diff = EncoderGetCounter(&Encoder);
}

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
