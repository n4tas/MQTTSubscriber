#include "../include/mail.h"

CURL *curl = NULL;
struct curl_slist *recipients = NULL;

char *payload_string(char *payload_text, int size, char *email, char *uci_string)
{   
    snprintf(payload_text, size,
            "To: %s\r\n"
            "Subject: MQTT Subscriber Notification\r\n"
            "MIME-Version: 1.0\r\n"
            "Content-Type: text/plain; charset=UTF-8\r\n"
            "\r\n"
            "Hello,\r\n"
            "\r\n"
            "You have received a new MQTT subscriber notification.\r\n"
            "\r\n"
            "%s"
            "\r\n"
            "Best regards,\r\n"
            "MQTT Broker\r\n",
        email, uci_string);
    return payload_text;
}

static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;
    char *payload_temp = upload_ctx->payload_text;
    size_t room = size * nmemb;
    if ((size == 0) || (nmemb == 0) || (room < 1))
        return 0;
    data = &payload_temp[upload_ctx->bytes_read];
    if (data) {
        size_t len = strlen(data);
        if (room < len) 
            len = room;
        memcpy(ptr, data, len);
        upload_ctx->bytes_read += len;
        return len;
    }
    return 0;
}

int send_mail(char *uci_string) {
    CURLcode res;
    struct upload_status upload_ctx = { 0 }; 
    char email[MAX_UCI_STRING_SIZE];
    char password[MAX_UCI_STRING_SIZE];
    if (uci_credentials(email, password) != 0)
        return -1;
    int size = strlen(email) + strlen(uci_string) + 210;
    char *payload_text = (char *)malloc(size);
    if (payload_text == NULL) 
        return -1;
    upload_ctx.payload_text = payload_string(payload_text, size, email, uci_string);
    if (upload_ctx.payload_text == NULL) {
        free(payload_text);
        return -1;
    }
    curl = curl_easy_init();
    if (curl == NULL) {
        free(payload_text);
        return -1;
    }
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
    if (res != CURLE_OK) 
        fprintf(stderr, "Mail send failed: %s\n", curl_easy_strerror(res));
    free(payload_text);
    curl_slist_free_all(recipients); 
    curl_easy_cleanup(curl);
    recipients = NULL;
    curl = NULL;
    return 0;
}