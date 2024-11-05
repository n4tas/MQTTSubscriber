#include "../include/mqtt_sub.h"
#include "../include/args.h"
#include "../include/uci.h"
#include "../include/database.h"

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
    if (mosquitto_init(argc, argv, &mosq, &uci_data) != 0)
        return -1;
    cleanup(mosq, uci_data);
    return 0;
}

