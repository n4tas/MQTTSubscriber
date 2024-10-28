#include "../include/args.h"

struct arguments arguments = {0};

error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'h':
            arguments->host = arg;
            break;
        case 'p':
            arguments->port = atoi(arg);
            break;
        case 'd':
            arguments->daemon = 1;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1)
                argp_usage(state);
            break;

        case ARGP_KEY_END:
            if (state->arg_num < 0)
                argp_usage(state);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

void load_arguments(int argc, char **argv)
{   
    if (argc == 0 || argc == 1){
        fprintf(stdout, "Using default host (0.0.0.0) and port (1883) \n");
        arguments.host = "0.0.0.0";
        arguments.port = 1883;
        return;
    }
    static struct argp argp = {options, parse_opt, 0, 0};
    memset(&arguments, 0, sizeof(arguments)); 
    argp_parse(&argp, argc, argv, ARGP_NO_HELP, 0, &arguments);
}