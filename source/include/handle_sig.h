#pragma once
#ifndef SIG_H
#define SIG_H
#include "mqtt_sub.h"
#include "database.h"
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <signal.h>
/**
 * Structure for signal handling context, containing references to:
 * - mosq: the active mosquitto MQTT client instance
 * - uci_data: user configuration data associated with MQTT topics
 */
typedef struct signal_context{
    struct mosquitto *mosq;
    struct uci_topic_data *uci_data;
} signal_context_t;

/**
 * Retrieves the current signal handling context.
 *
 * Returns:
 * - A pointer to the current `signal_context_t` structure.
 */
signal_context_t* get_signal_context();

/**
 * Sets the signal handling context with the provided mosquitto client and UCI data.
 *
 * Parameters:
 * - mosq: Pointer to the Mosquitto client instance.
 * - uci_data: Pointer to the topic configuration data.
 */
void set_signal_context(struct mosquitto *mosq, struct uci_topic_data *uci_data);

/**
 * Signal handler function to respond to received signals (e.g., SIGINT).
 *
 * Parameters:
 * - sig: The signal number received by the process.
 */
void handle_signal(int sig);
#endif