#pragma once
#include "uci.h"
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

/** Global variable for CURL session */
extern CURL *curl;

/* Global variable for the list of email recipients */
extern struct curl_slist *recipients;

/** Structure to track upload status */
struct upload_status {
    size_t bytes_read;        // Tracks bytes already read from payload
    char *payload_text;       // Pointer to the email content (payload)
};

/**
 * Function to format email payload text.
 * @param payload_text - Pointer to store formatted payload text.
 * @param size - Size of the payload.
 * @param email - Recipient email address.
 * @param uci_string - Content to include in the email body.
 * @return Formatted payload text.
 */
char *payload_string(char *payload_text, int size, char *email, char *uci_string);

/**
 * Function to perform server lookup.
 */
int server_lookup();

/**
 * Function to send email.
 * @param uci_string - Content to include in the email body.
 */
int send_mail(char *uci_string);