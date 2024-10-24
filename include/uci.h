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

struct uci_topic_data {
    char ***events;
    char **topics;
    int topics_count;
    int events_count;
};

int uci_credentials(char *username, char *password);
int uci_topics(char ***topics, int *total_topics_count);
int uci_events(char ****events, char **topics, int *total_events_count, int total_topics_count);
void uci_events_free(char ***events, int total_events_count);
void uci_topics_free(char **topics, int total_topics_count);
int uci_event_funcionality(struct uci_topic_data **uci_data, char *payload);
int check_event_logic(struct uci_topic_data **uci_data);
void event_topic_synchronization(struct uci_topic_data **uci_data);
void cleanup(struct mosquitto *mosq, struct uci_topic_data *uci_data);