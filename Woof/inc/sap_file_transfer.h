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

#ifndef __SAP_FILE_TRANSFER_H__
#define __SAP_FILE_TRANSFER_H__
/**
 * @file            sap_file_transfer.h
 * @brief           SAP File-Transfer SDK interface API
 */
#include "sap.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           The structure type for the file transaction handle
 * @since_tizen     2.3.1
 */
typedef struct sap_file_transaction_s *sap_file_transaction_h;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Function to destroy file transaction handle
 *                  which was received in #sap_file_transfer_incoming_file_cb()
 *                  and #sap_file_transfer_send()
 *
 * @since_tizen     2.3.1
 * @returns         #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 */
int sap_file_transfer_destroy(sap_file_transaction_h file_transaction);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           Enumerations for the results of FT transactions.
 * @since_tizen     2.3.1
 */
typedef enum {
	/**  FT Transfer is successful. */
	SAP_FT_TRANSFER_SUCCESS = 0x00,
	/** This error is received when the channel I/O has problem */
	SAP_FT_TRANSFER_FAIL_CHANNEL_IO = 0x01,
	/** This error is received when the file to be sent/received doesn't exist
	 for the path is invalid */
	SAP_FT_TRANSFER_FAIL_FILE_IO = 0x02,
	/** This error is received when the file transfer command has dropped */
	SAP_FT_TRANSFER_FAIL_CMD_DROPPED = 0x03,
	/** This error is received during file transfer the remote end is unresponsive */
	SAP_FT_TRANSFER_FAIL_PEER_UNRESPONSIVE = 0x04,
	/**  This error is received when during file transfer service connection was lost
	(e.g. could be due to BT off) */
	SAP_FT_TRANSFER_FAIL_PEER_CONN_LOST = 0x05,
	/** This error is received when peer cancel file transfer */
	SAP_FT_TRANSFER_FAIL_PEER_CANCELLED = 0x09,
	/** This error is received when peer doesn't have enough space to store the file. */
	SAP_FT_TRANSFER_FAIL_SPACE_NOT_AVAILABLE = 0x0B
} sap_ft_transfer_e;

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function gets triggered when App receive a file request from the remote peer.
 * @since_tizen     2.3.1
 *
 *
 * @param[in]       peer_agent      Handle of a peer agent from whom file request has come,
 *                                  This handle was received in #sap_peer_agent_updated_cb()
 *
 * @param[in]       file_transaction    File sender App will receive this handle file_transaction habdle as out
 *                                      parameter value of #sap_file_transfer_send(),
 *                                      receiver App will receive this file_transaction handle in
 *                                      #sap_file_transfer_incoming_file_cb()
 *
 * @param[in]       file_path           This is source file path at sender side
 *                                      Receiver will call #sap_file_transfer_receive() to accept/reject the file transfer request
 *                                      In #sap_file_transfer_receive, file_path is destination file path
 *                                      Note: Since source file path could be from different File System,
 *                                      call #sap_file_transfer_receive() with a path valid in destination File System.
 *                                      Application should take care of this.
 *
 * @param[in]       user_data           The user data passed from the register callback function
 */
