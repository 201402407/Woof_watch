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
#define LOG_TAG "woof"

#if !defined(PACKAGE)
#define PACKAGE "org.example.woof"
#endif

#define EDJ_FILE "edje/woof.edj"
#define GRP_MAIN "main"


#endif /* __woof_H__ */
