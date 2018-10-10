/*
 * Copyright (c) 2013-2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * PROPRIETARY/CONFIDENTIAL
 *
 * This software is the confidential and proprietary information of
 * SAMSUNG ELECTRONICS ("Confidential Information").
 * You shall not disclose such Confidential Information and shall
 * use it only in accordance with the terms of the license agreement
 * you entered into with SAMSUNG ELECTRONICS.
 * SAMSUNG make no representations or warranties about the suitability
 * of the software, either express or implied, including but not
 * limited to the implied warranties of merchantability, fitness for
 * a particular purpose, or non-infringement.
 * SAMSUNG shall not be liable for any damages suffered by licensee as
 * a result of using, modifying or distributing this software or its derivatives.
 */

#ifndef __SAP_H__
#define __SAP_H__
/**
 * @file            sap.h
 * @brief           SAP SDK interface API
 * @addtogroup      SAMSUNG_ACCESSORY_PROTOCOL
 * @{
 */
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           The structure type for the agent handle
 * @since_tizen     2.3.1
 */
typedef struct sap_agent_s *sap_agent_h;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           The structure type for the peer agent handle
 * @since_tizen     2.3.1
 */
typedef struct sap_peer_agent_s *sap_peer_agent_h;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           The structure type for the remote peer socket handle
 * @since_tizen     2.3.1
 */
typedef struct sap_socket_s *sap_socket_h;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for the status codes that may be used to convey generic states as
 *                  needed for SAP interaction.
 * @since_tizen     2.3.1
 */
typedef enum {
	/** When SAP privilege has not been added in project's manifest file.
	 * <br>Privilege: "http://developer.samsung.com/tizen/privilege/accessoryprotocol".*/
	SAP_RESULT_PERMISSION_DENIED = -13,
	/** When a request has timed out. */
	SAP_RESULT_TIMEOUT = -2,
	/** On failure of a request. */
	SAP_RESULT_FAILURE = -1,
	/** On successful execution of the request. */
	SAP_RESULT_SUCCESS = 0,

} sap_result_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for response codes that shall be sent with
 *                  #sap_agent_initialized_cb() callback.
 * @since_tizen     2.3.1
 */
typedef enum {
	/** Initialization was successful*/
	SAP_AGENT_INITIALIZED_RESULT_SUCCESS = 0,
	/** App calls #sap_agent_initialize() with same ASP ID again */
	SAP_AGENT_INITIALIZED_RESULT_DUPLICATED = 1,
	/** Invalid arguments, like ASP ID NULL etc. Apps should check the input parameters & retry */
	SAP_AGENT_INITIALIZED_RESULT_INVALID_ARGUMENTS = 2,
	/** Any other internal error. App can retry registration */
	SAP_AGENT_INITIALIZED_RESULT_INTERNAL_ERROR = 0xFF
} sap_agent_initialized_result_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for SAP service agent roles.
 *                  SAP provides two types of roles for service agents,
 *                  one will be a provider agent and second will be a consumer.
 *                  To Use SAP framework service agent should have one of these two roles.
 * @since_tizen     2.3.1
 */
typedef enum {
	/** The service agent provider can provide some services based on
	    App's objectives(decided by application developers) using SAP framework.
	    e.g. Location Provider :- Role of Location provider is always send location data to
	    the location consumer.  */
	SAP_AGENT_ROLE_PROVIDER = 0,
	/** The service agent consumer will use the services provided by service agent provider,
	  These services will depend upon the objectives that peer applications had agreed upon.
	  e.g. Location Consumer: Job of location consumer here will always be to
	   receive the location data from the remote Location Service Agent Provider.*/
	SAP_AGENT_ROLE_CONSUMER = 1
} sap_agent_role_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function gets triggered after initialization of service agent
 *                  #sap_agent_initialize() is completed .
 * @since_tizen     2.3.1
 *
 * @param[in]       agent               Handle of app's sap agent which was created using
 *                                      #sap_agent_create().
 * @param[in]       result              Result of agent initialize request .
 *                                      see #sap_agent_initialized_result_e for detailed result code(s).
 *
 * @param[in]       user_data           The user data passed from the register callback function
 *
 * @pre             This callback will be triggered if it was registered
 *                  using #sap_agent_initialize().
 *
 * @see             sap_agent_initialize()
 *
 */
