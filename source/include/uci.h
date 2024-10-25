#pragma once
#include "mqtt_sub.h"
#include <uci.h>
#include <stdio.h>
#include <ctype.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define EVENT_TOPICS_NAME_INDEX 0
#define EVENT_PARAMETER_INDEX 1
#define EVENT_DATA_TYPE_INDEX 2
#define EVENT_COMPARISON_TYPE_INDEX 3
#define EVENT_REFERENCE_INDEX 4

/**
 * Structure to store UCI topic data.
 * 
 * Contains the events and topics retrieved from the UCI configuration.
 * - `events`: a 3D array containing information for each event (topic, parameter, data type, comparison type, reference).
 * - `topics`: an array containing MQTT topics as defined in UCI.
 * - `topics_count`: number of topics.
 * - `events_count`: number of events associated with topics.
 */
struct uci_topic_data {
    char ***events;
    char **topics;
    int topics_count;
    int events_count;
};

/**
 * Retrieves the username and password from the UCI "mqtt_login" configuration.
 * 
 * The function reads the "username" and "password" values from the specified
 * UCI package, storing them in the provided `username` and `password` buffers.
 *
 * Parameters:
 * - `username`: buffer to store the retrieved username.
 * - `password`: buffer to store the retrieved password.
 *
 * Returns:
 * - 0 on success, -1 if there was an error accessing UCI or retrieving credentials.
 */
int uci_credentials(char *username, char *password);

/**
 * Retrieves a list of MQTT topics from the UCI "mqtt_topics" configuration.
 * 
 * The function allocates memory and fills the `topics` array with topic names found
 * in the UCI package, updating the `total_topics_count`.
 *
 * Parameters:
 * - `topics`: pointer to store the array of topics.
 * - `total_topics_count`: pointer to store the total number of topics.
 *
 * Returns:
 * - 0 on success, -1 if any errors occurred during topic retrieval.
 */
int uci_topics(char ***topics, int *total_topics_count);

/**
 * Retrieves event data from the UCI configuration and maps it to corresponding topics.
 * 
 * Reads each event configuration from UCI, validates it against available topics, and
 * populates the `events` array with event attributes. The function updates the event count.
 *
 * Parameters:
 * - `events`: 3D array to hold event data (topic name, parameter, data type, comparison type, reference).
 * - `topics`: array of topics to validate event-topic relationships.
 * - `total_events_count`: pointer to store the total number of events.
 * - `total_topics_count`: total count of topics.
 *
 * Returns:
 * - 0 on success, -1 if event retrieval or validation fails.
 */
int uci_events(char ****events, char **topics, int *total_events_count, int total_topics_count);

/**
 * Frees memory allocated for the event data structure.
 * 
 * Parameters:
 * - `events`: array of events to free.
 * - `total_events_count`: total count of events to be freed.
 */
void uci_events_free(char ***events, int total_events_count);

/**
 * Frees memory allocated for the topics array.
 * 
 * Parameters:
 * - `topics`: array of topics to free.
 * - `total_topics_count`: total count of topics to be freed.
 */
void uci_topics_free(char **topics, int total_topics_count);

/**
 * Processes event functionality by iterating over events and triggering the correct handling
 * function based on each event's data type.
 *
 * Parameters:
 * - `uci_data`: pointer to UCI topic data.
 * - `payload`: incoming payload string.
 *
 * Returns:
 * - 0 after processing all events.
 */
int uci_event_funcionality(struct uci_topic_data **uci_data, char *payload);

/**
 * Verifies event configurations by checking if the comparison type and data type are logically valid.
 *
 * Parameters:
 * - `uci_data`: pointer to UCI topic data.
 *
 * Returns:
 * - 0 if logic checks out, -1 if any validation fails.
 */
int check_event_logic(struct uci_topic_data **uci_data);

/**
 * Syncs event configurations with topic data to ensure consistency.
 * 
 * Used to keep the event-topic mappings up to date.
 *
 * Parameters:
 * - `uci_data`: pointer to UCI topic data to sync.
 */
void event_topic_synchronization(struct uci_topic_data **uci_data);

/**
 * Cleans up all resources and memory allocations for MQTT and UCI data structures.
 * 
 * Parameters:
 * - `mosq`: pointer to the MQTT client instance to clean up.
 * - `uci_data`: pointer to the UCI topic data structure to free.
 */
void cleanup(struct mosquitto *mosq, struct uci_topic_data *uci_data);