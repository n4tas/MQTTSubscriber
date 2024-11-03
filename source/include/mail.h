#pragma once
#include "uci.h"
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

struct upload_status {
    size_t bytes_read;
    char *payload_text;
};

char *payload_string(char *email, char *uci_string);

int server_lookup();

int send_mail(char *uci_string);