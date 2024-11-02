#include "../include/uci.h"

int uci_credentials(char *username, char *password)
{
    struct uci_context *ctx = NULL;
    struct uci_package *pkg = NULL;
    struct uci_section *sec = NULL;
    struct uci_element *e = NULL;
    ctx = uci_alloc_context();
    if (!ctx) {
        fprintf(stderr, "Failed to allocate UCI context\n");
        return -1;
    }
    if (uci_load(ctx, "mqtt_login", &pkg) != UCI_OK) {
        fprintf(stderr, "Failed to load UCI configuration file ");
        uci_perror(ctx, " Error");
        uci_free_context(ctx);
        return -1;
    }
    uci_foreach_element(&pkg->sections, e) {
        sec = uci_to_section(e);
        strcpy(username, uci_lookup_option_string(ctx, sec, "email"));
        strcpy(password, uci_lookup_option_string(ctx, sec, "password"));
        if (!username || !password) {
            printf("Email or Password not found in section: %s\n", sec->e.name);
        } 
    }
    uci_unload(ctx, pkg);
    uci_free_context(ctx);
    return 0;
}

static int uci_generic_topics_count(struct uci_package *pkg, struct uci_element *e, struct uci_section *sec)
{   
    static int count = 0;
    uci_foreach_element(&pkg->sections, e) {
        sec = uci_to_section(e);
        if (strcmp(sec->type, "topic") == 0) 
            count++;
    }
    return count;
}

static int get_topics(char ***topics, int total_topics_count, struct uci_context *ctx, struct uci_package *pkg, struct uci_section *sec, struct uci_element *e)
{   
    int counter_i = 0;
    *topics = (char **)realloc(*topics, total_topics_count * sizeof(char*));
    uci_foreach_element(&pkg->sections, e) {
        sec = uci_to_section(e);
        const char *string = uci_lookup_option_string(ctx, sec, "topic");
        if (string == NULL)
            return -1;
        (*topics)[counter_i] = (char *)malloc(strlen(string) + 1 * sizeof(char));
        if ((*topics)[counter_i] == NULL) {
            uci_topics_free(*topics, counter_i);
            return -1;
        }
        strcpy((*topics)[counter_i], string);
        counter_i++;
        if (counter_i == total_topics_count)
            break;
    }
    return 0;
}

int uci_topics(char ***topics, int *total_topics_count)
{   
    struct uci_context *ctx = NULL;
    struct uci_package *pkg = NULL;
    struct uci_section *sec = NULL;
    struct uci_element *e = NULL;
    ctx = uci_alloc_context();
    if (!ctx) {
        fprintf(stderr, "Failed to allocate UCI context\n");
        free(*topics);
        return -1;
    }
    if (uci_load(ctx, "mqtt_topics", &pkg) != UCI_OK) {
        fprintf(stderr, "Failed to load UCI configuration file");
        free(*topics);
        uci_perror(ctx, " Error");
        uci_free_context(ctx);
        return -1;
    }
    *total_topics_count = uci_generic_topics_count(pkg, e, sec);
    if (*total_topics_count < 1){
        free(*topics);
        uci_unload(ctx, pkg);
        uci_free_context(ctx);
        return -1;
    }
    if (get_topics(topics, *total_topics_count, ctx, pkg, sec, e) != 0){
        free(*topics);
        uci_unload(ctx, pkg);
        uci_free_context(ctx);
        return -1;
    }
    uci_unload(ctx, pkg);
    uci_free_context(ctx);
    return 0;
}

static int uci_generic_events_count(struct uci_package *pkg, struct uci_element *e, struct uci_section *sec)
{   
    static int count = 0;
    uci_foreach_element(&pkg->sections, e) {
        sec = uci_to_section(e);
        if (strcmp(sec->type, "event") == 0) 
            count++;
    }
    return count;
}

static int check_event_validity(char **topics, const char *event_topics_name, int total_topics_count)
{   
    for (int i = 0; i < total_topics_count; i++){
        if (strcmp(event_topics_name, topics[i]) == 0)
            return 0;
    }
    return -1;
}

