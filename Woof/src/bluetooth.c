#include "woof.h"
#include "bluetooth.h"

static int connected = 0;
struct priv priv_data;

static gboolean _find_peer_agent(gpointer user_data);

static void on_agent_initialized(sap_agent_h agent,
				 sap_agent_initialized_result_e result,
				 void *user_data)
{
	switch (result) {
	case SAP_AGENT_INITIALIZED_RESULT_SUCCESS:

		dlog_print(DLOG_DEBUG, LOG_TAG, "agent is initialized");

		priv_data.agent = agent;

		break;

	case SAP_AGENT_INITIALIZED_RESULT_DUPLICATED:
		dlog_print(DLOG_ERROR, LOG_TAG, "duplicate registration");

		break;

	case SAP_AGENT_INITIALIZED_RESULT_INVALID_ARGUMENTS:
		dlog_print(DLOG_ERROR, LOG_TAG, "invalid arguments");

		break;

	case SAP_AGENT_INITIALIZED_RESULT_INTERNAL_ERROR:
		dlog_print(DLOG_ERROR,LOG_TAG, "internal sap error");

		break;

	default:
		dlog_print(DLOG_ERROR,LOG_TAG, "unknown status (%d)", result);

		break;

	}
}


static void _on_device_status_changed(sap_device_status_e status,
				      sap_transport_type_e transport_type,
				      void *user_data)
{
	dlog_print(DLOG_INFO, LOG_TAG, "%s, status :%d", __func__, status);

	connected = status;

	switch (transport_type) {
	case SAP_TRANSPORT_TYPE_BT:
		dlog_print(DLOG_DEBUG, LOG_TAG, "transport_type (%d): bt", transport_type);
		break;

	case SAP_TRANSPORT_TYPE_BLE:
		dlog_print(DLOG_DEBUG, LOG_TAG, "transport_type (%d): ble", transport_type);
		break;

	case SAP_TRANSPORT_TYPE_TCP:
		dlog_print(DLOG_DEBUG, LOG_TAG, "transport_type (%d): tcp/ip", transport_type);
		break;

	case SAP_TRANSPORT_TYPE_USB:
		dlog_print(DLOG_DEBUG, LOG_TAG, "transport_type (%d): usb", transport_type);
		break;

	case SAP_TRANSPORT_TYPE_MOBILE:
		dlog_print(DLOG_DEBUG, LOG_TAG, "transport_type (%d): mobile", transport_type);
		break;

	default:
		dlog_print(DLOG_ERROR, LOG_TAG, "unknown transport_type (%d)", transport_type);
		break;
	}

	switch (status) {
	case SAP_DEVICE_STATUS_DETACHED:
		dlog_print(DLOG_DEBUG, LOG_TAG, "device is not connected.");
		sap_peer_agent_destroy(priv_data.peer_agent);
		priv_data.peer_agent = NULL;

		file_on_progress = 0;

		show_message_popup(ad->naviframe, "Device Disconnected.");

		remove_send_button();

		break;

	case SAP_DEVICE_STATUS_ATTACHED:
		dlog_print(DLOG_DEBUG, LOG_TAG, "Attached calling find peer now");

		show_popup("Connected. Call find peer.");

		break;

	default:
		dlog_print(DLOG_ERROR, LOG_TAG, "unknown status (%d)", status);
		break;
	}
}

gboolean agent_initialize()
{
	int result = 0;

	do {
		result = sap_agent_initialize(priv_data.agent, "/sample/filetransfer", SAP_AGENT_ROLE_CONSUMER,
					      on_agent_initialized, NULL);

		dlog_print(DLOG_DEBUG, LOG_TAG, "SAP >>> getRegisteredServiceAgent() >>> %d", result);
	} while (result != SAP_RESULT_SUCCESS);

	return TRUE;
}

gboolean initialize_sap(void)
{
	sap_agent_h agent = NULL;

	sap_agent_create(&agent);

	priv_data.agent = agent;

	agent_initialize();

	sap_set_device_status_changed_cb(_on_device_status_changed, NULL);

	return TRUE;
}

void
bluetooth_main(void *data) {

}
