#ifndef __wooftest_H__
#define __wooftest_H__

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
#define PACKAGE "org.example.wooftest"
#endif

#define EDJ_ABSOLUTE_FILE "/opt/usr/apps/org.example.wooftest/res/edje/wooftest.edj"
#define MAINIMAGE_DIR "/opt/usr/apps/org.example.wooftest/mainimage"
#define MAIN_LAYOUT "main_layout"
#define SUB_LAYOUT "sub_layout"

typedef struct appdata{
	Evas_Object* win;
	Evas_Object* layout;
	Evas_Object* conform;
	Evas_Object* naviframe;
	Evas_Object* splash_layout;
	Evas_Object* main_layout;
	Evas_Object* sub_layout;
	Evas_Object* bluetooth_image;
	Evas_Object* notification_image;
	Evas_Object* box;

	Ecore_Timer *timer1;
} appdata_s;

void _create_main_layout_start(void *data);

#endif /* __wooftest_H__ */
