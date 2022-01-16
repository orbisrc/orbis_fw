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

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

/**********************
 *  STATIC VARIABLES
 **********************/
extern lv_disp_drv_t disp_drv;
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    static lv_color_t disp_buf1[LV_HOR_RES * 40];
    static lv_color_t disp_buf2[LV_HOR_RES * 40];
    static lv_disp_draw_buf_t buf;
    lv_disp_draw_buf_init(&buf, disp_buf1, disp_buf2, LV_HOR_RES * 40);

    lv_disp_drv_init(&disp_drv);

    disp_drv.draw_buf = &buf;
    disp_drv.flush_cb = disp_flush;
    disp_drv.hor_res = LV_HOR_RES;
    disp_drv.ver_res = LV_VER_RES;
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    STLCDinit();
}

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    lv_draw_area(area->x1, area->y1, area->x2, area->y2, color_p);
}

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
