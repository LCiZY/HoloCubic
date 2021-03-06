#ifndef APP_TOGETHER_GUI_H
#define APP_TOGETHER_GUI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
#define ANIEND                      \
    while (lv_anim_count_running()) \
        lv_task_handler(); //等待动画完成

    void together_gui_init(void);
    void together_gui_del(void);
    void display_together(long long networkTime, uint32_t t);
    void display_us(const char *file_name);

#ifdef __cplusplus
} /* extern "C" */
#endif


#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
    extern const lv_img_dsc_t app_together_icon;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif