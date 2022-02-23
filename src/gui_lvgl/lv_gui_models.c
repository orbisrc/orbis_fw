#include "lv_gui_ai.h"
#include "lv_gui_common.h"
#include "lv_gui.h"
#include "lv_gui_main_screen.h"
#include "lv_gui_models.h"
#include "core/iosettings.h"
#include "stconfig.h"

#define GUI_MODELS_NUMBER MODEL_MEMORY_NUM
#define GUI_MODELS_ITEM_MENU_HEIGHT 118
#define GUI_MODELS_ITEM_MENU_WIDTH 120
#define GUI_MODELS_SCREEN_HEIGHT 320

static lv_obj_t *lv_model_menu(lv_obj_t *parent);

static lv_obj_t *lv_keyboard_(lv_obj_t *parent, void *user_data);

static void back_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_screen_change(lv_gui_main_screen());
    }
}

static void close_menu_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_screen_change(lv_gui_models());
    }
}

static void load_profile_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    ModelSettingsTypeDef *profile = e->target->parent->user_data;

    if (code == LV_EVENT_CLICKED)
    {
        CommonSettings.CurrentModelID = profile->ID;
        STloadProfile(&ModelSettings[CommonSettings.CurrentModelID]);
        settings_changed();
        lv_screen_change(lv_gui_main_screen());
    }
}

static void edit_profile_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    ModelSettingsTypeDef *profile = e->target->parent->user_data;
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_keyboard_(obj->parent->parent, profile);
        lv_obj_del(obj->parent);
    }
}

static void model_button_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    ModelSettingsTypeDef *profile = e->user_data;

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_t *menu = lv_model_menu(obj->parent->parent);
        lv_obj_set_user_data(menu, profile);

        if (GUI_MODELS_SCREEN_HEIGHT - obj->coords.y1 < GUI_MODELS_ITEM_MENU_HEIGHT)
        {
            lv_obj_set_pos(menu, obj->coords.x1, GUI_MODELS_SCREEN_HEIGHT - GUI_MODELS_ITEM_MENU_HEIGHT);
            return;
        }
        lv_obj_set_pos(menu, obj->coords.x1, obj->coords.y1);
    }
}

static void keyboard_accept_handler(lv_event_t *e)
{
    LV_LOG_USER("Keyboard OK!");

    lv_obj_t *text = lv_keyboard_get_textarea(e->current_target);

    ModelSettingsTypeDef *profile = e->user_data;

    strlcpy(profile->Name, lv_textarea_get_text(text), MAX_RC_NAME);
    settings_changed();
    lv_screen_change(lv_gui_models());
}

static void keyboard_close_handler(lv_event_t *e)
{
    LV_LOG_USER("Keyboard Close!");

    lv_screen_change(lv_gui_models());
}

static lv_obj_t *lv_keyboard_(lv_obj_t *parent, void *user_data)
{
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, 240, 156);
    lv_obj_center(container);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_color(container, lv_palette_darken(LV_PALETTE_BLUE, 2), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(container, lv_palette_darken(LV_PALETTE_GREY, 4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(container, GUI_MARGIN, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *keyboard = lv_keyboard_create(container);
    lv_obj_set_width(keyboard, 232);
    lv_obj_set_height(keyboard, 100);
    lv_obj_add_event_cb(keyboard, keyboard_accept_handler, LV_EVENT_READY, user_data);
    lv_obj_add_event_cb(keyboard, keyboard_close_handler, LV_EVENT_CANCEL, NULL);


    lv_obj_t *text = lv_textarea_create(container);
    lv_obj_align(text, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_align(keyboard, LV_ALIGN_TOP_MID, 0, GUI_MARGIN * 11);
    lv_textarea_set_placeholder_text(text, "Enter profile name");
    lv_obj_set_size(text, 232, 40);
    lv_group_remove_obj(text);

    lv_keyboard_set_textarea(keyboard, text);

    lv_obj_add_flag(text, LV_STATE_DISABLED);
    lv_obj_add_flag(keyboard, LV_STATE_FOCUS_KEY);

    return container;
}

static lv_obj_t *lv_model_menu(lv_obj_t *parent)
{
    lv_group_t *group = lv_group_get_default();
    lv_group_remove_all_objs(group);

    lv_obj_t *list = lv_list_create(parent);
    lv_obj_set_size(list, GUI_MODELS_ITEM_MENU_WIDTH, GUI_MODELS_ITEM_MENU_HEIGHT);
    lv_obj_set_style_border_color(list, lv_palette_darken(LV_PALETTE_BLUE, 2), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *btn;
    btn = lv_list_add_btn(list, LV_SYMBOL_DOWNLOAD, "Load");
    lv_obj_add_event_cb(btn, load_profile_button_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_EDIT, "Edit");
    lv_obj_add_event_cb(btn, edit_profile_button_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_CLOSE, "Close");
    lv_obj_add_event_cb(btn, close_menu_button_handler, LV_EVENT_CLICKED, NULL);

    return list;
}

lv_obj_t *lv_gui_models(void)
{
    lv_obj_t *screen = lv_screen("Profiles", back_button_handler);

    lv_obj_t *list = lv_list_create(screen);
    lv_obj_set_size(list, 232, 288);

    lv_obj_t *btn;
    for (uint32_t i = 0; i < GUI_MODELS_NUMBER; i++)
    {
        btn = lv_list_add_btn(list, LV_SYMBOL_BULLET, ModelSettings[i + 1].Name);
        lv_obj_add_event_cb(btn, model_button_handler, LV_EVENT_CLICKED, &ModelSettings[i + 1]);
    }

    lv_obj_align_to(list, screen, LV_ALIGN_TOP_MID, 0, GUI_SCREEN_START_MARGIN);

    return screen;
}