static int get_events(char ****events, char **topics, int *total_events_count, int total_topics_count, struct uci_context *ctx, struct uci_package *pkg, struct uci_section *sec, struct uci_element *e)
{   
    int event_array = 0;
    *events = (char ***)realloc(*events, sizeof(char **) * (*total_events_count));
    if (*events == NULL){
        fprintf(stderr, "Memory reallocation failed [EVENTS]");
        return -1;
    }
    uci_foreach_element(&pkg->sections, e) {
        sec = uci_to_section(e);
        if (strcmp(sec->type, "event") != 0) 
            continue; 
        const char *event_topics_name = uci_lookup_option_string(ctx, sec, "topic");
        if (check_event_validity(topics, event_topics_name, total_topics_count) != 0){
            free(*events);
            *total_events_count = 0;
            return -1;
        }
        const char *event_parameter = uci_lookup_option_string(ctx, sec, "parameter");
        const char *event_data_type = uci_lookup_option_string(ctx, sec, "data_type");
        const char *event_comparison_type = uci_lookup_option_string(ctx, sec, "comparison_type");
        const char *event_reference = uci_lookup_option_string(ctx, sec, "reference");
        if (event_topics_name == NULL || 
            event_parameter == NULL || 
            event_data_type == NULL || 
            event_comparison_type == NULL ||
            event_reference == NULL) {
            fprintf(stderr, "Configuration parse failed\n");
            free(*events);
            return -1;
        }
        (*events)[event_array] = (char **)malloc(sizeof(char*) * 5);  
        (*events)[event_array][EVENT_TOPICS_NAME_INDEX] = (char *)malloc(strlen(event_topics_name) + 1 * sizeof(char));
        (*events)[event_array][EVENT_PARAMETER_INDEX] = (char *)malloc(strlen(event_parameter) + 1 * sizeof(char));
        (*events)[event_array][EVENT_DATA_TYPE_INDEX] = (char *)malloc(strlen(event_data_type) + 1 * sizeof(char));
        (*events)[event_array][EVENT_COMPARISON_TYPE_INDEX] = (char *)malloc(strlen(event_comparison_type) + 1 * sizeof(char));
        (*events)[event_array][EVENT_REFERENCE_INDEX] = (char *)malloc(strlen(event_reference) + 1 * sizeof(char));
        strcpy((*events)[event_array][EVENT_TOPICS_NAME_INDEX], event_topics_name);
        strcpy((*events)[event_array][EVENT_PARAMETER_INDEX], event_parameter);
        strcpy((*events)[event_array][EVENT_DATA_TYPE_INDEX], event_data_type);
        strcpy((*events)[event_array][EVENT_COMPARISON_TYPE_INDEX], event_comparison_type);
        strcpy((*events)[event_array][EVENT_REFERENCE_INDEX], event_reference);
        ++event_array;
        if (event_array == *total_events_count)
            break;
    }
    if (event_array < 1){
        uci_events_free(*events, *total_events_count);
        *total_events_count = 0;
        return -1;
    }
    return 0;
}

int uci_events(char ****events, char **topics, int *total_events_count, int total_topics_count)
{
    struct uci_context *ctx = NULL;
    struct uci_package *pkg = NULL;
    struct uci_section *sec = NULL;
    struct uci_element *e = NULL;
    ctx = uci_alloc_context();
    if (!ctx) {
        fprintf(stderr, "Failed to allocate UCI context\n");
        free(*events);
        return -1;
    }
    if (uci_load(ctx, "mqtt_topics", &pkg) != UCI_OK) {
        fprintf(stderr, "Failed to load UCI configuration file");
        uci_perror(ctx, "Error");
        free(*events);
        uci_free_context(ctx);
        return -1;
    }
    *total_events_count = uci_generic_events_count(pkg, e, sec);
    if (*total_events_count == 0) {
        free(*events);
        uci_unload(ctx, pkg);
        uci_free_context(ctx);
        return -1;
    }
    if (get_events(events, topics, total_events_count, total_topics_count, ctx, pkg, sec, e) != 0) {
        uci_unload(ctx, pkg);
        uci_free_context(ctx);
        return -1;
    }
    uci_unload(ctx, pkg);
    uci_free_context(ctx);
    return 0;
}

