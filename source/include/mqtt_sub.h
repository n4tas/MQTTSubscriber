#pragma once
#include "handle_sig.h"
#include "uci.h"
#include "daemon.h"
#include "args.h"
#include "database.h"
#include <mosquitto.h>
#include <stdio.h>

//#include <curl/curl.h>

/**
 * Initializes mosquitto and connects to the MQTT broker, setting up necessary configurations.
 *
 * Parameters:
 * - argc: Argument count from command line input.
 * - argv: Argument vector from command line input.
 * - mosq: Pointer to the Mosquitto client instance.
 * - uci_data: Pointer to UCI topic data structure.
 *
 * Returns:
 * - 0 on success, or -1 on failure.
 */
int mosquitto_init(int argc, char **argv, struct mosquitto **mosq, struct uci_topic_data **uci_data);

/**
 * Callback function to handle received messages from subscribed topics.
 *
 * Parameters:
 * - mosq: Pointer to the Mosquitto client instance.
 * - userdata: Pointer to user data, which contains the UCI data.
 * - message: Pointer to the received Mosquitto message.
 */
void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);

/**
 * Cleans up the mosquitto library, disconnecting and freeing the client instance.
 *
 * Parameters:
 * - mosq: Pointer to the mosquitto client instance.
 */
void clean_up_libmosquitto(struct mosquitto *mosq);