#include "../include/handle_sig.h"

signal_context_t* get_signal_context() {
    static signal_context_t context;
    return &context;
}

void set_signal_context(struct mosquitto *mosq, struct uci_topic_data *uci_data) {
    signal_context_t *context = get_signal_context();
    context->mosq = mosq;
    context->uci_data = uci_data;
}

void handle_signal(int sig) {
    signal_context_t *context = get_signal_context();
    cleanup(context->mosq, context->uci_data);
    exit(sig);
}