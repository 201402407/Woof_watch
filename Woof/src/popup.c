#include "woof.h"

typedef struct _item_data
{
	int index;
	Elm_Object_Item *item;
} item_data;

Evas_Object *top = NULL;

typedef struct _Item_Data
{
	int index;
	Evas_Object *genlist;
	Elm_Object_Item *item;
} Item_Data;

typedef struct _Testitem
{
	Elm_Object_Item *item;
	const char *text;
	const char *path;
} Testitem;

typedef struct _spin_date spin_date;
struct _spin_date
{
	Evas_Object *scroller;
	Evas_Object *spinner1;
	Evas_Object *spinner2;
	Evas_Object *layout;

	double posx1, posx2, prev1, prev2;
};
static void
_popup_hide_cb(void *data, Evas_Object *obj, void *event_info)
{
	if(!obj) return;
	elm_popup_dismiss(obj);
}

static void
_popup_hide_finished_cb(void *data, Evas_Object *obj, void *event_info)
{
	if(!obj) return;
	evas_object_del(obj);
}


void
_popup_small_process_loading(void *data, Evas_Object *obj, void *event_info)
{
	dlog_print(DLOG_INFO, LOG_TAG, " # image1 popup progress loading ");
	Evas_Object *popup, *layout, *progressbar;
	//Evas_Object *parent_nv = data;
	appdata_s *ad = data;

	dlog_print(DLOG_INFO, LOG_TAG, " # image1 popup progress loading 1");
	popup = elm_popup_add(ad->win);
	elm_object_style_set(popup, "circle");
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _popup_hide_cb, NULL);
	evas_object_smart_callback_add(popup, "dismissed", _popup_hide_finished_cb, NULL);
	dlog_print(DLOG_INFO, LOG_TAG, " # image1 popup progress loading 2");

	layout = elm_layout_add(popup);
	elm_layout_file_set(layout, EDJ_ABSOLUTE_FILE, POPUP_PROGRESSBAR);
	elm_object_content_set(popup, layout);
	elm_object_part_text_set(layout, "elm.text", "sound recognition loading ..");
	const char *temp = elm_object_part_text_get(layout, "elm.text");
	dlog_print(DLOG_INFO, LOG_TAG, temp);
	evas_object_show(layout);

	dlog_print(DLOG_INFO, LOG_TAG, " # image1 popup progress loading 3");
	progressbar = elm_progressbar_add(layout);
	elm_object_style_set(progressbar, "process/popup/small");
	evas_object_size_hint_align_set(progressbar, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(progressbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_progressbar_pulse(progressbar, EINA_TRUE);
	elm_object_part_content_set(layout, "elm.swallow.content", progressbar);
	evas_object_show(progressbar);
	dlog_print(DLOG_INFO, LOG_TAG, " # image1 popup progress loading 4");
	evas_object_show(popup);
	dlog_print(DLOG_INFO, LOG_TAG, " # image1 popup progress loading 5");
}
