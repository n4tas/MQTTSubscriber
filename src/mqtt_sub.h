#pragma once
#include "handle_sig.h"
#include "uci.h"
#include "daemon.h"
#include "args.h"
#include "database.h"
#include <mosquitto.h>
#include <stdio.h>
//#include <curl/curl.h>

int mosquitto_init(int argc, char **argv, struct mosquitto **mosq, struct uci_topic_data **uci_data);
void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void clean_up_libmosquitto(struct mosquitto *mosq);