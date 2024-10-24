#pragma once
#ifndef SIG_H
#define SIG_H

#include "mqtt_sub.h"
#include "database.h"
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <signal.h>

typedef struct {
    struct mosquitto *mosq;
    struct uci_topic_data *uci_data;
} signal_context_t;

signal_context_t* get_signal_context();

void set_signal_context(struct mosquitto *mosq, struct uci_topic_data *uci_data);

void handle_signal(int sig);

#endif