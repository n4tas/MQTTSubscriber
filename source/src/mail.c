#include "../include/mail.h"

char *payload_string(char *email, char *uci_string)
{   
    int size = strlen(email) + strlen(uci_string) + 50;
    printf("size is: %d\n", size);
    char *payload_text = (char *)malloc(size);
    if (payload_text == NULL) 
        return NULL;
    snprintf(payload_text, size,
        "To: %s\r\n"
        "Subject: MQTT Subscriber Notification\r\n"
        "\r\n"
        "%s\r\n", email, uci_string);
    return payload_text;
}

static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data = upload_ctx->payload_text + upload_ctx->bytes_read;
    size_t room = size * nmemb;
    if ((size == 0) || (nmemb == 0) || (room < 1)) {
        return 0;
    }
    if (*data) {
        size_t len = strlen(data);
        if (room < len) {
            len = room;
        }
        memcpy(ptr, data, len);
        upload_ctx->bytes_read += len;
        return len;
    }
    free(upload_ctx->payload_text);
    return 0;
}

int send_mail(char *uci_string)
{
	CURL *curl;
    CURLcode res;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx = { 0 };
    char email[64];
    char password[64];
    if (uci_credentials(email, password) != 0)
        return -1;
    upload_ctx.payload_text = payload_string(email, uci_string);
    if (upload_ctx.payload_text == NULL) 
        return -1;
    curl = curl_easy_init();    
    if (curl != NULL) {
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, email);
        recipients = curl_slist_append(recipients, email);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_USERNAME, email);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            syslog(LOG_ERR, "Mail System Failed\n");
        curl_slist_free_all(recipients);  // handle recipients
    }
    return 0;
}