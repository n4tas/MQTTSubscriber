#include "../include/mqtt_sub.h"

void on_connect(struct mosquitto *mosq, void *userdata, int result)
{   
    struct uci_topic_data *uci_data = userdata;
	printf("Connected to MQTT broker\n");           
    char **topics = NULL;        
    topics = (char **) malloc(sizeof(char));    
    if (topics == NULL) {
        fprintf(stderr, "Memory allocation failed [topics]\n");
        return;
    }   
    int topics_count = 0;    
    int events_count = 0;                                 
    if (uci_topics(&topics, &topics_count) != 0)
        return;
    uci_data->topics = topics;
    uci_data->topics_count = topics_count; 
    uci_data->events = NULL;
    uci_data->events_count = 0; 
    for (int i = 0; i < topics_count; i++)              
        mosquitto_subscribe(mosq, NULL, topics[i], 0);                                 
    char ***events = (char ***)malloc(sizeof(char));
    if (events == NULL) {
        cleanup(NULL, uci_data);
        return;
    }              
    if (uci_events(&events, topics, &events_count, topics_count) != 0){  
        cleanup(NULL, uci_data);
        return;
    }
    uci_data->events = events;
    uci_data->events_count = events_count;
    set_signal_context(mosq, uci_data);
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{   
    struct uci_topic_data *uci_data = userdata;
    if (table_insert(message->topic, (char *)message->payload) != 0)
        fprintf(stderr, "Table insertion failed ");
    uci_event_funcionality(&uci_data, (char *)message->payload);             
}

void clean_up_libmosquitto(struct mosquitto *mosq)
{   
    mosquitto_loop_stop(mosq, false);
    mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
}

int mosquitto_init(int argc, char **argv, struct mosquitto **mosq, struct uci_topic_data **uci_data)
{   
	char username[24];
	char password[24];
    if (mosquitto_lib_init() != MOSQ_ERR_SUCCESS){
        fprintf(stderr, "Mosquitto library returned non-zero value");
        return -1;
    }
    *mosq = mosquitto_new(NULL, true, uci_data);
    if (!(*mosq)){
        fprintf(stderr, "Error: failed to create mosquitto client\n");
        clean_up_libmosquitto(*mosq);
        return -1;
    }
    if (strcmp(arguments.host, "0.0.0.0") != 0) {
        uci_credentials(username, password);
        mosquitto_username_pw_set(*mosq, username, password);
    }
    if (mosquitto_connect(*mosq, arguments.host, arguments.port, 5) != MOSQ_ERR_SUCCESS){
        clean_up_libmosquitto(*mosq);
        return -1;
    }
    mosquitto_connect_callback_set(*mosq, on_connect);
    mosquitto_loop_start(*mosq);
    mosquitto_message_callback_set(*mosq, on_message);
    return 0;
}                              