bool param_zero(int *params)
{
    --(*params);
    if (*params == 0)
        return true;
    return false;
}

int check_event_logic(struct uci_topic_data **uci_data)
{   
    for (int i = 0; i < (*uci_data)->events_count; i++){
        if (strcmp((*uci_data)->events[i][EVENT_DATA_TYPE_INDEX], "alphanumeric") == 0) {
            if ((strcmp((*uci_data)->events[i][EVENT_COMPARISON_TYPE_INDEX], "!=") != 0) &&
                (strcmp((*uci_data)->events[i][EVENT_COMPARISON_TYPE_INDEX], "=") != 0))
                    return -1;
        }
        if (strcmp((*uci_data)->events[i][EVENT_DATA_TYPE_INDEX], "numeric") == 0){
            if (atoi((*uci_data)->events[i][EVENT_REFERENCE_INDEX]) == 0)
                return -1;
        }
    }
    return 0;
}

void uci_event_alphanumeric(struct uci_topic_data **uci_data, int current, char *payload)
{   
    cJSON *json_payload = cJSON_Parse(payload);
    if (json_payload == NULL){
        fprintf(stderr, "JSON payload error\n");
        return;
    }
    int params = cJSON_GetArraySize(json_payload);
    char *string = json_payload->child->string;
    char *valuestring = json_payload->child->valuestring;
    if (string == NULL || valuestring == NULL){
        fprintf(stderr, "JSON payload error\n");
        return;
    }
    while (1){
        if ((strcmp((*uci_data)->events[current][EVENT_COMPARISON_TYPE_INDEX], "=") == 0) &&
            (strcmp(string, (*uci_data)->events[current][EVENT_PARAMETER_INDEX]) == 0)){
                if ((strcmp(valuestring, (*uci_data)->events[current][EVENT_REFERENCE_INDEX])) == 0){
                    printf("mail sent: uci_event_alphanumeric = %s\n", (*uci_data)->events[current][EVENT_REFERENCE_INDEX]);                
                    goto exit;
                }
            }
        if ((strcmp((*uci_data)->events[current][EVENT_COMPARISON_TYPE_INDEX], "!=") == 0) &&
            (strcmp(string, (*uci_data)->events[current][EVENT_PARAMETER_INDEX]) == 0)){
                if ((strcmp(valuestring, (*uci_data)->events[current][EVENT_REFERENCE_INDEX])) != 0){
                    printf("mail sent: uci_event_alphanumeric != %s\n", (*uci_data)->events[current][EVENT_REFERENCE_INDEX]);
                    goto exit;
                }
        }
        if (param_zero(&params) == true)
            goto exit;
        string = json_payload->child->next->string;
        valuestring = json_payload->child->next->valuestring;
    }
exit:    
    cJSON_Delete(json_payload);
}

