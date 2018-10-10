#include "woof.h"

// 팝업을 위한.
appdata_s *global_appdata;

static void _reject_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (!obj) return;
	evas_object_del(data);
	reject_file();
}

static void _accept_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (!obj) return;
	evas_object_del(data);
	accept_file();
}

static void _popup_hide_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (!obj) return;
	elm_popup_dismiss(obj);
}

static void _popup_hide_finished_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (!obj) return;
	evas_object_del(obj);
}

static void popup_title_text_check_button(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *popup;
	Evas_Object *btn;
	Evas_Object *icon;
	Evas_Object *layout;
	appdata_s *ad = data;
	popup = elm_popup_add(global_appdata->naviframe);
	elm_object_style_set(popup, "circle");
	//uxt_popup_set_rotary_event_enabled(popup, EINA_TRUE);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	dlog_print(DLOG_INFO, LOG_TAG, "4");
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _popup_hide_cb, NULL);
	dlog_print(DLOG_INFO, LOG_TAG, "5");
	evas_object_smart_callback_add(popup, "dismissed", _popup_hide_finished_cb, NULL);
	dlog_print(DLOG_INFO, LOG_TAG, "2");

	layout = elm_layout_add(popup);
	elm_layout_theme_set(layout, "layout", "popup", "content/circle/buttons2");
	elm_object_part_text_set(layout, "elm.text.title", "");
	dlog_print(DLOG_INFO, LOG_TAG, "3");

	elm_object_part_text_set(layout, "elm.text", "Do you want to receive file?");
	elm_object_content_set(popup, layout);
	dlog_print(DLOG_INFO, LOG_TAG, "4");

	btn = elm_button_add(popup);
	elm_object_style_set(btn, "popup/circle/left");
	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_content_set(popup, "button1", btn);
	evas_object_smart_callback_add(btn, "clicked", _accept_cb, popup);
	dlog_print(DLOG_INFO, LOG_TAG, "5");

	icon = elm_image_add(btn);
	elm_image_file_set(icon, "/opt/usr/apps/org.tizen.filetransferreceiver/res/images/tw_ic_popup_btn_check.png", NULL);
	evas_object_size_hint_weight_set(icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_content_set(btn, "elm.swallow.content", icon);
	evas_object_show(icon);
	dlog_print(DLOG_INFO, LOG_TAG, "6");

	btn = elm_button_add(popup);
	elm_object_style_set(btn, "popup/circle/right");
	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_content_set(popup, "button2", btn);
	evas_object_smart_callback_add(btn, "clicked", _reject_cb, popup);
	dlog_print(DLOG_INFO, LOG_TAG, "7");

	icon = elm_image_add(btn);
	elm_image_file_set(icon, "/opt/usr/apps/org.tizen.filetransferreceiver/res/images/tw_ic_popup_btn_delete.png", NULL);
	evas_object_size_hint_weight_set(icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_content_set(btn, "elm.swallow.content", icon);
	evas_object_show(icon);
	dlog_print(DLOG_INFO, LOG_TAG, "8");

	evas_object_show(popup);
	dlog_print(DLOG_INFO, LOG_TAG, "9");
	global_appdata->icon = icon;
	global_appdata->layout = layout;
	global_appdata->popup = popup;
	global_appdata->btn = btn;
	dlog_print(DLOG_INFO, LOG_TAG, "10");
}

void show_file_req_popup()
{
	popup_title_text_check_button(global_appdata->naviframe, NULL, NULL);
	dlog_print(DLOG_INFO, LOG_TAG, "popup here ?");
}
void hide_file_req_popup()
{
	elm_popup_dismiss(global_appdata->popup);
}

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
layout_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

/* naviframe의 최상단 stack item pop 시키기. */
void
naviframe_pop_cb(void *data, Evas_Object *obj, void *event_info) {
	dlog_print(DLOG_INFO, LOG_TAG, " # naviframe pop cb ");
    Evas_Object *naviframe = data;
    elm_naviframe_item_pop(naviframe);
}

void
_popup_message(char *string) {
	show_message_popup(global_appdata->naviframe, string);
}
/* 두 번째 sub layout 생성 함수 */
/*
static void
create_sub_layout(void *data, Evas_Object *obj, void *event_info) {
    Evas_Object *naviframe = data;

    Evas_Object* sub_layout;
    sub_layout = elm_layout_add(naviframe);
    elm_layout_file_set(sub_layout, EDJ_ABSOLUTE_FILE, SUB_LAYOUT);
    evas_object_size_hint_weight_set(sub_layout, EVAS_HINT_EXPAND,
    EVAS_HINT_EXPAND);
    evas_object_show(sub_layout);

    Evas_Object *sub_button;
    sub_button = elm_button_add(sub_layout);
    elm_object_text_set(sub_button,
            "<align=center><font_size=17>Go To <br> Main Layout</font_size></align>");
    evas_object_resize(sub_button, 150, 50);
    evas_object_move(sub_button, 110, 200);
    elm_object_content_set(sub_layout, sub_button);
    evas_object_smart_callback_add(sub_button, "clicked", naviframe_pop_cb, naviframe);
    evas_object_show(sub_button);
    elm_naviframe_item_push(naviframe, NULL, NULL, NULL, sub_layout, "empty");
}
*/

static Eina_Bool
_timeout(void *data EINA_UNUSED)
{
	appdata_s *ad = (appdata_s *)data;
	evas_object_hide(ad->splash_layout);
	elm_naviframe_item_pop(ad->splash_layout);
	dlog_print(DLOG_INFO, LOG_TAG, "# timeout ");
	/* Main Layout */
	_create_main_layout_start(ad, NULL, NULL);
	ad->timer1 = NULL;
	return ECORE_CALLBACK_CANCEL;
}

static void start_splash(appdata_s *ad)
{

	dlog_print(DLOG_INFO, LOG_TAG, "# start splash ");

	ad->splash_layout = elm_layout_add(ad->naviframe);
	elm_layout_file_set(ad->splash_layout, EDJ_ABSOLUTE_FILE, SPLASH_LAYOUT);
	// elm_object_style_set(layout, "circle");
	evas_object_size_hint_weight_set(ad->splash_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(ad->naviframe, ad->splash_layout);
	elm_naviframe_item_push(ad->naviframe, NULL, NULL, NULL, ad->splash_layout,"empty");
	// elm_object_content_set(popup, layout);

	evas_object_show(ad->splash_layout);

	ad->timer1 = ecore_timer_add(2.0, _timeout, ad);
	/*
	if (timer1)
	  {

	     ecore_timer_del(timer1);
	     timer1 = NULL;
	     dlog_print(DLOG_INFO, LOG_TAG, "# splash timer delete ");
	  }
	dlog_print(DLOG_INFO, LOG_TAG, "# splash end ");
	*/
}

/*
void show_message_popup(Evas_Object* obj, char *message)
{
	//_pop_clicked_cb(obj, message);
	dlog_print(DLOG_INFO, LOG_TAG, "device connected message pass");
}
*/

static void
create_base_gui(appdata_s *ad)
{
	/*
		 * Widget Tree
		 * Window
		 *  - conform
		 *   - layout main
		 *    - naviframe */

	/* Window */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_conformant_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	dlog_print(DLOG_INFO, LOG_TAG, "# window setting success. ");
	/* Conformant */
	ad->conform = elm_conformant_add(ad->win);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);
	dlog_print(DLOG_INFO, LOG_TAG, "# conformant setting success. ");
	// Eext Circle Surface Creation
	ad->circle_surface = eext_circle_surface_conformant_add(ad->conform);
	dlog_print(DLOG_INFO, LOG_TAG, "# eext circle surface setting success. ");
	/* Base Layout */
	ad->layout = elm_layout_add(ad->conform);
	evas_object_size_hint_weight_set(ad->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_theme_set(ad->layout, "layout", "application", "default");
	eext_object_event_callback_add(ad->layout, EEXT_CALLBACK_BACK, layout_back_cb, ad);
	evas_object_show(ad->layout);

	elm_object_content_set(ad->conform, ad->layout);
	dlog_print(DLOG_INFO, LOG_TAG, "# base layout setting success. ");
	/* naviframe */
	ad->naviframe = elm_naviframe_add(ad->layout);
	evas_object_size_hint_weight_set(ad->naviframe, EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);

	elm_object_part_content_set(ad->layout, "elm.swallow.content", ad->naviframe);
	//evas_object_show(ad->naviframe);
	dlog_print(DLOG_INFO, LOG_TAG, "# naviframe setting success. ");

	// 팝업 설정
	global_appdata = ad;
	/* Splash */
	start_splash(ad);

	/* Window */
		/*
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_conformant_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
*/
	/* Conformant */
	/*
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);
	dlog_print(DLOG_INFO, LOG_TAG, "gui zzzz");
*/

	/* Bluetooth Image Layout */
	//ad->bluetooth_image = elm_layout_add(ad->naviframe);

	//ad->bluetooth_image = elm_image_add(ad->naviframe);
	//if(elm_image_file_set(ad->bluetooth_image, EDJ_ABSOLUTE_FILE, "main_layout")) {
	//			dlog_print(DLOG_INFO, LOG_TAG, "noti 0");
	//	}

/*
	elm_layout_file_set(ad->bluetooth_image, EDJ_ABSOLUTE_FILE, MAIN_LAYOUT);
	evas_object_size_hint_weight_set(ad->bluetooth_image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_image_fill_outside_set(ad->bluetooth_image, EINA_TRUE);
	elm_image_object_size_get(ad->bluetooth_image, 300, 300);
	if(elm_image_fill_outside_get(ad->bluetooth_image))
		show_message_popup(ad->naviframe, "outside get");
//		elm_image_smooth_set(ad->bluetooth_image, EINA_TRUE);

	elm_object_content_set(ad->naviframe, ad->bluetooth_image);
	evas_object_show(ad->bluetooth_image);
*/

	/* Notification Image Layout */
	/* Main Layout */
/*
	ad->bluetooth_image = elm_layout_add(ad->naviframe);
	elm_layout_file_set(ad->bluetooth_image, EDJ_ABSOLUTE_FILE, MAIN_LAYOUT);
	evas_object_size_hint_weight_set(ad->bluetooth_image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(ad->naviframe, ad->bluetooth_image);
	elm_naviframe_item_push(ad->naviframe, NULL, NULL, NULL, ad->bluetooth_image,"empty");
	evas_object_show(ad->bluetooth_image);
*/
	/*
	Evas_Object* lay;
	const Evas_Object* text1;
	Evas_Object* text2;
	Evas_Object* text3;
	lay = elm_layout_add(ad->naviframe);
	elm_layout_file_set(lay, EDJ_ABSOLUTE_FILE, "testing");
	evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(ad->naviframe, lay);
	elm_naviframe_item_push(ad->naviframe, NULL, NULL, NULL, lay,"empty");
	//evas_object_show(lay);
	text1 = elm_layout_edje_get(lay);
	text2 = edje_object_part_external_object_get(text1, "txt1_title");
	text3 = edje_object_part_external_object_get(text1, "txt2_title");
	dlog_print(DLOG_INFO, LOG_TAG, "text2");
	//evas_object_show(text2);
	dlog_print(DLOG_INFO, LOG_TAG, "text3");

	ad->notification_image = elm_layout_add(ad->naviframe);
	elm_layout_file_set(ad->notification_image, EDJ_ABSOLUTE_FILE, SUB_LAYOUT);
	evas_object_size_hint_weight_set(ad->notification_image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(ad->naviframe, ad->notification_image);
	elm_naviframe_item_push(ad->naviframe, NULL, NULL, NULL, ad->notification_image,"empty");
	evas_object_show(ad->notification_image);
	*/
	//app_get_resource(EDJ_FILE, edj_path, (int)PATH_MAX);
	/*
	ad->main_layout = elm_layout_add(ad->naviframe);
	elm_layout_file_set(ad->main_layout, EDJ_ABSOLUTE_FILE, MAIN_LAYOUT);
	evas_object_size_hint_weight_set(ad->main_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(ad->naviframe, ad->main_layout);
	elm_naviframe_item_push(ad->naviframe, NULL, NULL, NULL, ad->main_layout,"empty");
	evas_object_show(ad->main_layout);
*/
	/* Main Button */
/*
	Evas_Object *main_button;
	main_button = elm_button_add(ad->main_layout);
	elm_object_text_set(main_button,
			"<align=center><font_size=17>Go To <br> Sub Layout</font_size></align>");
	evas_object_resize(main_button, 150, 50);
	evas_object_move(main_button, 110, 200);
	elm_object_content_set(ad->main_layout, main_button);
	evas_object_smart_callback_add(main_button, "clicked", create_sub_layout, ad->naviframe);
	evas_object_show(main_button);
	*/
	//elm_naviframe_item_push(ad->naviframe, NULL, NULL, NULL, ad->main_layout,"empty");
	//elm_naviframe_item_push(ad->naviframe,NULL,NULL,NULL,ad->layout, "empty");

	/* Show window after base gui is set up */
	evas_object_show(ad->win);

}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
	   Initialize UI resources and application's data
	   If this function returns true, the main loop of application starts
	   If this function returns false, the application is terminated */
	appdata_s *ad = data;
	//initialize_sap();
	create_base_gui(ad);
	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main() is failed. err = %d", ret);
	}

	return ret;
}
