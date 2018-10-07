#ifndef __woof_H__
#define __woof_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "AALOG_WOOF"

#if !defined(PACKAGE)
#define PACKAGE "org.example.woof"
#endif

#define EDJ_ABSOLUTE_FILE "/opt/usr/apps/org.example.woof/res/edje/woof.edj"
#define MAINIMAGE_DIR "/opt/usr/apps/org.example.woof/res/mainimages"
#define ICON_DIR "/opt/usr/apps/org.example.woof/res/images"
#define MAIN_LAYOUT "woof/index_thumbnail"
#define SPLASH_LAYOUT "woof/splash_layout"
#define SUB_LAYOUT "sub_layout"

typedef struct appdata{
	Evas_Object* win;
	Evas_Object* layout;
	Evas_Object* conform;
	Evas_Object* naviframe;
	Evas_Object* splash_layout;
	Evas_Object* main_layout;
	Evas_Object* box;
	Eext_Circle_Surface *circle_surface;
	Ecore_Timer *timer1;
} appdata_s;

void _create_main_layout_start(void *data, Evas_Object *obj, void *event_info);
void naviframe_pop_cb(void *data, Evas_Object *obj, void *event_info);

#endif /* __woof_H__ */