void uci_event_numeric(struct uci_topic_data **uci_data, int current, char *payload)
{
    cJSON *json_payload = cJSON_Parse(payload);
    if (json_payload == NULL)
        return;
    int params = cJSON_GetArraySize(json_payload);
    char *string = json_payload->child->string;
    int valueint = json_payload->child->valueint;
    if (string == NULL || valueint == 0)
        return;
    printf("valuestring %s valueint %d\n", string, valueint);
    while (1){
        if ((strcmp((*uci_data)->events[current][EVENT_COMPARISON_TYPE_INDEX], "=") == 0) &&
            (strcmp(string, (*uci_data)->events[current][EVENT_PARAMETER_INDEX]) == 0)){
            if (valueint == atoi((*uci_data)->events[current][EVENT_REFERENCE_INDEX])){
                printf("mail sent: uci_event_numeric == %s\n", (*uci_data)->events[current][EVENT_REFERENCE_INDEX]);                
                    goto exit;
            }
        }
        if ((strcmp((*uci_data)->events[current][EVENT_COMPARISON_TYPE_INDEX], "!=") == 0) &&
            (strcmp(string, (*uci_data)->events[current][EVENT_PARAMETER_INDEX]) == 0)){
            if (valueint != atoi((*uci_data)->events[current][EVENT_REFERENCE_INDEX])){
                printf("mail sent: uci_event_numeric != %s\n", (*uci_data)->events[current][EVENT_REFERENCE_INDEX]);                
                goto exit;
            }
        }
        if ((strcmp((*uci_data)->events[current][EVENT_COMPARISON_TYPE_INDEX], ">") == 0) &&
            (strcmp(string, (*uci_data)->events[current][EVENT_PARAMETER_INDEX]) == 0)){
                if (valueint > atoi((*uci_data)->events[current][EVENT_REFERENCE_INDEX])){
                    printf("mail sent: uci_event_numeric > %s\n", (*uci_data)->events[current][EVENT_REFERENCE_INDEX]);                
                    goto exit;
                }
        }
        if ((strcmp((*uci_data)->events[current][EVENT_COMPARISON_TYPE_INDEX], "<") == 0) &&
            (strcmp(string, (*uci_data)->events[current][EVENT_PARAMETER_INDEX]) == 0)){
                if (valueint < atoi((*uci_data)->events[current][EVENT_REFERENCE_INDEX])){
                    printf("mail sent: uci_event_numeric < %s\n", (*uci_data)->events[current][EVENT_REFERENCE_INDEX]);                
                    goto exit;
                }
        }
        if ((strcmp((*uci_data)->events[current][EVENT_COMPARISON_TYPE_INDEX], ">=") == 0) &&
            (strcmp(string, (*uci_data)->events[current][EVENT_PARAMETER_INDEX]) == 0)){
                if (valueint >= atoi((*uci_data)->events[current][EVENT_REFERENCE_INDEX])){
                    printf("mail sent: uci_event_numeric >= %s\n", (*uci_data)->events[current][EVENT_REFERENCE_INDEX]);                
                    goto exit;
                }
        }
        if ((strcmp((*uci_data)->events[current][EVENT_COMPARISON_TYPE_INDEX], "<=") == 0) &&
            (strcmp(string, (*uci_data)->events[current][EVENT_PARAMETER_INDEX]) == 0)){
                if (valueint <= atoi((*uci_data)->events[current][EVENT_REFERENCE_INDEX])){
                    printf("mail sent: uci_event_numeric <= %s\n", (*uci_data)->events[current][EVENT_REFERENCE_INDEX]);                
                    goto exit;
                }
        if (param_zero(&params) == true)
            goto exit;
        string = json_payload->child->next->string;
        valueint = json_payload->child->next->valueint;
        }
    }
exit:    
    cJSON_Delete(json_payload);
}

int uci_event_funcionality(struct uci_topic_data **uci_data, char *payload)
{   
    for (int i = 0; i < (*uci_data)->events_count; i++){
        if (strcmp((*uci_data)->events[i][EVENT_DATA_TYPE_INDEX], "alphanumeric") == 0)
            uci_event_alphanumeric(uci_data, i, payload); 
        if (strcmp((*uci_data)->events[i][EVENT_DATA_TYPE_INDEX], "numeric") == 0){
            uci_event_numeric(uci_data, i, payload);
        }
    }
    return 0;
}

void uci_events_free(char ***events, int total_events_count)
{
    if (events == NULL)
        return;
    for (int i = 0; i < total_events_count; i++){
        for (int j = 0; j < 5; j++)
            free(events[i][j]);
        free(events[i]);
    }
    free(events);
}

void uci_topics_free(char **topics, int total_topics_count)
{   
    if (topics == NULL || topics[0] == NULL)
        return;
    for (int i = 0; i < total_topics_count; i++)
        free(topics[i]);
    free(topics);
}

void cleanup(struct mosquitto* mosq, struct uci_topic_data *uci_data)
{   
    if (mosq != NULL)
        clean_up_libmosquitto(mosq);
    if (uci_data != NULL) {
        if (uci_data->topics != NULL)
            uci_topics_free(uci_data->topics, uci_data->topics_count);
        if (uci_data->events != NULL)
            uci_events_free(uci_data->events, uci_data->events_count);
    }
}