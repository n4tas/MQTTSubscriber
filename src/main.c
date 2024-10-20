#include "mqtt_sub.h"
#include "args.h"
#include "uci.h"
#include "database.h"

int main(int argc, char **argv){
    signal(SIGINT, handle_signal);
    signal(SIGCONT, handle_signal);
    signal(SIGSEGV, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGCHLD, handle_signal);
    signal(SIGHUP, handle_signal);
    load_arguments(argc, argv);
    struct mosquitto *mosq = NULL;
    struct uci_topic_data *uci_data = NULL;
    if (mosquitto_init(argc, argv, &mosq, &uci_data) != 0){
        syslog(LOG_ERR, "Mosquitto initialization failed");
        return -1;
    }
    set_signal_context(mosq, uci_data);
    clean_up_libmosquitto(mosq);
    uci_topics_free(uci_data->topics, uci_data->topics_count);
    if (uci_data->events != NULL)
        uci_events_free(uci_data->events, uci_data->events_count); 
    free(uci_data->events);
    return 0;
}

