#ifndef __bluetooth_H__
#define __bluetooth_H__

#include "woof.h"
#include <stdio.h>
#include <glib.h>
#include <dlog.h>
#include <app_common.h>

#include <string.h>
#include <sap.h>
#include <sap_file_transfer.h>

Eina_Bool bluetooth_main(void *data);
Eina_Bool sap_set_device_status_change(void *data);
#endif /* __bluetooth_H__ */