typedef  void (*sap_file_transfer_incoming_file_cb)(sap_peer_agent_h peer_agent,
						    sap_file_transaction_h file_transaction,
						    const char *file_path,
						    void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           The function will register a callback which will in turn notify any incoming
 *                  file transfer requests.
 * @since_tizen     2.3.1
 *
 * @param[in]       agent           Handle of agent which was received as an out parameter of
 *                                  #sap_agent_create().
 *
 * @param[in]       callback        The callback function to register
 * @param[in]       user_data       The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_file_transfer_set_done_cb()
 * @see             sap_file_transfer_done_cb()
 * @see             sap_file_transfer_set_progress_cb()
 * @see             sap_file_transfer_progress_cb()
 *
 */
int sap_file_transfer_set_incoming_file_cb(sap_agent_h agent,
					   sap_file_transfer_incoming_file_cb callback,
					   void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function gets triggered to update file progress status to App
 * @since_tizen     2.3.1
 *
 * @param[in]       file_transaction    File sender App will receive this handle file_transaction handle as out
 *                                      parameter value of #sap_file_transfer_send(),
 *                                      receiver App will receive this file_transaction handle in
 *                                      #sap_file_transfer_incoming_file_cb()
 *
 * @param[in]       percentage_progress     send/receive file progress expressed as % (e.g. 10%, 90%)
 * @param[in]       user_data               The user data passed from the register callback function
 *
 */
typedef  void (*sap_file_transfer_progress_cb)(sap_file_transaction_h file_transaction,
					       unsigned short int percentage_progress,
					       void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           The function will register a callback which will provide percentage progress of a file transfer
 *                  operation in intervals.
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       file_transaction    Handle of current file transaction.
 *                                      File sender App will receive this handle file_transaction handle as out
 *                                      parameter value of #sap_file_transfer_send(),
 *                                      receiver App will receive this file_transaction handle in
 *                                      #sap_file_transfer_incoming_file_cb()
 *
 * @param[in]       callback            The callback function to register
 * @param[in]       user_data           The user data to be passed to the callback function
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_file_transfer_progress_cb()
 *
 */
int sap_file_transfer_set_progress_cb(sap_file_transaction_h file_transaction,
				      sap_file_transfer_progress_cb callback,
				      void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This callback function gets triggered when sending file is completed or error occurred during File transfer
 * @since_tizen     2.3.1
 *
 * @param[in]       file_transaction    File sender App will receive this handle file_transaction handle as out
 *                                      parameter value of #sap_file_transfer_send(),
 *                                      receiver App will receive this file_transaction handle in
 *                                      #sap_file_transfer_incoming_file_cb()
 * @param[in]       result              result code defined in #sap_ft_transfer_e
 * @param[in]       file_path           file path.
 * @param[in]       user_data           The user data passed from the register callback function
 *
 * @see             sap_ft_transfer_e
 *
 */
typedef  void (*sap_file_transfer_done_cb)(sap_file_transaction_h file_transaction,
					   sap_ft_transfer_e result,
					   const char *file_path,
					   void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           The function will register a callback which will in turn notify when a file transfer is
 *                  completed with its status
 *
 * @since_tizen     2.3.1
 *
 * @param[in]       file_transaction    Handle of current file transaction.
 *                                      File sender App will receive this handle file_transaction handle as out
 *                                      parameter value of #sap_file_transfer_send(),
 *                                      receiver App will receive this file_transaction handle in
 *                                      #sap_file_transfer_incoming_file_cb()
 *
 * @param[in]       callback        The callback function to register
 * @param[in]       user_data       The user data to be passed to the callback function
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @return          #sap_result_e
 *
 * @see             sap_file_transfer_done_cb()
 */
int sap_file_transfer_set_done_cb(sap_file_transaction_h file_transaction,
				  sap_file_transfer_done_cb callback,
				  void *user_data);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This function is called by app to send a file
 * @since_tizen     2.3.1
 *
 * @param[in]       peer_agent          Handle of a peer agent to which file is to be sent,
 *                                      which was received in #sap_peer_agent_updated_cb()
 * @param[in]       file_path           This is absolute file path for file to be send.
 *                                      if file doesn't exist, permission denied or invalid path,
 *                                      App will be notified in #sap_file_transfer_done_cb() with
 *                                      error code #SAP_FT_TRANSFER_FAIL_FILE_IO
 *
 * @param[out]      file_transaction    handle of current file transaction
 *
 * @return          #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_file_transfer_set_done_cb()
 * @see             sap_file_transfer_done_cb()
 * @see             sap_file_transfer_set_progress_cb()
 * @see             sap_file_transfer_progress_cb()
 *
 */
int sap_file_transfer_send(sap_peer_agent_h peer_agent,
			   const char *file_path,
			   sap_file_transaction_h *file_transaction);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This function is called by app to accept a file request
 * @since_tizen     2.3.1
 *
 * @param[in]       file_transaction    handle of current file transaction received in
 *                                      #sap_file_transfer_incoming_file_cb()
 * @param[in]       file_path           This is absolute file path where file to be saved. If file path is invalid,
 *                                      permission denied App will be notified in #sap_file_transfer_done_cb()
 *                                      with error code #SAP_FT_TRANSFER_FAIL_FILE_IO
 *
 * @returns         #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_file_transfer_set_done_cb()
 * @see             sap_file_transfer_done_cb()
 * @see             sap_file_transfer_set_progress_cb()
 * @see             sap_file_transfer_progress_cb()
 */
int sap_file_transfer_receive(sap_file_transaction_h file_transaction,
			      const char *file_path);


/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This function is called by app to reject a file request
 * @since_tizen     2.3.1
 *
 * @param[in]       file_transaction    handle of current file transaction received in
 *                                      #sap_file_transfer_incoming_file_cb()
 * @returns         #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_file_transfer_receive()
 */
int sap_file_transfer_reject(sap_file_transaction_h file_transaction);

/**
 * @ingroup         SAMSUNG_ACCESSORY_PROTOCOL
 * @brief           This function is called by the app to cancel the file transfer
 * @since_tizen     2.3.1
 *
 * @param[in]       file_transaction    Identifies the File transfer transaction to be cancelled.
 *                                      File sender App will receive this handle file_transaction handle as out parameter value
 *                                      of #sap_file_transfer_send(),
 *                                      receiver App will receive this file_transaction handle in
 *                                      #sap_file_transfer_incoming_file_cb()
 *
 * @returns         #sap_result_e
 *
 * @privlevel       public
 * @privilege       %http://developer.samsung.com/tizen/privilege/accessoryprotocol
 *
 * @see             sap_file_transfer_set_incoming_file_cb()
 * @see             sap_file_transfer_incoming_file_cb()
 */
int sap_file_transfer_cancel(sap_file_transaction_h file_transaction);

#ifdef __cplusplus
}
#endif

#endif /* __SAP_FILE_TRANSFER_H__ */

