#ifndef I3_CLIENT_H
#define I3_CLIENT_H
#endif

// generic includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// dependency includes
#include "MQTTClient.h"
#include "limits.h"

// project includes

#define RESULT_INIT INT_MIN
#define I3_CLIENT_TYPE_PUBLISHER    "publisher"
#define I3_CLIENT_TYPE_SUBSCRIBER   "subscriber"
// enumerated result values (for error handling)
#define I3_RESULT_ERROR_FLAG    0x80 
typedef enum I3_RESULT
{
    I3_RESULT_SUCCESS                      = 0,
    
    I3_RESULT_USER_CONFIG_ERROR            = I3_RESULT_ERROR_FLAG | 0x01,
    I3_RESULT_MEMORY_ERROR                 = I3_RESULT_ERROR_FLAG | 0x02,
    I3_RESULT_IO_ERROR                     = I3_RESULT_ERROR_FLAG | 0x03,
    I3_RESULT_INVALID_ARGUMENT_ERROR       = I3_RESULT_ERROR_FLAG | 0x04,
    I3_RESULT_CONNECT_ERROR                = I3_RESULT_ERROR_FLAG | 0x05,
    I3_RESULT_PUBLISH_ERROR                = I3_RESULT_ERROR_FLAG | 0x06,
    I3_RESULT_SUBSCRIBE_ERROR              = I3_RESULT_ERROR_FLAG | 0x07,
} I3_RESULT;

/**
 * Control block for I3 mqtt client. 
 * 
 * Includes default values with paho initializers. 
 */
struct i3_client_block
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts;
    MQTTClient_message pubmsg;
    MQTTClient_deliveryToken token;
};
typedef struct i3_client_block i3_client_handle;

// *************************************** I3-SDK APIs ***************************************
/**
 * @brief initializes #i3_client_block and calls MQTTClient_create
 * 
 * @param _i3_client            #i3_client_handle
 * @param endpoint_address      <tt>const char* const</tt> "broker address"
 * @param client_id             <tt>const char* const</tt> "my_account_name$my_hub_name$my_device_name"
 * @param account               <tt>const char* const</tt> "my_account_name"
 * @param password              <tt>const char* const</tt> "my_account_password"
 * 
 * @retval  0                   on success
 * @retval  -1                  on failure
 */
int i3_client_create(i3_client_handle* _i3_client, const char* const endpoint_address, const char* const client_id,
                    const char* const account, const char* const password, const char* const client_type);

/**
 * @brief sets connection options
 * 
 * @param _i3_client            #i3_client_handle
 * 
 * @retval  0                   on success
 * @retval  -1                  on failure
 */
int i3_set_connection_options(i3_client_handle* _i3_client);

/**
 * @brief calls MQTTClient_connect()
 * 
 * @param _i3_client            #i3_client_handle
 * 
 * @retval  0                   on success
 * @retval  -1                  on failure
 */
int i3_connect(i3_client_handle* _i3_client);

/**
 * @brief registers callback functions
 * 
 * @param _i3_client            #i3_client_handle
 * @param context               <tt>void*</tt> context pointer
 * @param connection_lost       <tt>void*</tt> pointer to connection lost callback function
 * @param message_arrived       <tt>void*</tt> pointer to message arrived callback function
 * @param message_delivered     <tt>void*</tt> pointer to message delivered callback function
 * 
 * @retval  0                   on success
 * @retval  -1                  on failure 
 */
int i3_set_callbacks(i3_client_handle* _i3_client, void* context, void* connection_lost,
                    void* message_arrived, void* message_delivered);

/**
 * @brief publishes message to I3 topic
 * 
 * @param _i3_client            #i3_client_handle
 * @param payload               <tt>unsigned char*</tt> payload to publish
 * @param payload_length        <tt>size_t</tt> length of payload data
 * @param topic                 <tt>const char* const</tt> the topic to be published on
 * @param qos                   <tt>int</tt> quality of service selector
 *                                  0: Fire and forget
 *                                  1: At least once - the message will be delivered, but may be
 *                                      delivered more than once in some circumstances
 *                                  2: Once and ony once
 * @param timeout               <tt>unsigned long</tt> the time to wait for publish ack in ms
 * @param retain                <tt>int</tt> 1 to keep message after publish, 0 to clear message 
 * 
 * @retval  0                   on success
 * @retval  -1                  on failure
 */
int i3_publish(i3_client_handle* _i3_client, const char* const topic, void* payload, size_t payload_length);

/**
 * @brief subscribes client to I3 topic
 * 
 * @param _i3_client            #i3_client_handle
 * @param topic                 <tt>const char* const</tt> the topic to be published on
 * @param qos                   <tt>int</tt> quality of service selector
 *                                  0: Fire and forget
 *                                  1: At least once - the message will be delivered, but may be
 *                                      delivered more than once in some circumstances
 *                                  2: Once and ony once
 * 
 * @retval  0                   on success
 * @retval  -1                  on failure
 */
int i3_subscribe(i3_client_handle* _i3_client, const char* const topic);

/**
 * @brief unsubscribes client from I3 topic
 * 
 * @param _i3_client            #i3_client_handle
 * @param topic                 <tt>const char* const</tt> the topic to be published on
 * 
 * @retval  0                   on success
 * @retval  -1                  on failure
 */
int i3_unsubscribe(i3_client_handle* _i3_client, const char* const topic);
/**
 * @brief calls MQTTClient_disconnect()
 * 
 * @param _i3_client            #i3_client_handle
 * @param timeout               <tt>int</tt> time to wait for disconnect ack in ms
 * 
 * @retval  0                   on success
 * @retval  -1                  on failure
 */
int i3_disconnect(i3_client_handle* _i3_client, int timeout);

/**
 * @brief calls MQTTClient_destroy())
 * 
 * @param _i3_client            #i3_client_handle
 */
void i3_client_destroy(i3_client_handle* _i3_client);

#undef I3_CLIENT_H