typedef  void (*sap_agent_initialized_cb)(sap_agent_h agent,
					  sap_agent_initialized_result_e result,
					  void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Functions to create an instace of a SAP agent.
 * @since_tizen     2.3.1
 *
 * @param[out]      agent           A handle will be returned which will act as a sap agent,
 *                                  Then same handle will be used for all SAP interactions.
 *                                  This handle should be destroyed by the app using #sap_agent_destroy() method.
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 */
int sap_agent_create(sap_agent_h *agent);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Functions to destroy created instance of the SAP agent.
 * @since_tizen     2.3.1
 *
 * @param[in]       agent           Handle of agent which was received as an out parameter of
 *                                  #sap_agent_create().
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 */
int sap_agent_destroy(sap_agent_h agent);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This function is called by the application to get updated agent handle based on
 *                  service agent role and ASP profile ID
 * @since_tizen     2.3.1
 *
 * @param[in]       agent           Handle of agent which was received as an out parameter of
 *                                  #sap_agent_create().
 *
 * @param[in]       profile_id      Identifier that uniquely represents a SAP profile.
 * @param[in]       role            Role of service agent(provider/consumer).
 *                                  see #sap_agent_role_e for definitions of service agent roles.
 * @param[in]       callback        The callback function to register
 * @param[in]       user_data       The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_agent_initialized_cb()
 * @see             sap_agent_deinitialize()
 */
int sap_agent_initialize(sap_agent_h agent,
			 const char *profile_id,
			 sap_agent_role_e role,
			 sap_agent_initialized_cb callback,
			 void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for response codes that shall be sent with
 *                  #sap_agent_deinitialized_cb() callback.
 * @since_tizen     2.3.1
 */
typedef enum {
	/** De-initialization was successful*/
	SAP_AGENT_DEINITIALIZED_RESULT_SUCCESS = 0,
	/** De-initialization request failed because invalid sap agent handle was
	 supplied while calling #sap_agent_deinitialize() method.*/
	SAP_AGENT_DEINITIALIZED_RESULT_INVALID_ARGUMENTS = 1
} sap_agent_deinitialized_result_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This is the callback function.This function gets triggered after
 *                  #sap_agent_deinitialize() is completed
 * @since_tizen     2.3.1
 *
 * @param[in]       agent               Handle of agent which is de-initialized,
 *                                      But it should be destroyed by the application by calling #sap_agent_destroy().
 * @param[in]       result              Result of de-initialization request
 * @param[in]       user_data           The user data passed from the register function
 *
 * @see                                 sap_agent_deinitialize()
 */

typedef  void (*sap_agent_deinitialized_cb)(sap_agent_h agent,
					    sap_agent_deinitialized_result_e result,
					    void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This will de-initialize the agent handle.
 * @since_tizen     2.3.1
 *
 * @param[in]       agent               Handle of agent which was received as an out parameter of
 *                                      #sap_agent_create(), which got updated
 *                                      in sap_agent_initialized_cb().
 *                                      After this agent handle will not be valid for any SAP interactions.
 * @param[in]       callback            The callback function to register
 * @param[in]       user_data           The user data to be passed to the registered callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_agent_deinitialized_cb()
 * @see             sap_agent_initialize()
 */
int sap_agent_deinitialize(sap_agent_h agent,
			   sap_agent_deinitialized_cb callback,
			   void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumeration for #sap_agent_find_peer_agent() response codes.
 * @since_tizen     2.3.1
 */
typedef enum {
	/** Peer agent found successfully.
	* App can create service connection using #sap_agent_request_service_connection() */
	SAP_PEER_AGENT_FOUND_RESULT_FOUND = 0,
	/** When #sap_agent_find_peer_agent() was called, remote device was not connected or service transport type
	 *  and device connection type are different.
	 *  Wait for #sap_device_status_changed_cb() before retrying again */
	SAP_PEER_AGENT_FOUND_RESULT_DEVICE_NOT_CONNECTED = 1,
	/** ASP Id or service intended in #sap_agent_find_peer_agent() not found in remote device.
	 *  Service is not supported at other side. Check valid agent ID and retry again */
	SAP_PEER_AGENT_FOUND_RESULT_SERVICE_NOT_FOUND = 2,
	/** Request timed out without any response from the peer device.
	 *  Retry again, it will succeed if service comes up at other side */
	SAP_PEER_AGENT_FOUND_RESULT_TIMEDOUT = 3,
	/** Searching for peer agent has some internal error.
	 *  Recheck passed parameters and retry */
	SAP_PEER_AGENT_FOUND_RESULT_INTERNAL_ERROR = 0xFF
} sap_peer_agent_found_result_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for status of the peer agent
 * @since_tizen     2.3.1
 */
typedef enum {
	/** Defines that the peer is not available. */
	SAP_PEER_AGENT_STATUS_UNAVAILABLE = 0,
	/** Defines that the peer is unavailable */
	SAP_PEER_AGENT_STATUS_AVAILABLE = 1
} sap_peer_agent_status_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations of various transport types.
 * @since_tizen     2.3.1
 */
typedef enum {
	/** All connectivities*/
	SAP_TRANSPORT_TYPE_ALL = 0x00,
	/** BT connectivity*/
	SAP_TRANSPORT_TYPE_BT = 0x01,
	/** BLE connectivity*/
	SAP_TRANSPORT_TYPE_BLE = 0x02,
	/** TCP connectivity*/
	SAP_TRANSPORT_TYPE_TCP = 0x04,
	/** USB connectivity*/
	SAP_TRANSPORT_TYPE_USB = 0x08,
	/** MOBILE connectivity*/
	SAP_TRANSPORT_TYPE_MOBILE = 0x10,
	/** End of Connectivity*/
	SAP_TRANSPORT_TYPE_NONE = 0xFF
} sap_transport_type_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Functions to destroy handle of peer_agent which was received in #sap_peer_agent_updated_cb()
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent      Handle of peer agent which should be destroyed by SAP SDK
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 */
int sap_peer_agent_destroy(sap_peer_agent_h peer_agent);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Getter method to retrieve peer agent id from the peer agent handle
                    which was received in #sap_peer_agent_updated_cb()
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent      Handle of peer agent which should be destroyed by SAP SDK
 * @param[out]      peer_agent_id   Pointer where peer agent id should be saved.
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 */
int sap_peer_agent_get_peer_agent_id(sap_peer_agent_h peer_agent, unsigned int *peer_agent_id);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Getter method to retrieve profile version from the peer agent handle
                    which was received in #sap_peer_agent_updated_cb()
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of peer agent which should be destroyed by SAP SDK
 * @param[out]      profile_version     Pointer where profile version of remote peer should be saved.
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 */
int sap_peer_agent_get_profile_version(sap_peer_agent_h peer_agent, char **profile_version);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Getter method to retrieve application name from the peer agent handle
 *                  which was received in #sap_peer_agent_updated_cb()
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of peer agent which should be destroyed by SAP SDK
 * @param[out]      app_name            Pointer where application name of remote peer should be saved.
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 */
int sap_peer_agent_get_app_name(sap_peer_agent_h peer_agent, char **app_name);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Getter method to retrieve device name from the peer agent handle
 *                  which was received in #sap_peer_agent_updated_cb()
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of peer agent which should be destroyed by SAP SDK
 * @param[out]      device_name         Pointer where device name of remote peer should be saved.
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 */
int sap_peer_agent_get_device_name(sap_peer_agent_h peer_agent, char **device_name);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Getter method to retrieve device's mac address from the peer agent handle
 *                  which was received in #sap_peer_agent_updated_cb()
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of peer agent which should be destroyed by SAP SDK
 * @param[out]      device_address      Pointer where device address of remote peer should be saved.
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 */
int sap_peer_agent_get_device_address(sap_peer_agent_h peer_agent, char **device_address);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Getter method to retrieve vendor name from the peer agent handle
 *                  which was received in #sap_peer_agent_updated_cb()
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of peer agent which should be destroyed by SAP SDK
 * @param[out]      vendor_id           Pointer where vendor details of remote peer should be saved.
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 */
int sap_peer_agent_get_device_vendor_id(sap_peer_agent_h peer_agent, char **vendor_id);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Getter method to retrieve connectivity details from the peer agent handle
 *                  which was received in #sap_peer_agent_updated_cb()
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of peer agent which should be destroyed by SAP SDK
 * @param[out]      transport_type      Pointer where connectivity details of remote peer should be saved.
 *                                      see #sap_transport_type_e for reference.
 *                                      If device is connected via multiple connectivities then received
 *                                      transport type will be bitwise OR of respective connectivities.
 *
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 */
int sap_peer_agent_get_transport_type(sap_peer_agent_h peer_agent, sap_transport_type_e *transport_type);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function gets triggered when a matching Peer Agent is registered on a connected device.
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of the remote matching peer agent, as indentified by sap_agent handle
 *                                      which was supplied in #sap_agent_find_peer_agent()
 * @param[in]       status              Tells availability of the Peer.
 *                                      Refer #sap_peer_agent_status_e for detailed status codes.
 * @param[in]       result              Detailed result codes.
 * @param[in]       user_data           The user data passed from the register function
 *
 * @return
 *
 * @post            Upon receiving the #SAP_PEER_AGENT_STATUS_AVAILABLE the App can initiate a service Connection ,
 *                  Upon receiving the #SAP_PEER_AGENT_STATUS_UNAVAILABLE the App has to wait for the
 *                  #SAP_PEER_AGENT_STATUS_AVAILABLE.
 *
 * @see             sap_peer_agent_status_e
 */
typedef  void (*sap_peer_agent_updated_cb)(sap_peer_agent_h peer_agent,
					   sap_peer_agent_status_e status,
					   sap_peer_agent_found_result_e result,
					   void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This function call finds Peer Agent with matching ASP ID as identified by sap agent handle.
 * @since_tizen     2.3.1
 *
 * @param[in]       agent               Agent handle which was created using #sap_agent_create() and
 *                                      updated in #sap_agent_initialized_cb() callback.
 * @param[in]       callback            The callback function to register
 * @param[in]       user_data           The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_peer_agent_updated_cb()
 *
 * @remarks         The API needs to be called from App's context
 *
 */
int sap_agent_find_peer_agent(sap_agent_h agent,
			      sap_peer_agent_updated_cb callback,
			      void *user_data);



/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for result of service connection creation
 * @since_tizen     2.3.1
 */
typedef enum {
	/** Service Connection is successful. App can continue to send data using
	 #sap_socket_send_data()/#sap_socket_send_secure_data */
	SAP_CONNECTION_SUCCESS = 0,
	/** Service Connection already exists. Connection already exist use the same.
	 *  If #sap_socket_send_data()/#sap_socket_send_secure_data fails again, create a new connection */
	SAP_CONNECTION_ALREADY_EXIST = 1,
	/** Device is not reachable. It is not connected or connection is lost.
	 *  Need to wait for connection again to retry */
	SAP_CONNECTION_FAILURE_DEVICE_UNREACHABLE = 2,
	/** The peer agent details (peer ID etc.) are invalid.
	 *  Peer Agent is invalid, so do #sap_agent_find_peer_agent() again and retry service connection */
	SAP_CONNECTION_FAILURE_INVALID_PEERAGENT = 3,
	/** Connection failed due to network error.
	 *  Try again. If error keeps happening, wait for connection and then retry.*/
	SAP_CONNECTION_FAILURE_NETWORK = 4,
	/** The remote peer did not response to request.
	 *  Since there is no response from peer, the other side might be down or service is not supported
	 *  in the peer. App can retry later */
	SAP_CONNECTION_FAILURE_PEERAGENT_NO_RESPONSE = 5,
	/** The remote peer rejected the service connection.
	 *  Peer has rejected, App can retry again */
	SAP_CONNECTION_FAILURE_PEERAGENT_REJECTED = 6,
	/** A connection request is already being processed.
	  *App should wait till it is completed*/
	SAP_CONNECTION_IN_PROGRESS = 7,
	/** Unknown error */
	SAP_CONNECTION_FAILURE_UNKNOWN = 0xFF
} sap_service_connection_result_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for error codes that shall be sent with #sap_service_connection_established_cb()
 *                  callback, called when error occurs in an already established service connection.
 * @since_tizen     2.3.1
 */
typedef enum {
	/** Connection loss because peer device got disconnected.
	 *  Based on App logic retry can be done with #sap_agent_request_service_connection */
	SAP_CONNECTION_TERMINATED_REASON_PEER_DISCONNECTED = 0,
	/** Connection loss because peer device is lost.
	 *  Wait #sap_device_status_changed_cb() again to retry */
	SAP_CONNECTION_TERMINATED_REASON_DEVICE_DETACHED = 1,
	/** Connection loss due to unknown reason.
	 *  Mostly development error, app can retry */
	SAP_CONNECTION_TERMINATED_REASON_UNKNOWN = 0xFF
} sap_service_connection_terminated_reason_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function will get triggered for following reasons
 *              <br>  - when a new incoming service connection request is received from a remote peer.
 *              <br>  - to notify the success or failure of service connection request initiated by the
 *                      application. It Will be sent only to the initiator of the connection.
 *              <br>  - when a service connection is terminated as a result of a trigger from remote peer.
 *              <br>  - when error occurs but was not relayed as part of a request-response transaction.
 *              <br>  - If the other side has called #sap_peer_agent_terminate_service_connection()
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of peer agent with which connection is established
 *
 * @param[in]       socket              Socket to be used to send data.
 *                                      This socket should be destroyed by the application after when
 *                                      this service connection has been terminated.
 *                                      To destroy this socket #sap_socket_destroy() method should be used.
 *
 * @param[in]       result              Error Code defined in #sap_service_connection_result_e
 *
 * @param[in]       user_data           The user data passed from the register function
 */
typedef void (*sap_service_connection_established_cb)(sap_peer_agent_h peer_agent,
						      sap_socket_h socket,
						      sap_service_connection_result_e result,
						      void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This function is used to establish a service connection with
 *                  specified peer agent.
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       agent               Agent handle which was created using #sap_agent_create() and
 *                                      updated in #sap_agent_initialized_cb() callback.
 * @param[in]       peer_agent          Handle of peer agent with whom the service connection
 *                                      needs to be established, it was received in #sap_peer_agent_updated_cb()
 *
 * @param[in]       callback            The callback function to register
 * @param[in]       user_data           The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_service_connection_established_cb()
 *
 * @remarks         #SAP_RESULT_SUCCESS indicates that request for service connection has been successfully initiated.
 *                  The actual service connection creation success/failure is indicated in callback
 *                  #sap_service_connection_established_cb().
 *                  The API needs to be called from App's context
 */
int sap_agent_request_service_connection(sap_agent_h agent,
					 sap_peer_agent_h peer_agent,
					 sap_service_connection_established_cb callback,
					 void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function will get triggered for following reasons
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of peer agent from whom service connection request
 *                                      has been received.
 *                                      it was received in #sap_peer_agent_updated_cb()
 * @param[in]       socket              Socket to be used to send data to requesting peer agent.
 *                                      This socket should be destroyed by the application after
 *                                      this service connection has been terminated.
 *                                      To destroy socket #sap_socket_destroy() method should be used.
 *
 * @param[in]       result              Detailed result code, defined in #sap_service_connection_result_e.
 *
 * @param[in]       user_data          The user data passed from the register function
 */
typedef void (*sap_service_connection_requested_cb)(sap_peer_agent_h peer_agent,
						    sap_socket_h socket,
						    sap_service_connection_result_e result,
						    void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This function is used to register a callback to received service connection
 *                  requests from remote peers.
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       agent               Agent handle which was created using #sap_agent_create() and
 *                                      updated in #sap_agent_initialized_cb() callback.
 *
 * @param[in]       callback            The callback function to register
 * @param[in]       user_data           The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 */
int sap_agent_set_service_connection_requested_cb(sap_agent_h agent,
						  sap_service_connection_requested_cb callback,
						  void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function will get triggered to notify that the existing
 *                  service connection with the peer agent has been terminated.
 *                  It gets triggered when remote peer calls #sap_peer_agent_terminate_service_connection()
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Agent handle which was created using #sap_agent_create() and
 *                                      updated in #sap_agent_initialized_cb() callback.
 *                                      it was received in #sap_peer_agent_updated_cb()
 * @param[in]       socket              Socket denoting service connection between the peers.
 *                                      It is no longer valid now, data transfer will not happen
 *                                      through this socket.
 *                                      This socket should be destroyed by application using #sap_socket_destroy() method.
 *
 * @param[in]       result              Detailed result code, defined in #sap_service_connection_result_e.
 *
 * @param[in]       user_data           The user data passed from the register function
 */
typedef void (*sap_service_connection_terminated_cb)(sap_peer_agent_h peer_agent,
						     sap_socket_h socket,
						     sap_service_connection_terminated_reason_e result,
						     void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This function is used to register a callback to receive terminated service connection
 *                  updates from remote peers.
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Agent handle which was created using #sap_agent_create() and
 *                                      updated in #sap_agent_initialized_cb() callback.
 *
 * @param[in]       callback            The callback function to register
 * @param[in]       user_data           The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 */
int sap_peer_agent_set_service_connection_terminated_cb(sap_peer_agent_h peer_agent,
							sap_service_connection_terminated_cb callback,
							void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Accept an incoming service connection request.
 * @details         This is typically called from the registered callback #sap_service_connection_requested_cb().
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent         Handle of peer agent whose service connection request
 *                                     can be accepted, this handle was received in #sap_peer_agent_updated_cb()
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_peer_agent_reject_service_connection()
 *
 * @remarks         The API needs to be called from App's context
 *
 */
int sap_peer_agent_accept_service_connection(sap_peer_agent_h peer_agent);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Reject an incoming service connection request.
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent         Handle of peer agent whoes service connection request will be rejected,
 *                                     This handle was received in #sap_peer_agent_updated_cb().
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_peer_agent_accept_service_connection()
 *
 * @remarks         The API needs to be called from App's context
 *
 */
int sap_peer_agent_reject_service_connection(sap_peer_agent_h peer_agent);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Terminate existing service connection present with the specified peer agent
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent         Handle of peer agent with whom the existing service connection has to be terminated,
 *                                     this handle was received in #sap_peer_agent_updated_cb()
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @remarks         #SAP_RESULT_SUCCESS indicates that request for terminate service connection has been succesfully processed.
 *                  The Api needs to be called in App's context
 *
 */
int sap_peer_agent_terminate_service_connection(sap_peer_agent_h peer_agent);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for SAP certificate types
 * @since_tizen     2.3.1
 */
typedef enum {
	/** Unused/invalid authentication type */
	SAP_AUTHENTICATION_TYPE_NONE = 0X00,
	/** X509 Certificates. */
	SAP_AUTHENTICATION_TYPE_CERTIFICATE_X509 = 0x01
} sap_cert_type_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for status codes received in #sap_peer_agent_auth_info_received_cb()
 * @since_tizen     2.3.1
 */
typedef enum {
	/** Authentication success. */
	SAP_AUTHENTICATION_SUCCESS = 0x00,
	/** If null/empty string is returned in authentication response */
	SAP_AUTHENTICATION_FAILURE_TOKEN_NOT_GENERATED = 0x01,
	/** Authentication feature is not supported by Peer device */
	SAP_AUTHENTICATION_FAILURE_PEER_AGENT_NOT_SUPPORTED = 0x02
} sap_authentication_status_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           The structure to store SAP certificate details and signature details.
 * @since_tizen     2.3.1
 */
typedef struct {
	/** Certificate type */
	sap_cert_type_e type;
	/** Signature of certification. It is encoded by base64 */
	char *singature;
} sap_authentication_token_s;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function gets triggered to notify the #sap_peer_agent_request_auth_info
 *                  response
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent                 Handle of peer agent for whom response of authentication request
 *                                             has been received.
 *                                             this peer agent handle was received in #sap_peer_agent_updated_cb()
 *
 * @param[in]       status                     status code  Status code value is one of the enums defined in #sap_authentication_status_e.
 * @param[in]       authentication_token       #sap_authentication_token_s containing Signature and certificate type .
 *                                             Refer #sap_authentication_status_e description.
 * @param[in]       user_data                  The user data passed from the register function
 *
 */
typedef void (*sap_peer_agent_auth_info_received_cb)(sap_peer_agent_h peer_agent,
						     sap_authentication_status_e status,
						     sap_authentication_token_s *authentication_token,
						     void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Authenticate Peer Agent of the remote peer device
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of peer agent for whom the authentication response has been received.
 *                                      this peer agent handle was received in #sap_peer_agent_updated_cb()
 *
 * @param[in]       callback            The callback function to register
 * @param[in]       user_data           The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_peer_agent_auth_info_received_cb()
 *
 */
int sap_peer_agent_request_auth_info(sap_peer_agent_h peer_agent,
				     sap_peer_agent_auth_info_received_cb callback,
				     void *user_data);


/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for the status of sap data transfer operation.
 * @since_tizen     2.3.1
 */
typedef enum {
	/** Data is sent successfully. App can continue to #sap_socket_send_data() again. */
	SAP_SOCKET_RESULT_SUCCESS = 0,
	/** Invalid arguments, like improper channel id etc.
	 *  App need to recheck the passed parameters and retry. */
	SAP_SOCKET_RESULT_INVALID_ARGUMENTS = 1,
	/** The service connection indicated by the service handle does not exists.
	 *  So create service connection using #sap_agent_request_service_connection() and retry. */
	SAP_SOCKET_RESULT_INVALID_SERVICE_CONNECTION = 2,
	/** The service connection indicated by the service handle does not exists.
	 *  So create service connection using #sap_agent_request_service_connection() and retry. */
	SAP_SOCKET_RESULT_NETWORK_FAILURE = 3,
	/** Send buffer is full.
	 *  On receiving this application should wait for #sap_socket_ready_to_send_cb() before sending further data. */
	SAP_SOCKET_RESULT_BUFFER_FULL = 4,
	/** Any other internal error. App should recheck the parameters passed and retry. */
	SAP_SOCKET_RESULT_INTERNAL_ERROR = 0xFF
} sap_socket_result_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Send data over a service connection. If send fails with #SAP_SOCKET_RESULT_BUFFER_FULL app
 *                  should wait for #sap_socket_ready_to_send_cb() callback.
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       socket              The handle for the service connection.
 * @param[in]       channel_id          The channel id on which data has to be sent
 * @param[in]       payload_length      The length of payload data..
 * @param[in]       buffer              The data buffer to be sent
 *
 * @return          #sap_socket_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_socket_set_ready_to_send_cb()
 */
int sap_socket_send_data(sap_socket_h socket,
			 unsigned short int channel_id,
			 unsigned int payload_length,
			 void *buffer);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Send secure data over a service connection. If send fails with #SAP_SOCKET_RESULT_BUFFER_FULL
 *                  app should wait for #sap_socket_ready_to_send_cb() callback.
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       socket              The handle for the service connection.
 * @param[in]       channel_id          The channel id on which data has to be sent
 * @param[in]       payload_length      The length of payload data..
 * @param[in]       buffer              The data buffer to be sent
 *
 * @return          #sap_socket_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_socket_set_ready_to_send_cb()
 *
 */
int sap_socket_send_secure_data(sap_socket_h socket,
				unsigned short int channel_id,
				unsigned int payload_length,
				void *buffer);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function gets triggered when the application receives data from an active
 *                  service connection.
 * @since_tizen     2.3.1
 *
 * @param[in]       socket              The handle of the service connection on which data has been received.
 * @param[in]       channel_id          The channel id on which data has been received
 * @param[in]       payload_length      The length of payload data.
 * @param[in]       buffer              The data buffer that has been received
 * @param[in]       user_data           The user data passed from the register function
 */
typedef  void (*sap_socket_data_received_cb)(sap_socket_h socket,
					     unsigned short int channel_id,
					     unsigned int payload_length,
					     void *buffer,
					     void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 *
 * @brief           To register a callback function so as to receive data from an active service connection.
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       socket          The handle for the service connection.
 * @param[in]       callback        The callback function to register
 * @param[in]       user_data       The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_socket_data_received_cb()
 *
 */
int sap_socket_set_data_received_cb(sap_socket_h socket,
				    sap_socket_data_received_cb callback,
				    void *user_data);


/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function gets triggered when space is available for application to write.
 *                  Application should wait for this callback if send returns failure with
 *                  #SAP_SOCKET_RESULT_BUFFER_FULL
 * @since_tizen     2.3.1
 *
 * @param[in]       socket              The handle of the service connection on which data has been received.
 * @param[in]       channel_id          The channel identifier on which the #sap_socket_ready_to_send_cb()
 *                                      callback is invoked.
 * @param[in]       available_space     How much space is available in bytes
 * @param[in]       user_data           The user data passed from the register function
 *
 * @pre             It should be registered using #sap_socket_set_ready_to_send_cb() method call.
 *
 */
typedef  void (*sap_socket_ready_to_send_cb)(sap_socket_h socket,
					     unsigned short int channel_id,
					     unsigned int available_space,
					     void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           To register a callback function so as to recieve space available notification from SAP framework
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       socket          Handle of the service connection using which data can
 *                                  be sent to peer when #sap_socket_ready_to_send_cb() has been invoked.
 * @param[in]       callback        The callback function to register
 * @param[in]       user_data       The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_socket_ready_to_send_cb()
 *
 */
int sap_socket_set_ready_to_send_cb(sap_socket_h socket,
				    sap_socket_ready_to_send_cb callback,
				    void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Functions to destroy an instance of the sap_socket handle.
 * @since_tizen     2.3.1
 *
 * @param[in]       socket           Handle of sap_socket which was received in
 *                                   #sap_service_connection_requested_cb() and
 *                                   #sap_service_connection_established_cb()
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 */
int sap_socket_destroy(sap_socket_h socket);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for SAP connection status
 * @since_tizen     2.3.1
 */
typedef enum {
	/** device is detached */
	SAP_DEVICE_STATUS_DETACHED = 0x00,
	/** device is attached */
	SAP_DEVICE_STATUS_ATTACHED = 0x01
} sap_device_status_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function gets triggered when device status is changed
 *                  All valid sap_peer_agent and sap_socket handle should be destroyed after
 *                  receiving #sap_device_status_changed_cb with device disconnected status
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       status              Device connection status. see #sap_device_status_e
 *
 * @param[in]       transport_type      Connectivity over which connection has been established.
 *                                      see #sap_transport_type_e for reference.
 *                                      If device is connected via multiple connectivities then received
 *                                      transport type will be a bitwise OR of respective connectivities.
 *
 * @param[in]       user_data           The user data passed from the register function
 */
typedef  void (*sap_device_status_changed_cb)(sap_device_status_e status,
					      sap_transport_type_e transport_type,
					      void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           To register callback so as to receive notifications, when connection state with
 *                  the remote device gets changed.
 * @since_tizen     2.3.1
 *
 * @param[in]       callback        The callback function to register
 * @param[in]       user_data       The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_device_status_changed_cb()
 */
int sap_set_device_status_changed_cb(sap_device_status_changed_cb callback,
				     void *user_data);


/**
 * @ingroup             SAMSUNG_ACCESSORY_PROTOCOL
 * @brief			Enumerations for the status of sap data transfer operation.
 * @since_tizen         2.3.2
 */
typedef enum {
	/**  Data transfer over socket connection is supported*/
	SAP_FEATURE_SOCKET = 1,
	/**  Conenction less data transfer is supported */
	SAP_FEATURE_MESSAGE
} sap_feature_e;


/**
 * @ingroup             SAMSUNG_ACCESSORY_PROTOCOL
 * @brief			Checks if the specified feature is enabled by the peer
 *
 * @since_tizen         2.3.2
 *
 * @param[in]		peer_agent					The handle for the remote peer_agent
 * @param[in]		feature                                         The feature to be verified if enabled or disabled

 *
 * @return			Returns TRUE if enabled / FALSE if disabled
 *
 * @privlevel		public
 * @privilege		%http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @pre                         App should receive a valid peer agent handle in callback #sap_peer_agent_updated_cb(),
 *					by calling #sap_agent_find_peer_agent()
 */

bool sap_peer_agent_is_feature_enabled(sap_peer_agent_h peer_agent, sap_feature_e feature);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */
#endif /* __SAP_H__ */

