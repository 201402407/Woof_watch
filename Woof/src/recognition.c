#include "woof.h"


void
Testing(void *data) {
	appdata_s *ad = data;
	// 음성 인식 함수
	ad->timer1 = ecore_timer_add(2.0, sound_recognition_func, ad); // 비동기적 타이머.
	//sleep(2);
	//sound_recognition_func(ad);
}

// 임시
Eina_Bool
sound_recognition_func(void *data) {
	appdata_s *ad = data;
	dlog_print(DLOG_INFO, LOG_TAG, " # sound recognition function ");
	// 팝업 메세지 출력.
	_popup_message("sound recognition success!!");
	// 이전 프로세스 로딩 팝업 닫기.
	//elm_popup_dismiss(ad->popup);
	evas_object_del(ad->popup);
	ad->timer1 = NULL;
	return ECORE_CALLBACK_CANCEL;
}
