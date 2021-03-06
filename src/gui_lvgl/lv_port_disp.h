/**
 * @file lv_port_disp_.h
 *
 */

 /*Copy this file as "lv_port_disp.h" and set this value to "1" to enable content*/

#ifndef LV_PORT_DISP_TEMPL_H
#define LV_PORT_DISP_TEMPL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

/**********************
 * GLOBAL PROTOTYPES
 **********************/
extern lv_disp_drv_t disp_drv;

extern lv_indev_drv_t indev_drv;

extern lv_indev_t *enc_indev;

extern lv_group_t *default_group;

void lv_indev_encoder_group_create();

void lv_port_disp_init(void);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_DISP_TEMPL_H*/

